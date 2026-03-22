//Prototype Code for Earendil Handheld device

#include <SPI.h>
#include <RH_RF95.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_BMP3XX.h>
#include <math.h>
#include <Adafruit_GFX.h>
#include <Adafruit_GC9A01A.h>
#include <Adafruit_MMC56x3.h>
#include <Adafruit_Sensor.h>

#define _USE_MATH_DEFINES

//RFM Globals==============================================================================================================
// #define RFM95_CS   16
// #define RFM95_INT  21
// #define RFM95_RST  17
// // Change to 434.0 or other frequency, must match RX's freq!
// #define RF95_FREQ 915.0
// // Singleton instance of the radio driver
// RH_RF95 rf95(RFM95_CS, RFM95_INT);

//GPS Globals==============================================================================================================
// SoftwareSerial GPS_UART(1, 0); //Connect the GPS TX pin to GPS_UART[0], Connect the GPS RX pin to GPS_UART[1]
// #define PMTK_SET_NMEA_UPDATE_1HZ  "$PMTK220,1000*1F"
// #define PMTK_SET_NMEA_UPDATE_5HZ  "$PMTK220,200*2C"
// #define PMTK_SET_NMEA_UPDATE_10HZ "$PMTK220,100*2F"
// // turn on only the second sentence (GPRMC)
// #define PMTK_SET_NMEA_OUTPUT_RMCONLY "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29"
// // turn on GPRMC and GGA
// #define PMTK_SET_NMEA_OUTPUT_RMCGGA "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"
// // turn on ALL THE DATA
// #define PMTK_SET_NMEA_OUTPUT_ALLDATA "$PMTK314,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28"
// // turn off output
// #define PMTK_SET_NMEA_OUTPUT_OFF "$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"
// #define PMTK_Q_RELEASE "$PMTK605*31"


//Altimeter Globals========================================================================================================
Adafruit_BMP3XX bmp; //Variable definition for BMP code
#define SEALEVELPRESSURE_HPA (1012.0)//Lawrence, KS local sea-level pressure: 20260226
#define LAPSE_RATE (0.0065)// K/m
#define GAS_CONSTANT (287.05)// J/kg·K
#define GRAVITY (9.80665)// m/s²


//LCD Globals & Structs====================================================================================================
#define TFT_CS   25
#define TFT_DC   24
#define TFT_RST  12
#define TFT_MOSI 15
#define TFT_MISO 8
#define TFT_SCK  14
#define X_OFFSET 120
#define Y_OFFSET 120
#define OUTLINE_BLU 0x2CB6
#define OUTLINE_DARKBLU 0x1188

Adafruit_GC9A01A tft = Adafruit_GC9A01A(TFT_CS,TFT_DC,TFT_RST);//initialization for the adafruit screen structure
GFXcanvas16 canvas(240, 240);


//Magnetometer Globals=========================================================================================================================
Adafruit_MMC5603 mag;

struct Magmeter{
  public:
    float hard_min[3] = {9999, 9999, 9999}; //ensures its crazy out of scope from any actual values so it can be filled
    float hard_max[3] = {-9999, -9999, -9999};
    float hard_iron[3] = {0.0, 0.0, 0.0};

    float soft_min[3] = {999, 999, 999};
    float soft_max[3] = {-999, -999, -999};
    float soft_iron[3][3];

};

Magmeter magmeter;

