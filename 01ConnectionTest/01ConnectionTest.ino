#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClient.h>

#define MODEM_RX 16
#define MODEM_TX 17
HardwareSerial sim800(1);
TinyGsm modem(sim800);

char apn[] = "bsnlnet"; // Replace if needed

void setup() {
  Serial.begin(115200);
  delay(10);
  sim800.begin(9600, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(3000);

  Serial.println("🔄 Restarting modem...");
  modem.restart();

  Serial.print("🔎 SIM status: ");
  Serial.println(modem.getSimStatus());

  Serial.print("📶 Signal Quality: ");
  Serial.println(modem.getSignalQuality());

  Serial.print("📡 Waiting for network...");
  if (!modem.waitForNetwork(30000)) {
    Serial.println("❌ Network connection failed");
    return;
  }
  Serial.println("✅ Network connected");

  Serial.print("🌐 Connecting to GPRS...");
  if (!modem.gprsConnect(apn)) {
    Serial.println("❌ GPRS failed");
  } else {
    Serial.println("✅ GPRS connected");
  }
}

void loop() {
  // nothing
}
