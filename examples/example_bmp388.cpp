#include <Adafruit_BMP3XX.h>
#include <Wire.h>

Adafruit_BMP3XX bmp;

void setup() {
    // Initializing Serial port
    Serial.begin(115200);
    while (!Serial){}

    // bmp assigns the I2C address and I2C device (wire) 
    if (!bmp.begin_I2C()) {
        Serial.println("Initialization of BMP388 not succesful");
        while (1) {} // Halt program
    }

    Serial.print("BMP388 succesfully initialized");

    // Configuration

    // Oversampling refers to how many reading the sensor averages before giving an ouput
    bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_4X); 
    bmp.setPressureOversampling(BMP3_OVERSAMPLING_8X);

    // Smooths sensor noise
    bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);

    // How many reading per second the sensor performs
    bmp.setOutputDataRate(BMP3_ODR_25_HZ);


}

void loop() {
   if (!bmp.performReading()) {
        Serial.print("Failed to read values from sensors");
        return; // halt program
   }
   // bmp.temperature // -> extracts the temperature value in Celcius
   // bmp.pressure   // -> extracts the pressure value in Pascals
}