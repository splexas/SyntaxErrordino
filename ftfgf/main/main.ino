#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <Adafruit_BMP085.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);  // I2C address and screen size
Adafruit_BMP085 bmp;                // create a BMP180 object

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  SPI.begin();      // initialize the SPI library
  bmp.begin();      // initialize the BMP180 sensor
}

void loop() {
  float temperature = bmp.readTemperature();    // read the temperature
  lcd.setCursor(0, 0);        // set the cursor to the first line
  lcd.print("Temperature:");  // display the label
  lcd.setCursor(0, 1);        // set the cursor to the second line
  lcd.print(temperature);     // display the temperature
  lcd.print("C");             // display the temperature unit
  delay(1000);                // wait for 1 second
}