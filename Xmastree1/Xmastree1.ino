/*
 * LED connected to pin 2, 4, 5, 6, 7 and 8
 * RGB LED connectoed to pin 9(red), 10(green) and 11 (blue)
 * Speaker or buzzer connected to pin 3
 * Push button connected to analog pin 1
 * 
 * Flashes all the LEDs, and cycle color on RGB LED.
 * Plays Juingle bells when button is pushed
 */

#include "pitches.h"

const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;
unsigned int rgbColour[3];
int ticktime = 1000;
int currenttime;

int brightness = 0;
int fadeAmount = 1;

// notes in the melody:
int melody[] = {
  NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_G4, NOTE_C4, NOTE_D4, NOTE_E4,
  NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_G4,
  NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_G4, NOTE_C4, NOTE_D4, NOTE_E4,
  NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_G4, NOTE_G4, NOTE_F4, NOTE_D4, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 4, 2, 4, 4, 2, 4, 4, 4, 4, 1,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 2,
  4, 4, 2, 4, 4, 2, 4, 4, 4, 4, 1,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1
};

void setup() {
  // Start off with the LED off.
  setColourRgb(0,0,0);  
  // Start off with red.
  rgbColour[0] = 255;
  rgbColour[1] = 0;
  rgbColour[2] = 0;
  
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  digitalWrite(2, HIGH);
  digitalWrite(4, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, HIGH);

  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  pinMode(3, OUTPUT);
  
  
}

void loop() {
  // Choose the colours to increment and decrement.
  for (int decColour = 0; decColour < 3; decColour += 1) {
    int incColour = decColour == 2 ? 0 : decColour + 1;

    // cross-fade the two colours.
    for(int i = 0; i < 255; i += 1) {
      rgbColour[decColour] -= 1;
      rgbColour[incColour] += 1;
      setColourRgb(rgbColour[0], rgbColour[1], rgbColour[2]);
      
      currenttime = currenttime +5;
      if (currenttime > ticktime){
        changeLed();
        currenttime = 0;
      }

      brightness = brightness + fadeAmount;
      analogWrite(5, brightness);
      analogWrite(6, 255 - brightness);

      if (brightness <= 0 || brightness >= 255) {
        fadeAmount = -fadeAmount;
      }
      delay(5);
      if (digitalRead(15) == HIGH){
        playSong();
      }
    }
  }
}

void setColourRgb(unsigned int red, unsigned int green, unsigned int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

void changeLed(){
  digitalWrite(2, !digitalRead(2));
  digitalWrite(4, !digitalRead(4));
  digitalWrite(7, !digitalRead(7));
  digitalWrite(8, !digitalRead(8));
}
void playSong(){
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < sizeof(melody)/sizeof(int); thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(3, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(3);
  }
}
