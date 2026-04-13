#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MS8607.h>
#include <Wire.h>
#include <SD.h>
#include <TimeLib.h>


// Pin for SD card
const int CS_PIN = 10; 
File myFile;

// Multiplexer for I2C devices
#define PCAADDR 0x70

// Create the 9dof sensor object
Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();
bool sensorPresent = false; 

// Create the MS8607 sensor object
Adafruit_MS8607 ms8607;

void pcaselect(uint8_t i) {
  if (i > 3) return;
 
  Wire.beginTransmission(PCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

void logData() {
  // 1. Attempt to open the file
  myFile = SD.open("LOG.csv", FILE_WRITE);

  // 2. If it fails, the card might have been reinserted; try to re-initialize
  if (!myFile) {
    Serial.println("File open failed, attempting to re-initialize SD...");
    if (SD.begin(CS_PIN)) { // Use your actual CS pin number
        myFile = SD.open("LOG.csv", FILE_WRITE);
    }
  }
}

void setup() {
  pinMode(CS_PIN, OUTPUT); // SD card CS pin
  Serial.begin(9600);
  Wire.begin();
  delay(3000);

  Serial.println("\nPCAScanner ready!");
    
  // Scan for I2C devices on all 4 ports of the PCA9548A multiplexer
  for (uint8_t t=0; t<4; t++) {
    pcaselect(t);
    Serial.print("PCA Port #"); Serial.println(t);

    for (uint8_t addr = 0; addr<=127; addr++) {
      if (addr == PCAADDR) continue;

      Wire.beginTransmission(addr);
      if (!Wire.endTransmission()) {
        Serial.print("Found I2C 0x");  Serial.println(addr,HEX);
      } 
    }
  }
  Serial.println("\ndone");
  
  pcaselect(1); // Select the second port for the LSM9DS1
  sensorPresent = lsm.begin();
  if (!sensorPresent) {
    Serial.println("Warning: no LSM9DS1 detected. Continuing without sensor.");
  } else {
    Serial.println("Found LSM9DS1 9DOF");
  }

  // 1.) Set the accelerometer range
  // lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_2G);
  //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_4G);
  //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_8G);
  lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_16G);
  
  // 2.) Set the magnetometer sensitivity
  lsm.setupMag(lsm.LSM9DS1_MAGGAIN_4GAUSS);
  //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_8GAUSS);
  //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_12GAUSS);
  //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_16GAUSS);

  // 3.) Setup the gyroscope
  lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_245DPS);
  //lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_500DPS);
  //lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_2000DPS);

  // select first port for the MS8607
  pcaselect(0);
  if (!ms8607.begin()) {
    Serial.println("Failed to find MS8607 chip");
    while (1) { delay(10); }
  }
  Serial.println("MS8607 Found!");

  ms8607.setHumidityResolution(MS8607_HUMIDITY_RESOLUTION_OSR_8b);
  Serial.print("Humidity resolution set to ");
  switch (ms8607.getHumidityResolution()){
    case MS8607_HUMIDITY_RESOLUTION_OSR_12b: Serial.println("12-bit"); break;
    case MS8607_HUMIDITY_RESOLUTION_OSR_11b: Serial.println("11-bit"); break;
    case MS8607_HUMIDITY_RESOLUTION_OSR_10b: Serial.println("10-bit"); break;
    case MS8607_HUMIDITY_RESOLUTION_OSR_8b: Serial.println("8-bit"); break;
  }
  ms8607.setPressureResolution(MS8607_PRESSURE_RESOLUTION_OSR_4096);
  Serial.print("Pressure and Temperature resolution set to ");
  switch (ms8607.getPressureResolution()){
    case MS8607_PRESSURE_RESOLUTION_OSR_256: Serial.println("256"); break;
    case MS8607_PRESSURE_RESOLUTION_OSR_512: Serial.println("512"); break;
    case MS8607_PRESSURE_RESOLUTION_OSR_1024: Serial.println("1024"); break;
    case MS8607_PRESSURE_RESOLUTION_OSR_2048: Serial.println("2048"); break;
    case MS8607_PRESSURE_RESOLUTION_OSR_4096: Serial.println("4096"); break;
    case MS8607_PRESSURE_RESOLUTION_OSR_8192: Serial.println("8192"); break;
  }
  Serial.println("");
 
  myFile = SD.open("LOG.csv", FILE_WRITE);
  while (!SD.begin(CS_PIN) || !myFile) {
    Serial.println("Initialization of SD card failed!");
    logData(); // Try to re-initialize SD card
    delay(1000);
  }
  Serial.println("initialization done.");
 
  myFile.println("Timestamp (mm:ss:ms), Accel X (m/s^2), Accel Y (m/s^2), Accel Z (m/s^2), Magnetic Field X (gauss), Magnetic Field Y (gauss), Magnetic Field Z (gauss), Gyroscope X (dps), Gyroscope Y (dps), Gyroscope Z (dps), Temperature (°C), Pressure (hPa), Humidity (%)");
  myFile.flush();
  myFile.close();
}

