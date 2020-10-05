#include <EEPROM.h>

int distance = 0;
int distance_to_write = 0;
int latest_reading = 0;

#define BUTTON 12
#define LED 13
#define analogPin 0

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  // Read the 2 byte int from EEPROM addresses zero and one.
  distance = EEPROM.read(0);

  // We need to shift the first byte over 1 byte (8 bits), then Bitwise OR it with the next byte read from eeprom address 1.
  distance = distance << 8;
  distance = distance | EEPROM.read(1);

  Serial.print("Read value from eeprom: ");
  Serial.println(distance);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  latest_reading = analogRead(analogPin);

  // Is object is in ideal position?
  if(latest_reading <= distance) {
    digitalWrite(LED, HIGH);
  }
  else {
    digitalWrite(LED, LOW);
  }

  // Record current reading to EEPROM
  if (! digitalRead(BUTTON)) {
    // Flash the on-board LED that we are writing to EEPROM
    for (int i = 0; i < 3; i++) {
      digitalWrite(LED, HIGH);
      delay(50);
      digitalWrite(LED, LOW);
      delay(50);
    }

    distance_to_write = analogRead(analogPin);
    distance = distance_to_write;

    Serial.print("Writing to eeprom: ");
    Serial.println(distance);
    
    // Write the 2 byte int to EEPROM addresses zero and one.
    EEPROM.write(1, (byte) distance_to_write);

    distance_to_write = distance_to_write << 8;
    EEPROM.write(0, (byte) distance_to_write );
  }

  // Pause to slow down the loop a bit since we don't need to take readings very often
  delay(500);
  
}
