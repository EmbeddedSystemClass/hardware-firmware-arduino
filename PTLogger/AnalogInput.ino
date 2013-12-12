
int sensorPin = A0;

#define PT1000_VALUE_COUNT = 340   // Anzahl der Werte in PT1000 Tabelle
#define ADC_RANGE_MIN = 684  // ADC-Wert entspricht -50°C
#define ADC_RANGE_MAX = 1023 // ADC-Wert entspricht 157°C

// Tabelle mit Temperaturwerten für PT1000
const int PT1000[] PROGMEM = {
  -50, 	-50, 	-49, 	-49, 	-48, 	-48, 	-48, 	-47, 	-47, 	-46, 	-46, 	-45, 	-45, 	-44, 	-44, 	-44, 
  -43, 	-43, 	-42, 	-42, 	-41, 	-41, 	-40, 	-40, 	-39, 	-39, 	-38, 	-38, 	-38, 	-37, 	-37, 	-36, 
  -36, 	-35, 	-35, 	-34, 	-34, 	-33, 	-33, 	-32, 	-32, 	-31, 	-31, 	-30, 	-30, 	-29, 	-29, 	-29, 
  -28, 	-28, 	-27, 	-27, 	-26, 	-26, 	-25, 	-25, 	-24, 	-24, 	-23, 	-23, 	-22, 	-22, 	-21, 	-21, 
  -20, 	-20, 	-19, 	-19, 	-18, 	-18, 	-17, 	-17, 	-16, 	-16, 	-15, 	-15, 	-14, 	-14, 	-13, 	-13, 
  -12, 	-12, 	-11, 	-11, 	-10, 	-10, 	 -9, 	 -9, 	 -8, 	 -8, 	 -7, 	 -7, 	 -6, 	 -6, 	 -5, 	 -5, 
   -4, 	 -3, 	 -3, 	 -2, 	 -2, 	 -1, 	 -1, 	  0, 	  0, 	  1, 	  1, 	  2, 	  2, 	  3, 	  3, 	  4, 
    4, 	  5, 	  6, 	  6, 	  7, 	  7, 	  8, 	  8, 	  9, 	  9, 	 10, 	 10, 	 11, 	 12, 	 12, 	 13, 
   13, 	 14, 	 14, 	 15, 	 15, 	 16, 	 16, 	 17, 	 18, 	 18, 	 19, 	 19, 	 20, 	 20, 	 21, 	 22, 
   22, 	 23, 	 23, 	 24, 	 24, 	 25, 	 26, 	 26, 	 27, 	 27, 	 28, 	 28, 	 29, 	 30, 	 30, 	 31, 
   31, 	 32, 	 33, 	 33, 	 34, 	 34, 	 35, 	 35, 	 36, 	 37, 	 37, 	 38, 	 38, 	 39, 	 40, 	 40, 
   41, 	 41, 	 42, 	 43, 	 43, 	 44, 	 44, 	 45, 	 46, 	 46, 	 47, 	 48, 	 48, 	 49, 	 49, 	 50, 
   51, 	 51, 	 52, 	 52, 	 53, 	 54, 	 54, 	 55, 	 56, 	 56, 	 57, 	 58, 	 58, 	 59, 	 59, 	 60, 
   61, 	 61, 	 62, 	 63, 	 63, 	 64, 	 65, 	 65, 	 66, 	 67, 	 67, 	 68, 	 68, 	 69, 	 70, 	 70, 
   71, 	 72, 	 72, 	 73, 	 74, 	 74, 	 75, 	 76, 	 76, 	 77, 	 78, 	 78, 	 79, 	 80, 	 80, 	 81, 
   82, 	 83, 	 83, 	 84, 	 85, 	 85, 	 86, 	 87, 	 87, 	 88, 	 89, 	 89, 	 90, 	 91, 	 92, 	 92, 
   93, 	 94, 	 94, 	 95, 	 96, 	 96, 	 97, 	 98, 	 99, 	 99, 	100, 	101, 	101, 	102, 	103, 	104, 
  104, 	105, 	106, 	107, 	107, 	108, 	109, 	110, 	110, 	111, 	112, 	112, 	113, 	114, 	115, 	115, 
  116, 	117, 	118, 	118, 	119, 	120, 	121, 	122, 	122, 	123, 	124, 	125, 	125, 	126, 	127, 	128, 
  128, 	129, 	130, 	131, 	132, 	132, 	133, 	134, 	135, 	136, 	136, 	137, 	138, 	139, 	139, 	140, 
  141, 	142, 	143, 	144, 	144, 	145, 	146, 	147, 	148, 	148, 	149, 	150, 	151, 	152, 	153, 	153, 
  154, 	155, 	156, 	157
}


void setup() {
  Serial.begin(9600); // serielle Schnittstelle initialisieren
  
  // Referenz einstellen
}

void loop() {
  // Index auf PT1000 Tabelle
  unsigned int index = 0;
  
  // Temperaturwert
  int temperature = 0;

  // Analogwert lesen
  int sensorValue = analogRead(sensorPin);     
  
  // Sensorwert prüfen: liegt der Wert im Messbereich?
  if (sensorValue >= ADC_RANGE_MIN && sensorValue <= ADC_RANGE_MAX) {

    // Index bilden
    index = sensorValue - ADC_RANGE_MIN;
    
    // Temperaturwert aus Tabelle lesen
    int temperature = (int)(PGM_P)pgm_read_word(&PT1000[index]);
    
    // Temperaturwert ausgeben
    Serial.println("Temperatur:" + String(temperature));
    
  } else {
    
    Serial.println("Wert ausserhalb des Bereichs");
  
  }

  delay(1000);
  
}