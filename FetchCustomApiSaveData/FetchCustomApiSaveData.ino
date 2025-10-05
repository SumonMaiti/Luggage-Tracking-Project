// ----------------------------------
// ✅ Required Libraries and Settings
// ----------------------------------
#define TINY_GSM_MODEM_SIM800
#define SerialMon Serial

#include <TinyGsmClient.h>

// ----------------------------------
// ✅ SIM800L Serial Pins (ESP32)
// ----------------------------------
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
// ✅ Device & Server Config
// ----------------------------------
const char deviceId[] = "9062300500";
const char host[] = "luggagetracking.vercel.app";
const int port = 80;

// ----------------------------------
// ✅ Setup
// ----------------------------------
void setup() {
  SerialMon.begin(115200);
  delay(100);
  SerialMon.println("\n🚀 ESP32 + SIM800L HTTP GET");

  sim800.begin(MODEM_BAUD, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(3000);

  SerialMon.println("📶 Restarting modem...");
  modem.restart();
  delay(1000);

  SerialMon.print("📟 Modem Info: ");
  SerialMon.println(modem.getModemInfo());

  SerialMon.print("📱 Waiting for network...");
  if (!modem.waitForNetwork()) {
    SerialMon.println("❌ No network");
    while (true);
  }
  SerialMon.println("✅ Network connected");

  SerialMon.print("🌐 Connecting to GPRS...");
  if (!modem.gprsConnect(apn, user, pass)) {
    SerialMon.println("❌ GPRS connection failed");
    while (true);
  }
  SerialMon.println("✅ GPRS connected");
}

// ----------------------------------
// ✅ Loop — Send every 10 sec
// ----------------------------------
void loop() {
  fetchAPI();
  delay(10000); // wait 10 seconds
}

// ----------------------------------
// ✅ HTTP GET Request
// ----------------------------------
void fetchAPI() {
  unsigned long currentTime = millis();
  char url[200];
  snprintf(url, sizeof(url),
           "/save?id=%s&fieldName=currentTimeWithMillis&fieldVal=%lu",
           deviceId, currentTime);

  SerialMon.print("🌍 Connecting to ");
  SerialMon.println(host);

  if (!client.connect(host, port)) {
    SerialMon.println("❌ Server connection failed");
    return;
  }

  // Send GET request
  client.print(String("GET ") + url + " HTTP/1.1\r\n");
  client.print(String("Host: ") + host + "\r\n");
  client.print("Connection: close\r\n\r\n");

  // Wait until server sends response
  while (!client.available());

  // Read all available bytes from response
  while (client.available()) {
    char c = client.read();
    SerialMon.write(c); // Optional: print response
  }

  client.stop();
  SerialMon.println("\n✅ Data sent");
}
