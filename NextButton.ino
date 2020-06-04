#include <OneButton.h>
OneButton button1(2, true);
OneButton button2(3, true);
OneButton button3(4, true);
OneButton button4(5, true);

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);
#define BUSY_PIN 12


#include <FastLED.h>

#define NUM_LEDS 30
#define DATA_PIN 9
CRGB leds[NUM_LEDS];
#define UPDATES_PER_SECOND 100
#define BRIGHTNESS 100


int folder = 1,         //Sound folder numder
    soundCount = 86;    //Number of sound files in the folder
int sound1 = 1, sound2 = 2, sound3 = 3, sound4 = 4;
static unsigned long timer;
int btnPlay = 0,
    spin = 0,
    x = 5,
    fade = 4;
bool pressed = false,
     firstPlay = true;
uint8_t gHue = 0;


void setup()
{
  //sanity delay
  delay(1000);

  // link the button functions.
  button1.attachClick(click1);
  button1.attachDoubleClick(doubleclick1);
  button1.attachLongPressStop(longPressStop1);
  button1.attachDuringLongPress(longPress1);
  button2.attachClick(click2);
  button2.attachDoubleClick(doubleclick2);
  button2.attachLongPressStop(longPressStop2);
  button2.attachDuringLongPress(longPress2);
  button3.attachClick(click3);
  button3.attachDoubleClick(doubleclick3);
  button3.attachLongPressStop(longPressStop3);
  button3.attachDuringLongPress(longPress3);
  button4.attachClick(click4);
  button4.attachDoubleClick(doubleclick4);
  button4.attachLongPressStop(longPressStop4);
  button4.attachDuringLongPress(longPress4);

  // check mp3 player
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  Serial.println();
  Serial.println(F("Initialize NextButton"));
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true) {
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.volume(22);  //Set volume value. From 0 to 30
  pinMode(BUSY_PIN, INPUT);

  // initialize LED strip
  LEDS.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  LEDS.setBrightness(BRIGHTNESS);

  randomSeed(analogRead(A4));

}





void loop()
{

  if (firstPlay == true) {
    myDFPlayer.playFolder(4, 1);
  //  showBattery();
    firstPlay = false;
  }


  button1.tick();
  button2.tick();
  button3.tick();
  button4.tick();


  if ((digitalRead(BUSY_PIN)) == LOW) {
    switch (btnPlay) {
      case 1:
        confettiRed();          break;
      case 2:
        confettiYellow();       break;
      case 3:
        confettiGreen();        break;
      case 4:
        confettiBlue();         break;
    }
  }

  if (fade < 3) {
    spinEnd();
  }

  FastLED.show();
  fadeToBlackBy( leds, NUM_LEDS, 4);


  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
}






void click1() {
  myDFPlayer.playFolder(folder, sound1);
  btnPlay = 1;
  // timer = millis();   //timerbased LED

  Serial.println("Button 1 click.");
}
void doubleclick1() {
  sound1 = (random(1, soundCount));
  Serial.println("Button 1 doubleclick.");
  Serial.print("Sound 1: ");  Serial.println(sound1);
}
void longPress1() {
  btnPlay = 0;
  if (pressed == false) {
    myDFPlayer.loopFolder(2);
  }
  pressed = true;
  //fadeToBlackBy( leds, NUM_LEDS, 50);
  FastLED.clear();
  x = 5;
  spinIt();
}
void longPressStop1() {
  pressed = false;
  myDFPlayer.playFolder(3, 1);
  spinSlower();
  fade = 0;
  Serial.println("Button 1 longPress stop");
}


void click2() {
  myDFPlayer.playFolder(folder, sound2);
  btnPlay = 2;
  // timer = millis();   //timerbased LED
  Serial.println("Button 2 click.");
}
void doubleclick2() {
  sound2 = (random(1, soundCount));
  Serial.println("Button 2 doubleclick.");
  Serial.print("Sound 2: ");   Serial.println(sound2);
} // doubleclick1

void longPress2() {
  btnPlay = 0;
  if (pressed == false) {
    myDFPlayer.playFolder(2, 7
                         );
  }
  pressed = true;
  //fadeToBlackBy( leds, NUM_LEDS, 50);
  rainbow();
}
void longPressStop2() {
  pressed = false;
  myDFPlayer.pause();
  //myDFPlayer.playFolder(3, 1);
  Serial.println("Button 1 longPress stop");
}


