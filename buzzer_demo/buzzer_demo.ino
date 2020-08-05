/*
  DigitalReadSerial

  Reads a digital input on pin 2, prints the result to the Serial Monitor

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/DigitalReadSerial
*/

// digital pin 2 has a pushbutton attached to it. Give it a name:
int amount = 100;
int highBound = 200;
int lowBound = 10;
int modifier = 1;
int step = 1;

int ticker = 0;
int scale = 3;

int pins[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int offset = 2;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  for (int pin = 2; pin < 12; pin++) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
  }
  pinMode(13, INPUT_PULLUP);
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(0 + offset, HIGH);

  for (int pin = 1 + offset; pin < 8 + offset; pin++) {
    if (ticker % pin + 1 - offset == 0) {
      digitalWrite(pin, HIGH);
    }
  }
  delayMicroseconds(amount);        // delay in between reads for stability

  digitalWrite(0 + offset, LOW);
  for (int pin = 1 + offset; pin < 8 + offset; pin++) {
    if (ticker % pin + 1 - offset == 0) {
      digitalWrite(pin, LOW);
    }
  }
  delayMicroseconds(amount);        // delay in between reads for stability


  if ( amount < lowBound ) {
    modifier = 1;
  }
  if (amount > highBound ) {
    modifier = -1;
  }

  amount = amount + (step * modifier);
  ticker += 1;
}
