#include <LiquidCrystal_I2C.h>

#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {

Serial.begin(115200);
Wire.begin(D2, D1);   //Use predefined PINS consts
lcd.begin(20,4);      // The begin call takes the width and height. This
                      // Should match the number provided to the constructor.

lcd.backlight();      // Turn on the backlight.

lcd.home();

lcd.setCursor(0, 0);  // Move the cursor at origin
lcd.print("COUNTING SYSTEM");
lcd.setCursor(6, 1);
lcd.print("20");

}

void loop() {         

// do nothing here 

}