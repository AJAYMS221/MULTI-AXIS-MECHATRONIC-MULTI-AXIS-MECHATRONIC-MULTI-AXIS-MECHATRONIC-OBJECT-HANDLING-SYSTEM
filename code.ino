#define BLYNK_TEMPLATE_ID "TMPL3F7LwHXhd"
#define BLYNK_TEMPLATE_NAME "Multiaxis pick and place"
#define BLYNK_AUTH_TOKEN "jLfsxkcpBsejbp6cAjrbnHSs-ciuJMhK"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>

// ==================== BLYNK SETUP ====================
char auth[] = "jLfsxkcpBsejbp6cAjrbnHSs-ciuJMhK";
char ssid[] = "--------";
char pass[] = "********";

// ==================== SERVO PINS (ESP32 SAFE) ====================
const int basePin  = 18;
const int elbowPin = 19;
const int wristPin = 21;
const int gripPin  = 22;

// ==================== SERVOS ====================
Servo baseServo;
Servo elbowServo;
Servo wristServo;
Servo gripServo;

// ==================== ANGLE PRESETS ====================
const int BASE_HOME  = 100;
const int BASE_PLACE = 60;

const int ELBOW_HOME = 20;
const int ELBOW_PICK = 65;
const int ELBOW_LIFT = 45;

const int WRIST_HOME  = 90;
const int WRIST_PICK  = 90;
const int WRIST_PLACE = 100;

const int GRIP_OPEN   = 90;
const int GRIP_CLOSED = 30;

// ==================== L298N MOTOR PINS (NO ENABLE) ====================
#define IN1 26
#define IN2 25
#define IN3 33
#define IN4 32

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);

  // Attach servos
  baseServo.attach(basePin);
  elbowServo.attach(elbowPin);
  wristServo.attach(wristPin);
  gripServo.attach(gripPin);

  // Initial servo positions
  elbowServo.write(ELBOW_HOME);
  delay(300);
  baseServo.write(BASE_HOME);
  delay(300);
  wristServo.write(WRIST_HOME);
  delay(300);
  gripServo.write(GRIP_OPEN);
  delay(300);

  // Motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopMotors();

  Serial.println("ESP32 Manual Pick & Place with Movement Ready");
}

// ==================== PICK FUNCTION ====================
void pickObject() {
  wristServo.write(WRIST_PICK);
  delay(1000);

  elbowServo.write(ELBOW_PICK);
  delay(1000);

  gripServo.write(GRIP_CLOSED);
  delay(1000);

  elbowServo.write(ELBOW_LIFT);
  delay(1000);
}

// ==================== PLACE FUNCTION ====================
void placeObject() {
  baseServo.write(BASE_PLACE);
  delay(700);

  wristServo.write(WRIST_PLACE);
  delay(300);

  elbowServo.write(ELBOW_PICK);
  delay(300);

  gripServo.write(GRIP_OPEN);
  delay(400);
}

// ==================== HOME FUNCTION ====================
void homePosition() {
  elbowServo.write(ELBOW_HOME);
  delay(300);

  baseServo.write(BASE_HOME);
  delay(400);

  wristServo.write(WRIST_HOME);
  delay(300);

  gripServo.write(GRIP_OPEN);
  delay(200);
}

// ==================== MOTOR FUNCTIONS ====================
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// ==================== BLYNK BUTTONS ====================

// PICK button (V1)
BLYNK_WRITE(V1) {
  if (param.asInt() == 1) {
    pickObject();
  }
}

// PLACE button (V6)
BLYNK_WRITE(V6) {
  if (param.asInt() == 1) {
    placeObject();
  }
}

// HOME button (V7)
BLYNK_WRITE(V7) {
  if (param.asInt() == 1) {
    homePosition();
  }
}

// Movement buttons (UNCHANGED)
BLYNK_WRITE(V2) {
  int pinstate1 = param.asInt();
  if (pinstate1 == 1) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN4, LOW);
  } else {
    stopMotors();
  }
}

BLYNK_WRITE(V3) {
  int pinstate2 = param.asInt();
  if (pinstate2 == 1) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN4, HIGH);
  } else {
    stopMotors();
  }
}

BLYNK_WRITE(V4) {
  int pinstate3 = param.asInt();
  if (pinstate3 == 1) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN4, HIGH);
  } else {
    stopMotors();
  }
}

BLYNK_WRITE(V5) {
  int pinstate9 = param.asInt();
  if (pinstate9 == 1) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN4, LOW);
  } else {
    stopMotors();
  }
}

void loop() {
  Blynk.run();
}
