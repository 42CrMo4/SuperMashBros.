/*

SuperMashBros. 2020

Spielmechanik für Cocktail Roboter

*/

// setting up Pin arrays for buttons and LEDs to access them efficiently.
//It is important that LED1 and Button1 are in the same location within their respective array
const int commonPin = 2;
int buttonPins[] = {4, 5, 6, 7};
int ledPins[] = {10, 11, 12, 13};

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
int arraySize = (sizeof (ledPins) / sizeof(ledPins[0]));

//State arrays have fixed size because Arduino does not support adaptive "stl vectors" without extra libraries.
// Increase if you want to add more than 15 leds/buttons
bool buttonStateArray [15];
bool ledStateArray [15];
int onIntervalArray [15];
int offIntervalArray [15];
int currentMillisArray [15];
int previousMillisArray [15];


void setup()
{
  //initialise the led pins (according to the array) as output
  for (p = 0; p < arraySize; ++p) {
    pinMode(ledPins[p], OUTPUT);
  }

  // interrupt button seting
  configureCommon(); // Setup pins for interrupt
  attachInterrupt(digitalPinToInterrupt(commonPin), pressInterrupt, FALLING); //interrupt

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
    ledStateArray[p] = digitalRead(ledPins[p]);
    //    buttonStateArray[p] = digitalRead(buttonPins[p]);

    if ((ledStateArray[p] == HIGH) && (currentMillisArray[p] - previousMillisArray[p] >= onIntervalArray[p]))  {
      //Set the duration of the next off interval for the respective array slot
      offIntervalArray[p] = random(min_interval_off, max_interval_off);
      //Change the led state
      ledStateArray[p] = LOW;  // Turn it off
      previousMillisArray[p] = currentMillisArray[p];  // Remember the time
      digitalWrite(ledPins[p], ledStateArray[p]);  // Update the actual LED
    }
    else if ((ledStateArray[p] == LOW) && (currentMillisArray[p] - previousMillisArray[p] >= onIntervalArray[p])) {
      //Set the duration of the next off interval for the respective array slot
      onIntervalArray[p] = random(min_interval_on, max_interval_on);
      //Change the led state
      ledStateArray[p] = HIGH;  // turn it on
      previousMillisArray[p] = currentMillisArray[p];   // Remember the time
      digitalWrite(ledPins[p], ledStateArray[p]);
    }    // Update the actual LED
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
  if (ledStateArray[button] == HIGH) {
    if (countertemp == 0) {   // kann man das hier nicht auch in das obere if statement dazu geben?
      ledStateArray[button] = LOW;
      digitalWrite(ledPins[button], ledStateArray[button]);
      counter = counter + 1;
      Serial.println (counter);
    }
    countertemp == 1;
  }
  else if (ledStateArray[button] == LOW) {
    if (countertemp == 0) {
      if (counter > 0) {
        counter = counter - 1;
      }
      Serial.println (counter);
    }
    countertemp == 1;
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
