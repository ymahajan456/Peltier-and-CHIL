
const int sensorPin = A0;
const int current_ctrl_pin = 3;

int req_temp = 0;
int sensor_temp = 0;
int current = 0;
const int Kp = 0;
const int Kd = 0;
const int Ki = 0;

// Specified as degree celsius change per voltage change
const int Kt = 100;

int voltage = 0;
int current_error = 0;
int diff_error = 0;
int integral_error = 0;
int old_error = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0){
    req_temp = Serial.parseInt();
  }
  sensor_temp = analogRead(sensorPin);
  
  sensor_temp = Kt*(sensor_temp*5/1023);
  
  current_error = req_temp - sensor_temp;
  integral_error = integral_error + current_error;
  /////////////////////////////////////////////////////////////////
  error = Kp*(current_error) + Kd*(diff_error) + Ki*integral_error;
  /////////////////////////////////////////////////////////////////
  diff_error = error - old_error;
  old_error = error;

  if(error > 255){
    current = 255;
  }
  else if(error < 0){
    current = 0;
  }
  else{
    current = error;
  }
  // Gives a PWM output ranging from 0 to 255
  analogWrite(current_ctrl_pin,current); 
}
