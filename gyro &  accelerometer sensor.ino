#include <MPU9250_WE.h> // librarie pe care o folosim, trebuie descarcata din librariile din arduino studio IDE

MPU6500_WE mpu = MPU6500_WE(0x68); // obiectul declarat cu pinii i2c

void setup() {
  Serial.begin(115200);
  delay(1000); 
  
  Wire.begin(4, 5); 
    
  if(!mpu.init()){ // testarea conectivitatii
    Serial.println("MPU - nu este conectat!!!"); 
    while(1){} // crearea unui loop in caz de necoentivitate
  }

  Serial.println("MPU sensor conectat cu succes");
  Serial.println("------------------------------------------------- \n");

  mpu.autoOffsets(); // configurarea automata a senzorului pt neglijarea greselilor din fabirca
  
  mpu.enableGyrDLPF();
  mpu.setGyrDLPF(MPU9250_DLPF_6); 
  
  Serial.println("Sistemul calibrat");
  Serial.println("------------------------------------------------- \n");
}

void loop() {
  xyzFloat accValue = mpu.getGValues();     // accelaratia pe axa 0xyz (masurata in G)
  xyzFloat angles   = mpu.getAngles();      // unghiul de roatatie pe axa 0xyz (masurata in grade)
  xyzFloat gyrValue = mpu.getGyrValues();   // viteza de rotatie pe axa 0xyz (masurata in grade / secunda)

 // afisari
  Serial.println(" Cat de repede ma misc pe 0xyz:");

  Serial.print(" X: ");
  Serial.print(accValue.x);
  Serial.println(" [G]");

  Serial.print(" Y: ");
  Serial.print(accValue.y);
  Serial.println(" [G]");

  Serial.print(" Z: ");
  Serial.print(accValue.z);
  Serial.println(" [G] \n");


  Serial.println(" Ungiurile de rotatie:");
  Serial.print(" Pitch: ");
  Serial.print(angles.y);
  Serial.println(" °") ;

  Serial.print(" Roll:");
  Serial.print(angles.x);
  Serial.println(" ° \n");


  Serial.println("Viteza de rotatie pe axe:");
  Serial.print(" X: ");
  Serial.print(gyrValue.x);
  Serial.println(" °/s");

  Serial.print(" Y: ");
  Serial.print(gyrValue.y);
  Serial.println(" °/s"); 

  Serial.print(" Z: ");
  Serial.print(gyrValue.z);
  Serial.println(" °/s \n");

  delay(100);
}
