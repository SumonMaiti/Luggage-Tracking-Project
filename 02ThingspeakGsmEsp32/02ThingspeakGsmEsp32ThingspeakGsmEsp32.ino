#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClient.h>

// 🔧 SIM800L Settings (GPIO for ESP32)
#define MODEM_RX 16
#define MODEM_TX 17
HardwareSerial sim800(1);
TinyGsm modem(sim800);
TinyGsmClient client(modem);

// 🌐 ThingSpeak Settings
const char server[] = "api.thingspeak.com";
const int port = 80;
const String apiKey = "16FCSO4YK6LKRO3G"; // 🔑 Replace with your actual Write API key

// 📶 BSNL GPRS Settings
const char apn[] = "bsnlnet";
const char user[] = "";
const char pass[] = "";

void setup()
{
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

  Serial.println("📡 Waiting for network...");
  if (!modem.waitForNetwork(30000))
  {
    Serial.println("❌ Network not found. Halting...");
    while (true)
      ;
  }
  Serial.println("✅ Network connected");

  Serial.println("🌐 Connecting to GPRS...");
  if (!modem.gprsConnect(apn, user, pass))
  {
    Serial.println("❌ GPRS connection failed. Halting...");
    while (true)
      ;
  }
  Serial.println("✅ GPRS connected");

  sendDataToThingSpeak(89.0, 92.0); // 🟢 You can pass any values here
}

void loop()
{
  // Optional: call sendDataToThingSpeak() repeatedly with delay if required
}

// 📤 Send Data Function
void sendDataToThingSpeak(float field1, float field2)
{
  Serial.println("📤 Sending data to ThingSpeak...");

  String url = "/update?api_key=" + apiKey +
               "&field1=" + String(field1) +
               "&field2=" + String(field2);

  int attempts = 0;
  bool connected = false;

  while (!connected && attempts < 3)
  {
    Serial.print("🌐 Connecting to ");
    Serial.print(server);
    Serial.print(":");
    Serial.println(port);

    connected = client.connect(server, port);
    if (!connected)
    {
      Serial.println("❌ Connection failed. Retrying...");
      delay(2000);
      attempts++;
    }
  }

  if (!connected)
  {
    Serial.println("❌ Could not connect to ThingSpeak.");
    return;
  }

  // 📨 Send HTTP GET Request
  client.print("GET " + url + " HTTP/1.1\r\n");
  client.print("Host: ");
  client.println(server);
  client.println("User-Agent: ESP32-SIM800L");
  client.println("Connection: close");
  client.println(); // 👈 Don't forget this blank line

  // 📥 Read Response
  unsigned long timeout = millis();
  while (client.connected() && millis() - timeout < 5000)
  {
    while (client.available())
    {
      String line = client.readStringUntil('\n');
      Serial.println(line);
      timeout = millis(); // Reset timeout when data is received
    }
  }

  client.stop();
  Serial.println("✅ Data sent successfully!");
}
