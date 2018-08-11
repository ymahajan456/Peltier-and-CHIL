#include <SPI.h>

// set pin 10 as the slave select for the digital pot:
const int slaveSelectPinDAC = 10;
const int relaySwitch = 8;

const int analogVoltage = A0;
const int analogCurrent = A1;
const int pwmPin = 3;

int PWM = 0;
int toSend = 0;
int i = 0;
int add = 1;

float V1 = 0, V2 = 0, current = 0, VDUT = 0;
bool state = true;

void MCP4912Write(unsigned int value) {
  value = value % 4096; //Convert to 12 bits;
  value += 3 << 12; 
  // take the SS pin low to select the chip:
  SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
  digitalWrite(slaveSelectPinDAC, LOW);
  //send in the address and value via SPI:
  SPI.transfer16(value);
  // take the SS pin high to de-select the chip:
  digitalWrite(slaveSelectPinDAC, HIGH);
  SPI.endTransaction();
}

/*float MCP3008Read(int channel){
  SPI.beginTransaction(SPISettings(30000, MSBFIRST, SPI_MODE0));
  digitalWrite(slaveSelectPinADC, LOW);
  delay(10);
  SPI.transfer(0x01);
  int value = (8 + channel) << 12;
  int data = SPI.transfer16(value);
  digitalWrite(slaveSelectPinADC, HIGH);
  delay(10);
  SPI.endTransaction();
  
  return (data * 5.0 / 1023.0);
}*/

void setup() {
  // set the slaveSelectPin as an output:
  pinMode(relaySwitch, OUTPUT);
  pinMode(slaveSelectPinDAC, OUTPUT);
  pinMode(pwmPin, OUTPUT);

  digitalWrite(relaySwitch, LOW);
  Serial.begin(9600);
  // initialize SPI:
  SPI.begin();
}

void loop() {
  if(Serial.available()){
    PWM = Serial.parseInt();
    analogWrite(pwmPin, PWM);
  } 
  
  MCP4912Write(819*(i/100.0));
  delay(100);
  V1 = (5.0*analogRead(analogVoltage))/1023;
  V2 = (5.0*analogRead(analogCurrent))/1023;

  VDUT = (V2 - V1/11.0) * (state ? 1 : -1);
  current = (V1*10)/1.1;
  
  //Serial.print("V1: ");
  if (VDUT < 0)
    Serial.print(" -");
  Serial.print(abs(VDUT));
  Serial.print(", ");
  if(VDUT < 0)
    Serial.print("-");
  Serial.println(abs(current));
  if(current > 20 || abs(VDUT) > 4.5 ){
    add = -1;
    i--;
  }
  else if (i == 0){
    digitalWrite(relaySwitch, (state ? HIGH: LOW));
    state  = !state;
    i++;
    add = 1;
  }
  else
    i += add;
  delay(30);
  
}
