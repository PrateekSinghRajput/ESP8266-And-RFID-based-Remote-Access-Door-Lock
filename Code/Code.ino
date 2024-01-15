//Prateek
//wwww.justdoelectronics.com
//https://www.youtube.com/c/JustDoElectronics/videos

#include <SPI.h>
#include <MFRC522.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define SS_PIN 4
#define RST_PIN 2

int elock = D0;

MFRC522 mfrc522(RST_PIN, SS_PIN);
char auth[] = "BvnkNlDDxM1NdUmq0PfAeZLUwa8o85Mx";

char ssid[] = "singh";
char pass[] = "pratik123";

SimpleTimer timer;
int fflag = 0;
int eflag = 0;
int jflag = 0;
WidgetTerminal terminal(V2);

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  pinMode(elock, OUTPUT);
  digitalWrite(elock, LOW);
  SPI.begin();
  mfrc522.PCD_Init();
  timer.setInterval(1000L, iot_rfid);
}

void loop() {

  timer.run();
  Blynk.run();
}

void iot_rfid() {


  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }


  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }


  Serial.print("Card UID:");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], DEC);
  }
  Serial.println();


  byte piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);

  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI
      && piccType != MFRC522::PICC_TYPE_MIFARE_1K
      && piccType != MFRC522::PICC_TYPE_MIFARE_4K) {

    return;
  }


  if (((mfrc522.uid.uidByte[0] == 9) && (mfrc522.uid.uidByte[1] == 191) && (mfrc522.uid.uidByte[2] == 233) && (mfrc522.uid.uidByte[3] == 212)) && (fflag == 1)) {
    Serial.println("Pratek");
    Blynk.virtualWrite(V2, "Prateek");
    digitalWrite(elock, HIGH);
    delay(10000);
    digitalWrite(elock, LOW);
  }

  else if (((mfrc522.uid.uidByte[0] == 108) && (mfrc522.uid.uidByte[1] == 159) && (mfrc522.uid.uidByte[2] == 37) && (mfrc522.uid.uidByte[3] == 217)) && (eflag == 1)) {
    Serial.println("Singh");
    Blynk.virtualWrite(V2, "Singh");
    digitalWrite(elock, HIGH);
    delay(5000);
    digitalWrite(elock, LOW);
  } else
    Serial.println("unregistered user");
}


BLYNK_WRITE(V3) {
  fflag = param.asInt();  // assigning incoming value from pin V3 to a variable
}

BLYNK_WRITE(V4) {
  eflag = param.asInt();  // assigning incoming value from pin V4 to a variable
}
