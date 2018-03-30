#include <RFID.h>
#include <SPI.h>
#include <Wire.h>

#define SS_PIN 10
#define RST_PIN 9
#define SLV_ADDR 0x12
#define WORD 5

RFID rfid(SS_PIN, RST_PIN);

void initData(boolean);
void printData();
void requestEvent();

int dataReceived;
byte datas[WORD];

void setup() {
  // Serial Port initialization
  Serial.begin(9600);

  // RFID initialization
  SPI.begin();
  rfid.init();

  // i²c initialization
  Wire.begin(SLV_ADDR);
  Wire.onRequest(requestEvent);

  // data initialization
  initData(false);
}

void loop() {
  if(rfid.isCard()) {
    if(rfid.readCardSerial()) {
      initData(true);
      printData();
      delay(1000);
      initData(false);
    }
  }
}

void initData(boolean a) {
  int i;
  for(i=0; i<WORD; i++) {
    if(a)
      datas[i] = rfid.serNum[i];
    else
      datas[i] = 0;
  }
}

void printData() {
  int i;
  for(i=0; i<WORD; i++) {
    Serial.print(rfid.serNum[i]);
    Serial.print(" ");
  }
  Serial.println("");
}

void requestEvent() {
  int i;
  Serial.print("send : ");
  for(i=0; i<WORD; i++) {
    Serial.print(datas[i]);
    Serial.print(" ");
  }
  Serial.println("");

  Wire.write(datas, WORD);
}