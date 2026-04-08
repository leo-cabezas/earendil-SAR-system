#include <Earendil_AccelGyro.h> // ATTENTION: Add all library dependencies to this header.

// ─── Sensor instance ───────────────────────────────────────────────
Adafruit_LSM6DSOX sox;

// ─── Shared handles ────────────────────────────────────────────────
EventGroupHandle_t  gyroEventGroup = NULL;
// static QueueHandle_t    s_lcdQueue    = NULL;
// static QueueHandle_t    s_buttonQueue = NULL;
// static SemaphoreHandle_t s_i2cMutex  = NULL;

// ─── Calibration state ─────────────────────────────────────────────
static float gyroBias[3]    = {0.0f, 0.0f, 0.0f};
static float accelOffset[3] = {0.0f, 0.0f, 0.0f};
static float accelScale[3]  = {1.0f, 1.0f, 1.0f};

// ─── Internal helpers ──────────────────────────────────────────────

static void lcdPrint(uint8_t line, const char* msg)
{
    printf("%s\n", msg);
    fflush(stdout);
    // if (s_lcdQueue == NULL) return;
    // LCDMessage_t m;
    // m.line = line;
    // snprintf(m.text, LCD_MSG_LEN, "%s", msg);
    // xQueueSend(s_lcdQueue, &m, pdMS_TO_TICKS(50));
}

// // Blocks until BTN_CONFIRM or BTN_CANCEL is received from the ISR queue.
// // Returns the button ID received.
// static uint8_t waitForButton(void)
// {
//     uint8_t btn = 0;
//     while (btn != BTN_CONFIRM && btn != BTN_CANCEL) {
//         xQueueReceive(s_buttonQueue, &btn, portMAX_DELAY);
//     }
//     return btn;
// }