//Setup Code===================================================================================================================================
void setup() {
  //LCD Screen Setup Code
  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(GC9A01A_BLACK);

  //RFM First Setup Code Block
  // pinMode(RFM95_RST, OUTPUT);
  // digitalWrite(RFM95_RST, HIGH);

  Serial.begin(115200);
  while (!Serial) delay(1);
  Serial.println("Earendil: Handheld Prototype");

  //RFM Setup
  // manual reset
  // digitalWrite(RFM95_RST, LOW);
  // delay(10);
  // digitalWrite(RFM95_RST, HIGH);
  // delay(10);
  // while (!rf95.init()) {
  //   Serial.println("LoRa radio init failed");
  //   Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
  //   while (1);
  // }
  // Serial.println("LoRa radio init OK!");
  // // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  // if (!rf95.setFrequency(RF95_FREQ)) {
  //   Serial.println("setFrequency failed");
  //   while (1);
  // }
  // Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  // // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
  // // The default transmitter power is 13dBm, using PA_BOOST.
  // // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // // you can set transmitter powers from 5 to 23 dBm:
  // rf95.setTxPower(5, false);


  //GPS Setup
  // GPS_UART.begin(9600);
  // delay(2000);
  // GPS_UART.println(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // GPS_UART.println(PMTK_SET_NMEA_UPDATE_1HZ);
  // Serial.println("Get version!");
  // GPS_UART.println(PMTK_Q_RELEASE);


  //BMP390 Setup Code
  if (!bmp.begin_I2C()) {
    Serial.println("BMP390 not found. Check wiring!");
    while (1);
  }
  Serial.println("BMP390 found!");
  //The below parameters were chosen based on the documentation for the BMP390
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_2X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_16X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_7);
  bmp.setOutputDataRate(BMP3_ODR_1_5_HZ);


  //Magnetometer Setup Code
  Wire.begin();
  // I2C scan
  byte error, address;
  int nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
      nDevices++;
    }
    else if (error == 4) {
      Serial.print("Unknown error at 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0) Serial.println("No I2C devices found.");
  else Serial.println("I2C scan complete.");
  // Initialize MMC5603
  Serial.println("Initializing MMC5603...");
  if (!mag.begin(0x30, &Wire)) {
    Serial.println("MMC5603 not detected!");
    while (1);
  }
  Serial.println("MMC5603 detected!");

}

//int16_t packetnum = 0;  // packet counter, we increment per xmission

void loop() {
  tft.fillScreen(GC9A01A_BLACK);
  // char radiopacket[100];
  // int index = 0;

  // while (1){
  //   if (GPS_UART.available() && (GPS_UART.read() == '$')){
  //     radiopacket[0] = '$';
  //     index++;
  //     break;
  //   }
  // }
  // while (index < sizeof(radiopacket) - 1) {  // Leave room for null terminator
  //     if (GPS_UART.available()) {
  //         char c = GPS_UART.read();
  //         radiopacket[index++] = c;
  //         if (c == '*') {  // End condition (newline/carriage return) BUT NOT SURE WHAT THE END CONDITION IS FOR GPS DATA
  //             break;
  //         }
  //     }
  // }
  // radiopacket[index] = '\0';  // Null terminate

  //BMP390 Operations and Readings==================================================================================================
  float temp = 0.0;
  float pressure = 0.0;
  float altitude = 0.0;
  
  if (bmp.performReading()) {
    temp = bmp.temperature;
    pressure = bmp.pressure / 100.0;
    altitude = hypsometric_altitude((temp + 273.15),pressure);
    Serial.print("Temp: "); Serial.print(temp); Serial.println(" C");
    Serial.print("Pressure: "); Serial.print(pressure); Serial.println(" hPa");
    Serial.print("Approx. Altitude = "); Serial.print(altitude); Serial.println(" m");

  Serial.println();
  }

  //Magnetometer Operations and Readings===========================================================================================
  sensors_event_t event;
  mag.getEvent(&event);

  float mag_data[3] = {event.magnetic.x, event.magnetic.y, event.magnetic.z};
  float heading = -1*atan2(mag_data[0],mag_data[1])*180.0/M_PI;

  Serial.print("Raw:       ");
  Serial.print("X: "); Serial.print(mag_data[0], 2);
  Serial.print("  Y: "); Serial.print(mag_data[1], 2);
  Serial.print("  Z: "); Serial.println(mag_data[2], 2);

  //Print data to the LCD Screen====================================================================================================
  tft.setCursor(50, 50);//sets the cursor to draw the text. x is from left to right byt a larger y is downward
  tft.setTextColor(GC9A01A_WHITE);
  tft.setTextSize(3);//scale the text to be bigger
  tft.print(temp, 2);//prints out the temperature in Celsius to 2 decimal places
  tft.setCursor(50, 90);
  tft.print(altitude, 2);
  tft.setCursor(50, 130);
  tft.print(heading,2);


  // Serial.println("Transmitting..."); // Send a message to rf95_server

  // //itoa(packetnum++, radiopacket+13, 10);
  // Serial.print("Sending "); 
  // Serial.println(radiopacket);
  // //radiopacket[99] = 0;

  // Serial.println("Sending...");
  // delay(10);
  // rf95.send((uint8_t *)radiopacket, 100);

  // Serial.println("Waiting for packet to complete...");
  // delay(10);
  // rf95.waitPacketSent();
  // // Now wait for a reply
  // uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  // uint8_t len = sizeof(buf);

  // Serial.println("Waiting for reply...");
  // if (rf95.waitAvailableTimeout(1000)) {
  //   // Should be a reply message for us now
  //   if (rf95.recv(buf, &len)) {
  //     Serial.print("Got reply: ");
  //     Serial.println((char*)buf);
  //     Serial.print("RSSI: ");
  //     Serial.println(rf95.lastRssi(), DEC);
  //   } else {
  //     Serial.println("Receive failed");
  //   }
  // } else {
  //   Serial.println("No reply, is there a listener around?");
  // }
  delay(5000);
}

float hypsometric_altitude(float temp, float pressure){
  float altitude = (temp / LAPSE_RATE) * (pow(SEALEVELPRESSURE_HPA / pressure, GAS_CONSTANT * LAPSE_RATE / GRAVITY) - 1);
  return altitude;
}

