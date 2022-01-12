/*************************************************** 
  Version 1.0
  This programs measures a voltage to determine if a lever is being touched and uses a digital pin as an output.
  LED 13 is also toggled to signal the lever being touched.
 ****************************************************/

const int touchLeverPin = A0;  //select analog pin for the lever
const int ledPin = 13;      // select the pin for the LED
const int output = 6;       // digital output for the device
int sensorValue = 0;        // variable to store the value coming from the 
const int M = 700;          // value in bits (Max is 1023) to detect whether the lever is being touched or not
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //pinMode(A0,INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(output, OUTPUT);
  
  Serial.println("Begin");
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorValue = analogRead(touchLeverPin);
  Serial.println(sensorValue);
  if (sensorValue > M){
    digitalWrite(ledPin, LOW); 
    digitalWrite(output, LOW); //When the lever is not touched, the output is low
  }
  else{
  digitalWrite(ledPin, HIGH);
  digitalWrite(output, HIGH);  //When the lever is touched, the output is high
  }
  delay(10);
}
