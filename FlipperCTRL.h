#ifndef FLIPPERCTRL
#define FLIPPERCTRL

#include <Adafruit_GFX.h>

#define FLIPPERCTRL_REGISTERS 7

class FlipperCTRL : public Adafruit_GFX {
	private:
		uint8_t _mwidth;
		uint8_t _xoffset;
		uint8_t _yoffset;
		uint8_t _modules;
		uint8_t _datapin;
		uint8_t _clockpin;
		uint8_t _latchpin;
		uint8_t *_addresses;
		uint8_t _data[FLIPPERCTRL_REGISTERS];
		void setCol(uint8_t col, boolean high);
		void unsetRows();
		void setRow(uint8_t row, boolean high);
		void setModule(uint8_t module);
	public:
		FlipperCTRL(uint8_t modulewidth, uint8_t moduleheight, uint8_t x_offset, uint8_t y_offset, uint8_t modules, uint8_t datapin, uint8_t clockpin, uint8_t latchpin);
		void begin();
		void setAddress(uint8_t module, uint8_t address);
		void setLED(boolean led, boolean on);
		void drawPixel(int16_t x, int16_t y, uint16_t color);
};

#endif //FLIPPERCTRL