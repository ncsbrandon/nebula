String inputString = "";

void setup() {
  Serial.begin(115200);
}

void loop() {
  inputString = "ZERO";
  Serial.print("ZERO: ");
  Serial.println(getParam(1));
  
  inputString = "DO,14,1";
  Serial.print("DO,");
  Serial.print(getParam(1));
  Serial.print(",");
  Serial.println(getParam(2));
  
  inputString = "DI,15";
  Serial.print("DI,");
  Serial.println(getParam(1));
  
  inputString = "AO,16,50";
  Serial.print("AO,");
  Serial.print(getParam(1));
  Serial.print(",");
  Serial.println(getParam(2));

  inputString = "AI,17";
  Serial.print("AI,");
  Serial.println(getParam(1));

  delay(1000);
}

String getParam(int n) {
  int i = 0;
  while(n > 0) {
    int j = inputString.indexOf(',', i);
    if(j == -1)
      return "";
    j++;
    i = j;
    n--;
  }
  int k = inputString.indexOf(',', i);
  if(k == -1)
    return inputString.substring(i);
  return inputString.substring(i, k);
}