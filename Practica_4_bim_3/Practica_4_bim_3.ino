#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN); 

int ledVerde = 2;    
int ledRojo = 3;     
int buzzer = 4; 

byte uidcorrecto[] = {0xB3, 0x93, 0xD8, 0x24};  

void setup() {
  pinMode(ledVerde, OUTPUT);
  pinMode(ledRojo, OUTPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
}

void loop() {
  
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  } 
  
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  } 
  
  Serial.print("UID tag: ");
  byte uidSize = mfrc522.uid.size;
  for (byte i = 0; i < uidSize; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) {
      Serial.print('0');  
    }
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
 
  bool tagCorrecto = true;
  if (uidSize != sizeof(uidcorrecto)) {
    tagCorrecto = false;  
  } else {
    for (byte i = 0; i < uidSize; i++) {
      if (mfrc522.uid.uidByte[i] != uidcorrecto[i]) {
        tagCorrecto = false;
        break;
      }
    }
  }

  if (tagCorrecto) {   
    digitalWrite(ledVerde, HIGH);
    tone(buzzer, 200);  
    delay(4000);        
    digitalWrite(ledVerde, LOW);
    noTone(buzzer);
  } else {   
    digitalWrite(ledRojo, HIGH);
    tone(buzzer, 400);  
    delay(1000);        
    digitalWrite(ledRojo, LOW);
    noTone(buzzer);
  }
  delay(1000);   
}