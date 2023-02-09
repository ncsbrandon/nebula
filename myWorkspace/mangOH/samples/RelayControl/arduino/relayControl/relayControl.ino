#include <SwiBridge.h>
#include <Bridge.h>

#define KEY_COUNT_REQ      "RelayCountReq"
#define KEY_COUNT_RSP      "RelayCountRsp"

#define KEY_GETSTATE_REQ   "GetRelayStateReq"
#define KEY_GETSTATE_RSP   "GetRelayStateRsp"

#define KEY_SETSTATE_REQ   "SetRelayStateReq"

#define MIN_GPIO 2
#define MAX_GPIO 13

void setup() {
  uint8_t i;

  Serial.begin(9600);
  Serial.println("RelayControl");
  
  for (i = MIN_GPIO; i <= MAX_GPIO; i++) {
    // setup gpio pin
    pinMode(i, OUTPUT);

    // turn all relays off
    digitalWrite(i, HIGH);
  }

  // initialize bridge and mailbox
  Bridge.begin(115200);
  SwiBridge.begin();

  Serial.println("Start Session: ");
  SwiBridge.startSession("", "", 0, CACHE);

  SwiBridge.subscribe(KEY_COUNT_REQ);
  SwiBridge.subscribe(KEY_GETSTATE_REQ);
  SwiBridge.subscribe(KEY_SETSTATE_REQ);
}

// Pin is 1-indexed so offset by 1
// then map to the [MIN_GPIO, MAX_GPIO] range
int pinToGpio(int pin) {
  return (pin + MIN_GPIO);
}

void handleSetState(String message)
{
  // format for commands is "<pin>,<value>"
  int index = message.indexOf(',');
  String pinStr = message.substring(0, index);
  String valueStr = message.substring(index + 1);

  if (Serial) {
    Serial.print("SetState Pin(");
    Serial.print(pinStr);
    Serial.print(") Value(");
    Serial.print(valueStr);
    Serial.println(")");
  }

  int pin = pinStr.toInt();
  int value = valueStr.toInt();

  if (pinToGpio(pin) > MAX_GPIO) {
    if (Serial)
      Serial.println("Invalid pin number");
    return;
  }

  digitalWrite(pinToGpio(pin), value);
}

void handleGetState(String pinStr)
{
  // format for commands is "<pin>"
  int pin = pinStr.toInt();
  int value;

  if (pinToGpio(pin) > MAX_GPIO) {
    if (Serial)
      Serial.println("Invalid pin number");
    return;
  }

  value = digitalRead(pinToGpio(pin));
  String valueStr = String(value);

  if (Serial) {
    Serial.print("GetState Pin(");
    Serial.print(pinStr);
    Serial.print(") Value(");
    Serial.print(valueStr);
    Serial.println(")");
  }

  String response = pinStr + "," + valueStr;
  SwiBridge.pushString(KEY_GETSTATE_RSP, response);
}

void handleCount()
{
  SwiBridge.pushInteger(KEY_COUNT_RSP, (1 + MAX_GPIO - MIN_GPIO));
}

void loop() {
  // wait for gpio data
  String message;
  String current;
  String content;

  while (SwiBridge.dataAvailable())
  {
    SwiBridge.readMessage(message);
    while (message != "")
    {
      current = message.substring(0, message.indexOf("\n"));
      message = message.substring(message.indexOf("\n") + 1);

      // Set state ?
      if (current.startsWith("|" KEY_SETSTATE_REQ)) {
        content = SwiBridge.parseString(KEY_SETSTATE_REQ, current);
        if (Serial)
          Serial.println("SetState request");
        handleSetState(content);
      }

      // Get state ?
      else if (current.startsWith("|" KEY_GETSTATE_REQ)) {
        content = SwiBridge.parseString(KEY_GETSTATE_REQ, current);
        if (Serial)
          Serial.println("GetState request");
        handleGetState(content);
      }

      // Count ?
      else if (current.startsWith("|" KEY_COUNT_REQ)) {
        if (Serial)
          Serial.println("Count request");
        handleCount();
      }

      else {
        if (Serial)
          Serial.println("Unhandled request");
      }
    }
  }
}


