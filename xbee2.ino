#include <SoftwareSerial.h>
#include <Servo.h>
#include "DFRobot_TCS34725.h"

Servo myServo;
Servo myServo2;
DFRobot_TCS34725 tcs = DFRobot_TCS34725(&Wire, TCS34725_ADDRESS,TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
SoftwareSerial mySerial = SoftwareSerial(9,8);
int c=0;
int estado=1;
int valServo2=0;
uint16_t clear, red, green, blue, red2, green2, blue2;

int differentcolors(int r, int g, int b, int r2, int g2, int b2){
  int num1=abs(r-r2);
  int num2=abs(g-g2);
  int num3=abs(b-b2);
    
  int res=0;
  if (num1>=1000 || num2>=1000 || num3>=1000)
    res=1;

  return res;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(9,INPUT);
  pinMode(8,OUTPUT);
  myServo.attach(3);
  myServo2.attach(2);

  Serial.begin(9600);
  mySerial.begin(9600);
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (mySerial.available() > 0) {
        c=mySerial.read();
        Serial.println(c);
        //val=!val;
        if (c==1)
          estado=!estado;
        if (c==2 && estado){
          valServo2=(valServo2+180)%360;
          myServo2.write(valServo2);
        }
    }
    
    if (estado && c>2){
      myServo.write(c);
    }


    if (!estado){
      //Serial.println("EN ESTADO AUTO");
      tcs.getRGBC(&red, &green, &blue, &clear);
      //tcs.lock();  
      int val=0;
      red2=red;
      green2=green;
      blue2=blue;
      myServo2.write(0);
      while (!differentcolors(red,green,blue,red2,green2,blue2)){
        myServo.write(val);
        delay(50);
        val+=2;
        tcs.getRGBC(&red2, &green2, &blue2, &clear);
        //tcs.lock();
        Serial.print(red);
        Serial.print("\t");
        Serial.print(green);
        Serial.print("\t");
        Serial.println(blue);
        Serial.print(red2);
        Serial.print("\t");
        Serial.print(green2);
        Serial.print("\t");
        Serial.println(blue2);
      }
      estado=!estado;
      Serial.println("Saliendo del moto auto");
      myServo2.write(180);
    }
}
