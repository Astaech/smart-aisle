#include <RFID.h>
#include <SPI.h>
#include <Wire.h>

#define SS_PIN 10
#define RST_PIN 9
#define SLV_ADDR 0x12
#define WORD 5

RFID rfid(SS_PIN, RST_PIN);

void initData();
void printData();
void receiveData();
void sendData();
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
  initData();
}

void loop() {
  if(rfid.isCard()) {
    if(rfid.readCardSerial()) {
      printData();
      delay(2000);
    }
  }
}

void initData(){
  int i;
  for(i=0; i<WORD; i++) {
    datas[i] = 22;
  }
}

void printData(){
  int i;
  for(i=0; i<WORD; i++) {
    Serial.print(rfid.serNum[i]);
    Serial.print(" ");
  }
  Serial.println("");
}

void requestEvent(){
  int i;
  for(i=0; i<WORD; i++) {
    datas[i] = rfid.serNum[i];
  }

  Serial.print("send : ");
  for(i=0; i<WORD; i++) {
    Serial.print(datas[i]);
    Serial.print(" ");
  }
  Serial.println("");

  Wire.write(datas, WORD);
  initData();
}

void receiveData(){
  while(Wire.available()) {
    dataReceived = Wire.read();
    Serial.print("received : ");
    Serial.println(dataReceived);
    sendData();
  }
}

void sendData(){
  int i;
  for(i=0; i<WORD; i++) {
    datas[i] = rfid.serNum[i];
  }

  Serial.print("send : ");
  for(i=0; i<WORD; i++) {
    Serial.print(datas[i]);
    Serial.print(" ");
  }
  Serial.println("");

  Wire.write(datas, WORD);
  initData();
}