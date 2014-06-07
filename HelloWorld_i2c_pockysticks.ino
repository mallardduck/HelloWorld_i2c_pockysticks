//Pockysticks SproutBox
//This is a work in progress to make a Sproutboard fucntion for my project.
//Version 0.0.1
//Branch RGBserialTEST

#include <SoftwareSerial.h>
#include "Wire.h"
#include "LiquidCrystal.h"
#include "DHT.h"

#define DHTPIN 8     // what pin we're connected to
#define REDLITE 5
#define GREENLITE 6
#define BLUELITE 7

// Main program Settings
int wait = 5;      // 10ms internal crossFade delay; increase for slower fades
int hold = 300;       // Optional hold when a color is complete, before the next crossFade
int DEBUG = 0;      // DEBUG counter; if set to 1, will write values back via serial
int loopCount = 60; // How often should DEBUG report?
int repeat = 3;     // How many times should we loop before stopping? (0 for no stop)
int j = 0;          // Loop counter for repeat

// Temp/Humidity Sensor
#define DHTTYPE DHT11   // DHT 11 

// Color arrays [analog values for pin output]
  int white[3]  = { 40, 0, 0 };
  int grey[3]  = { 100, 100, 100 };
  int black[3] = { 255, 255, 255 };
  int red[3]    = { 0, 255, 255 };
  int green[3]  = { 255, 0, 255 };
  int blue[3]   = { 255, 255, 0 };
  int yellow[3] = { 20, 30, 240 };
  int orange[3] = { 0, 100, 240 };
  int teal[3] = { 250, 70, 70 };
  int dimWhite[3] = { 120, 100, 100 };

// Set initial color
int redVal = yellow[0];
int grnVal = yellow[1]; 
int bluVal = yellow[2];

// Initialize color variables
int prevR = redVal;  int prevG = grnVal;  int prevB = bluVal;

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

DHT dht(DHTPIN, DHTTYPE);

// Create a software serial port!
SoftwareSerial lcd = SoftwareSerial(0,2); 

void setup() {
  lcd.begin(9600);
  // set up the LCD's number of rows and columns: 
  lcd.write(0xFE);
  lcd.write(0xD1);
  lcd.write(16);  // 16 columns
  lcd.write(2);   // 2 rows
  lcd.setCursor(0,0);
  lcd.print("DHT11 test!");
  dht.begin();

  pinMode(REDLITE, OUTPUT);
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
//    lcd.setCursor(0,1);
//    lcd.print("Temp.: ");lcd.print(f);lcd.println(" *F");
}

  lcd.setCursor(0,1);
  lcd.println("Grey");
  crossFade(grey);
  lcd.setCursor(0,1);
  lcd.println("Yellow");
  crossFade(yellow);
  lcd.setCursor(0,1);
  lcd.println("black");
  crossFade(black);
  lcd.setCursor(0,1);
  lcd.println("red");
  crossFade(red);
  lcd.setCursor(0,1);
  lcd.println("grey");
  crossFade(grey);
  lcd.setCursor(0,1);
  lcd.println("orange");
  crossFade(orange);
  crossFade(black);
  lcd.setCursor(0,1);
  lcd.println("green");
  crossFade(green);
  crossFade(black);
  lcd.setCursor(0,1);
  lcd.println("teal");
  crossFade(teal);
  crossFade(black);
   lcd.setCursor(0,1);
  lcd.println("blue");
  crossFade(blue);
}

int calculateStep(int prevValue, int endValue) {
  int step = endValue - prevValue; // What's the overall gap?
  if (step) {                      // If its non-zero, 
    step = 1020/step;              //   divide by 1020
  } 
  return step;
}

int calculateVal(int step, int val, int i) {

  if ((step) && i % step == 0) { // If step is non-zero and its time to change a value,
    if (step > 0) {              //   increment the value if step is positive...
      val += 1;           
    } 
    else if (step < 0) {         //   ...or decrement it if step is negative
      val -= 1;
    } 
  }
  // Defensive driving: make sure val stays in the range 0-255
  if (val > 255) {
    val = 255;
  } 
  else if (val < 0) {
    val = 0;
  }
  return val;
}

void crossFade(int color[3]) {
  // Convert to 0-255
  int R = color[0];
  int G = color[1];
  int B = color[2];

  int stepR = calculateStep(prevR, R);
  int stepG = calculateStep(prevG, G); 
  int stepB = calculateStep(prevB, B);

  for (int i = 0; i <= 1020; i++) {
    redVal = calculateVal(stepR, redVal, i);
    grnVal = calculateVal(stepG, grnVal, i);
    bluVal = calculateVal(stepB, bluVal, i);

    analogWrite(REDLITE, redVal);   // Write current values to LED pins
    analogWrite(GREENLITE, grnVal);      
    analogWrite(BLUELITE, bluVal); 

    delay(wait); // Pause for 'wait' milliseconds before resuming the loop

    if (DEBUG) { // If we want serial output, print it at the 
      if (i == 0 or i % loopCount == 0) { // beginning, and every loopCount times
        lcd.setCursor(0,0);
        lcd.print("Loop/RGB: #");
        lcd.print(i);
        lcd.setCursor(0,1);
        lcd.print(redVal);
        lcd.print(" / ");
        lcd.print(grnVal);
        lcd.print(" / ");  
        lcd.println(bluVal); 
      } 
      DEBUG += 1;
    }
  }
  // Update current values for next loop
  prevR = redVal; 
  prevG = grnVal; 
  prevB = bluVal;
  delay(hold); // Pause for optional 'wait' milliseconds before resuming the loop
}
