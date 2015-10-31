/* The Lonely But Tender Ghost likes it when you squeeze him gently but doesn't like it when you squeeze him too hard.
 * The light turns green and he makes a happy sound when you squeeze him just right.
 * If you squeeze too hard the light turns red and he makes a distressed sound.
 * Otherwise, the light blinks white.
 * If a period of time passes, he will get lonely. 
 * If he's lonely, the light will turn yellow and he'll make a lonely sound until you squeeze him again.*/

#include "pitches.h"

long lastSqueezeTime = 0;   //calculating when the last time the ghost was squeezed
long lonelyTime = 10000;    //the amount of time the ghost can go unsqueezed before making a sound
long blinkInterval = 1000;  //interval at which to blink
long blinkMillis = 0;
int redState = LOW;
int greenState = LOW;
int blueState = LOW;

//melody when the ghost is squeezed to force 2 and the light is green:
int greenMelody[] = {
  NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5
};
int greenNoteDurations[] = {
  16, 16, 16, 16,
};

//melody when the loneliness timer hits:
int lonelyMelody[] = {
  NOTE_G6, NOTE_FS6, NOTE_G6, NOTE_A6
};
int lonelyNoteDurations[] = {
  16, 16, 16, 16,
};

//melody when the ghost is squeezed to force 3 and the light is red:
int redMelody[] = {
  NOTE_GS7, NOTE_FS7, NOTE_F7, NOTE_A7
};
int redNoteDurations[] = {
  32, 32, 32, 32,
};


//setting the pins for each leg of the RGB LED
const int redPin = 11;
const int greenPin = 10;
const int bluePin = 9;

//creating a variable for the value of the force sensor
int forceSensorValue = 0;

void setup() {
  Serial.begin(9600);
  //setting the RGB LED as an output
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {
  forceSensorValue = analogRead(A0); //reading the value of the FSR on A0
  int force = map(forceSensorValue, 800, 1000, 1, 3); //creating 3 different states of force: 1, 2, and 3

  long currentMillis = millis(); //setting currentMillis to the same as the millisecond counter

  //if you don't squeeze the FSR before the loneliness timer runs out...
  //...turn the light yellow (red + green)
  //...and play the lonelyMelody
  if (currentMillis - lastSqueezeTime > lonelyTime) {
    redState = HIGH;
    greenState = HIGH;
    blueState = LOW;
    for (int lonelyThisNote = 0; lonelyThisNote < 4; lonelyThisNote++) {
      int lonelyNoteDuration = 1000 / lonelyNoteDurations[lonelyThisNote];
      tone(8, lonelyMelody[lonelyThisNote], lonelyNoteDuration);
      delay(lonelyNoteDuration);
    }
  }

  //if you squeeze the FSR really hard to force 3...
  //...make the light red...
  //...and play the redMelody
  if (force == 3) {
    redState = HIGH;
    greenState = LOW;
    blueState = LOW;
    for (int redThisNote = 0; redThisNote < 4; redThisNote++) {
      int redNoteDuration = 1000 / redNoteDurations[redThisNote];
      tone(8, redMelody[redThisNote], redNoteDuration);
      delay(redNoteDuration);
    }
  }
  //if you squeeze the FSR just right to force 2...
  //...make the light green...
  //...play the greenMelody...
  //...and reset the loneliness timer
  else if (force == 2) {
    redState = LOW;
    greenState = HIGH;
    blueState = LOW;
    lastSqueezeTime = currentMillis;
    for (int greenThisNote = 0; greenThisNote < 4; greenThisNote++) {
      int greenNoteDuration = 1000 / greenNoteDurations[greenThisNote];
      tone(8, greenMelody[greenThisNote], greenNoteDuration);
      delay(greenNoteDuration);
    }

  }
  //if you don't push the FSR at all, or it's only pushed to force 1...
  //...turn the light off, but also make it blink
  else {
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);

    if (currentMillis - blinkMillis >= blinkInterval) {
      // save the last time you blinked the LED
      blinkMillis = currentMillis;
      // if the LED is off turn it on and vice-versa:
      if (redState == LOW && greenState == LOW && blueState == LOW) {
        redState = HIGH;
        greenState = HIGH;
        blueState = HIGH;
      }
      else {
        redState = LOW;
        greenState = LOW;
        blueState = LOW;
      }
    }

  }
  Serial.println(force);
  digitalWrite(redPin, redState);
  digitalWrite(greenPin, greenState);
  digitalWrite(bluePin, blueState);
}