void loop() {

  // Read the LSM9DS1 (9dof) sensor data for acceleration, magnetometer, and gyroscope
  pcaselect(1); // Select the second port for the LSM9DS1
  // Read the sensor data
  lsm.read();  /* ask it to read in the data */

  /* Get a new sensor event */ 
  sensors_event_t a, m, g, tem;
  lsm.getEvent(&a, &m, &g, &tem); 

  // Print the data to the Serial Monitor
  // Serial.print("\nAccel X: "); Serial.print(a.acceleration.x); Serial.print(" m/s^2");
  // Serial.print("\tY: "); Serial.print(a.acceleration.y); Serial.print(" m/s^2");
  // Serial.print("\tZ: "); Serial.print(a.acceleration.z); Serial.print(" m/s^2");
  // Serial.print("\nMag X: "); Serial.print(m.magnetic.x);     Serial.print(" gauss");
  // Serial.print("\tY: "); Serial.print(m.magnetic.y);     Serial.print(" gauss");
  // Serial.print("\tZ: "); Serial.print(m.magnetic.z);     Serial.print(" gauss");
  // Serial.print("\nGyro X: "); Serial.print(g.gyro.x);         Serial.print(" dps");
  // Serial.print("\tY: "); Serial.print(g.gyro.y);         Serial.print(" dps");
  // Serial.print("\tZ: "); Serial.print(g.gyro.z);         Serial.print(" dps");
  // Serial.println();

  // Read the MS8607 sensor data for temperature, pressure and humidity
  pcaselect(0); // Select the first port for the MS8607
  sensors_event_t temp, pressure, humidity;
  ms8607.getEvent(&pressure, &temp, &humidity);
  // Serial.print("Temperature: ");Serial.print(temp.temperature); Serial.println(" degrees C");
  // Serial.print("Pressure: ");Serial.print(pressure.pressure); Serial.println(" hPa");
  // Serial.print("Humidity: ");Serial.print(humidity.relative_humidity); Serial.println(" %rH");
  // Serial.println("");

  //   // Save data to SD card
  long currentTime = millis();

  // if (currentTime - lastLogTime >= loggingInterval) {
  //   lastLogTime = currentTime;

    // Open the file and write the data
    myFile = SD.open("LOG.csv", FILE_WRITE);
    if (!myFile) {
      logData(); // Try to re-initialize SD card
      delay(1000);
    }
    if (myFile) {
      int mn = 0;
      int s = 0;
      int ms = 0;
      ms = currentTime % 1000;
      s = (currentTime / 1000) % 60;
      mn = (currentTime / 60000) % 60;
      myFile.print("'" + String(mn) + ":" + String(s) + ":" + String(ms)); myFile.print(","); // Write timestamp
      myFile.print(a.acceleration.x); myFile.print(","); // Write accel X
      myFile.print(a.acceleration.y); myFile.print(","); // Write accel Y
      myFile.print(a.acceleration.z); myFile.print(","); // Write accel Z
      myFile.print(m.magnetic.x); myFile.print(","); // Write mag X
      myFile.print(m.magnetic.y); myFile.print(","); // Write mag Y
      myFile.print(m.magnetic.z); myFile.print(","); // Write mag Z
      myFile.print(g.gyro.x); myFile.print(","); // Write gyro X
      myFile.print(g.gyro.y); myFile.print(","); // Write gyro Y
      myFile.print(g.gyro.z); myFile.print(","); // Write gyro Z
      myFile.print(temp.temperature); myFile.print(","); // Write temperature
      myFile.print(pressure.pressure); myFile.print(","); // Write pressure
      myFile.print(humidity.relative_humidity); myFile.println(); // Write humidity
      myFile.flush(); // Ensure data is written to the SD card
      myFile.close(); // Close the file immediately after writing
    }
    else {
      Serial.println("No SD card present.");
    }
}



