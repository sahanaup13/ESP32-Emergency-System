#include <Wire.h>
#include <SPI.h>

// ==================== PIN DEFINITIONS ====================
#define POT_MIC_PIN   1     // Simulates INMP441 Mic level
#define BUTTON_PIN    3     // Emergency Push Button
#define LED_GREEN     4     // Main Status / Alert LED
#define BUZZER_PIN    5     // Alert Buzzer

#define MPU6050_ADDR  0x68

// Mock Coordinates for Simulation
float currentLat = 14.5121;
float currentLon = 75.8012;

const int16_t ACCEL_IMPACT_THRESHOLD = 25000;
const int     MIC_NOISE_THRESHOLD   = 3000;

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Initialize I2C for MPU6050
  Wire.begin();
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  digitalWrite(LED_GREEN, HIGH);
  Serial.println("=========================================");
  Serial.println("   ESP32-S3 SYSTEM INITIALIZATION OK     ");
  Serial.println("=========================================");
}

void transmitVirtualLoRa(int16_t accX, int16_t accY, int micVal, const char* triggerSource) {
  String payload = "ALERT! Trigger: " + String(triggerSource) +
                   " | GPS: " + String(currentLat, 4) + "," + String(currentLon, 4) +
                   " | AccX: " + String(accX) + " | AccY: " + String(accY) +
                   " | Mic: " + String(micVal);

  Serial.println("[LORA TRANSMITTING] -> " + payload);
}

void loop() {
  // 1. Read Accelerometer
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_ADDR, 6, true);

  int16_t accX = Wire.read() << 8 | Wire.read();
  int16_t accY = Wire.read() << 8 | Wire.read();
  int16_t accZ = Wire.read() << 8 | Wire.read();

  // 2. Read Potentiometer (Mic Simulation)
  int micLevel = analogRead(POT_MIC_PIN);

  // 3. Check Triggers
  bool buttonPressed = (digitalRead(BUTTON_PIN) == LOW);
  bool motionImpact   = (abs(accX) > ACCEL_IMPACT_THRESHOLD || abs(accY) > ACCEL_IMPACT_THRESHOLD);
  bool loudNoise      = (micLevel > MIC_NOISE_THRESHOLD);

  if (buttonPressed || motionImpact || loudNoise) {
    tone(BUZZER_PIN, 1200);

    // Blink Green LED to indicate Alert State
    for (int i = 0; i < 5; i++) {
      digitalWrite(LED_GREEN, LOW);
      delay(100);
      digitalWrite(LED_GREEN, HIGH);
      delay(100);
    }

    const char* source = buttonPressed ? "Panic Button" : (motionImpact ? "High Impact" : "Loud Sound");
    transmitVirtualLoRa(accX, accY, micLevel, source);
  } else {
    digitalWrite(LED_GREEN, HIGH); // Solid ON during normal operation
    noTone(BUZZER_PIN);
  }

  delay(100);
}
