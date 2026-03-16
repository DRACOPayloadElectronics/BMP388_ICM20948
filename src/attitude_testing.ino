#include <Servo.h>
#include <ICM_20948.h>
#include <Wire.h>
#include <SD.h>
#include <math.h>

// Setting up the ICM for I2C
ICM_20948_I2C ICM;
bool AD0_VAL = 1;

Servo esc;
// const int ESC_PIN = 9;
// const int MIN_US  = 1000;

// this states will be used to identify the logic that will be loaded
enum State {
  PAD,
  FLIGHT,
  LANDED
};

int currentState;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // icm setup
  ICM.begin(Wire, AD0_VAL);

  while (ICM.status != ICM_20948_Stat_Ok) {
    Serial.println("Failed to connect with ICM_20948");
    delay(500);
  }
  Serial.println("Connection Succesful with ICM_20948");

  // Setting the sensitivity for the accelerometer and the gyroscope
  ICM_20948_fss_t FSS;

  FSS.a = gpm2;

  FSS.g = dps250;

  // Magnometer configuration -> set to true if magnometer is going to be utilized
  ICM.startupMagnetometer(false);

  // esc setup
  esc.attach(9);
  delay(1);
  esc.writeMicroseconds(1500);   // arm at minimum
  delay(5000);

  currentState = PAD;
}

void loop() 
{
  switch (currentState) {
    case PAD:
      // handle pad logic
      detectLaunch();
      break;

    case FLIGHT:
      // handle flight logic 
      Serial.println("flight state detected");
      detectLanding();
      break;

    case LANDED: {
      int orientation = payloadOrientation();
      handleEsc(orientation);
      Serial.println("landed state detected");
      break;
    }

    default:
      Serial.println("invalid state entered");
      // failsafe code if state is invalid
  }
}

enum Orientation {
  UPRIGHT,
  INVERTED,
  UNKNOWN
};

int payloadOrientation() {

    if (ICM.dataReady()) {
      ICM.getAGMT();

      // acceleration in the Z direction which will be utilized to compute an estimated attitude 
      // upright or inverted
      int16_t accZ = ICM.accZ(); // milli g's
      
      // gyroscope values
      int16_t gyrZ = ICM.gyrZ();
      int16_t gyrX = ICM.gyrX();
      int16_t gyrY = ICM.gyrY();

      // deciding attitude depending on Z axis acceleration
      // attitude detection should start after the gyro values have stabilized after landing or stabilized after a tilt could have been detected
      if (accZ > 300) { 
        return UPRIGHT;
      }
      else if (accZ < -300) { 
        return INVERTED; 
      }
      else { return UNKNOWN; }
  }
}

void handleEsc(int orientation) {
  switch (orientation) {
    case UPRIGHT:
      esc.writeMicroseconds(1400);
      break;
    case INVERTED:
      esc.writeMicroseconds(1600);
      break;
    default:
      Serial.println("payload entered invalid orientation");
      // handle failsafe code
  }
}

void detectLaunch() {
  if (ICM.dataReady()) {
    ICM.getAGMT();

    // accelerometer values
    int16_t accZ = ICM.accZ();
    int16_t accX = ICM.accX();
    int16_t accY = ICM.accY();

    // we get the resultant to identify a strong acceleration 
    float accResultant = sqrtf((accZ * accZ) + (accX * accX) + (accY * accY));

    // if the resultant is bigger than 1600 milli g or 1.6 g, we can be sure that we have entered the flight state
    if (accResultant > 1600) {
      currentState = FLIGHT;
    }
  }
}

void detectLanding() {
  if (ICM.dataReady()) {
    ICM.getAGMT();

    // accelerometer values
    int16_t accZ = ICM.accZ();
    int16_t accX = ICM.accX();
    int16_t accY = ICM.accY();

    // gyroscope values
    int16_t gyrZ = ICM.gyrZ();
    int16_t gyrX = ICM.gyrX();
    int16_t gyrY = ICM.gyrY();

    // detecting landing logic...

  }
}