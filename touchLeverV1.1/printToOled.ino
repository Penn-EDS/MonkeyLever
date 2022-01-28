//Uses the SSD1306 Oled

void printToOled(int TValue, char dividerBitVoltage[5]){

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.print("Press enter to set");
    display.setCursor(0,9);
    display.print("TVAL");
    display.setCursor(0,20);
    display.setTextSize(2);
    display.print("TVal:");
    display.print(TValue);
    //display.setCursor(0,20);
    //display.print("R:");
    //display.print(resValue);
    //display.setCursor(100,20);
    //display.print("%");
    display.setCursor(0,40);  
    display.print("Div:");
    display.print(dividerBitVoltage);  
    display.display();
}
