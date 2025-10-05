#define BLYNK_TEMPLATE_ID "TMPL3vvuCh4AM"
#define BLYNK_TEMPLATE_NAME "Test"
#define BLYNK_AUTH_TOKEN "cUyc1Qh-H9bgtD3oTXqMzxsz425Yo5t1"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Replace with your actual WiFi credentials
char ssid[] = "Soumojit Shome";
char pass[] = "soumojit9062300@";

// Built-in LED on ESP32 (usually GPIO 2)
const int ledPin = 2;

BLYNK_WRITE(V0)
{
  int pinValue = param.asInt(); // get value from app
  digitalWrite(ledPin, pinValue);
}

void setup()
{
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop()
{
  Blynk.run();
}
