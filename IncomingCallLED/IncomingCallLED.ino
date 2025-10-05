#include <HardwareSerial.h>

#define MODEM_RX 16 // SIM800L TX ➝ ESP32 RX
#define MODEM_TX 17 // SIM800L RX ➝ ESP32 TX (via voltage divider)
#define LED_PIN 2   // Built-in LED

HardwareSerial sim800(2); // UART2

void setup()
{
  Serial.begin(115200);
  sim800.begin(9600, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(1000);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println("📶 SIM800L Call Detection Started");

  sendAT("AT");
  sendAT("ATE0");
  sendAT("AT+CLIP=1");
}

void loop()
{
  String res = readSIM800Line();
  String cleaned = cleanString(res);

  if (cleaned.length() > 0)
  {
    Serial.println("Res: " + cleaned);

    if (cleaned == "RING")
    {
      Serial.println("📞 Incoming Call!");
      handleBlinkUntilCallEnds();
    }
  }

  // Always keep LED OFF outside incoming call
  digitalWrite(LED_PIN, LOW);
  delay(1000);
}

// === Blink LED until call ends ===
void handleBlinkUntilCallEnds()
{
  while (true)
  {
    digitalWrite(LED_PIN, HIGH);
    delay(300);
    digitalWrite(LED_PIN, LOW);
    delay(300);

    if (sim800.available())
    {
      String res = readSIM800Line();
      String cleaned = cleanString(res);

      if (cleaned.indexOf("NO CARRIER") >= 0)
      {
        Serial.println("📴 Call Ended");
        break;
      }
    }
  }

  digitalWrite(LED_PIN, LOW); // Ensure LED OFF after loop
}

// === Read line from SIM800 ===
String readSIM800Line()
{
  if (!sim800.available())
    return "";
  String res = sim800.readStringUntil('\n');
  res.trim();
  return res;
}

// === Send AT command ===
void sendAT(String cmd)
{
  Serial.println("Sending: " + cmd);
  sim800.println(cmd);
  delay(500);

  while (sim800.available())
  {
    String res = sim800.readStringUntil('\n');
    res.trim();
    if (res.length() > 0)
    {
      Serial.println("Reply: " + res);
    }
  }
}

// === Remove garbage from response ===
String cleanString(String input)
{
  String output = "";
  for (int i = 0; i < input.length(); i++)
  {
    if (isPrintable(input[i]))
    {
      output += input[i];
    }
  }
  return output;
}
