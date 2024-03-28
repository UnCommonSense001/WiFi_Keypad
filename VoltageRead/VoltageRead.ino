using namespace std;

#define voltageReadPin A0

bool buttonFlag = false;
int columnPins[] = {16, 4, 12, 5};
String matrix[4][4] = {
  {"1", "2", "3", "A"},
  {"4", "5", "6", "B"},
  {"7", "8", "9", "C"},
  {"*", "0", "#", "D"}
};

void setup() {
    Serial.begin(115200);
    pinMode(voltageReadPin, INPUT);
    for(int colPin: columnPins) {
      pinMode(colPin, OUTPUT);
      digitalWrite(colPin, HIGH);
    }
}

String readMatrix() {
  for (int col=0; col<sizeof columnPins/sizeof columnPins[0]; col++) {
    int colPin = columnPins[col];
    digitalWrite(colPin, LOW);
    int rawReading = analogRead(voltageReadPin);
    float voltage = rawReading * (3.3 / 1023.0);
    digitalWrite(colPin, HIGH);
    if(voltage <= 3 && !buttonFlag)
    {
      buttonFlag = true;
      if (voltage < 0.4)
        return matrix[0][col];
      if (voltage < 1.2)
        return matrix[1][col];
      if (voltage < 2.0)
        return matrix[2][col];
    }
    if(voltage > 3)
    {
      buttonFlag = false;
    }
    // if (voltage < 0.4)
    //   if(!buttonFlag) {
    //     buttonFlag = true;
    //     return matrix[0][col];
    //   }
    // if (voltage < 1.2)
    //   if(!buttonFlag) {
    //     buttonFlag = true;
    //     return matrix[1][col];
    //   }
    // if (voltage < 2.0)
    //   if(!buttonFlag) {
    //     buttonFlag = true;
    //     return matrix[2][col];
    //   }
    // if (voltage < 3.0)
    //   if(!buttonFlag) {
    //     buttonFlag = true;
    //     return matrix[3][col];
    //   }
  }
  // buttonFlag = false;
  return "";
}

void loop() {
    String curVal = readMatrix();
    if(!curVal.equals(""))
      Serial.println(curVal);
    // Serial.println("\t buttonFlag: " + buttonFlag);
    // rawReading = analogRead(voltageReadPin);
    // voltage = rawReading * (3.3 / 1023.0);
    // Serial.print("Voltage: ");
    // Serial.println(voltage);
}