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

const int source = A1;              //Analog reading of the baseResistance at the source side.
const int vo2 = A2;                 //Analog reading of the baseResistance at the potentiometer side. Used to calculate current.
const int touchLever = A0;          //Select analog pin for the lever
const int thresholdValueInput = A3; //This analog pins reads the voltage divider of a potentiometer to act as a knob.
const int ledPin = 13;              //Select the pin for the LED
const int output = 6;               //Digital output for the device

//const int M = 700;                //Value in bits (Max is 1023) to detect whether the lever is being touched or not. 
                                    //Old variable from when it was const and given on the screen.*/
int thresholdValue = 0;             //Threshold Voltage to detect touch. Input by the user.
int sensorValue = 0;                //Variable to store the value coming from the touchLeverPin.
int sourceBitValue = 0;             //Variables to store analog readings
int vo2BitValue = 0;
int touchLeverBitValue = 0;

float baseResistance = 776.6;       //The units are kΩ. This resistance prevents the potentiometer that leads to make a short circuit, it also 
                                    //raises resistance floor. It will also be used to measure the resistance of the potentiometer. 
                                    //Its value is determined beforehand.
float potentiometer = 0.0;          //Variable to store potentiometer resistance.

void setup() {
  // put your setup code here, to run once:
  delay(200); //Let Arduino start

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x64)
  display.clearDisplay();
  
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(output, OUTPUT);
  Serial.println("Begin");
}

void loop() {
  // put your main code here, to run repeatedly:

  thresholdValue = analogRead(thresholdValueInput);  //Read values from the analog inputs
  sourceBitValue = analogRead(source);
  vo2BitValue = analogRead(vo2);
  touchLeverBitValue = analogRead(touchLever);

  potentiometer = potReading(sourceBitValue,vo2BitValue,touchLeverBitValue); //Get current resistance value of the potentiometer.

  printToOled(thresholdValue, touchLeverBitValue, potentiometer);  //update display

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
