#include <Wire.h>
#include "MAX30105.h" // libraria cu care lucram pt. programarea obiectului. Trebuie descarcata din librariile din arduinoIDE
#include "spo2_algorithm.h"


MAX30105 maxs; // declararea obiectului

#define MAX_BRIGHTNESS 255

// Setari pt senzor
const long PRAG_DE_PIELE = 50000; // Valoarea minimă de IR pentru a considera că brățara e pe mână
bool brataraPusa = false;         // Memoria stării brățării

// Buffere pentru datele de precizie 
uint32_t irBuffer[100]; 
uint32_t redBuffer[100];  
int32_t bufferLength = 100; 
int32_t spo2; 
int8_t validSPO2; 
int32_t heartRate; 
int8_t validHeartRate; 

void setup() {
  Serial.begin(115200);
  Wire.begin(4, 5); // pinii de i2c

  if (!maxs.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("MAX30102 nu a fost gasit"); // detectarea conectivitatii senzorului
    while (1); // crearea unui loop in caz de neconectivitate
  }

  Serial.println("Senzor gasit");

  byte ledBrightness = 60; 
  byte sampleAverage = 4; 
  byte ledMode = 2;
  int sampleRate = 100; 
  int pulseWidth = 411; 
  int adcRange = 4096; 

  maxs.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);
  
  Serial.println("Sistem pornit. Bratara asteapta sa fie pusa pe mana...");
  delay(1000);
}

void loop() {
  // 1. Verificăm cantitatea lumină Infraroșie care se întoarce înainte să facem orice calcul
  long irValue = maxs.getIR();

  // Cazul in care Bratare este in aer
  if (irValue < PRAG_DE_PIELE) {
    if (brataraPusa) {
      Serial.println("\n❌ Bratara a fost data jos!");
      brataraPusa = false; // Resetăm starea
    }
    delay(500); 
    return; // Comanda "return" oprește loop-ul aici și îl ia de la capăt. Nu mai merge la calcule.
  }

  // Daca bratara a fost tocmai pusa pe mana
  if (!brataraPusa && irValue > PRAG_DE_PIELE) {
    Serial.println("\n✅ Bratara detectata pe mana! Incarc buffer-ul (Asteapta 4 secunde)...");
    
    // Umplem de la zero buffer-ul de 100 de citiri
    for (byte i = 0 ; i < bufferLength ; i++) {
      while (maxs.available() == false) maxs.check();
      redBuffer[i] = maxs.getRed();
      irBuffer[i] = maxs.getIR();
      maxs.nextSample(); 
    }
    
    // Prima calculare de SpO2
    maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
    brataraPusa = true; // Salvăm starea ca să nu mai umplem buffer-ul de la zero tura viitoare
  }

  // Daca bratara este deja pe mana (Actualizare continuă și fluidă)
  if (brataraPusa) {
    // Mutăm ultimele 75 de citiri în față
    for (byte i = 25; i < 100; i++) {
      redBuffer[i - 25] = redBuffer[i];
      irBuffer[i - 25] = irBuffer[i];
    }

    // Citim doar 25 de eșantioane noi (update mai rapid pe ecran)
    for (byte i = 75; i < 100; i++) {
      while (maxs.available() == false) maxs.check();

      redBuffer[i] = maxs.getRed();
      irBuffer[i] = maxs.getIR();
      maxs.nextSample(); 
      
      // Verificăm de siguranță să nu fi luat mâna brusc în mijlocul citirilor
      if (irBuffer[i] < PRAG_DE_PIELE) {
        brataraPusa = false;
        return; 
      }
    }

    // Recalculăm algoritmul clinic
    maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);

    // Afișăm rezultatele
    if (validHeartRate && validSPO2) {
      Serial.print("❤️ Puls: ");
      Serial.print(heartRate, DEC);
      Serial.print(" BPM | 🩸 Oxigen: ");
      Serial.print(spo2, DEC);
      Serial.println(" %");
    } else {
      Serial.println("⏳ Calculez parametrii... Stai nemiscat!");
    }
  }
}
