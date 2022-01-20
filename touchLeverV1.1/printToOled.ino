//Uses the SSD1306 Oled

void printToOled(int TValue,int dividerBitVoltage, int resValue){

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.print("TVal:");
    display.print(TValue);
    display.setCursor(0,20);
    display.print("R:");
    display.print(resValue);
    display.setCursor(100,20);
    display.print("kO");
    display.setCursor(0,40);  
    display.print("Div:");
    display.print(dividerBitVoltage);  
    display.display();
}
