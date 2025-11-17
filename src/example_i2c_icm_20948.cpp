#include <ICM_20948.h>
#include <Wire.h>

// Setting up for I2C
ICM_20948_I2C ICM;

void setup() {
    // Serial Initialization
    Serial.begin(115200);
    // If serial fails, halt program
    while (!Serial) {}

    // I2C Initialization
    Wire.begin();
    Wire.setClock(400000);

    // ICM Initialization

    // Check specification of the board to validate the AD0 pin and value behavior
    bool AD0_VAL = 1;
    ICM.begin(Wire,AD0_VAL);

    while (ICM.status != ICM_20948_Stat_Ok) {
        Serial.println("Failed to connect with ICM_20948");
        Serial.println("Trying again...");
        delay(500);
    }
    Serial.println("Connection Succesful");

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
    }
}