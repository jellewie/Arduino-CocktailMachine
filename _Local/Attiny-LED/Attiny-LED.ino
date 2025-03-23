#include <Adafruit_NeoPixel.h>

// Sample code for using a breadboard Arduino to drive WS2812B LED strip with Adafruit NeoPixel library
// I got mine on e-bay:
// http://www.ebay.com/itm/181268207260?ssPageName=STRK:MEWNX:IT&_trksid=p3984.m1439.l2649
// Note - colors for data and ground vary
// I used a 16MHZ Crystal Oscilator
const uint8_t PAO_LED = 4;         //To which pin the <LED> is connected to

// Parameter 1 = number of pixels in strip - I used 12
// Parameter 2 = PAO_LED number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
const uint8_t TotalLEDs = 1;                                        //The total amounts of LEDs in the strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(TotalLEDs, PAO_LED, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show();  // Initialize all pixels to 'off'
}

void loop() {
  strip.setPixelColor(0, strip.Color(255, 0, 0));
  strip.show();
  delay(50);
  strip.setPixelColor(0, strip.Color(0, 255, 0));
  strip.show();
  delay(50);
  strip.setPixelColor(0, strip.Color(0, 0, 255));
  strip.show();
  delay(50);
}
