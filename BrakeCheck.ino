#include "Arduino_BMI270_BMM150.h"

// --- SETTINGS ---
const float accelLimit_mps2 = 2.2352;              // limit in m/s^2
const float accelLimit_g = accelLimit_mps2 / 9.80665;  // convert to g

bool inBurst = false;   // true while acceleration is above the limit

int warningCount = 0;   //Warning count

void setup() {                                              //Verify the IMU works. Try a sample set of data.
  Serial.begin(9600);
  while (!Serial);

  Serial.println("Started");

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz\n");

  Serial.println("Acceleration in m/s^2 (X, Y, Z)");
}

char getGrade(int warningCount) {           //Set criteria for grades ABCDF. Turn into a usable char
  if (warningCount <= 1) return 'A';
  if (warningCount == 2) return 'B';
  if (warningCount == 3) return 'C';
  if (warningCount <= 5) return 'D';
  return 'F';   // 6 or more
}

void loop() {
  float x, y, z;

  if (IMU.accelerationAvailable()) {      //read accelerometer data, add a delay as to remove bursts of warnings.
    IMU.readAcceleration(x, y, z);

    Serial.print(x, 4);
    Serial.print('\t');
    Serial.print(y, 4);
    Serial.print('\t');
    Serial.print(z, 4);
    delay(500);

    float magnitude = sqrt(x * x + y * y + z * z);    //function for magnitude of acceleration

    if (magnitude > accelLimit_mps2) {                  //If the magnitude of acceleration is over the limit:
      warningCount++;                                   //Increase warning count
      Serial.print("\t <-- WARNING! Limit exceeded ("); //Print warning message, total warnings, and the grade associated.
      Serial.print(warningCount);
      Serial.print(" total)");// We just crossed ABOVE the limit → trigger once
      Serial.print(" Current grade: ");
      Serial.println(getGrade(warningCount));
  }
}

    Serial.println();
}

