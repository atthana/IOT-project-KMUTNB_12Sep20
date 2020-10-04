#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define b 34

int analogPin = 15;
int val = 0;
int buzz = 2;
int ledPin = 4;
int count = 0;
int button = 36;
int valb = 0;
LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup()
{
    Serial.begin(9600);
    pinMode(buzz, OUTPUT);
    pinMode(ledPin, OUTPUT);
    pinMode(button, INPUT);
    pinMode(b, INPUT);
    lcd.begin();
}
void loop()
{
    val = analogRead(analogPin);
    Serial.print("val = ");
    Serial.println(val);
    Serial.print("count = ");
    Serial.println(count);
    digitalWrite(buzz, 1);
    if (valb == 0)
    {
        count = 0;
        lcd.setCursor(0, 1);
        lcd.print("          ");
        delay(500);
    }
    if (val > 4000)
    {
        digitalWrite(ledPin, 0);
        digitalWrite(buzz, 0);
    }
    else
    {
        digitalWrite(ledPin, 1);
        digitalWrite(buzz, 1);
        delay(100);
        count++;
    }
    digitalWrite(buzz, 0);

    digitalWrite(ledPin, 0);
    lcd.setCursor(0, 0);
    lcd.print("count: ");
    lcd.setCursor(0, 1);
    lcd.print(count);
    delay(100);
    valb = digitalRead(b);
    Serial.println(valb);
    delay(500);
}