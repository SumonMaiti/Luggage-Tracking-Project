// ----------------------------------
// ✅ Required Libraries and Settings
// ----------------------------------
#define TINY_GSM_MODEM_SIM800
#define SerialMon Serial

#include <TinyGsmClient.h>

// ----------------------------------
// ✅ SIM800L Serial Pins (ESP32)
// ----------------------------------
// SIM800L RX → ESP32 TX (GPIO17)
// SIM800L TX → ESP32 RX (GPIO16)
#define MODEM_RX 16
#define MODEM_TX 17
#define MODEM_BAUD 9600

// ----------------------------------
// ✅ APN Settings for BSNL SIM
// ----------------------------------
const char apn[]  = "bsnlnet"; 
const char user[] = "";
const char pass[] = "";

// ----------------------------------
// ✅ Create Serial and GSM Clients
// ----------------------------------
HardwareSerial sim800(1);
TinyGsm modem(sim800);
TinyGsmClient client(modem);

// ----------------------------------
// ✅ Setup
// ----------------------------------
void setup() {
  SerialMon.begin(115200);
  delay(100);
  SerialMon.println("\n🚀 ESP32 + SIM800L HTTP GET");

  // Start SIM800L UART
  sim800.begin(MODEM_BAUD, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(3000);

  // Restart and check modem
  SerialMon.println("📶 Restarting modem...");
  modem.restart();
  delay(1000);

  SerialMon.print("📟 Modem Info: ");
  SerialMon.println(modem.getModemInfo());

  // Connect to network
  SerialMon.print("📱 Waiting for network...");
  if (!modem.waitForNetwork()) {
    SerialMon.println("❌ No network");
    while (true);
  }
  SerialMon.println("✅ Network connected");

  // Connect GPRS
  SerialMon.print("🌐 Connecting to GPRS...");
  if (!modem.gprsConnect(apn, user, pass)) {
    SerialMon.println("❌ GPRS connection failed");
    while (true);
  }
  SerialMon.println("✅ GPRS connected");

  // Perform HTTP GET
  fetchAPI();
}

// ----------------------------------
// ✅ Loop (not used)
// ----------------------------------
void loop() {
  // Nothing here
}

// ----------------------------------
// ✅ HTTP GET Function via SIM800L
// ----------------------------------
void fetchAPI() {
  const char server[] = "jsonplaceholder.typicode.com"; // Public test API (HTTP only)
  const int port = 80;
  const char resource[] = "/todos/1";

  SerialMon.print("🌍 Connecting to ");
  SerialMon.println(server);

  if (!client.connect(server, port)) {
    SerialMon.println("❌ Connection to server failed");
    return;
  }

  // Send GET request
  client.print(String("GET ") + resource + " HTTP/1.1\r\n");
  client.print(String("Host: ") + server + "\r\n");
  client.print("Connection: close\r\n\r\n");

  unsigned long timeout = millis();
  while (client.connected() && millis() - timeout < 10000) {
    while (client.available()) {
      char c = client.read();
      SerialMon.write(c);
    }
  }

  client.stop();
  SerialMon.println("\n✅ GET request complete");
}
