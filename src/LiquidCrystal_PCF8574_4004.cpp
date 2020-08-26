/// \file LiquidCrystal_PCF8574_4004.cpp
/// \brief LiquidCrystal library with PCF8574 I2C adapter.
///
/// \author Matthias Hertel, http://www.mathertel.de
/// \copyright Copyright (c) 2019 by Matthias Hertel.
///
/// ChangeLog see: LiquidCrystal_PCF8574_4004.h

#include "LiquidCrystal_PCF8574_4004.h"

#include <Wire.h>

/// Definitions on how the PCF8574 is connected to the LCD

/// These are Bit-Masks for the special signals and background light
#define PCF_RS 0x01
#define PCF_EN_TOP 0x02
#define PCF_EN_BOTTOM 0x04
#define PCF_BACKLIGHT 0x08
// the 0xF0 bits are used for 4-bit data to the display.

// a nibble is a half Byte

LiquidCrystal_PCF8574_4004::LiquidCrystal_PCF8574_4004(int i2cAddr)
{
  _i2cAddr = i2cAddr;
  _backlight = 0;

  _entrymode = 0x02; // like Initializing by Internal Reset Circuit
  _displaycontrol = 0x04;
  _active_part = TOP_PART;
} // LiquidCrystal_PCF8574_4004


void LiquidCrystal_PCF8574_4004::begin(int cols, int lines)
{
  // _cols = cols ignored !
  _lines = lines;

  int functionFlags = 0;

  if (lines > 1) {
    functionFlags |= 0x08;
  }

  // initializing the display
  Wire.begin();
  _write2Wire(0x00, LOW, NO_PART);
  delayMicroseconds(50000);

  // after reset the mode is this
  _displaycontrol = 0x04;
  _entrymode = 0x02;

  // sequence to reset. see "Initializing by Instruction" in datatsheet
  for (int dp = TOP_PART; dp <= BOTTOM_PART; dp++)
    _sendNibble(0x03, false, dp);
  delayMicroseconds(4500);
  for (int dp = TOP_PART; dp <= BOTTOM_PART; dp++)
    _sendNibble(0x03, false, dp);
  delayMicroseconds(200);
  for (int dp = TOP_PART; dp <= BOTTOM_PART; dp++)
    _sendNibble(0x03, false, dp);
  delayMicroseconds(200);
  for (int dp = TOP_PART; dp <= BOTTOM_PART; dp++)
    _sendNibble(0x02, false, dp);   // finally, set to 4-bit interface

  // Instruction: Function set = 0x20
  for (int dp = TOP_PART; dp <= BOTTOM_PART; dp++)
    _send(0x20 | functionFlags, false, dp);

  display();
  clear();
  leftToRight();
} // begin()


void LiquidCrystal_PCF8574_4004::clear()
{
  // Instruction: Clear display = 0x01
  for (int dp = TOP_PART; dp <= BOTTOM_PART; dp++)
    _send(0x01, false, dp);
  delayMicroseconds(1600); // this command takes 1.5ms!
} // clear()


void LiquidCrystal_PCF8574_4004::init()
{
  clear();
} // init()


void LiquidCrystal_PCF8574_4004::home()
{
  // Instruction: Return home = 0x02
  for (int dp = TOP_PART; dp <= BOTTOM_PART; dp++)
    _send(0x02, false, dp);
  delayMicroseconds(1600); // this command takes 1.5ms!
  _active_part = TOP_PART;
} // home()


/// Set the cursor to a new position.
void LiquidCrystal_PCF8574_4004::setCursor(int col, int row)
{
  int row_offsets[] = {0x00, 0x40, 0x14, 0x54};
  // Instruction: Set DDRAM address = 0x80
  if (row >= 2) {
    row -= 2;
    _active_part = BOTTOM_PART;
  } else
    _active_part = TOP_PART;
  _send(0x80 | (row_offsets[row] + col), false, _active_part);
} // setCursor()


// Turn the display on/off (quickly)
void LiquidCrystal_PCF8574_4004::noDisplay()
{
  // Instruction: Display on/off control = 0x08
  _displaycontrol &= ~0x04; // display
  for (int dp = TOP_PART; dp <= BOTTOM_PART; dp++)
    _send(0x08 | _displaycontrol, false, dp);
} // noDisplay()


void LiquidCrystal_PCF8574_4004::display()
{
  // Instruction: Display on/off control = 0x08
  _displaycontrol |= 0x04; // display
  for (int dp = TOP_PART; dp <= BOTTOM_PART; dp++)
    _send(0x08 | _displaycontrol, false, dp);
} // display()


// Turns the underline cursor on/off
void LiquidCrystal_PCF8574_4004::cursor()
{
  // Instruction: Display on/off control = 0x08
  _displaycontrol |= 0x02; // cursor
  for (int dp = TOP_PART; dp <= BOTTOM_PART; dp++)
    _send(0x08 | _displaycontrol, false, dp);
} // cursor()


void LiquidCrystal_PCF8574_4004::noCursor()
{
  // Instruction: Display on/off control = 0x08
  _displaycontrol &= ~0x02; // cursor
  for (int dp = TOP_PART; dp <= BOTTOM_PART; dp++)
    _send(0x08 | _displaycontrol, false, dp);
} // noCursor()


// Turn on and off the blinking cursor
void LiquidCrystal_PCF8574_4004::blink()
{
  // Instruction: Display on/off control = 0x08
  _displaycontrol |= 0x01; // blink
  for (int dp = TOP_PART; dp <= BOTTOM_PART; dp++)
    _send(0x08 | _displaycontrol, false, dp);
} // blink()


