#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// DHT11 Settings
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define TRIG_FRONT 9
#define ECHO_FRONT 12
#define TRIG_LEFT A3
#define ECHO_LEFT 13
#define TRIG_RIGHT A1
#define ECHO_RIGHT A2

// Buzzer
#define BUZZER_PIN 4

// MQ-135 Settings
#define MQ135_AOUT A0
#define MQ135_DOUT 3

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Motor Driver (HW95) Pins
#define MOTOR_IN1 5
#define MOTOR_IN2 6
#define MOTOR_IN3 7
#define MOTOR_IN4 8
#define ENABLE1_PIN 10
#define ENABLE2_PIN 11

unsigned long lastDisplayUpdate = 0;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  dht.begin();

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(MQ135_DOUT, INPUT);

  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(MOTOR_IN3, OUTPUT);
  pinMode(MOTOR_IN4, OUTPUT);
  pinMode(ENABLE1_PIN, OUTPUT);
  pinMode(ENABLE2_PIN, OUTPUT);

  pinMode(TRIG_FRONT, OUTPUT);
  pinMode(ECHO_FRONT, INPUT);
  pinMode(TRIG_LEFT, OUTPUT);
  pinMode(ECHO_LEFT, INPUT);
  pinMode(TRIG_RIGHT, OUTPUT);
  pinMode(ECHO_RIGHT, INPUT);

  Serial.println("Sistem başlatıldı.");
}

long readDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 30000);
  if (duration == 0) return 500;
  return duration * 0.034 / 2;
}

void moveForward() {
  
  digitalWrite(ENABLE1_PIN, 125);
  digitalWrite(MOTOR_IN1, HIGH);
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(ENABLE2_PIN, 100);
  digitalWrite(MOTOR_IN3, HIGH);
  digitalWrite(MOTOR_IN4, LOW);
}

void turnRight() {
  digitalWrite(ENABLE1_PIN, 100);
  digitalWrite(MOTOR_IN1, HIGH);
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(ENABLE2_PIN, 100);
  digitalWrite(MOTOR_IN3, LOW);
  digitalWrite(MOTOR_IN4, HIGH);
  delay(500);
  stopMotors();
}

void turnLeft() {
  digitalWrite(ENABLE1_PIN, 100);
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, HIGH);
  digitalWrite(ENABLE2_PIN, 100);
  digitalWrite(MOTOR_IN3, HIGH);
  digitalWrite(MOTOR_IN4, LOW);
  delay(500);
  stopMotors();
}

void stopMotors() {
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_IN3, LOW);
  digitalWrite(MOTOR_IN4, LOW);
  digitalWrite(ENABLE2_PIN, LOW);
  digitalWrite(ENABLE2_PIN, LOW);
}

void loop() {
  float temperature = dht.readTemperature();
  delay(100);
  float humidity = dht.readHumidity();
  int mq135_analog = analogRead(MQ135_AOUT);
  bool alert = (temperature > 35.0) || (mq135_analog > 500);
  digitalWrite(BUZZER_PIN, alert ? HIGH : LOW);

  long distanceFront = readDistance(TRIG_FRONT, ECHO_FRONT);
  long distanceLeft = readDistance(TRIG_LEFT, ECHO_LEFT);
  long distanceRight = readDistance(TRIG_RIGHT, ECHO_RIGHT);

  Serial.print("Front: "); Serial.print(distanceFront);
  Serial.print(" | Left: "); Serial.print(distanceLeft);
  Serial.print(" | Right: "); Serial.println(distanceRight);

  if (distanceFront < 15) {
  if (distanceRight > 15 && distanceLeft > 15) {
    // İki yan da açık, hangisi daha uzaksa oraya dön
    if (distanceRight > distanceLeft) {
      turnRight();
    } else {
      turnLeft();
    }
    } else if (distanceRight > 15) {
      turnRight();
    } else if (distanceLeft > 15) {
      turnLeft();
    } else {
      // İki yan da kapalı, hangisi daha uzaksa oraya dön
      if (distanceRight >= distanceLeft) {
        turnRight();
      } else {//sol sağa için silinebilir
        turnLeft();
      }
    }
  } else {
    if (distanceRight < 10 && distanceLeft >= 10) {
    // Sağ duvara çok yakın, sola yönel
    turnLeft();
  } else if (distanceLeft < 10 && distanceRight >= 10) {
    // Sol duvara çok yakın, sağa yönel
    turnRight();
  
  } else {
    // Hiçbir yan tehdit yok, düz git
    moveForward();
  }
  }

  if (millis() - lastDisplayUpdate >= 1000) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(temperature, 1);
    lcd.print("C H:");
    lcd.print(humidity, 1);
    lcd.print("%");

    lcd.setCursor(0, 1);
    lcd.print("MQ:");
    lcd.print(mq135_analog);
    lcd.print(" L:");
    lcd.print(distanceLeft);
    lcd.print(" R:");
    lcd.print(distanceRight);

    lastDisplayUpdate = millis();

    Serial.print("Sıcaklık: ");
    Serial.print(temperature);
    Serial.print(" C | Nem: ");
    Serial.print(humidity);
    Serial.print(" % | MQ135: ");
    Serial.println(mq135_analog);
  }

  delay(100);
}
