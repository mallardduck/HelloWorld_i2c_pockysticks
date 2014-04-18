// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "Wire.h"
#include "LiquidCrystal.h"
#include "DHT.h"

#define DHTPIN 2     // what pin we're connected to
#define REDLITE 3
#define GREENLITE 5
#define BLUELITE 6

// Color arrays
int black[3]  = { 0, 0, 0 };
int white[3]  = { 100, 100, 100 };
int red[3]    = { 100, 0, 0 };
int green[3]  = { 0, 100, 0 };
int blue[3]   = { 0, 0, 100 };
int yellow[3] = { 40, 95, 0 };
int dimWhite[3] = { 30, 30, 30 };

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11 
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect via i2c, default address #0 (A0-A2 not jumpered)
LiquidCrystal lcd(0);

// you can change the overall brightness by range 0 -> 255
int brightness = 255;

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // set up the LCD's number of rows and columns: 
  lcd.begin(16, 2);
  lcd.println("DHT11 test!");
  lcd.setCursor(0,1);
  dht.begin();

  pinMode(REDLITE, OUTPUT);
  pinMode(GREENLITE, OUTPUT);
  pinMode(BLUELITE, OUTPUT);
}

void loop() {
   for (int i = 0; i < 255; i++) {
setBacklight(i, 0, 255-i);
delay(5);
}
for (int i = 0; i < 255; i++) {
setBacklight(255-i, i, 0);
delay(5);
}
for (int i = 0; i < 255; i++) {
setBacklight(0, 255-i, i);
delay(5);
}
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float Temp = dht.readTemperature();
  float f = ((9/5)*Temp)+32;

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(Temp) || isnan(h)) {
    lcd.println("Failed to read from DHT");
  } else {
    lcd.print("Humidity: ");lcd.print(h);lcd.print(" %\t");
    lcd.print("");
    lcd.print("Temperature: ");lcd.print(f);lcd.println(" *F");
  }
}


void setBacklight(uint8_t r, uint8_t g, uint8_t b) {
// normalize the red LED - its brighter than the rest!
r = map(r, 0, 255, 0, 100);
g = map(g, 0, 255, 0, 150);
r = map(r, 0, 255, 0, brightness);
g = map(g, 0, 255, 0, brightness);
b = map(b, 0, 255, 0, brightness);
// common anode so invert!
r = map(r, 0, 255, 255, 0);
g = map(g, 0, 255, 255, 0);
b = map(b, 0, 255, 255, 0);
analogWrite(REDLITE, r);
analogWrite(GREENLITE, g);
analogWrite(BLUELITE, b);
}
