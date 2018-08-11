#include <Wire.h>

int LM35PlusPin = A0;
int LM35MinusPin = A1;

int PWMPin = 2;

float p = 40.0, i = 0.0001;
int setPoint = 35;
long integral = 0;
short error = 0;

float conversionTimes[8] = {0.14, 0.204, 0.332, 0.588, 1.1, 2.116, 4.156, 8.224};
int averages[8] = {1, 4, 16, 64, 128, 256, 512, 1024};

void INA260Setup(){  
  int averagesIndex = 4;
  int conversionIndex = 0;
  
  Wire.beginTransmission(0b1000000);
  Wire.write(0x00);                                     //Point to the configuration register
  Wire.write((0b110 << 4) + (averagesIndex << 1) + 1);  //MSB
  Wire.write((conversionIndex << 3) + 0b101);           //LSB - Mode Continuous Shunt Current
  Wire.endTransmission();
}

float readCurrent(){
  Wire.beginTransmission(0b1000000);
  Wire.write(0x01);                     //Point the Register Pointer to the Current Register
  Wire.endTransmission();               //Stop Transmitting

  int readValue = 0;
  byte c = 0;
  //Read from the Device
  Wire.requestFrom(0b1000000, 2);       //Request 2 bytes from device with address 64
  while (Wire.available()){
    readValue += c;
    c = Wire.read();     //Receive a byte as a character
  }
  readValue = (int) word(readValue, c);
  return ((float) readValue * 1.25);             //Conversion to mA - given least count 1.25mA
}

void setup() {
  // put your setup code here, to run once:
  Wire.begin();         // join I2C Bus
  Serial.begin(9600);   // Start Serial to print output
  
  pinMode(LM35PlusPin, INPUT);
  pinMode(LM35MinusPin, INPUT);
  pinMode(PWMPin, OUTPUT);
  
  INA260Setup();
}

void loop() {
  if (Serial.available()){
      setPoint = Serial.parseInt();
  }
  error = (setPoint - (analogRead(LM35PlusPin) - analogRead(LM35MinusPin))*5/10.23);
  analogWrite(PWMPin, max(0, min(225, p*error + i*integral)));
  integral += error;
  Serial.print(setPoint - error);
  Serial.print(", ");
  Serial.println(readCurrent());
  delay(10);
}
