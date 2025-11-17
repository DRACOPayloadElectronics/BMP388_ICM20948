#include <Arduino.h>
#include <ICM_20948.h>
#include <Wire.h>

// CS can be defined or open for assignment
// Defining CS Pin
#define CS_PIN 10

// SPI Connection
ICM_20948_SPI ICM;

void setup() {
    Serial.begin(115200);
    while (!Serial){}

    SPI.begin();

    ICM.begin(CS_PIN, SPI);

    while (!ICM.status != ICM_20948_Stat_Ok) {
        Serial.println("Attempting connection again");
        delay(500);
    }
    Serial.println("ICM_20948 Connection Succesful");

    // Setting the sensitivity for the accelerometer and the gyroscope
    ICM_20948_fss_t FSS;

    FSS.a = gpm2;

    FSS.g = dps250;

    // Magnometer configuration -> set to true if magnometer is going to be utilized
    ICM.startupMagnetometer(false);
}

void loop() {
    if (ICM.dataReady()){
        // collect data once the sensor is ready
        ICM.getAGMT();
    }
}

float getAcceleration(float rawValue){
    
}