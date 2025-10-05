#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_RX_BUFFER 256

#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>

const char FIREBASE_HOST[] = "luggage-tracking-c3485-default-rtdb.asia-southeast1.firebasedatabase.app";
const String FIREBASE_AUTH = "QeE1HOQ2VWRk0nI2mRnQNcDC6KftAJuPlQnasFAn";
const String FIREBASE_PATH = "/"; // root path
const int HTTP_PORT = 80; // HTTP instead of HTTPS

char apn[] = "bsnlnet";
char user[] = "";
char pass[] = "";

#define MODEM_RX 16
#define MODEM_TX 17
HardwareSerial sim800(1);
TinyGsm modem(sim800);
TinyGsmClient gsmClient(modem); // Non-secure client
HttpClient http(gsmClient, FIREBASE_HOST, HTTP_PORT);

unsigned long lastSendTime = 0;
const long interval = 10000;

void setup()
{
  Serial.begin(115200);
  sim800.begin(9600, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(3000);

  Serial.println("🔄 Restarting modem...");
  modem.restart();
  Serial.print("📟 Modem Info: ");
  Serial.println(modem.getModemInfo());

  Serial.print("📶 Connecting to network...");
  if (!modem.waitForNetwork())
  {
    Serial.println("❌ Network failed");
    while (true);
  }
  Serial.println("✅ Network connected");

  Serial.print("🌐 Connecting to GPRS...");
  if (!modem.gprsConnect(apn, user, pass))
  {
    Serial.println("❌ GPRS failed");
    while (true);
  }
  Serial.println("✅ GPRS connected");

  http.connectionKeepAlive();
}

void loop()
{
  unsigned long currentMillis = millis();
  if (currentMillis - lastSendTime >= interval)
  {
    lastSendTime = currentMillis;

    String data = "{\"lastOnline\":" + String(currentMillis) + "}";
    String url = FIREBASE_PATH + ".json?auth=" + FIREBASE_AUTH;

    Serial.println("📡 Sending data to Firebase...");
    Serial.println("🔗 URL: " + url);
    Serial.println("📦 Payload: " + data);

    http.beginRequest();
    http.patch(url);
    http.sendHeader("Content-Type", "application/json");
    http.sendHeader("Content-Length", data.length());
    http.beginBody();
    http.print(data);
    http.endRequest();

    int statusCode = http.responseStatusCode();
    String response = http.responseBody();

    Serial.print("📨 Status Code: ");
    Serial.println(statusCode);
    Serial.print("📥 Response: ");
    Serial.println(response);

    if (!http.connected())
    {
      Serial.println("🔌 HTTP disconnected, reconnecting...");
      http.stop();
      delay(2000);
    }

    Serial.println("✅ Done\n");
  }
}