void click3() {
  myDFPlayer.playFolder(folder, sound3);
  btnPlay = 3;
  // timer = millis();   //timerbased LED
  Serial.println("Button 3 click.");
}
void doubleclick3() {
  sound3 = (random(1, soundCount));
  Serial.println("Button 3 doubleclick.");
  Serial.print("Sound 3: ");  Serial.println(sound3);
}
void longPress3() {
  btnPlay = 0;
  if (pressed == false) {
    myDFPlayer.playFolder(2, 5);
  }
  pressed = true;
  //fadeToBlackBy( leds, NUM_LEDS, 50);
  cylon();
}
void longPressStop3() {
  pressed = false;
  myDFPlayer.pause();
  //myDFPlayer.playFolder(3, 1);
  Serial.println("Button 1 longPress stop");
}



void click4() {
  myDFPlayer.playFolder(folder, sound4);
  btnPlay = 4;
  // timer = millis();   //timerbased LED
  Serial.println("Button 1 click.");
}
void doubleclick4() {
  sound4 = (random(1, soundCount));
  Serial.println("Button 4 doubleclick.");
  Serial.print("Sound 4: ");  Serial.println(sound4);
}
void longPress4() {
  btnPlay = 0;
  if (pressed == false) {
    myDFPlayer.enableLoop();
    myDFPlayer.playFolder(2, 6);
  }
  pressed = true;
  //fadeToBlackBy( leds, NUM_LEDS, 50);
  juggle();
}
void longPressStop4() {
  pressed = false;
  myDFPlayer.pause();
  //myDFPlayer.playFolder(3, 1);
  Serial.println("Button 1 longPress stop");
}

void spinIt() {
  spin++;
  if (spin > NUM_LEDS) {
    spin = 0;
  }
  leds[spin] = CRGB::White;
  delay(x);
}
void spinSlower() {
  for (x = 5; x < 230; x++) {
    spinIt();
    x += 2;
    FastLED.show();
    FastLED.clear();
  }
}
void spinEnd() {
  delay(50);
  for (int c = 255; c > 5; c--) {
    leds[spin] = CRGB(c, c, c);
    FastLED.show();
    delay(1);
  }
  for (int c = 5; c < 255; c++) {
    leds[spin] = CRGB(c, c, c);
    FastLED.show();
    delay(1);
  }
  fade++;
}

void confettiRed()
{
  // random colored speckles that blink in and fade smoothly

  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( 220 + random8(64), 200, 255);
}
void confettiBlue()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( 140 + random8(64), 200, 255);
}
void confettiYellow()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( 30 + random8(40), 200, 255);
}
void confettiGreen()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( 60 + random8(40), 200, 255);
}


void rainbow()
{
  EVERY_N_MILLISECONDS( 20 ) {
    gHue++;
  };
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void fadeall() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(250);
  }
}


void cylon() {
  static uint8_t hue = 0;
  Serial.print("x");
  // First slide the led in one direction
  for (int i = 0; i < NUM_LEDS; i++) {
    // Set the i'th led to red
    leds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(14);
  }
  Serial.print("x");

  // Now go in the other direction.
  for (int i = (NUM_LEDS) - 1; i >= 0; i--) {
    // Set the i'th led to red
    leds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(15);
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for ( int i = 0; i < 8; i++) {
    leds[beatsin16( i + 7, 0, NUM_LEDS - 1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void showBattery() {
  int voltage = (analogRead(A2)  * (5.0 / 1023.0)) * 1000;
  Serial.println(voltage);
  int showBattery = map(voltage, 3200, 4200, 0, 30);
  for (int i = 0; i <= showBattery; i++) {
    leds[i] = CRGB(0, 50, 0);
    FastLED.show();
    delay(30);
  }
  for (int j = 0; j < 80; j++) {
    for (int i = showBattery; i < NUM_LEDS; i++) {
      leds[i] = CRGB(j, 0, 0);
    }
    FastLED.show();
    delay(12);
  }
  for (int j = 80; j >=0; j--) {
    for (int i = showBattery; i < NUM_LEDS; i++) {
      leds[i] = CRGB(j, 0, 0);
    }
    FastLED.show();
    delay(12);
  }
}

void printDetail(uint8_t type, int value) {
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
