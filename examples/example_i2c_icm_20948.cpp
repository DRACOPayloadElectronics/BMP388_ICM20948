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
    // Wire.setClock(400000); // Uncomment to set a preffered clock value

    // ICM Initialization

    // Check specification of the board to validate the AD0 pin and value behavior
    bool AD0_VAL = 1;
    ICM.begin(Wire,AD0_VAL);

    while (ICM.status != ICM_20948_Stat_Ok) {
        Serial.println("Failed to connect with ICM_20948");
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
        ICM.getAGMT(); // updates readings

        ICM.accX(); // accelerometer value for x axis in milli g
    }
}

// Key note measurements should be returned in milli g (1g ~= 9.8 m/s^2), therefore we could use unit conversion.
// Example:

/// @brief 
/// @param val_g -> acceleration value in milli g
/// @return val_ms -> acceleration value in m/s^2
float getAccelInMS(float val_g){
    return val_g * 0.00980665;
}