#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>


#define PWMA   6
#define AIN2   A0
#define AIN1   A1
#define PWMB   5
#define BIN1   A2
#define BIN2   A3
#define PIN    7
#define rxPin  4
#define txPin  3


SoftwareSerial mySerial(rxPin, txPin);


#define Addr  0x20
#define beep_on  PCF8574Write(0xDF & PCF8574Read())
#define beep_off PCF8574Write(0x20 | PCF8574Read())


byte value;
int Speed = 215;
Adafruit_NeoPixel RGB = Adafruit_NeoPixel(4, PIN, NEO_GRB + NEO_KHZ800);


bool Reculer = false;
bool Avancer = false;


void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);


  pinMode(PWMA, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  analogWrite(PWMA, 150);
  analogWrite(PWMB, 150);
  
  RGB.begin();
  stop();
}


void loop() {
  int i = 0;
  char someChar[32] = {0};


  if (Serial.available()) {
    do {
      someChar[i++] = Serial.read();
      delay(3);
    } while (Serial.available() > 0);


    mySerial.println(someChar);
    Serial.println(someChar);
  }


  while (mySerial.available()) {
    value = mySerial.read();
    Serial.print("Received: ");
    Serial.println(value);


if (value != 0xFF) { 
    if (value == 'A') {
        forward();
        RGB.setPixelColor(0, RGB.Color(0, 255, 0));
        RGB.setPixelColor(1, RGB.Color(0, 255, 0));
        RGB.setPixelColor(2, RGB.Color(0, 255, 0));
        RGB.setPixelColor(3, RGB.Color(0, 255, 0));
        RGB.show();
        Serial.println("Avance");
    } 
    else if (value == 'R') {
        backward();
        RGB.setPixelColor(0, RGB.Color(255, 0, 0));
        RGB.setPixelColor(1, RGB.Color(255, 0, 0));
        RGB.setPixelColor(2, RGB.Color(255, 0, 0));
        RGB.setPixelColor(3, RGB.Color(255, 0, 0)); // Rouge pour reculer
        RGB.show();
        Serial.println("Recule");
    } 
    else if (value == 'D') {
        right();
        RGB.setPixelColor(0, RGB.Color(0, 0, 255));
        RGB.setPixelColor(1, RGB.Color(0, 0, 255));
        RGB.setPixelColor(2, RGB.Color(0, 0, 255));
        RGB.setPixelColor(3, RGB.Color(0, 0, 255)); // Bleu pour tourner à droite
        RGB.show();
        Serial.println("Droite");
    } 
    else if (value == 'G') {
        left();
        RGB.setPixelColor(0, RGB.Color(255, 255, 0));
        RGB.setPixelColor(1, RGB.Color(255, 255, 0));
        RGB.setPixelColor(2, RGB.Color(255, 255, 0));
        RGB.setPixelColor(3, RGB.Color(255, 255, 0)); // Jaune pour tourner à gauche
        RGB.show();
        Serial.println("Gauche");
    } 
    else if (value == 'S') {
        stop();
        Avancer = false;
        RGB.setPixelColor(0, RGB.Color(255, 255, 255));
        RGB.setPixelColor(1, RGB.Color(255, 255, 255));
        RGB.setPixelColor(2, RGB.Color(255, 255, 255));
        RGB.setPixelColor(3, RGB.Color(255, 255, 255)); // Blanc pour stop
        RGB.show();
        Serial.println("Stopping");
    }

    while (value != 0xFF) {
        PCF8574Write(0x1F | PCF8574Read());
        value = PCF8574Read() | 0xE0;
        delay(10);
    }
    }
  }


}


// Fonction Avancer

void forward() {
  analogWrite(PWMA, Speed);
  analogWrite(PWMB, Speed);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
}
//Fonction Reculer

void backward()
{
  analogWrite(PWMA,Speed);
  analogWrite(PWMB,Speed);
  digitalWrite(AIN1,HIGH);
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,HIGH); 
  digitalWrite(BIN2,LOW);  
}
//Fonction Droite

void right()
{
  analogWrite(PWMA,50);
  analogWrite(PWMB,50);
  digitalWrite(AIN1,LOW);
  digitalWrite(AIN2,HIGH);
  digitalWrite(BIN1,HIGH); 
  digitalWrite(BIN2,LOW);  
}
//Fonction Gauche

void left()
{
  analogWrite(PWMA,50);
  analogWrite(PWMB,50);
  digitalWrite(AIN1,HIGH);
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,LOW); 
  digitalWrite(BIN2,HIGH);  
}
void stop() {
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
}


void PCF8574Write(byte data) {
  Wire.beginTransmission(Addr);
  Wire.write(data);
  Wire.endTransmission(); 
}


byte PCF8574Read() {
  int data = -1;
  Wire.requestFrom(Addr, 1);
  if (Wire.available()) {
    data = Wire.read();
  }
  return data;
}