#include <SoftwareSerial.h>


SoftwareSerial mySerial = SoftwareSerial(9,8);
int val=0;
int estado=0;
int valAnt[4];
void setup() {
  // put your setup code here, to run once:
  pinMode(9,INPUT);
  pinMode(8,OUTPUT);
  pinMode(18,INPUT);
  pinMode(3,INPUT);
  pinMode(2,INPUT);
  pinMode(A0,INPUT);
  Serial.begin(9600);
  mySerial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(18),bottonAjust1,RISING);
  attachInterrupt(digitalPinToInterrupt(3),bottonAutomatic,RISING);
  attachInterrupt(digitalPinToInterrupt(2),bottonAdjust2,RISING);
}

void bottonAjust1(){
    if (!estado && val>2){
      mySerial.write(val);
      Serial.println(val);
    }
}

void bottonAutomatic(){
    Serial.println("A");
    estado=!estado;
    mySerial.write(1);
}

void bottonAdjust2(){
  Serial.println("B");
  mySerial.write(2);
}

int media(int primerValor){
  int resultado=0;
  for (int i=0;i<4;i++){
      resultado+=valAnt[i];
  }
  for (int i=1;i<4;i++){
    valAnt[i-1]=valAnt[i];
  }
  valAnt[3]=primerValor;
  return resultado/4;
}
void loop() {
  val = analogRead(A0)/4;
  val = media(val);
}
