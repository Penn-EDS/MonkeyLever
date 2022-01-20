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

#include <stdbool.h>
#include <SPI.h>
#include <Wire.h>
#include <Bounce2.h>
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

const int source = A1;              //Analog reading of the baseResistance at the source side.
const int vo2 = A2;                 //Analog reading of the baseResistance at the potentiometer side. Used to calculate current.
const int touchLever = A0;          //Select analog pin for the lever
const int thresholdValueInput = A3; //This analog pins reads the voltage divider of a potentiometer to act as a knob.
const int ledPin = 13;              //Select the pin for the LED
const int output = 6;               //Digital output for the device

int thresholdValue = 1012;           //Threshold Voltage to detect touch. Input by the user. Default to 800
int sensorValue = 0;                //Variable to store the value coming from the touchLeverPin.
int sourceBitValue = 0;             //Variables to store analog readings
int vo2BitValue = 0;
int touchLeverBitValue = 0;

int enter = HIGH;                   //Variables to hold button state
int up = HIGH;
int debTime = 50;                    //Debounce time
int ones = 0;
int tens = 0;
int hundreds = 0;
int thousands = 0;

float baseResistance = 776.6;       //The units are kΩ. This resistance prevents the potentiometer that leads to make a short circuit, it also 
                                    //raises resistance floor. It will also be used to measure the resistance of the potentiometer. 
                                    //Its value is determined beforehand.
float potentiometer = 0.0;          //Variable to store potentiometer resistance.
float topVoltDiv = 0.0;             //Variable to sum both pot and base resistance to show on the display.

bool tvalSet = false;                //flag to see if TVal has been set

int unit = 1;                      //Variable to cycle through units

Bounce debEnter = Bounce();         //Bounce object for the buttons
Bounce debUp = Bounce(); 

void setup() {
  // put your setup code here, to run once:
  delay(200); //Let Arduino start

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x64)
  display.clearDisplay();

  pinMode(BUTTON_PIN_1,INPUT_PULLUP);   // Setup the first button with an internal pull-up :
  debEnter.attach(BUTTON_PIN_1);         // After setting up the button, setup the Bounce instance :
  debEnter.interval(debTime);                  // interval in ms
     
  pinMode(BUTTON_PIN_2,INPUT_PULLUP);   // Setup the second button with an internal pull-up :
  debUp.attach(BUTTON_PIN_2);            // After setting up the button, setup the Bounce instance :
  debUp.interval(debTime);                     // interval in ms

  pinMode(ledPin, OUTPUT);
  pinMode(output, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("Begin");

  ones = thresholdValue / 10;
  ones = thresholdValue * 10;
  ones = thresholdValue - ones;
  tens = thresholdValue / 100;
  tens = thresholdValue * 100;
  tens = thresholdValue - tens;

  while(not tvalSet){
    displayTvalSet(thresholdValue);  
    debEnter.update();  // Update the Bounce instances
    debUp.update();

    enter = debEnter.read();  // Get the updated value for buttons
    up = debUp.read();
    
    //Serial.println(ones);
    //Serial.println(tens);
    //Serial.println(hundreds);
    //Serial.println("**************");  


      ones = thresholdValue / 10;
      Serial.println(ones);
      ones = ones * 10;
      Serial.println(ones);
      ones = thresholdValue - ones;
      Serial.println(ones);
      Serial.println("**************");
      tens = thresholdValue / 100;
      tens = tens * 100;
      tens = thresholdValue - tens;
      hundreds = thresholdValue / 1000;
      hundreds = hundreds * 1000;
      hundreds = thresholdValue - hundreds;  
    if (enter == LOW){
      unit += 1;
      if (unit >= 5){
        tvalSet = true;
      }
    }

    if (up == LOW){          
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
          if ((hundreds) < 90){
            thresholdValue += 10;
          }
          else{
            thresholdValue -= 90;
          }
          break;
        case 3:
          if (thousands < 900){
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
}

void loop() {
  // put your main code here, to run repeatedly:

  //thresholdValue = analogRead(thresholdValueInput);  //Read values from the analog inputs
  sourceBitValue = analogRead(source);
  vo2BitValue = analogRead(vo2);
  touchLeverBitValue = analogRead(touchLever);

  potentiometer = potReading(sourceBitValue,vo2BitValue,touchLeverBitValue); //Get current resistance value of the potentiometer.

  topVoltDiv = baseResistance + potentiometer;

  if (touchLeverBitValue <951){
    printToOled(thresholdValue, touchLeverBitValue, topVoltDiv);  //update display
  }
  else{
    printToOled(thresholdValue, touchLeverBitValue, -1);  //update display
  }
  
  if (touchLeverBitValue > thresholdValue){
    digitalWrite(ledPin, LOW); 
    digitalWrite(output, LOW); //When the lever is not touched, the output is low
  }
  else{
  digitalWrite(ledPin, HIGH);
  digitalWrite(output, HIGH);  //When the lever is touched, the output is high
  }
  
  delay(25);
}
