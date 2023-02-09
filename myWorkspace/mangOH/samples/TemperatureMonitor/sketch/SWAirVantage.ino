/*
  Read/Write Data to/from AirVantage

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/AirVantage

 */

#include <Bridge.h>
#include <AirVantage.h>

#define AV_URL        "na.airvantage.net"
#define AV_PASSWORD   "AirVantage"
//#define AV_ASSET      "SwiMangohBridgeTest"
#define APP_NAME      "home"
#define APP_HOME_NODE "BedRoom1"

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  // Initialize Bridge and Mailbox
  Bridge.begin();
  AirVantage.begin();
  digitalWrite(13, HIGH);

  // Initialize Serial
  Serial.begin(9600);

  // Wait until a Serial Monitor is connected.
  while (!Serial);

  Serial.print("Start Session: ");
  Serial.print(AV_URL);
  Serial.print(" ");
  Serial.println(AV_PASSWORD);
  AirVantage.startSession(AV_URL, AV_PASSWORD, 1, CACHE);
  
  AirVantage.subscribe(APP_NAME "." APP_HOME_NODE "." "Humidity");

/*  Serial.print("Start Session: ");
  Serial.println(AV_ASSET);
  AirVantage.startSession(AV_ASSET, "");
  */
}

void loop() {
  int airCon = analogRead(A0); // read analog pin A0
  Serial.print("Air Conditioner: ");
  Serial.println(airCon);
  AirVantage.pushInteger(APP_NAME "." APP_HOME_NODE "." "AirConditioner", airCon);

  float humidity = (float)analogRead(A0)/100.0; // read analog pin A0
  Serial.print("Humidity: ");
  Serial.println(humidity);
  AirVantage.pushFloat(APP_NAME "." APP_HOME_NODE "." "Humidity", 3, humidity);

  bool light = airCon % 2; // read analog pin A0
  Serial.print("Light: ");
  Serial.println(light);
  AirVantage.pushBoolean(APP_NAME "." APP_HOME_NODE "." "Light", light);

  bool shutters = airCon % 4; // read analog pin A0
  Serial.print("Shutters: ");
  Serial.println(shutters);
  AirVantage.pushBoolean(APP_NAME "." APP_HOME_NODE "." "Shutters", shutters);

  String room = (airCon % 2) ? "Bedroom":"Kitchen";
  Serial.print("BedRoom1: ");
  Serial.println(room);
  AirVantage.pushString("BedRoom1", room);
  
  String message;
  // read all the data available from Air Vantage
  while (AirVantage.dataAvailable())
  {
    AirVantage.readMessage(message);
    Serial.println(message);
  }

  // wait 10 seconds
  delay(10000);
}
