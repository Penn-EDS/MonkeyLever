//This program reads the current of a voltage divider to determine one of the resistors value (a potentiometer).
//For now the potentiometer value is being read correctly (tested with a 1kΩ resistor and a 5kΩ potentiometer)

//float convertADC = 5.0/1023.0;   //Constant used to convert ADC readings into voltage values.
//float baseResistance = 776.6;  //The units are kΩ. This resistance prevents the potentiometer that leads to make a short circuit, it also 
                             //raises resistance floor.It will also be used to measure the resistance of the potentiometer. 
                             //Its value is determined beforehand.
float baseResistance = 0.9898;
float potValue = 0.0;            //The units are kΩ. The calculated value for the potentiometer.
const int vo1 = A1;                //analog reading of the baseResistance at the source side.
const int vo2 = A2;                //analog reading of the baseResistance at the potentiometer side.
const int vo3 = A0;                ///analog reading of the other side of the potentiometer.
int vo1BitValue = 0;
int vo2BitValue = 0;
int vo3BitValue = 0;
float vo1DecValue = 0.0;
float vo2DecValue = 0.0;
float vo3DecValue = 0.0;
float resCurrent = 0.0;  //Current that passses through the baseResistor. The current units are mA.


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  Serial.println("Begin");
//  Serial.print("convertADC value = ");
//  Serial.println(convertADC);
  Serial.print("baseResistance value = ");
  Serial.println(baseResistance);
}

void loop() {
  // put your main code here, to run repeatedly:
   vo1BitValue = analogRead(vo1);
   vo2BitValue = analogRead(vo2);
   vo3BitValue = analogRead(vo3);
   vo1DecValue = vo1BitValue * (5.0/1023.0);
   vo2DecValue = vo2BitValue * (5.0/1023.0);
   vo3DecValue = vo3BitValue * (5.0/1023.0);
   resCurrent = (vo1DecValue - vo2DecValue) / baseResistance;
   potValue = (vo2DecValue - vo3DecValue) / resCurrent;
   Serial.println(vo1BitValue);
   Serial.println(vo2BitValue);
   Serial.println(vo3BitValue);   
   Serial.print("First Voltage = ");
   Serial.println(vo1DecValue);
   Serial.print("Second Voltage = ");
   Serial.println(vo1DecValue);
   Serial.print("Third Voltage = ");
   Serial.println(vo1DecValue);
   Serial.print("Current = ");
   Serial.println(vo1DecValue);
   Serial.print("Pot value is = ");
   Serial.println(potValue);
   delay(1000);

   
   
   
}
