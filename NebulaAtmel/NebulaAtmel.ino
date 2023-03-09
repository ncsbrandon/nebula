#include <Wire.h>
#include <SparkFunSX1509.h>
#include <SparkFun_ADS1015_Arduino_Library.h>

#define NAMEREQUEST F("NAME")
#define NAMEVALUE F("NAME,Nebula Atmel")
#define VERSIONREQUEST F("VERSION")
#define VERSIONVALUE F("VERSION,1.0.3Mar2023")
#define DOREQUEST F("DO")
#define DIREQUEST F("DI")
#define AOREQUEST F("AO")
#define AIREQUEST F("AI")

ADS1015 adc;
SX1509 io;

String inputString = "";      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void setup() {
  // setup serial
  Serial.begin(115200);
  while (!Serial) { ; }

  // print start
  //char buffer[48];
  //sprintf(buffer, "Starting %s %s", NAME, VERSION);
  handleNameRequest();
  handleVersionRequest();

  // setup i2C
  Wire.begin();
  //Wire.onReceive(receiveEvent);

  // connect to the ADC
  if (adc.begin(0x3C))
    Serial.println(F("MSG,ADC found"));
  else
    Serial.println(F("MSG,ADC not found"));

  // connect to the IO
  if (io.begin(0x3E))
    Serial.println(F("MSG,IO found"));
  else
    Serial.println(F("MSG,IO not found"));

  // setup IO pins
  io.pinMode(16, OUTPUT);
  io.pinMode(17, ANALOG_OUTPUT);
  io.pinMode(18, INPUT);
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

String getParam(int n) {
  int i = 0;
  while (n > 0) {
    int j = inputString.indexOf(',', i);
    if (j == -1)
      return "";
    j++;
    i = j;
    n--;
  }
  int k = inputString.indexOf(',', i);
  if (k == -1)
    return inputString.substring(i);
  return inputString.substring(i, k);
}

void loop() {
  if (stringComplete) {
    handleRequest();
    // clear the string
    inputString = "";
    stringComplete = false;
  }
}

void handleRequest() {
  if (inputString.startsWith(NAMEREQUEST))
    handleNameRequest();
  else if (inputString.startsWith(VERSIONREQUEST))
    handleVersionRequest();
  else if (inputString.startsWith(DOREQUEST))
    handleDigitalOutRequest();
  else if (inputString.startsWith(DIREQUEST))
    handleDigitalInRequest();
  else if (inputString.startsWith(AOREQUEST))
    handleAnalogOutRequest();
  else if (inputString.startsWith(AIREQUEST))
    handleAnalogInRequest();
  else
    Serial.println(F("MSG,Unknown request"));  
}

void handleNameRequest() {
  Serial.println(NAMEVALUE);
}

void handleVersionRequest() {
  Serial.println(VERSIONVALUE);
}

void handleDigitalOutRequest() {
  int pin = getParam(1).toInt();
  int value = getParam(2).toInt();
  io.digitalWrite(pin, value);

  Serial.print(DOREQUEST);
  Serial.print(',');
  Serial.print(pin);
  Serial.print(',');
  Serial.print(value);
}

void handleDigitalInRequest() {
  int pin = getParam(1).toInt();
  uint16_t value = io.digitalRead(pin);

  Serial.print(DIREQUEST);
  Serial.print(',');
  Serial.print(pin);
  Serial.print(',');
  Serial.print(value);
}

void handleAnalogOutRequest() {
  int pin = getParam(1).toInt();
  int value = getParam(2).toInt();
  io.analogWrite(pin, value);

  Serial.print(AOREQUEST);
  Serial.print(',');
  Serial.print(pin);
  Serial.print(',');
  Serial.print(value);
}

void handleAnalogInRequest() {
  int pin = getParam(1).toInt();
  uint16_t value = adc.getSingleEnded(pin);

  Serial.print(AIREQUEST);
  Serial.print(',');
  Serial.print(pin);
  Serial.print(',');
  Serial.print(value);
}