static void countdown(int n){
    char cd_buf[32];
    lcdPrint(0, "Sampling in ");
    for (; n>0; n--){
        snprintf(cd_buf, sizeof(cd_buf), "%d...\n", n);
        lcdPrint(0, cd_buf);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// Collects n averaged accel+gyro samples under the I2C mutex.
static bool collectSamples(float outAccel[3], float outGyro[3], int n)
{
    double sumA[3] = {0.0, 0.0, 0.0};
    double sumG[3] = {0.0, 0.0, 0.0};

    for (int i = 0; i < n; i++) {
        //if (xSemaphoreTake(s_i2cMutex, pdMS_TO_TICKS(I2C_TIMEOUT_MS)) != pdTRUE) {
        //    return false; // I2C bus unavailable
        //}
        sensors_event_t accel, gyro, temp;
        sox.getEvent(&accel, &gyro, &temp);
        //xSemaphoreGive(s_i2cMutex);

        sumA[0] += accel.acceleration.x;
        sumA[1] += accel.acceleration.y;
        sumA[2] += accel.acceleration.z;
        sumG[0] += gyro.gyro.x;
        sumG[1] += gyro.gyro.y;
        sumG[2] += gyro.gyro.z;

        vTaskDelay(pdMS_TO_TICKS(2));
    }

    for (int i = 0; i < 3; i++) {
        outAccel[i] = (float)(sumA[i] / n);
        outGyro[i]  = (float)(sumG[i] / n);
    }
    return true;
}

// ─── Calibration routine ───────────────────────────────────────────

static bool gyroCalibrate(void)
{
    char buf[LCD_MSG_LEN];
    float avgA[3], avgG[3];

    // ── Gyro bias ──────────────────────────────────────────────────
    lcdPrint(0, "Gyro Calibration");
    lcdPrint(1, "Place flat+still");
    // lcdPrint(2, "Confirm/Cancel");

    // if (waitForButton() == BTN_CANCEL) {
    //     lcdPrint(0, "Calibration");
    //     lcdPrint(1, "Cancelled.");
    //     lcdPrint(2, "");
    //     return false;
    // }

    lcdPrint(1, "Sampling gyro...");
    lcdPrint(2, "");

    if (!collectSamples(avgA, avgG, CALIB_SAMPLES)) {
        //lcdPrint(1, "I2C Error!");
        lcdPrint(1, "Collection Error!");
        return false;
    }

    gyroBias[0] = avgG[0];
    gyroBias[1] = avgG[1];
    gyroBias[2] = avgG[2];

    snprintf(buf, LCD_MSG_LEN, "X:%.4f", gyroBias[0]);
    lcdPrint(1, "Gyro bias done:");
    lcdPrint(2, buf);
    vTaskDelay(pdMS_TO_TICKS(1500));

    // ── Accel 6-position calibration ──────────────────────────────
    // Position order: +Z, -Z, +X, -X, +Y, -Y
    const char* posLabels[6] = {
        "+Z UP: put chip face up",
        "-Z UP: put chip face dn",
        "+X UP: put left edge dn",
        "-X UP: put rght edge dn",
        "+Y UP: put btm edge dn",
        "-Y UP: put top edge dn"
    };
    const int   activeAxis[6] = { 2,  2,  0,  0,  1,  1 };
    const float expectSign[6] = {+1, -1, +1, -1, +1, -1 };

    float posData[6][3];

    lcdPrint(0, "Get ready, Y arrow away, X arrow right");

    for (int p = 0; p < 6; p++) {
        snprintf(buf, LCD_MSG_LEN, "%d/6: %s", p + 1, posLabels[p]);
        lcdPrint(0, "Accel Calibration");
        lcdPrint(1, buf);
        vTaskDelay(pdMS_TO_TICKS(3000));
        // lcdPrint(2, "Confirm/Cancel");

        // if (waitForButton() == BTN_CANCEL) {
        //     lcdPrint(0, "Calibration");
        //     lcdPrint(1, "Cancelled.");
        //     lcdPrint(2, "");
        //     return false;
        // }

        countdown(5);

        if (!collectSamples(posData[p], avgG, CALIB_SAMPLES)) {
            //lcdPrint(1, "I2C Error!");
            lcdPrint(1, "Collection Error!");
            return false;
        }

        snprintf(buf, LCD_MSG_LEN, "%.3f %.3f %.3f",
                 posData[p][0], posData[p][1], posData[p][2]);
        lcdPrint(2, buf);
        vTaskDelay(pdMS_TO_TICKS(3000));
    }

    // Compute offset and scale per axis
    for (int ax = 0; ax < 3; ax++) {
        float posReading = 0.0f, negReading = 0.0f;
        for (int p = 0; p < 6; p++) {
            if (activeAxis[p] == ax) {
                if (expectSign[p] > 0) posReading = posData[p][ax];
                else                    negReading = posData[p][ax];
            }
        }
        accelOffset[ax] = (posReading + negReading) / 2.0f;
        accelScale[ax]  = GRAVITY / ((posReading - negReading) / 2.0f);
    }

    lcdPrint(0, "Calibration Done");
    snprintf(buf, LCD_MSG_LEN, "Offsets: %.3f %.3f %.3f",
             accelOffset[0], accelOffset[1], accelOffset[2]);
    lcdPrint(1, buf);
    snprintf(buf, LCD_MSG_LEN, "Scale:   %.3f %.3f %.3f",
             accelScale[0], accelScale[1], accelScale[2]);
    lcdPrint(2, buf);
    vTaskDelay(pdMS_TO_TICKS(2000));

    return true;
}

// ─── Setup ─────────────────────────────────────────────────────────

// void gyroSetup(QueueHandle_t lcdQueue,
//                QueueHandle_t buttonQueue,
//                SemaphoreHandle_t i2cMutex)
// {
void gyroSetup(){
    // printf("Accell-oscope setup initiating.\n");
    // s_lcdQueue    = lcdQueue;
    // s_buttonQueue = buttonQueue;
    // s_i2cMutex    = i2cMutex;

    // gyroEventGroup = xEventGroupCreate();
    // configASSERT(gyroEventGroup != NULL);

    //if (xSemaphoreTake(s_i2cMutex, pdMS_TO_TICKS(I2C_TIMEOUT_MS)) == pdTRUE) {
        bool found = sox.begin_I2C();
        //xSemaphoreGive(s_i2cMutex);
        if (!found) {
            lcdPrint(0, "LSM6DSOX");
            lcdPrint(1, "NOT FOUND");
            while (1) { 
                vTaskDelay(pdMS_TO_TICKS(100));
                printf(":(");
            }
        }
    //}

    lcdPrint(0, "LSM6DSOX Found!");
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    // Log accel range
    // sox.setAccelRange(LSM6DS_ACCEL_RANGE_2_G);
    char a_buf[LCD_MSG_LEN];
    switch (sox.getAccelRange()) {
        case LSM6DS_ACCEL_RANGE_2_G:  snprintf(a_buf, LCD_MSG_LEN, "Accel: +-2G");  break;
        case LSM6DS_ACCEL_RANGE_4_G:  snprintf(a_buf, LCD_MSG_LEN, "Accel: +-4G");  break;
        case LSM6DS_ACCEL_RANGE_8_G:  snprintf(a_buf, LCD_MSG_LEN, "Accel: +-8G");  break;
        case LSM6DS_ACCEL_RANGE_16_G: snprintf(a_buf, LCD_MSG_LEN, "Accel: +-16G"); break;
        default:                      snprintf(a_buf, LCD_MSG_LEN, "Accel: Unknown"); break;
    }
    lcdPrint(1, a_buf);
    vTaskDelay(pdMS_TO_TICKS(1000));

    // sox.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS );
    switch (sox.getGyroRange()) {
    case LSM6DS_GYRO_RANGE_125_DPS: snprintf(a_buf, LCD_MSG_LEN, "Gyro Range: 125 degrees/s"); break;
    case LSM6DS_GYRO_RANGE_250_DPS: snprintf(a_buf, LCD_MSG_LEN, "Gyro Range: 250 degrees/s"); break;
    case LSM6DS_GYRO_RANGE_500_DPS: snprintf(a_buf, LCD_MSG_LEN, "Gyro Range: 500 degrees/s"); break;
    case LSM6DS_GYRO_RANGE_1000_DPS: snprintf(a_buf, LCD_MSG_LEN, "Gyro Range: 1000 degrees/s"); break;
    case LSM6DS_GYRO_RANGE_2000_DPS: snprintf(a_buf, LCD_MSG_LEN, "Gyro Range: 2000 degrees/s"); break;
    case ISM330DHCX_GYRO_RANGE_4000_DPS: snprintf(a_buf, LCD_MSG_LEN, "Gyro Range: N/A"); break; // unsupported range for the DSOX
    }
    lcdPrint(1, a_buf);
    vTaskDelay(pdMS_TO_TICKS(1000));

    // sox.setAccelDataRate(LSM6DS_RATE_12_5_HZ);
    switch (sox.getAccelDataRate()) {
    case LSM6DS_RATE_SHUTDOWN: snprintf(a_buf, LCD_MSG_LEN, "Accel Data Rate:  0 Hz"); break;
    case LSM6DS_RATE_12_5_HZ: snprintf(a_buf, LCD_MSG_LEN, "Accel Data Rate:  12.5 Hz"); break;
    case LSM6DS_RATE_26_HZ: snprintf(a_buf, LCD_MSG_LEN, "Accel Data Rate:  26 Hz"); break;
    case LSM6DS_RATE_52_HZ: snprintf(a_buf, LCD_MSG_LEN, "Accel Data Rate:  52 Hz"); break;
    case LSM6DS_RATE_104_HZ: snprintf(a_buf, LCD_MSG_LEN, "Accel Data Rate:  104 Hz"); break;
    case LSM6DS_RATE_208_HZ: snprintf(a_buf, LCD_MSG_LEN, "Accel Data Rate:  208 Hz"); break;
    case LSM6DS_RATE_416_HZ: snprintf(a_buf, LCD_MSG_LEN, "Accel Data Rate:  416 Hz"); break;
    case LSM6DS_RATE_833_HZ: snprintf(a_buf, LCD_MSG_LEN, "Accel Data Rate:  833 Hz"); break;
    case LSM6DS_RATE_1_66K_HZ: snprintf(a_buf, LCD_MSG_LEN, "Accel Data Rate:  1.66 KHz"); break;
    case LSM6DS_RATE_3_33K_HZ: snprintf(a_buf, LCD_MSG_LEN, "Accel Data Rate:  3.33 KHz"); break;
    case LSM6DS_RATE_6_66K_HZ: snprintf(a_buf, LCD_MSG_LEN, "Accel Data Rate:  6.66 KHz"); break;
    }
    lcdPrint(1, a_buf);
    vTaskDelay(pdMS_TO_TICKS(1000));

    // sox.setGyroDataRate(LSM6DS_RATE_12_5_HZ);
    switch (sox.getGyroDataRate()) {
    case LSM6DS_RATE_SHUTDOWN: snprintf(a_buf, LCD_MSG_LEN, "Gyro Data Rate:  0 Hz"); break;
    case LSM6DS_RATE_12_5_HZ: snprintf(a_buf, LCD_MSG_LEN, "Gyro Data Rate:  12.5 Hz"); break;
    case LSM6DS_RATE_26_HZ: snprintf(a_buf, LCD_MSG_LEN, "Gyro Data Rate:  26 Hz"); break;
    case LSM6DS_RATE_52_HZ: snprintf(a_buf, LCD_MSG_LEN, "Gyro Data Rate:  52 Hz"); break;
    case LSM6DS_RATE_104_HZ: snprintf(a_buf, LCD_MSG_LEN, "Gyro Data Rate:  104 Hz"); break;
    case LSM6DS_RATE_208_HZ: snprintf(a_buf, LCD_MSG_LEN, "Gyro Data Rate:  208 Hz"); break;
    case LSM6DS_RATE_416_HZ: snprintf(a_buf, LCD_MSG_LEN, "Gyro Data Rate:  416 Hz"); break;
    case LSM6DS_RATE_833_HZ: snprintf(a_buf, LCD_MSG_LEN, "Gyro Data Rate:  833 Hz"); break;
    case LSM6DS_RATE_1_66K_HZ: snprintf(a_buf, LCD_MSG_LEN, "Gyro Data Rate:  1.66 KHz"); break;
    case LSM6DS_RATE_3_33K_HZ: snprintf(a_buf, LCD_MSG_LEN, "Gyro Data Rate:  3.33 KHz"); break;
    case LSM6DS_RATE_6_66K_HZ: snprintf(a_buf, LCD_MSG_LEN, "Gyro Data Rate:  6.66 KHz"); break;
    }
    lcdPrint(1, a_buf);
    vTaskDelay(pdMS_TO_TICKS(1000));
}

// ─── Reading (calibration applied) ────────────────────────────────

void gyroReading(GyroMetrics_t metrics)
{
    //if (xSemaphoreTake(s_i2cMutex, pdMS_TO_TICKS(I2C_TIMEOUT_MS)) != pdTRUE) return;

    sensors_event_t accel, gyro, temp;
    sox.getEvent(&accel, &gyro, &temp);
    //xSemaphoreGive(s_i2cMutex);

    metrics[0] = (accel.acceleration.x - accelOffset[0]) * accelScale[0];
    metrics[1] = (accel.acceleration.y - accelOffset[1]) * accelScale[1];
    metrics[2] = (accel.acceleration.z - accelOffset[2]) * accelScale[2];
    metrics[3] = gyro.gyro.x - gyroBias[0];
    metrics[4] = gyro.gyro.y - gyroBias[1];
    metrics[5] = gyro.gyro.z - gyroBias[2];
}

// ─── Display (snprintf for debug) ─────────────────────────────────

void gyroShow(GyroMetrics_t metrics)
{
    printf("\n============ACCELL-OSCOPE READING===============\nAX:%.2f AY:%.2f AZ:%.2f\nGX:%.2f GY:%.2f GZ:%.2f\n================================================\n", 
        metrics[0], 
        metrics[1], 
        metrics[2], 
        metrics[3], 
        metrics[4], 
        metrics[5]
    );
    // char buf[LCD_MSG_LEN];
    // snprintf(buf, LCD_MSG_LEN, "AX:%.2f AY:%.2f AZ:%.2f",
    //          metrics[0], metrics[1], metrics[2]);
    // lcdPrint(0, buf);
    // snprintf(buf, LCD_MSG_LEN, "GX:%.2f GY:%.2f GZ:%.2f",
    //          metrics[3], metrics[4], metrics[5]);
    // lcdPrint(1, buf);
}

// ─── FreeRTOS task ─────────────────────────────────────────────────
void vAccelGyro(void* pvParameters){
  (void) pvParameters;
  vTaskDelay(pdMS_TO_TICKS(10000));
  printf("Accell-oscope started!\n");

  GyroMetrics_t metrics;
//   EventBits_t   bits;
  gyroSetup();
  gyroCalibrate();
  
  while (1){
    // bits = xEventGroupGetBits(gyroEventGroup);

    // if (bits & GYRO_EVT_CALIBRATE_REQUEST) {
    //     // Clear the request bit before starting
    //     xEventGroupClearBits(gyroEventGroup, GYRO_EVT_CALIBRATE_REQUEST);

    //     bool success = gyroCalibrate();

    //     // Signal completion or cancellation back to the requester
    //     xEventGroupSetBits(gyroEventGroup, success ? GYRO_EVT_CALIBRATE_COMPLETE : GYRO_EVT_CALIBRATE_CANCELLED);
    // } else {
    //     // Normal operation: read and expose sensor data
        gyroReading(metrics);
        gyroShow(metrics); //For debugging
    // }
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
