/*

SuperMashBros. 2020

Spielmechanik für Cocktail Roboter

*/

// constants won't change. They're used here to set pin numbers:

// Button Pins
const int buttonPin_1     =   7;     // the number of the pushbutton pin
const int buttonPin_2     =   6;     // the number of the pushbutton pin
const int buttonPin_3     =   5;     // the number of the pushbutton pin
const int buttonPin_4     =   4;     // the number of the pushbutton pin
const int buttonPin_5     =   3;     // the number of the pushbutton pin

// LED Pins
const int ledPin_1  =  13;     // the number of the LED pin
const int ledPin_2    =  12;     // the number of the LED pin
const int ledPin_blue   =  11;     // the number of the LED pin
const int ledPin_yellow =  10;     // the number of the LED pin
const int ledPin_orange =   9;     // the number of the LED pin


// variables will change:

// Buttons
int buttonState_1 = 0;         // variable for reading the pushbutton status
int buttonState_2 = 0;         // variable for reading the pushbutton status
int buttonState_3 = 0;         // variable for reading the pushbutton status
int buttonState_4 = 0;         // variable for reading the pushbutton status
int buttonState_5 = 0;         // variable for reading the pushbutton status

// LED Status
int ledState_1  = LOW;         //variable for reading ledstate on/off
int ledState_2    = LOW;         //variable for reading ledstate on/off
int ledState_blue   = LOW;         //variable for reading ledstate on/off
int ledState_yellow = LOW;         //variable for reading ledstate on/off
int ledState_orange = LOW;         //variable for reading ledstate on/off

//misc
unsigned long previousMillis_1 = 0;
unsigned long previousMillis_2 = 0;
int counter = 0;             // counter for button press
int countertemp = 0;         // temporärer counter
int min_interval_on = 1000;
int max_interval_on = 5000;
int min_interval_off = 500;
int max_interval_off = 2000;
int interval_1 = 1000;           // interval at which to blink (milliseconds)
int interval_2 = 1000;  

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin_1, OUTPUT);
  pinMode(ledPin_2, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin_1, INPUT);
  pinMode(buttonPin_2, INPUT);
  // Öffnet die serielle Schnittstelle bei 9600 Bit/s:
  Serial.begin(9600);
  randomSeed(analogRead(0));
}

void loop() {
  //Auswertung Button State (Boolean)
  buttonState_1 = digitalRead(buttonPin_1);
   buttonState_2 = digitalRead(buttonPin_2);
  //Lesen aktuelle Zeit seit Programmstart
  unsigned long currentMillis = millis();
  
  // Check ob Intervall für nächstes Auslösen LED überschritten
  if (currentMillis - previousMillis_1 >= interval_1) {
    // save the last time you blinked the LED
    previousMillis_1 = currentMillis;
    //    Randomisieren des Intervalls
    if (ledState_1 == HIGH) { //Define Off State intervall
      interval_1 = random(min_interval_off, max_interval_off);
    } else {                //Define On State intervall
      interval_1 = random(min_interval_on, max_interval_on);
    }
//       Serial.println(interval_1);

           // if the LED is off turn it on and vice-versa:
    if (ledState_1 == LOW) {
      ledState_1 = HIGH;  
    } else {
      ledState_1 = LOW;
     }

         digitalWrite(ledPin_1, ledState_1);

  }

   // Check ob Intervall für nächstes Auslösen LED überschritten
  if (currentMillis - previousMillis_2 >= interval_2) {
    // save the last time you blinked the LED
    previousMillis_2 = currentMillis;
    //    Randomisieren des Intervalls
    if (ledState_2 == HIGH) { //Define Off State intervall
      interval_2 = random(min_interval_off, max_interval_off);
    } else {                //Define On State intervall
      interval_2 = random(min_interval_on, max_interval_on);
    }
//       Serial.println(interval_2);


//          if the LED is off turn it on and vice-versa:
    if (ledState_2 == LOW) {
      ledState_2 = HIGH;  
    } else {
      ledState_2 = LOW;
     }

    // set the LED with the ledState of the variable:

    digitalWrite(ledPin_2, ledState_2);
  }

  // check if the pushbutton is pressed and the LED is on
  if (buttonState_1 == HIGH & ledState_1 == HIGH||buttonState_2 == HIGH & ledState_2 == HIGH) {
    // Schleife fürs erhöhen zählen
    if (countertemp == 0) {
      counter = counter + 1;
      Serial.println(counter);
    }
    // varable zum counter erhöhen verhindern.
    countertemp = 1;
  }

  // check if the pushbutton is pressed and the LED is off
  else if (buttonState_1 == HIGH & ledState_1 == LOW||buttonState_2 == HIGH & ledState_2 == LOW) {
    // Schleife fürs verringern Zähler
    if (countertemp == 0) {
      if (counter > 0) {
        counter = counter - 1;
      }
      Serial.println(counter);
    }
    // variable zum counter erhöhen verhindern.
    countertemp = 1;
  }

  else {
    // varable zum counter erhöhen zurücksetzen
    countertemp = 0;
  }

}
