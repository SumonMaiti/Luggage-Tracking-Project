// ------------------------------
// ✅ GSM Modem Config
// ------------------------------
#define TINY_GSM_MODEM_SIM800   // Define GSM modem model first
#define SerialMon Serial        // Debug Serial
#define MODEM_RX 16             // ESP32 RX <== SIM800L TX
#define MODEM_TX 17             // ESP32 TX ==> SIM800L RX
#define MODEM_BAUD 9600         // SIM800L default baud rate

#include <TinyGsmClient.h>

// ------------------------------
// ✅ SIM Credentials
// ------------------------------
const char apn[]  = "bsnlnet";  // APN for BSNL SIM
const char user[] = "";         // No username for BSNL
const char pass[] = "";         // No password for BSNL

// ------------------------------
// ✅ Hardware Serial for SIM800L
// ------------------------------
HardwareSerial SerialAT(1);       // UART1
TinyGsm modem(SerialAT);          // Modem instance
TinyGsmClient client(modem);      // Client for TCP/HTTP

void setup() {
  // Start serial monitor
  SerialMon.begin(115200);
  delay(10);
  SerialMon.println();
  SerialMon.println("⏳ Starting SIM800L Internet Test...");

  // Start serial for SIM800L
  SerialAT.begin(MODEM_BAUD, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(3000);  // Give modem time to boot

  SerialMon.println("📶 Checking modem...");
  if (!modem.testAT()) {
    SerialMon.println("❌ Modem not responding");
    SerialMon.flush();
    while (true);  // Halt
  }
  SerialMon.println("✅ Modem is OK");

  modem.restart();
  String modemInfo = modem.getModemInfo();
  SerialMon.print("📟 Modem Info: ");
  SerialMon.println(modemInfo);

  // Check signal strength
  int csq = modem.getSignalQuality();
  SerialMon.print("📶 Signal Strength (CSQ): ");
  SerialMon.println(csq);
  if (csq <= 10) {
    SerialMon.println("⚠️ Weak signal! Try moving to window or check antenna.");
  }

  // Wait for network registration
  SerialMon.println("📡 Connecting to cellular network...");
  if (!modem.waitForNetwork(30000)) {
    SerialMon.println("❌ Network registration failed");
    SerialMon.flush();
    while (true);  // Halt
  }
  SerialMon.println("✅ Network registered");

  // Connect to GPRS (internet)
  SerialMon.println("🌐 Connecting to GPRS...");
  if (!modem.gprsConnect(apn, user, pass)) {
    SerialMon.println("❌ GPRS connection failed");
    SerialMon.flush();
    while (true);  // Halt
  }
  SerialMon.println("✅ GPRS connected!");

  // Test internet connection by opening TCP socket to Google DNS
  SerialMon.println("🌍 Connecting to 8.8.8.8:80 ...");
  if (!client.connect("8.8.8.8", 80)) {
    SerialMon.println("❌ TCP connection failed");
  } else {
    SerialMon.println("✅ Internet OK! SIM800L is online 🎉");
    client.stop();  // Close TCP
  }

  modem.gprsDisconnect();
  SerialMon.println("🔌 Disconnected from GPRS");
}

void loop() {
  // Nothing in loop
}
