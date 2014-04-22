// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "Wire.h"
#include "LiquidCrystal.h"
#include "DHT.h"

#define DHTPIN 7     // what pin we're connected to
#define GREENLITE 5
#define BLUELITE 6

int wait = 5;      // 10ms internal crossFade delay; increase for slower fades
int hold = 0;       // Optional hold when a color is complete, before the next crossFade
int DEBUG = 0;      // DEBUG counter; if set to 1, will write values back via serial
int loopCount = 60; // How often should DEBUG report?
int repeat = 3;     // How many times should we loop before stopping? (0 for no stop)
int j = 0;          // Loop counter for repeat

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11 
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect via SPI. Data pin is #3, Clock is #2 and Latch is #4, Green is #5, Blue is #6
LiquidCrystal lcd(3, 2, 4);

// you can change the overall brightness by range 0 -> 255

int blueTemp= 0; int greenTemp= 0; int redTemp= 0;


// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  // set up the LCD's number of rows and columns: 
  lcd.begin(16, 2);
  lcd.print("DHT11 test!");
  lcd.setCursor(0,1);
  dht.begin();

  pinMode(GREENLITE, OUTPUT);
  pinMode(BLUELITE, OUTPUT);
  }

void loop() {
  if (repeat) { // Do we loop a finite number of times?
    j += 1;
    if (j >= repeat) { // Are we there yet?
      exit(j);         // If so, stop.
    }
  }
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float Temp = dht.readTemperature();
  float f = ((9/5)*Temp)+32;

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if ( isnan(h)) {
    lcd.println("Failed to read from DHT");
  } else {
    lcd.setCursor(0,0);
    lcd.print("Humidity: ");lcd.print(h);lcd.print(" %\t");
    lcd.setCursor(0,1);
    lcd.print("Temp.: ");lcd.print(f);lcd.println(" *F");
  }
    analogWrite(GREENLITE, 255);  // IDK WTF, but at 0(for both of these) you get white, but at 255 on this one you get a purple color. 
    analogWrite(BLUELITE, 255); // IDK WTF, but at 0(for both of these) you get white, but at 255 on this one you get a yellow/green color. 


    lcd.setBacklight(OFF);
}


