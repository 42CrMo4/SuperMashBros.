/*
SuperMashBros. 2020
Spielmechanik für Cocktail Roboter
*/

#include <FastLED.h>

// setting up Pin arrays for buttons and LEDs to access them efficiently.
//It is important that LED1 and Button1 are in the same location within their respective array
const int commonPin = 2;
int buttonPins[] = {4, 5, 6, 7};
CRGB colourArray[] = {CRGB::Black,CRGB::Red,CRGB::Green,CRGB::Blue};
#define NUM_LEDS 4
#define DATA_PIN 53

//misc
int p = 0;
unsigned long previousMillis = 0;
unsigned long lastFire = 0;   //counter for button interrupt time since press
int counter = 0;             // counter for button press
int countertemp = 0;         // temporärer counter
int min_interval_on = 1000;
int max_interval_on = 2000;
int min_interval_off = 500;
int max_interval_off = 700;
int interval = 1000;           // interval at which to blink (milliseconds) for the first time

//"sizeof" gives the size in bytes, so to get the number of elements it is neccessary to divide by one element
//int arraySize = (sizeof (ledPins) / sizeof(ledPins[0]));
int arraySize = NUM_LEDS;

//State arrays have fixed size because Arduino does not support adaptive "stl vectors" without extra libraries.
// Increase if you want to add more than 15 leds/buttons
bool buttonStateArray [15];
int ledStateArray [15];
int onIntervalArray [15];
int offIntervalArray [15];
int currentMillisArray [15];
int previousMillisArray [15];

CRGB leds[NUM_LEDS];

void setup()
{
  //initialise the led pins (according to the array) as output
//  for (p = 0; p < arraySize; ++p) {
//    pinMode(ledPins[p], OUTPUT);
//  }

  // interrupt button seting
  configureCommon(); // Setup pins for interrupt
  attachInterrupt(digitalPinToInterrupt(commonPin), pressInterrupt, FALLING); //interrupt

  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);

  // Open the serial port at 9600 Bit/s:
  Serial.begin(9600);
  randomSeed(analogRead(0));
}

void loop() {
  //blink LEDs randomly and independent from each other
  for (int p = 0; p < arraySize; p++) {
    //Write the current time into each slot of an array
    unsigned long currentMillis = millis();
    currentMillisArray[p] = currentMillis;
    //Read the ledState & buttonState of the respective array slot
//    ledStateArray[p] = digitalRead(ledPins[p]);
    //    buttonStateArray[p] = digitalRead(buttonPins[p]);

    if ((ledStateArray[p] > 0 ) && (currentMillisArray[p] - previousMillisArray[p] >= onIntervalArray[p]))  {
      //Set the duration of the next off interval for the respective array slot
      offIntervalArray[p] = random(min_interval_off, max_interval_off);
      //Change the led state
      ledStateArray[p] = 0;  // Turn it off
      previousMillisArray[p] = currentMillisArray[p];  // Remember the time
      leds[p] = colourArray[0];  // Update the actual LED
    }
    else if ((ledStateArray[p] == 0 ) && (currentMillisArray[p] - previousMillisArray[p] >= onIntervalArray[p])) {
      //Set the duration of the next off interval for the respective array slot
      onIntervalArray[p] = random(min_interval_on, max_interval_on);
      //Change the led state
      ledStateArray[p] = 1;  // turn it on
      previousMillisArray[p] = currentMillisArray[p];   // Remember the time
      leds[p] = colourArray[1];  // Update the actual LED
    }    
    FastLED.show();
  }
}


//interrupt function and finding the correct pin
void pressInterrupt() { // ISR
  if (millis() - lastFire < 200) { // Debounce
    return;
  }
  lastFire = millis();

  configureDistinct(); // Setup pins for testing individual buttons

  for (int i = 0; i < sizeof(buttonPins) / sizeof(int); i++) { // Test each button for press
    if (!digitalRead(buttonPins[i])) {
      press(i);
    }
  }

  configureCommon(); // Return to original state
}

// compare button press to LED and count
void press(int button) {
  if (ledStateArray[button] > 0 ) {
    if (countertemp == 0) {   
      ledStateArray[button] = 0;
      leds[button] = colourArray[0];
//      digitalWrite(ledPins[button], ledStateArray[button]);
      counter = counter + 1;
      Serial.println (counter);
    }
    countertemp == 1;   // Set if statement to false to stop the if loop
  }
  else if (ledStateArray[button] == 0) {
    if (countertemp == 0) {
      if (counter > 0) {
        counter = counter - 1;
      }
      Serial.println (counter);
    }
    countertemp == 1;   // Set if statement to false to stop the if loop
  }
}

// set pin mode to detect any button press
void configureCommon() {
  pinMode(commonPin, INPUT_PULLUP);

  for (int i = 0; i < sizeof(buttonPins) / sizeof(int); i++) {
    pinMode(buttonPins[i], OUTPUT);
    digitalWrite(buttonPins[i], LOW);
  }
}

// set pin mode to detect the correct pin for the button press
void configureDistinct() {
  pinMode(commonPin, OUTPUT);
  digitalWrite(commonPin, LOW);

  for (int i = 0; i < sizeof(buttonPins) / sizeof(int); i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
}
