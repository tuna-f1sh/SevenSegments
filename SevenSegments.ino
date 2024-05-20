#include <Stepper.h>

#define STEPS_PER_REVOLUTION (64*32)
#define STEPS_PER_DIGIT ((int)(STEPS_PER_REVOLUTION * 200L / 360))
#define HALL_SENSOR_PIN 2

#if (defined(ARDUINO_SAMD_ZERO) || defined(ARDUINO_SAM_ZERO)) && defined(SERIAL_PORT_USBVIRTUAL)
  // Required for Serial on Zero based boards
  #define Serial SERIAL_PORT_USBVIRTUAL
#endif

Stepper myStepper = Stepper(STEPS_PER_REVOLUTION, 8, 10, 9, 11);
int currentDigit = 0; // Current digit is tracked
bool everySecond = false;
int _millis = 0;

void setup() {
  pinMode(HALL_SENSOR_PIN, INPUT);
  myStepper.setSpeed(12);
  delay(1000); // Delay to allow for serial monitor to connect

  Serial.print("Stepper CLI ready. Commands:");
  Serial.println("home,");
  Serial.println("mstep,<steps>");
  Serial.println("m[d],<digit>");
  Serial.println("n,");
  Serial.println("p,");
  Serial.println("cycle,<en>");
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim(); // Remove any whitespace
    input.toLowerCase(); // Convert command to lowercase
    int commaIndex = input.indexOf(',');
    String command = input.substring(0, commaIndex);
    int value = commaIndex > 0 ? input.substring(commaIndex + 1).toInt() : 0;

    if (command == "home") {
      homeStepper();
      Serial.println("Homed");
    } else if (command == "mstep") {
      manualHome(value);
      Serial.print("Manually stepped by ");
      Serial.print(value);
      Serial.println(" steps");
    } else if (command == "m" || command == "d") {
      moveToDigit(value);
      Serial.print("Moved to digit: ");
      Serial.println(value);
    } else if (command == "n") {
      moveToDigit(currentDigit < 9 ? currentDigit + 1 : 0);
      Serial.println("Moved to next digit");
    } else if (command == "p") {
      moveToDigit(currentDigit > 0 ? currentDigit - 1 : 9);
      Serial.println("Moved to previous digit");
    } else if (command == "mdigit") {
      setManualDigit(value);
    } else if (command == "cycle") {
      everySecond = value == 1;
      Serial.print("Cycle mode ");
      Serial.println(everySecond ? "enabled" : "disabled");
    } else {
      Serial.println("Unknown command");
    }
  }

  if (everySecond && (millis() - _millis) >= 1000) {
    moveToDigit(currentDigit < 9 ? currentDigit + 1 : 0);
    _millis = millis();
  }
}

void homeStepper() {
  while(digitalRead(HALL_SENSOR_PIN) == HIGH) {
    myStepper.step(-1); // Adjust for reverse direction
    delay(5);
  }
  currentDigit = 0; // Reset digit position after homing
}

void manualHome(int steps) {
  myStepper.step(-steps); // Adjust for reverse direction
                          // Note: This function now adjusts steps without changing the digit count
}

void moveToDigit(int digit) {
  int stepsToMove;
  if (digit >= currentDigit) {
    stepsToMove = (digit - currentDigit) * STEPS_PER_DIGIT;
  } else {
    stepsToMove = (10 - currentDigit + digit) * STEPS_PER_DIGIT;
  }

  myStepper.step(-stepsToMove); // Move forward (adjust direction as needed)
  currentDigit = digit; // Update current digit to the new one
}

void setManualDigit(int digit) {
  if (digit >= 0 && digit <= 9) {
    currentDigit = digit;
    Serial.print("Current digit manually set to: ");
    Serial.println(digit);
  } else {
    Serial.println("Digit out of range. Must be between 0 and 9.");
  }
}

