float potReading(int vo1BitValue,int vo2BitValue,int vo3BitValue){

float vo1DecValue = 0.0;            //Decimal value for the voltage readings
float vo2DecValue = 0.0;
float vo3DecValue = 0.0;
float resCurrent = 0.0;             //Current that passses through the baseResistor. Units will be calculated in mA.
float potValue = 0.0;            //The units are kΩ. The calculated value for the potentiometer.
float convertFromADC = 5.0/1023.0;  //Constant used to convert ADC readings into voltage values.

   vo1DecValue = vo1BitValue * convertFromADC;
   vo2DecValue = vo2BitValue * convertFromADC;
   vo3DecValue = vo3BitValue * convertFromADC;
   resCurrent = (vo1DecValue - vo2DecValue) / baseResistance;
   potValue = (vo2DecValue - vo3DecValue) / resCurrent;


   Serial.println(vo1BitValue);
   Serial.println(vo2BitValue);
   Serial.println(vo3BitValue);   
   Serial.print("First Voltage = ");
   Serial.println(vo1DecValue);
   Serial.print("Second Voltage = ");
   Serial.println(vo2DecValue);
   Serial.print("Third Voltage = ");
   Serial.println(vo3DecValue);
   Serial.print("Current = ");
   Serial.print(resCurrent * 1000);
   Serial.println("uA");
   Serial.print("Pot value is = ");
   Serial.println(potValue);

   return potValue;
  
}
