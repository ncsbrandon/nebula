// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.

#include <Wire.h>

static const char NAME[] = "Nebula Atmel";
static const char VERSION[] = "v1.0.3Mar2023           ";

static const int REQUEST_VERSION = 112;

static const int MCUADDR = 0x3E;

void setup() {
  // setup i2C
  Wire.begin(MCUADDR);
  Wire.onReceive(receiveEvent);

  // setup serial
  Serial.begin(115200);
  while (!Serial) {
    ;  // wait for serial port to connect
  }

  // print start
  char buffer[48];
  sprintf(buffer, "Starting %s %s", NAME, VERSION);
  Serial.println(buffer);
}

void loop() {
  delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  //Serial.print("receiveEvent [");
  //Serial.print(howMany);
  //Serial.println("]");

  // receive the request
  while (Wire.available()) {
    int c = Wire.read();

    // don't know what these are
    if(c == 0 || c == 128 || c == 4)
      continue;

    // ingore the LEDs on the bus
    if(c >= 14 && c <= 17)
      continue;

    Serial.print("request [");
    Serial.print(c);
    Serial.println("]");

    if (REQUEST_VERSION == c) {
      handleVersionRequest();
    }
  }
}

void handleVersionRequest() {
  Serial.println("handleVersionRequest");
  Wire.write(VERSION, 24);
}
