#define BLYNK_TEMPLATE_ID "TMPL3BpZKCI0d"
#define BLYNK_TEMPLATE_NAME "Distance Monitor"
#define BLYNK_AUTH_TOKEN "q-6KHbsS4BF6F5LHfMjgGH2bcJ0JkE-0"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Mobile hotspot credentials
char ssid[] = "Soumojit Shome";
char pass[] = "soumojit9062300@";

// Buzzer pin (D4 on ESP32)
#define BUZZER_PIN 4

BlynkTimer timer;

void sendDistanceToBlynk() {
  long rssi = WiFi.RSSI();  // Signal strength in dBm

  // Path-loss model formula (n = 2.0 is environment factor)
  float distance = pow(10.0, ((-69 - rssi) / (10 * 2.0)));

  Serial.print("RSSI: ");
  Serial.print(rssi);
  Serial.print(" dBm | Approx Distance: ");
  Serial.print(distance, 4);
  Serial.println(" meters");

  // Send RSSI & distance to Blynk
  Blynk.virtualWrite(V0, rssi);
  Blynk.virtualWrite(V1, distance);

  // Buzzer logic
  if (distance > 1) { // Less than or equal to 1 meter
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }
}

// Manual buzzer control from Blynk button (V2)
BLYNK_WRITE(V2) {
  int value = param.asInt(); // 0 = OFF, 1 = ON
  digitalWrite(BUZZER_PIN, value);
}

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  // Connect to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Send every 1 second
  timer.setInterval(1000L, sendDistanceToBlynk);
}

void loop() {
  Blynk.run();
  timer.run();
}
