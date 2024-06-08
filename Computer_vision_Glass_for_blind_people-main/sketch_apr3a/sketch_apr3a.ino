// Pin definitions
const int buzzerPin = 11;
const int ledPin = 12;
const int buttonPin = 10;
const int xAxisPin = A0;
const int yAxisPin = A1;

// Seven-segment display segment pins
const int A = 2;
const int B = 3;
const int C = 4;
const int D = 5;
const int E = 6;
const int F = 7;
const int G = 8;

const int segmentPins[7] = {A, B, C, D, E, F, G};

// Counter variable
int counter = 0;

// Segment display digits 0-9 using the provided pattern
const byte seven_seg_digits[10][7] = {
  { 1, 1, 1, 1, 1, 1, 0 },  // = 0
  { 0, 1, 1, 0, 0, 0, 0 },  // = 1
  { 1, 1, 0, 1, 1, 0, 1 },  // = 2
  { 1, 1, 1, 1, 0, 0, 1 },  // = 3
  { 0, 1, 1, 0, 0, 1, 1 },  // = 4
  { 1, 0, 1, 1, 0, 1, 1 },  // = 5
  { 1, 0, 1, 1, 1, 1, 1 },  // = 6
  { 1, 1, 1, 0, 0, 0, 0 },  // = 7
  { 1, 1, 1, 1, 1, 1, 1 },  // = 8
  { 1, 1, 1, 0, 0, 1, 1 }   // = 9
};

void setup() {
  // Set up the buzzer
  pinMode(buzzerPin, OUTPUT);
  // Play the piezo buzzer for one second
  tone(buzzerPin, 1000); // 1000 Hz tone
  delay(1000);
  noTone(buzzerPin);

  // Set up the LED
  pinMode(ledPin, OUTPUT);

  // Set up the joystick button
  pinMode(buttonPin, INPUT_PULLUP);

  // Set up the seven-segment display pins
  for (int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }
  
  // Initialize the seven-segment display
  displayDigit(0);
}

void loop() {
  // Read the joystick positions
  int xValue = analogRead(xAxisPin);
  int yValue = analogRead(yAxisPin);
  bool buttonPressed = digitalRead(buttonPin) == LOW;

  if (buttonPressed) {
    // Reset counter to 0 and blink LED for 2 seconds
    counter = 0;
    displayDigit(0);
    blinkLED(2000);
  } else {
    // Joystick movement handling
    if (yValue > 800) { // Up
      incrementCounter();
    } else if (yValue < 200) { // Down
      incrementCounter();
    } else if (xValue > 800) { // Right
      decrementCounter();
    } else if (xValue < 200) { // Left
      decrementCounter();
    }
  }

  delay(200); // Debounce delay
}

void incrementCounter() {
  if (counter < 50) {
    counter++;
    displayDigit(counter / 5);
  }
}

void decrementCounter() {
  if (counter > 0) {
    counter--;
    displayDigit(counter / 5);
  }
}

void displayDigit(int num) {
  // Display a number on the seven-segment display
  if (num < 0 || num > 9) return;

  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], seven_seg_digits[num][i]);
  }
}

void blinkLED(int duration) {
  // Blink the LED for the given duration (in milliseconds)
  unsigned long endTime = millis() + duration;
  while (millis() < endTime) {
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(500);
  }
}