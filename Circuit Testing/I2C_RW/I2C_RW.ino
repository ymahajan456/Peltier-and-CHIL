#include <Wire.h>
int readInterval = 20;

void INA260Setup(float msReadInterval){
  float conversionTimes[8] = {0.14, 0.204, 0.332, 0.588, 1.1, 2.116, 4.156, 8.224};
  int averages[8] = {1, 4, 16, 64, 128, 256, 512, 1024};

  int conversionIndex = 4;
  int averagesIndex = 0;
  if (msReadInterval > conversionTimes[7]){
    conversionIndex = 7;
  }
  else{
    for (int i = 0; i < 8; i++){
      if (msReadInterval / conversionTimes[conversionIndex] < 0.25){
         conversionIndex = i - 1;
         break;
      }
    }
  }
  for (int i = 0; i < 8; i++){
    if ((msReadInterval / conversionTimes[conversionIndex] / averages[i]) < 1){
      averagesIndex = i - 1;
      break;
    }
  }

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
  Wire.begin();         // join I2C Bus
  Serial.begin(9600);   // Start Serial to print output
  INA260Setup(readInterval);
}

float current = 0;
void loop() {
  //Code to read current from INA260 IC
  current = readCurrent();
  if (current < 0)
    Serial.print("-");
  Serial.print(abs(current), 2);
  Serial.println(" mA");         //Get to next line
  delay(readInterval);

}
