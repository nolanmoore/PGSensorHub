#include <Keypad.h>
#include <Wire.h>
#include <LSM303.h>

// Keypad
const byte KEYPAD_ROWS = 2;
const byte KEYPAD_COLS = 8;
char PGKeys[KEYPAD_ROWS][KEYPAD_COLS] = {
  {'R','L','D','U','P','S','B','A'},
  {'0','1','2','3','4','5','6','7'},
  {'8','9','E','X','.','.','.','.'}
};
byte rowPins[KEYPAD_ROWS] = {12, 11, 9}; //, 2}; // purple, green, blue
// purple, blue, grey, green, yellow, orange, red, white
// next to labels R3, R2, R1, R4, R5, R6, R7, R8 on board, respectively.
byte colPins[KEYPAD_COLS] = {8, 20, 10, 7, 6, 5, 4, 3};
Keypad pgKeypad = Keypad( makeKeymap(PGKeys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);

// Flex sensors
int flexPins[2] = {A1, A2};    // select the input pin for the potentiometer
int flexValue[2];  // variable to store the value coming from the sensor
int flexClosed[2] = {325,325};
int flexRelaxed[2] = {440, 440};
int flexOpen[2] = {470, 470};
int flexState[2] = {0,0};

int ledPin = 13;  // orange wire + black gnd
int ledState;
unsigned long ledTimer;

LSM303 compass;

char report[19];

void setup() {
  pinMode(ledPin, OUTPUT);
  ledState = HIGH;
  digitalWrite(ledPin, ledState);
  ledTimer = millis();
  Serial.begin(115200);
  Serial1.begin(115200);

  Wire.begin();
  compass.init();
  compass.enableDefault();
  Serial.println("entering loop");
}

void loop() {
  // Keypad
  char pressed = pgKeypad.getKey();
  if (pressed) {
    Serial.print(pressed);
    Serial1.print(pressed);
  } else {
    Serial.print("X");
    Serial1.print("X");
  }

  // Flex sensors
  for (int i = 0; i < 2; i++) {
    flexValue[i] = analogRead(flexPins[i]);
//    Serial.print(flexValue[i]);
//    Serial.print(",");
    if (withinTolerance(flexValue[i], flexClosed[i], 0.2)) {
      flexState[i] = 1;
    } else if (withinTolerance(flexValue[i], flexRelaxed[i], 0.25)) {
      flexState[i] = 2;
    } else if (withinTolerance(flexValue[i], flexOpen[i], 0.2)) {
      flexState[i] = 3;
    } else {
      flexState[i] = 0; // undefined
    }
  }
  char gesture;
  if (flexState[0] == 0 || flexState[1] == 0) {
    gesture = 'x';
  } else if (flexState[0] == 2 && flexState[1] == 2) {
    gesture = 'R'; // relaxed
  } else if (flexState[0] == 1 && flexState[1] == 1) {
    gesture = 'F'; // fist
  } else if (flexState[0] == 2 && flexState[1] == 1) {
    gesture = 'P'; // pointing
  }  else {
    gesture = 'X';
  }
  Serial.print(",");
  Serial.print(gesture);
  Serial1.print(",");
  Serial1.print(gesture);

  compass.read();

  snprintf(report, sizeof(report), ",%3d,%3d", compass.a.x/40, compass.a.y/40);
  Serial.println(report);
  Serial1.println(report);

  if (millis() - ledTimer >= 500) {
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
    ledTimer = millis();
  }

  delay(100);
}

boolean withinTolerance(int value, int center, float tolerance) {
  int radius = (int)(center * tolerance);
  if (value <= (center + radius) && value >= (center - radius)) {
    return true;
  } else {
    return false;
  }
}
