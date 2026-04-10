#define BTN 10

void setup() {

  Serial.begin(115200);

  pinMode(BTN, INPUT_PULLUP);;

  delay(100);



}

  bool S = false;

void loop() {

  if(digitalRead(BTN) == LOW && S == false){

    Serial.println("Butonul este acum apasat");

   S = true;

  }



  else if(digitalRead(BTN) == HIGH && S == true){

    Serial.println("Butonul NU este acum apasat. este NEAPASAT");

    S = false;

  }

  delay(50);



}
