#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>


#define PWMA   6           //Left Motor Speed pin (ENA)
#define AIN2   A0          //Motor-L forward (IN2).
#define AIN1   A1          //Motor-L backward (IN1)
#define PWMB   5           //Right Motor Speed pin (ENB)
#define BIN1   A2          //Motor-R forward (IN3)
#define BIN2   A3          //Motor-R backward (IN4)
#define PIN 7
#define rxPin 2
#define txPin 3
SoftwareSerial mySerial(rxPin, txPin);

#define Addr  0x20

#define beep_on  PCF8574Write(0xDF & PCF8574Read())
#define beep_off PCF8574Write(0x20 | PCF8574Read())

byte value;
int Speed = 255;
void PCF8574Write(byte data);
byte PCF8574Read();
void forward();
void backward();
void right();
void left();
void stop();
Adafruit_NeoPixel RGB = Adafruit_NeoPixel(4, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);


  Wire.begin();
  pinMode(PWMA,OUTPUT);                     
  pinMode(AIN2,OUTPUT);      
  pinMode(AIN1,OUTPUT);
  pinMode(PWMB,OUTPUT);       
  pinMode(AIN1,OUTPUT);     
  pinMode(AIN2,OUTPUT);  
  analogWrite(PWMA,150);
  analogWrite(PWMB,150);
  stop(); 
}

void loop() {

int i = 0;
char someChar[32] = {0};
// when characters arrive over the serial port...
if(Serial.available()) {
 do{
 someChar[i++] = Serial.read();
 delay(3);
 }while (Serial.available() > 0);
 mySerial.println(someChar);
 Serial.println(someChar);
}
while(mySerial.available())
 Serial.print((char)mySerial.read());
// when characters arrive over the serial port...
  value = mySerial.read();
  Serial.println(value);
  if(value != 0xFF)
  {
    switch(value)
    { 
      case 0xFE:
        forward();
        Serial.println("Forward");
        RGB.begin();
        RGB.setPixelColor(0, RGB.Color(0, 255, 0));
        RGB.setPixelColor(1, RGB.Color(0, 255, 0));
        RGB.setPixelColor(2, RGB.Color(0, 255, 0));
        RGB.setPixelColor(3, RGB.Color(0, 255, 0));
        RGB.show();
        break;
      case 0xFD:
        right();
        Serial.println("right");
        RGB.begin();
        RGB.setPixelColor(0, RGB.Color(255, 255, 0));
        RGB.setPixelColor(1, RGB.Color(255, 255, 0));
        RGB.setPixelColor(2, RGB.Color(255, 255, 0));
        RGB.setPixelColor(3, RGB.Color(255, 255, 0));
        RGB.show(); break;
      case 0xFB:
        left();
        Serial.println("left");break; 
      case 0xF7:
        backward();
        Serial.println("down");
        RGB.setPixelColor(0, RGB.Color(255, 0, 0));
        RGB.setPixelColor(1, RGB.Color(255, 0, 0));
        RGB.setPixelColor(2, RGB.Color(255, 0, 0));
        RGB.setPixelColor(3, RGB.Color(255, 0, 0));
        break;
      case 0xEF:
        forward();
        Serial.println("center");break;
    }
    while(value != 0xFF)
    {
      PCF8574Write(0x1F | PCF8574Read());
      value = PCF8574Read() | 0xE0;
      delay(10);
    }
    stop();  
  }
}

void PCF8574Write(byte data)
{
  Wire.beginTransmission(Addr);
  Wire.write(data);
  Wire.endTransmission(); 
}

byte PCF8574Read()
{
  int data = -1;
  Wire.requestFrom(Addr, 1);
  if(Wire.available()) {
    data = Wire.read();
  }
  return data;
}

void forward()
{
  analogWrite(PWMA,Speed);
  analogWrite(PWMB,Speed);
  digitalWrite(AIN1,LOW);
  digitalWrite(AIN2,HIGH);
  digitalWrite(BIN1,LOW);  
  digitalWrite(BIN2,HIGH); 
}

void backward()
{
  analogWrite(PWMA,Speed);
  analogWrite(PWMB,Speed);
  digitalWrite(AIN1,HIGH);
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,HIGH); 
  digitalWrite(BIN2,LOW);  
}

void right()
{
  analogWrite(PWMA,50);
  analogWrite(PWMB,50);
  digitalWrite(AIN1,LOW);
  digitalWrite(AIN2,HIGH);
  digitalWrite(BIN1,HIGH); 
  digitalWrite(BIN2,LOW);  
}

void left()
{
  analogWrite(PWMA,50);
  analogWrite(PWMB,50);
  digitalWrite(AIN1,HIGH);
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,LOW); 
  digitalWrite(BIN2,HIGH);  
}

void stop()
{
  analogWrite(PWMA,0);
  analogWrite(PWMB,0);
  digitalWrite(AIN1,LOW);
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,LOW); 
  digitalWrite(BIN2,LOW);  
}
