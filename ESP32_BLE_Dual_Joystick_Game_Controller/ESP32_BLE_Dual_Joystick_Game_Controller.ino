

/*
  ESP32 BLE Gamepad — 2 analog joysticks + 9 push buttons
  ----------------------------------------------------------
  Library required: "ESP32 BLE Gamepad" by lemmingDev
    Arduino IDE -> Tools -> Manage Libraries -> search "BleGamepad" -> Install
    (Depends on "NimBLE-Arduino" — install if prompted)

  Board: any ESP32 dev board (Tools -> Board -> ESP32 Dev Module)

  WIRING:
    Joystick 1 (Left)
      VRx -> GPIO34   (analog X)
      VRy -> GPIO35   (analog Y)
      SW  -> GPIO32   (click = L3)
      VCC -> 3.3V
      GND -> GND

    Joystick 2 (Right)
      VRx -> GPIO33   (analog Z)
      VRy -> GPIO36   (analog RZ)
      SW  -> GPIO25   (click = R3)
      VCC -> 3.3V
      GND -> GND

    9 Push Buttons (each: 1 leg -> GPIO, opposite leg -> GND)
      Button 1 -> GPIO23
      Button 2 -> GPIO22  
      Button 3 -> GPIO21
      Button 4 -> GPIO19
      Button 5 -> GPIO18
      Button 6 -> GPIO27
      Button 7 -> GPIO14
      Button 8 -> GPIO12
      Button 9 -> GPIO13

  All digital pins use internal pull-ups — no external resistors needed.
  Total BLE buttons reported = 11 (9 buttons + L3 + R3 joystick clicks)
*/

#include <BleGamepad.h>

// ---------- Joystick pins ----------
const int PIN_L_VRX = 34;
const int PIN_L_VRY = 35;
const int PIN_R_VRX = 33;
const int PIN_R_VRY = 36;

// ---------- Button pins (index 0,1 = joystick clicks; 2-10 = the 9 buttons) ----------
const int NUM_BUTTONS = 11;
const int buttonPins[NUM_BUTTONS] = {
  32, 25,                              // L3, R3 (joystick clicks)
  23, 22, 21, 19, 18, 27, 14, 12, 13    // Button 1-9
};
const int gamepadButtonId[NUM_BUTTONS] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

// ---------- Debounce state ----------
bool lastButtonState[NUM_BUTTONS];
bool currentButtonState[NUM_BUTTONS];
unsigned long lastDebounceTime[NUM_BUTTONS];
const unsigned long debounceDelay = 20; // ms

// ---------- Joystick calibration ----------
const int ADC_MIN = 0;
const int ADC_MAX = 4095;
const int ADC_CENTER = 2048;
const int DEADZONE = 60; // ignore tiny jitter around center

BleGamepad bleGamepad("ESP32 Gamepad", "DIY", 100);

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    lastButtonState[i] = HIGH;
    currentButtonState[i] = HIGH;
    lastDebounceTime[i] = 0;
  }

  BleGamepadConfiguration cfg;
  cfg.setAutoReport(false);
  cfg.setControllerType(CONTROLLER_TYPE_GAMEPAD);
  cfg.setButtonCount(NUM_BUTTONS);
  cfg.setWhichAxes(true, true, true, true, false, false, false, false); // X, Y, Z, RZ
  bleGamepad.begin(&cfg);

  Serial.println("ESP32 BLE Gamepad ready. Pair it from your phone/PC Bluetooth settings.");
}

int readAxis(int pin) {
  int raw = analogRead(pin);
  int d = raw - ADC_CENTER;
  if (abs(d) < DEADZONE) d = 0;
  return map(ADC_CENTER + d, ADC_MIN, ADC_MAX, 0, 32767);
}

void loop() {
  if (!bleGamepad.isConnected()) {
    delay(50);
    return;
  }

  // ---- Read and debounce all buttons ----
  for (int i = 0; i < NUM_BUTTONS; i++) {
    bool reading = digitalRead(buttonPins[i]);

    if (reading != lastButtonState[i]) {
      lastDebounceTime[i] = millis();
    }

    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      if (reading != currentButtonState[i]) {
        currentButtonState[i] = reading;
        if (currentButtonState[i] == LOW) {
          bleGamepad.press(gamepadButtonId[i]);
        } else {
          bleGamepad.release(gamepadButtonId[i]);
        }
      }
    }
    lastButtonState[i] = reading;
  }

  // ---- Read both joysticks ----
  int mappedLX = readAxis(PIN_L_VRX);
  int mappedLY = readAxis(PIN_L_VRY);
  int mappedRX = readAxis(PIN_R_VRX);
  int mappedRY = readAxis(PIN_R_VRY);

  bleGamepad.setAxes(mappedLX, mappedLY, mappedRX, mappedRY, 0, 0, 0, 0);

  bleGamepad.sendReport();

  delay(10); // ~100 reports/sec
}
