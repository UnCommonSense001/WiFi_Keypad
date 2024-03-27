using namespace std;

const int voltageReadPin = A0;
const int col1Pin = 5; // D1
const int col2Pin = 4; // D2
const int col3Pin = 2; // D4
const int col4Pin = 2; // D14
int rawReading;
float voltage;

void readVoltage() {
    rawReading = analogRead(voltageReadPin);
    voltage = rawReading * (3.3 / 1023.0);
    Serial.print("Voltage: ");
    Serial.println(voltage);
}

void setup() {
    pinMode(voltageReadPin, INPUT);
    Serial.begin(115200);
}

void loop() {
    readVoltage();
}