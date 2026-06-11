#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

#define MIC_PIN A0
#define HB_PIN  2

SoftwareSerial espSerial(10, 11);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ===============================
// THRESHOLD
// ===============================
const float noiseThreshold = 70.0;
const float speedThreshold = 30.0;

// ===============================
// SPEED SENSOR
// ===============================
volatile unsigned long pulseCount = 0;
float speed_kmh = 0;
unsigned long lastSpeedTime = 0;

void countPulse()
{
  pulseCount++;
}

// ===============================
// SETUP
// ===============================
void setup()
{
  Serial.begin(9600);
  espSerial.begin(9600);

  pinMode(HB_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(HB_PIN), countPulse, RISING);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("V-SMART");
  lcd.setCursor(0,1);
  lcd.print("Starting...");
  delay(1500);
  lcd.clear();
}

// ===============================
// LOOP
// ===============================
void loop()
{
  // ===============================
  // SOUND READING
  // ===============================
  int signalMax = 0;
  int signalMin = 1023;

  unsigned long start = millis();

  while (millis() - start < 100)
  {
    int v = analogRead(MIC_PIN);

    if (v > signalMax) signalMax = v;
    if (v < signalMin) signalMin = v;
  }

  int amplitude = signalMax - signalMin;

  // ===============================
  // SMOOTHING (BIAR STABIL)
  // ===============================
  static float noise = 0;
  noise = (noise * 0.75) + (amplitude * 0.25);

  // ===============================
  // SIMPLE SCALE (LEBIH RESPONSIF)
  // ===============================
  float dB = 25 + (noise * 0.15);

  // batas aman
  if (dB < 30) dB = 30;
  if (dB > 110) dB = 110;

  bool noiseViolation = (dB >= noiseThreshold);

  // ===============================
  // SPEED CALC
  // ===============================
  if (millis() - lastSpeedTime >= 1000)
  {
    noInterrupts();
    unsigned long pulses = pulseCount;
    pulseCount = 0;
    interrupts();

    speed_kmh = pulses * 0.051;
    lastSpeedTime = millis();
  }

  bool speedViolation = (speed_kmh >= speedThreshold);

  // ===============================
  // STATUS
  // ===============================
  bool violation = noiseViolation || speedViolation;
  String statusText = violation ? "PELAGARAN" : "AMAN";

  // ===============================
  // SERIAL MONITOR
  // ===============================
  Serial.print("Noise=");
  Serial.print(dB,0);
  Serial.print(" dB | Speed=");
  Serial.print(speed_kmh,1);
  Serial.print(" | ");
  Serial.println(statusText);

  // ===============================
  // SEND TO ESP32
  // ===============================
  espSerial.print(dB,0);
  espSerial.print(",");
  espSerial.print(speed_kmh,1);
  espSerial.print(",");
  espSerial.println(statusText);

  // ===============================
  // LCD DISPLAY
  // ===============================
  lcd.setCursor(0,0);
  lcd.print("N:");
  lcd.print(dB,0);
  lcd.print("dB   ");

  lcd.setCursor(8,0);
  lcd.print("S:");
  lcd.print(speed_kmh,1);
  lcd.print("   ");

  lcd.setCursor(0,1);
  lcd.print(statusText);
  lcd.print("      ");

  delay(150);
}