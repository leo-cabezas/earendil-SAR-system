#include <Earendil_AccelGyro.h>

namespace Earendil_AccelGyro {

    Adafruit_LSM6DSOX accelgyro;



// ─── Calibration state ─────────────────────────────────────────────
static float gyroBias[3]    = {0.0f, 0.0f, 0.0f};
static float accelOffset[3] = {0.0f, 0.0f, 0.0f};
static float accelScale[3]  = {1.0f, 1.0f, 1.0f};

// ─── Internal helpers ──────────────────────────────────────────────


    void setup()
    {
        gyroSetup();
    }

static void countdown(int n){
    char cd_buf[32];
    for (; n>0; n--){
        snprintf(cd_buf, sizeof(cd_buf), "%d...\n", n);
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
        accelgyro.getEvent(&accel, &gyro, &temp);
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

    // lcdPrint("Confirm/Cancel");

    // if (waitForButton() == BTN_CANCEL) {
    //     lcdPrint(0, "Calibration");
    //     lcdPrint(1, "Cancelled.");
    //     lcdPrint(2, "");
    //     return false;
    // }

    //lcdPrint("Sampling gyro...");
    //lcdPrint("");

    if (!collectSamples(avgA, avgG, CALIB_SAMPLES)) {
        //lcdPrint(1, "I2C Error!");
        //lcdPrint("Collection Error!");
        return false;
    }

    gyroBias[0] = avgG[0];
    gyroBias[1] = avgG[1];
    gyroBias[2] = avgG[2];

    snprintf(buf, LCD_MSG_LEN, "X:%.4f", gyroBias[0]);
    //lcdPrint("Gyro bias done:");
    //lcdPrint(buf);
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

    //lcdPrint("Get ready, Y arrow away, X arrow right");

    for (int p = 0; p < 6; p++) {
        snprintf(buf, LCD_MSG_LEN, "%d/6: %s", p + 1, posLabels[p]);
        //lcdPrint("Accel Calibration");
        //lcdPrint(buf);
        vTaskDelay(pdMS_TO_TICKS(3000));
        // lcdPrint("Confirm/Cancel");

        // if (waitForButton() == BTN_CANCEL) {
        //     lcdPrint(0, "Calibration");
        //     lcdPrint(1, "Cancelled.");
        //     lcdPrint(2, "");
        //     return false;
        // }

        countdown(5);

        if (!collectSamples(posData[p], avgG, CALIB_SAMPLES)) {
            //lcdPrint(1, "I2C Error!");
            //lcdPrint("Collection Error!");
            return false;
        }

        snprintf(buf, LCD_MSG_LEN, "%.3f %.3f %.3f",
                 posData[p][0], posData[p][1], posData[p][2]);
        //lcdPrint(2, buf);
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

    //lcdPrint("Calibration Done");
    snprintf(buf, LCD_MSG_LEN, "Offsets: %.3f %.3f %.3f",
             accelOffset[0], accelOffset[1], accelOffset[2]);
    //lcdPrint(buf);
    snprintf(buf, LCD_MSG_LEN, "Scale:   %.3f %.3f %.3f",
             accelScale[0], accelScale[1], accelScale[2]);
    //lcdPrint(buf);
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


    //if (xSemaphoreTake(s_i2cMutex, pdMS_TO_TICKS(I2C_TIMEOUT_MS)) == pdTRUE) {
        bool found = accelgyro.begin_I2C();
        //xSemaphoreGive(s_i2cMutex);
        if (!found) {
            //lcdPrint("LSM6DSOX");
            //lcdPrint("NOT FOUND");
            while (1) { 
                vTaskDelay(pdMS_TO_TICKS(100));
            }
        }
    //}

    //lcdPrint("LSM6DSOX Found!");
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    // Log accel range
    // accelgyro.setAccelRange(LSM6DS_ACCEL_RANGE_2_G);
    char a_buf[LCD_MSG_LEN];
    switch (accelgyro.getAccelRange()) {
        case LSM6DS_ACCEL_RANGE_2_G:  snprintf(a_buf, LCD_MSG_LEN, "Accel: +-2G");  break;
        case LSM6DS_ACCEL_RANGE_4_G:  snprintf(a_buf, LCD_MSG_LEN, "Accel: +-4G");  break;
        case LSM6DS_ACCEL_RANGE_8_G:  snprintf(a_buf, LCD_MSG_LEN, "Accel: +-8G");  break;
        case LSM6DS_ACCEL_RANGE_16_G: snprintf(a_buf, LCD_MSG_LEN, "Accel: +-16G"); break;
        default:                      snprintf(a_buf, LCD_MSG_LEN, "Accel: Unknown"); break;
    }
    //lcdPrint(a_buf);
    vTaskDelay(pdMS_TO_TICKS(1000));

    // accelgyro.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS );
    switch (accelgyro.getGyroRange()) {
    case LSM6DS_GYRO_RANGE_125_DPS: snprintf(a_buf, LCD_MSG_LEN, "Gyro Range: 125 degrees/s"); break;
    case LSM6DS_GYRO_RANGE_250_DPS: snprintf(a_buf, LCD_MSG_LEN, "Gyro Range: 250 degrees/s"); break;
    case LSM6DS_GYRO_RANGE_500_DPS: snprintf(a_buf, LCD_MSG_LEN, "Gyro Range: 500 degrees/s"); break;
    case LSM6DS_GYRO_RANGE_1000_DPS: snprintf(a_buf, LCD_MSG_LEN, "Gyro Range: 1000 degrees/s"); break;
    case LSM6DS_GYRO_RANGE_2000_DPS: snprintf(a_buf, LCD_MSG_LEN, "Gyro Range: 2000 degrees/s"); break;
    case ISM330DHCX_GYRO_RANGE_4000_DPS: snprintf(a_buf, LCD_MSG_LEN, "Gyro Range: N/A"); break; // unsupported range for the DSOX
    }
    //lcdPrint(a_buf);
    vTaskDelay(pdMS_TO_TICKS(1000));

    // accelgyro.setAccelDataRate(LSM6DS_RATE_12_5_HZ);
    switch (accelgyro.getAccelDataRate()) {
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
    //lcdPrint(a_buf);
    vTaskDelay(pdMS_TO_TICKS(1000));

    // accelgyro.setGyroDataRate(LSM6DS_RATE_12_5_HZ);
    switch (accelgyro.getGyroDataRate()) {
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

    vTaskDelay(pdMS_TO_TICKS(1000));
}

// ─── Reading (calibration applied) ────────────────────────────────

void gyroReading()
{
    //if (xSemaphoreTake(s_i2cMutex, pdMS_TO_TICKS(I2C_TIMEOUT_MS)) != pdTRUE) return;

    sensors_event_t accel, gyro, temp;
    accelgyro.getEvent(&accel, &gyro, &temp);
    //xSemaphoreGive(s_i2cMutex);

    Earendil_Data-> AccelGyro_Data.GyroMetrics[0] = (accel.acceleration.x - accelOffset[0]) * accelScale[0];
    Earendil_Data-> AccelGyro_Data.GyroMetrics[1] = (accel.acceleration.y - accelOffset[1]) * accelScale[1];
    Earendil_Data-> AccelGyro_Data.GyroMetrics[2] = (accel.acceleration.z - accelOffset[2]) * accelScale[2];
    Earendil_Data-> AccelGyro_Data.GyroMetrics[3] = gyro.gyro.x - gyroBias[0];
    Earendil_Data-> AccelGyro_Data.GyroMetrics[4] = gyro.gyro.y - gyroBias[1];
    Earendil_Data-> AccelGyro_Data.GyroMetrics[5] = gyro.gyro.z - gyroBias[2];


    //send data to a shared struct
}

// ─── Display (snprintf for debug) ─────────────────────────────────

void gyroShow()
{
    printf("\n============ACCELL-OSCOPE READING===============\nAX:%.2f AY:%.2f AZ:%.2f\nGX:%.2f GY:%.2f GZ:%.2f\n================================================\n", 
        Earendil_Data-> AccelGyro_Data.GyroMetrics[0], 
        Earendil_Data-> AccelGyro_Data.GyroMetrics[1], 
        Earendil_Data-> AccelGyro_Data.GyroMetrics[2], 
        Earendil_Data-> AccelGyro_Data.GyroMetrics[3], 
        Earendil_Data-> AccelGyro_Data.GyroMetrics[4], 
        Earendil_Data-> AccelGyro_Data.GyroMetrics[5]
    );
    // char buf[LCD_MSG_LEN];
    // snprintf(buf, LCD_MSG_LEN, "AX:%.2f AY:%.2f AZ:%.2f",
    //          metrics[0], metrics[1], metrics[2]);
    // lcdPrint(0, buf);
    // snprintf(buf, LCD_MSG_LEN, "GX:%.2f GY:%.2f GZ:%.2f",
    //          metrics[3], metrics[4], metrics[5]);
    // lcdPrint(1, buf);
}

}