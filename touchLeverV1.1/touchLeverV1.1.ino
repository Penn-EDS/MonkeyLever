/*************************************************** 
  Version 1.1
  Written by Pedro Quijano
  This programs measures a voltage to determine if a lever is being touched and uses a digital pin as an output.
  LED 13 is also toggled to signal the lever being touched.

  Revision added Oled display and manual threshold and internal resistance manipulation
 ****************************************************/
 /******************************
  ***********Oled Setup*********
  ******************************/

#include <string.h>
#include <stdbool.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//#define OLED_RESET -1
//Adafruit_SSD1306 display(OLED_RESET);

//#if (SSD1306_LCDHEIGHT != 64)
//#error("Height incorrect, please fix Adafruit_SSD1306.h!");
//#endif

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

 /******************************
  ***********Variables**********
  ******************************/

#define BUTTON_PIN_1 2              //Setting pins D2 and D3 as buttons
#define BUTTON_PIN_2 3              

char volDiv[2][6];                  //Strings to show for A0 and Threshold Value comparison

const int vo2 = A2;                 //Analog reading of the baseResistance at the potentiometer side. Used to calculate current.
const int touchLever = A0;          //Select analog pin for the lever
const int ledPin = 13;              //Select the pin for the LED
const int output = 6;               //Digital output for the device

int thresholdValue = 750;           //Threshold Voltage to detect touch. Input by the user. Default to 153 or 3 Volts.
int sensorValue = 0;                //Variable to store the value coming from the touchLeverPin.
int vo2BitValue = 0;
int touchLeverBitValue = 0;

int enter = HIGH;                   //Variables to hold button state
int up = HIGH;
int ones = 0;
int tens = 0;
int hundreds = 0;
int thousands = 0;

int potPercent[11] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
int potPerVal = 0;

float baseResistance = 1000;        //The units are kΩ. This resistance prevents the potentiometer that leads to make a short circuit, it also 
                                    //raises resistance floor. It will also be used to measure the resistance of the potentiometer. 
                                    //Its value is determined beforehand.
float potentiometer = 0.0;          //Variable to store potentiometer resistance.

bool tvalSet = false;               //flag to see if TVal has been set

int unit = 1;                       //Variable to cycle through units

void setup() {
  // put your setup code here, to run once:
  delay(1000); //Let Arduino start

  strcpy(volDiv[0], "Above");       //Strings for Voltage Divider and Touch Lever comparison
  strcpy(volDiv[1], "Below");

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x64)
  display.clearDisplay();

  pinMode(BUTTON_PIN_1,INPUT_PULLUP);   // Setup the first button with an internal pull-up
  pinMode(BUTTON_PIN_2,INPUT_PULLUP);   // Setup the second button with an internal pull-up 

  pinMode(ledPin, OUTPUT);
  pinMode(output, OUTPUT);

  Serial.begin(9600);
  Serial.println("Begin");

  enter = HIGH;
  tvalSet = HIGH;

  delay(100);   //At the moment this makes the program not run the threshold selection on startup
}

void loop() {
  // put your main code here, to run repeatedly:

  enter = digitalRead(BUTTON_PIN_1);
  
  if (enter == LOW){  //If enter button is pressed at any time in the program, threshold value selection starts
    delay(200);
    
    tvalSet = false;    //Resets conditions to exit loop
    unit = 1;
    
    while(not tvalSet){
      
      displayTvalSet(thresholdValue);

      enter = digitalRead(BUTTON_PIN_1);
      up = digitalRead(BUTTON_PIN_2);
    
      //Serial.println(ones);
      //Serial.println(tens);
      //Serial.println(hundreds);
      //Serial.println("**************");  
        ones = thresholdValue / 10;
        //Serial.println(ones);
        ones = ones * 10;
        //Serial.println(ones);
        ones = thresholdValue - ones;
        //Serial.println(ones);
        //Serial.println("**************");
        tens = thresholdValue / 100;
        tens = tens * 100;
        tens = thresholdValue - tens;
        hundreds = thresholdValue / 1000;
        Serial.println(hundreds);
        hundreds = hundreds * 1000;
        Serial.println(hundreds);
        hundreds = thresholdValue - hundreds;
        Serial.println(hundreds);  
        Serial.println("**************");
        
      if (enter == LOW){  //If enter is pressed, got to next unit
        delay(200);
        
        unit += 1;
        
        if (unit >= 5){   //If all units have been set, set variable to end threshold value selection
          tvalSet = true;
        }
      }

      if (up == LOW){     //Cycle through numbers 0 to 9 (in most cases)
        delay(200);
        
        switch(unit){
          case 1:
            if ((ones) < 9){
            thresholdValue += 1;
            }
            else{
              thresholdValue -= 9;
            }
            break;
          case 2:
            if ((tens) < 90){
              thresholdValue += 10;
            }
            else{
              thresholdValue -= 90;
            }
            break;
          case 3:
            if (hundreds < 900){
              thresholdValue += 100;
            }
            else{
              thresholdValue -= 900;
            }
            break;
          case 4:
            if ((thresholdValue) < 1000){
              thresholdValue += 1000;
            }
            else{
              thresholdValue -= 1000;
            }
            break;
        }
      }
    } 
    if (thresholdValue > 1023){  //Does not let threshold value be greater than the 10-bit value the ADC reads
      thresholdValue = 1023;
    }
  }
  
  vo2BitValue = analogRead(vo2);  //Read values from the analog inputs
  touchLeverBitValue = analogRead(touchLever);

  potentiometer = potReading(1023,vo2BitValue,touchLeverBitValue); //Get current resistance value of the potentiometer.
  potentiometer *= 2.5; //Calibration constant
  
  Serial.print("Potentiometer in kohms:");
  Serial.println(potentiometer);
  
  
    potPerVal = (potentiometer) / 450;  //Divides the potentiometer readings in 
  
  if (potPerVal > 10){
    potPerVal = 10;
  }
  else if (potPerVal < 0){
    potPerVal = 0;
  }
  
  if (touchLeverBitValue > thresholdValue){
    printToOled(thresholdValue, volDiv[0]);  //update display
    digitalWrite(ledPin, LOW); 
    digitalWrite(output, LOW); //When the lever is not touched, the output is low
  }
  else{
    printToOled(thresholdValue, volDiv[1]);  //update display
    digitalWrite(ledPin, HIGH);
    digitalWrite(output, HIGH);  //When the lever is touched, the output is high
  }
  
  delay(10);
}
