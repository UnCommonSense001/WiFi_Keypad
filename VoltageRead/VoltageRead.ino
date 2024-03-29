using namespace std;

#define voltageReadPin A0

// bool buttonFlag = false;
bool colFlags[] = {false, false, false, false};
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

int readCol(int colPin) {
  digitalWrite(colPin, LOW);
  int rawReading = analogRead(voltageReadPin);
  float voltage = rawReading * (3.3 / 1023.0);
  digitalWrite(colPin, HIGH);

  if (voltage < 0.4)
    return 0;
  if (voltage < 1.2)
    return 1;
  if (voltage < 2.0)
    return 2;
  if (voltage < 3)
    return 3;
  return -1;
}

String readMatrix() {
  for (int col=0; col<sizeof columnPins/sizeof columnPins[0]; col++) {
    int colPin = columnPins[col];
    int row = readCol(colPin);
    if(row != -1 && !colFlags[col]) {
      colFlags[col] = true;
      return matrix[row][col];
    }
    if(row == -1 && colFlags[col])
      colFlags[col] = false;
  }
  return "";
}

void loop() {
    String curVal = readMatrix();
    if(!curVal.equals(""))
      Serial.println(curVal);
}