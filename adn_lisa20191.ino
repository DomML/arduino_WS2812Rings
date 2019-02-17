#include <Adafruit_NeoPixel.h>

#define PIN 2	 // input pin Neopixel is attached to

#define NUMPIXELS      37// number of neopixels in strip

#undef max
#define max(a,b) ((a)>(b)?(a):(b))

#undef min
#define min(a,b) ((a)<(b)?(a):(b))

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 100; // timing delay in milliseconds

int redColor = 0;
int greenColor = 0;
int blueColor = 0;

uint32_t green = pixels.Color(0, 255, 0);
uint32_t greenLow = pixels.Color(0, 256/4-1, 0);
uint32_t redLow = pixels.Color(256/4-1, 0, 0);
uint32_t black = pixels.Color(0, 0, 0);
uint32_t color = pixels.Color(0, 0, 0);

const int buttonPin = 3;
volatile int buttonState = 0;

int mode = 0;
const int nbMode = 4;

void setup() {
  // Initialize the NeoPixel library.
  pixels.begin();
  pixels.show();
  Serial.begin(9600);
  int ar = analogRead(0);
  randomSeed(ar);
  //Serial.println(ar);
  
  pinMode(buttonPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(buttonPin), pin_ISR, RISING);
}

void pin_ISR() {
  buttonState = digitalRead(buttonPin);
  mode = (mode + 1) % nbMode;
  Serial.println(mode);
  Serial.println(buttonState);
  delay(25);
}

void lightPixels(int f, int l, uint32_t color, int time){
  for (int i=f; i <= l; i++) {
      pixels.setPixelColor(i, color);
    }
  pixels.show();
  if (time != 0){
    delay(time);
  }
}

void floorGreenBlink(){
  lightPixels(0, 0, green, delayval);
  lightPixels(1, 12, green, delayval);
  lightPixels(13, 36, green, delayval);
  lightPixels(0, 0, black, delayval);
  lightPixels(1, 12, black, delayval);
  lightPixels(13, 36, black, delayval);
}
void floorGreenFade(){
  int nbStep = 16;
//  lightPixels(0, 36, green, 100);
  for(int i=0; i < 256; i+=256/nbStep){
    Serial.println(i);
    color = pixels.Color(0, i, 0);
    lightPixels(0, 36, color, 0);
    pixels.show();
    delay(delayval);
  }
  for(int i=255; i > 0; i-=256/nbStep){
    Serial.println(i);
    color = pixels.Color(0, i, 0);
    lightPixels(0, 36, color, 0);
    pixels.show();
    delay(delayval);
  }
 }

int singleAnglePixel(int angle, int angleOffset, int nbLED, int ledOffset){
  float a = float(float(int(angle+angleOffset)%360)/360.0);
  float b = a * float(nbLED);
  int bi = b + ledOffset;
  return (bi);
}

void floorHorizLoop(int nbOfLight){
  //pixels.setPixelColor(0, green);
  int bi = 0;
  for (float i = 0.0; i < 360.00; i+=10.0){ 
  	pixels.clear();
    lightPixels(0, 36, redLow, 0);
    pixels.setPixelColor(0, green);
    for (int aOffset = 0; aOffset < 360; aOffset+=360/nbOfLight){
	  bi = singleAnglePixel(i, aOffset, 12, 1);
      pixels.setPixelColor(bi, green);
      bi = singleAnglePixel(i, aOffset, 24, 13);
      pixels.setPixelColor(bi, green);
    }
    pixels.show();
    delay(delayval);
  }
}

void allRandom(int nbLedToChange, int rScale, int gScale, int bScale){
  for (int i = 0; i < nbLedToChange; i++){
    int randLed = random(37);
    setColor();
    color = pixels.Color(int(redColor/rScale), int(greenColor/gScale), int(blueColor/bScale));
    pixels.setPixelColor(randLed, color);
  }
  pixels.show();
  delay(delayval);
}

void loop() {
  switch(mode){
    case 0:
      floorGreenBlink();
      break;
    case 1:
      floorGreenFade();
      break;
    case 2:
      floorHorizLoop(2);
      break;
    case 3:
      allRandom(36, 1, 1, 8);
      break;
    // Add all function integrating allRandom to blink joyfully
  }
}

// setColor()
// picks random values to set for RGB
void setColor(){
  redColor = random(0, 255);
  greenColor = random(0,255);
  blueColor = random(0, 255);
}