#include <FlipperCTRL.h>
//               mw  mh  ofx  ofy  num  dat  clk  lat
FlipperCTRL flip(21, 19,   7,   0,   1,  11,  13,  10);

void setup() {
	Serial.begin(115200);
	flip.setAddress(0, 0b00000001); //DIP 1 is ON on first module
	flip.begin();
	flip.setLED(0, 0);
	flip.print("Hello");
	flip.setLED(0, 0);
	flip.drawPixel(0,0,1); //turn off led
}

void loop() {
	//Nothing to do here
}