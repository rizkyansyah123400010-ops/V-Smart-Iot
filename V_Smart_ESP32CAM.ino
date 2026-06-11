#define BLYNK_TEMPLATE_ID "TMPL6wy4noucN"
#define BLYNK_TEMPLATE_NAME "BYLINK VSMART"
#define BLYNK_AUTH_TOKEN "YCd3l6_61HnIPRN7UuGRKI3i7qKvPt9I"

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <BlynkSimpleEsp32.h>
#include <UniversalTelegramBot.h>

// =============================
// WIFI
// =============================
char ssid[] = "hampura";
char pass[] = "kiduy111";

// =============================
// TELEGRAM
// =============================
String BOT_TOKEN = "8933599490:AAFplor64R7x5g96SSWSiEzOQscFemq1hbk";
String CHAT_ID   = "8518980895";

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

// =============================
// UART UNO
// =============================
HardwareSerial UnoSerial(1);

// =============================
// DATA (SUDAH DALAM dB DARI ARDUINO)
// =============================
float noiseValue = 0;
float speedValue = 0;
String statusValue = "AMAN";

// =============================
// CONTROL
// =============================
bool lastViolation = false;
unsigned long lastBlynkSend = 0;
unsigned long lastTelegramSend = 0;

// =============================
// SETUP
// =============================
void setup()
{
  Serial.begin(115200);

  UnoSerial.begin(9600, SERIAL_8N1, 13, -1);

  Serial.println("\n=== V-SMART START ===");

  WiFi.begin(ssid, pass);
  Serial.print("Connecting WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(300);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");

  client.setInsecure();

  Blynk.config(BLYNK_AUTH_TOKEN);

  Serial.print("Connecting Blynk");
  while (!Blynk.connect(3000))
  {
    Serial.print(".");
  }

  Serial.println("\nBlynk Connected");
}

// =============================
// LOOP
// =============================
void loop()
{
  Blynk.run();

  // =============================
  // UART PARSER
  // =============================
  static String buffer = "";

  while (UnoSerial.available())
  {
    char c = UnoSerial.read();

    if (c == '\n')
    {
      buffer.trim();

      if (buffer.length() > 0)
      {
        int p1 = buffer.indexOf(',');
        int p2 = buffer.indexOf(',', p1 + 1);

        if (p1 != -1 && p2 != -1)
        {
          // =============================
          // DATA SUDAH dB DARI ARDUINO
          // =============================
          noiseValue = buffer.substring(0, p1).toFloat();
          speedValue = buffer.substring(p1 + 1, p2).toFloat();
          statusValue = buffer.substring(p2 + 1);
          statusValue.trim();

          // DEBUG
          Serial.print("Noise dB: ");
          Serial.print(noiseValue);
          Serial.print(" | Speed: ");
          Serial.print(speedValue);
          Serial.print(" | ");
          Serial.println(statusValue);

          // =============================
          // TELEGRAM (COOLDOWN 2 DETIK)
          // =============================
          bool violation = (statusValue == "PELAGARAN");

          if (violation && !lastViolation && millis() - lastTelegramSend > 2000)
          {
            String msg = "🚨 V-SMART ALERT\n";
            msg += "Status: PELANGGARAN\n";
            msg += "Noise: " + String(noiseValue) + " dB\n";
            msg += "Speed: " + String(speedValue);

            bot.sendMessage(CHAT_ID, msg, "");

            lastTelegramSend = millis();
          }

          lastViolation = violation;
        }
      }

      buffer = "";
    }
    else
    {
      buffer += c;
    }
  }

  // =============================
  // BLYNK (500ms STABIL)
  // =============================
  if (millis() - lastBlynkSend >= 100)
  {
    lastBlynkSend = millis();

    if (Blynk.connected())
    {
      Blynk.virtualWrite(V0, speedValue);
      Blynk.virtualWrite(V1, noiseValue);
      Blynk.virtualWrite(V2, statusValue);
    }
    else
    {
      Blynk.connect(1000);
    }
  }
}