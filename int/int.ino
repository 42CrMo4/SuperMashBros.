/*

SuperMashBros. 2020

Spielmechanik für Cocktail Roboter

*/

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin


// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
int ledState = LOW;         //variable for reading ledstate on/off
unsigned long previousMillis = 0;
int counter = 0;             // counter for button press
int countertemp = 0;         // temporärer counter
int min_interval = 1000;
int max_interval = 5000;
int interval = 1000;           // interval at which to blink (milliseconds)

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  // Öffnet die serielle Schnittstelle bei 9600 Bit/s:
  Serial.begin(9600);
  randomSeed(analogRead(0));
}

void loop() {
  //Auswertung Button State (Boolean)
  buttonState = digitalRead(buttonPin);
  //Lesen aktuelle Zeit seit Programmstart
  unsigned long currentMillis = millis();
  // Check ob Intervall für nächstes Auslösen LED überschritten
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    //    Randomisieren des Intervalls
    interval = random(min_interval, max_interval);
    //    Serial.println(interval);

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }

  // check if the pushbutton is pressed and the LED is on
  if (buttonState == HIGH & ledState == HIGH) {
    // Schleife fürs erhöhen zählen
    if (countertemp == 0) {
      counter = counter + 1;
      Serial.println(counter);
    }
    // varable zum counter erhöhen verhindern.
    countertemp = 1;
  }

  // check if the pushbutton is pressed and the LED is off
  else if (buttonState == HIGH & ledState == LOW) {
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
