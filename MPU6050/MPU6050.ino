// ✅ Basic demo for MPU6050 with ESP32 using Adafruit library

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

void setup(void)
{
  Serial.begin(115200);
  delay(1000);

  Wire.begin(21, 22); // ESP32 I2C pins

  // 🔄 Manual wake-up in case the chip is sleeping
  Wire.beginTransmission(0x68);
  Wire.write(0x6B); // Power management register
  Wire.write(0);    // Wake up MPU6050
  Wire.endTransmission(true);

  Serial.println("🔍 Initializing MPU6050...");

  if (!mpu.begin(0x68))
  {
    Serial.println("❌ Failed to find MPU6050 chip. Check wiring.");
    while (1)
      delay(10);
  }
  Serial.println("✅ MPU6050 Found!");
}

void loop()
{
  // ✅ Get sensor events
  sensors_event_t accel, gyro, temp;
  mpu.getEvent(&accel, &gyro, &temp);

  // ✅ Print accelerometer values
  Serial.print("📈 Accel [m/s^2] => X: ");
  Serial.print(accel.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(accel.acceleration.y);
  Serial.print(", Z: ");
  Serial.println(accel.acceleration.z);

  // ✅ Print gyroscope values
  Serial.print("🎯 Gyro [rad/s] => X: ");
  Serial.print(gyro.gyro.x);
  Serial.print(", Y: ");
  Serial.print(gyro.gyro.y);
  Serial.print(", Z: ");
  Serial.println(gyro.gyro.z);

  // ✅ Print temperature
  Serial.print("🌡 Temp => ");
  Serial.print(temp.temperature);
  Serial.println(" °C");

  Serial.println("------------------------------");
  delay(500);
}

// =============================================================

/*
#include <Wire.h>

void setup() {
  Wire.begin(21, 22); // ESP32 SDA, SCL
  Serial.begin(115200);
  delay(1000);
  Serial.println("🔍 Scanning I2C bus...");

  byte count = 0;

  for (byte address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
      Serial.print("✅ I2C device found at 0x");
      Serial.println(address, HEX);
      count++;
      delay(10);
    }
  }

  if (count == 0) {
    Serial.println("❌ No I2C devices found. Check wiring!");
  } else {
    Serial.println("✅ I2C scan complete.");
  }
}

void loop() {}
*/