void LiquidCrystal_PCF8574_4004::noBlink()
{
  // Instruction: Display on/off control = 0x08
  _displaycontrol &= ~0x01; // blink
  for (int dp = TOP_PART; dp <= BOTTOM_PART; dp++)
    _send(0x08 | _displaycontrol, false, dp);
} // noBlink()


// These commands scroll the display without changing the RAM
void LiquidCrystal_PCF8574_4004::scrollDisplayLeft(void)
{
  // Instruction: Cursor or display shift = 0x10
  // shift: 0x08, left: 0x00
  for (int dp = TOP_PART; dp <= BOTTOM_PART; dp++)
    _send(0x10 | 0x08 | 0x00, false, dp);
} // scrollDisplayLeft()


void LiquidCrystal_PCF8574_4004::scrollDisplayRight(void)
{
  // Instruction: Cursor or display shift = 0x10
  // shift: 0x08, right: 0x04
  for (int dp = TOP_PART; dp <= BOTTOM_PART; dp++)
    _send(0x10 | 0x08 | 0x04, false, dp);
} // scrollDisplayRight()


// == controlling the entrymode

// This is for text that flows Left to Right
void LiquidCrystal_PCF8574_4004::leftToRight(void)
{
  // Instruction: Entry mode set, set increment/decrement =0x02
  _entrymode |= 0x02;
  for (int dp = TOP_PART; dp <= BOTTOM_PART; dp++)
    _send(0x04 | _entrymode, false, dp);
} // leftToRight()


// This is for text that flows Right to Left
void LiquidCrystal_PCF8574_4004::rightToLeft(void)
{
  // Instruction: Entry mode set, clear increment/decrement =0x02
  _entrymode &= ~0x02;
  for (int dp = TOP_PART; dp <= BOTTOM_PART; dp++)
    _send(0x04 | _entrymode, false, dp);
} // rightToLeft()


// This will 'right justify' text from the cursor
void LiquidCrystal_PCF8574_4004::autoscroll(void)
{
  // Instruction: Entry mode set, set shift S=0x01
  _entrymode |= 0x01;
  for (int dp = TOP_PART; dp <= BOTTOM_PART; dp++)
    _send(0x04 | _entrymode, false, dp);
} // autoscroll()


// This will 'left justify' text from the cursor
void LiquidCrystal_PCF8574_4004::noAutoscroll(void)
{
  // Instruction: Entry mode set, clear shift S=0x01
  _entrymode &= ~0x01;
  for (int dp = TOP_PART; dp <= BOTTOM_PART; dp++)
    _send(0x04 | _entrymode, false, dp);
} // noAutoscroll()


/// Setting the brightness of the background display light.
/// The backlight can be switched on and off.
/// The current brightness is stored in the private _backlight variable to have it available for further data transfers.
void LiquidCrystal_PCF8574_4004::setBacklight(int brightness)
{
  _backlight = brightness;
  // send no data but set the background-pin right;
  _write2Wire(0x00, true, NO_PART);
} // setBacklight()


// Allows us to fill the first 8 CGRAM locations
// with custom characters
void LiquidCrystal_PCF8574_4004::createChar(int location, int charmap[])
{
  location &= 0x7; // we only have 8 locations 0-7
  // Set CGRAM address
  for (int dp = TOP_PART; dp <= BOTTOM_PART; dp++) {
    _send(0x40 | (location << 3), false, dp);
    for (int i = 0; i < 8; i++) {
      _send(charmap[i], true, dp);
    }
  }
} // createChar()


/* The write function is needed for derivation from the Print class. */
inline size_t LiquidCrystal_PCF8574_4004::write(uint8_t ch)
{
  _send(ch, true, _active_part);
  return 1; // assume sucess
} // write()


// write either command or data
void LiquidCrystal_PCF8574_4004::_send(int value, bool isData, int dp)
{
  // write high 4 bits
  _sendNibble((value >> 4 & 0x0F), isData, dp);
  // write low 4 bits
  _sendNibble((value & 0x0F), isData, dp);
} // _send()


// write a nibble / halfByte with handshake
void LiquidCrystal_PCF8574_4004::_sendNibble(int halfByte, bool isData, int dp)
{
  _write2Wire(halfByte, isData, dp);
  delayMicroseconds(1); // enable pulse must be >450ns
  _write2Wire(halfByte, isData, NO_PART);
  delayMicroseconds(37); // commands need > 37us to settle
} // _sendNibble


// private function to change the PCF8674 pins to the given value
// Note:
// you may change this function what the display is attached to the PCF8574 in a different wiring.
void LiquidCrystal_PCF8574_4004::_write2Wire(int halfByte, bool isData, int dp)
{
  // map the given values to the hardware of the I2C schema
  int i2cData = halfByte << 4;
  if (isData)
    i2cData |= PCF_RS;
  // PCF_RW is never used.
  if (dp == TOP_PART)
    i2cData |= PCF_EN_TOP;
  else if (dp == BOTTOM_PART)
    i2cData |= PCF_EN_BOTTOM;

  if (_backlight > 0)
    i2cData |= PCF_BACKLIGHT;

  Wire.beginTransmission(_i2cAddr);
  Wire.write(i2cData);
  byte error = Wire.endTransmission();
  if (error != 0)
    Serial.println("ZHOPA");
} // write2Wire

// The End.
