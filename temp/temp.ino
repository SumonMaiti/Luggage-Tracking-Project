#include <TinyGPS++.h>
#include <HardwareSerial.h>

TinyGPSPlus gps;
HardwareSerial GPSserial(2); // Use UART2

void setup()
{
  Serial.begin(115200);                      // For Serial Monitor
  GPSserial.begin(9600, SERIAL_8N1, 16, 17); // RX=16, TX=17
  Serial.println("🔍 GPS Module Test Started");
}

void loop()
{
  static unsigned long lastPrint = 0;

  while (GPSserial.available())
  {
    gps.encode(GPSserial.read());
  }

  if (gps.location.isUpdated())
  {
    Serial.print("📍 Latitude: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("📍 Longitude: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("🕒 Time: ");
    Serial.print(gps.time.hour());
    Serial.print(":");
    Serial.print(gps.time.minute());
    Serial.print(":");
    Serial.println(gps.time.second());
    Serial.println("----------");
  }

  // Debug heartbeat every 5s
  if (millis() - lastPrint > 5000)
  {
    Serial.println("⏳ Waiting for GPS...");
    lastPrint = millis();
  }
}
