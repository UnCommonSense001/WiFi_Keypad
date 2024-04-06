using namespace std;

#define voltageReadPin A0

/* PINOUT
looking at my button matrix with pins on the left
D1
D6
D2
D0uuu
Then rest of pins to breadboard (for voltage ladder)
*/

bool colFlags[] = {false, false, false, false};
int columnPins[] = { 16, 4, 12, 5 };
char row0[4] = { '1', '2', '3', 'A' };
char row1[4] = {'4', '5', '6', 'B' };
char row2[4] = {'7', '8', '9', 'C' };
char row3[4] = {'*', '0', '#', 'D' };
char* matrix[4] = {row0, row1, row2, row3};

void setup() {
  Serial.begin(115200);
  pinMode(voltageReadPin, INPUT);
  for (int colPin : columnPins) {
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

char readMatrix() {
  for (int col = 0; col < sizeof row0 / sizeof row0[0]; col++) {
    int colPin = columnPins[col];
    int row = readCol(colPin);
    if (row != -1 && !colFlags[col]) {
      colFlags[col] = true;
      return matrix[row][col];
    }
    if (row == -1 && colFlags[col])
      colFlags[col] = false;
  }
  return '\0';
}

void loop() {
  char curVal = readMatrix();
  if (!curVal == '\0')
    Serial.println(curVal);
}