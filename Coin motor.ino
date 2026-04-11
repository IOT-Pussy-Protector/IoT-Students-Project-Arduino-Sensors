#define MV 0

void setup() {
  Serial.begin(115200);
  pinMode(MV, OUTPUT);
  delay(100);
}

  int cnt = 0;
void loop() {
  delay(10000);
  digitalWrite(MV, HIGH);
  delay(500);
  digitalWrite(MV, LOW);
  delay(250);
  digitalWrite(MV, HIGH);
  delay(250);
  digitalWrite(MV, LOW);
  delay(10000);

  cnt++;
  Serial.print("Motorul a facut vibratia nr:");
  Serial.println(cnt);
}
