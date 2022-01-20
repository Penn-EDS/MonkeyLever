//Uses the SSD1306 Oled

void displayTvalSet(int TValue){

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.print("Press enter for next digit");
    display.setTextSize(2);
    display.setCursor(0,20);
    display.setCursor(0,40);  
    display.print("Tval: ");
    display.print(TValue);  
    display.display();
}
