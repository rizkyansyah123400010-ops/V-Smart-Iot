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
// UART DARI ARDUINO UNO
// RX = GPIO13 ESP32-CAM
// UNO TX -> GPIO13
// =============================
HardwareSerial UnoSerial(1);

// =============================
// VARIABEL
// =============================
float noiseValue = 0;
float speedValue = 0;
String statusValue = "AMAN";

bool lastViolation = false;

unsigned long lastBlynkUpdate = 0;

// =============================
// SETUP
// =============================
void setup()
{
  Serial.begin(115200);

  // RX = GPIO13
  UnoSerial.begin(9600, SERIAL_8N1, 13, -1);

  Serial.println();
  Serial.println("V-SMART STARTING");

  WiFi.begin(ssid, pass);

  Serial.print("Connecting WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  client.setInsecure();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  Serial.println("Blynk Connected");
}

// =============================
// LOOP
// =============================
void loop()
{
  Blynk.run();

  // =========================
  // TERIMA DATA DARI UNO
  // =========================
  if (UnoSerial.available())
  {
    String data = UnoSerial.readStringUntil('\n');
    data.trim();

    int p1 = data.indexOf(',');
    int p2 = data.indexOf(',', p1 + 1);

    if (p1 > 0 && p2 > 0)
    {
      noiseValue = data.substring(0, p1).toFloat();
      speedValue = data.substring(p1 + 1, p2).toFloat();
      statusValue = data.substring(p2 + 1);

      Serial.println("======================");
      Serial.print("Noise  : ");
      Serial.println(noiseValue);

      Serial.print("Speed  : ");
      Serial.println(speedValue);

      Serial.print("Status : ");
      Serial.println(statusValue);

      bool violation =
        (statusValue == "PELANGGARAN");

      // =====================
      // TELEGRAM
      // =====================
      if (violation && !lastViolation)
      {
        String pesan;

        pesan += "🚨 V-SMART ALERT\n\n";
        pesan += "Status : PELANGGARAN\n";
        pesan += "Noise : ";
        pesan += String(noiseValue);
        pesan += "\n";

        pesan += "Speed : ";
        pesan += String(speedValue, 1);
        pesan += " km/jam";

        bot.sendMessage(CHAT_ID, pesan, "");
      }

      lastViolation = violation;
    }
  }

  // =========================
  // UPDATE BLYNK
  // =========================
  if (millis() - lastBlynkUpdate >= 1000)
  {
    Blynk.virtualWrite(V0, noiseValue);
    Blynk.virtualWrite(V1, speedValue);
    Blynk.virtualWrite(V2, statusValue);

    lastBlynkUpdate = millis();
  }
}