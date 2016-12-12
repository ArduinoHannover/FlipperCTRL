#include "FlipperCTRL.h"

FlipperCTRL::FlipperCTRL(uint8_t modulewidth, uint8_t moduleheight, uint8_t x_offset, uint8_t y_offset, uint8_t modules, uint8_t datapin, uint8_t clockpin, uint8_t latchpin) :
	Adafruit_GFX(modulewidth * modules, moduleheight),
	_mwidth(modulewidth),
	_xoffset(x_offset),
	_yoffset(y_offset),
	_modules(modules),
	_datapin(datapin),
	_clockpin(clockpin),
	_latchpin(latchpin) {
	_addresses = (uint8_t*) calloc(modules, sizeof(uint8_t));
}

void FlipperCTRL::begin(void) {
	pinMode(_datapin, OUTPUT);
	pinMode(_clockpin, OUTPUT);
	pinMode(_latchpin, OUTPUT);fillScreen(0);
}

void FlipperCTRL::setAddress(uint8_t module, uint8_t address) {
	if (module >= _modules) return; //sanity check
	_addresses[module] = ~address; //inverse logic
}

void FlipperCTRL::setCol(uint8_t col, boolean high) {
	/*
	 * Truthtable:
	 * A2 A1 A0  Output at
	 *  0  0  0  X
	 *  0  0  1  A
	 *  0  1  0  B
	 *  0  1  1  C
	 *  1  0  0  D
	 *  1  0  1  E
	 *  1  1  0  F
	 *  1  1  1  G
	 * 
	 *    B1 B0  Output at
	 *     0  0  Q0
	 *     0  1  Q1
	 *     1  0  Q2
	 *     1  1  Q3
	 */
	//inverse logic due to ULN2003
	//set all to high to have low outputs
	_data[5] |= 0b11111100; //but retain LED-state
	_data[5] &= ~((col/7) << 6); //set B0/B1 register
	_data[5] &= ~(((col % 7) + 1) << 3); //set A0-A2
	if (high)
		_data[5] &= 0b11111011;
}

void FlipperCTRL::unsetRows(void) {
	for (uint8_t i = 0; i < 5; i++) _data[i] = 0;
}

void FlipperCTRL::setRow(uint8_t row, boolean high) {
	unsetRows();
	//SET is bit 0-19, RESET on bit 20-39
	uint16_t bit = row+20*(!high);
	_data[bit / 8] = 1 << (bit & 7);
}

void FlipperCTRL::setModule(uint8_t module) {
	if (module >= _modules) return;
	_data[6] = _addresses[module];
}

void FlipperCTRL::setLED(boolean led, boolean on) {
	if (on) _data[5] |= 1 << led;
	else _data[5] &= ~(1 << led);
}

void FlipperCTRL::drawPixel(int16_t x, int16_t y, uint16_t color) {
	if (x < 0 || y < 0 || y > height() || x > width()) return;
	setModule(x / _mwidth);
	setRow(y + _yoffset, color);
	setCol((x % _mwidth) + _xoffset, !color);
	for (int8_t i = FLIPPERCTRL_REGISTERS-1; i >= 0; i--)
		shiftOut(_datapin, _clockpin, MSBFIRST, _data[i]);
	digitalWrite(_latchpin, HIGH);
	digitalWrite(_latchpin, LOW);
	//do not overheat...
	unsetRows();
	setCol(0, 0);
	for (int8_t i = FLIPPERCTRL_REGISTERS-1; i >= 0; i--)
		shiftOut(_datapin, _clockpin, MSBFIRST, _data[i]);
	digitalWrite(_latchpin, HIGH);
	digitalWrite(_latchpin, LOW);
}