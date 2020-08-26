# Arduino Library for 40x4 (4004) LiquidCrystal displays with I2C PCF8574 adapter

This is a library based on an excellent `LiquidCrystal_PCF8574`
library by Matthias Hertel. It is used for driving LiquidCrystal
displays 40 columns and 4 rows (LCD 4004) by using the I2C bus and an
PCF8574 I2C adapter.

The difference of these displays from all other LCD displays that can
be used with `LiquidCrystal_PCF8574` is that they are basically two
40x2 displays stacked on top of each other. This leads to differences
in software and hardware support that I couldn't find anywhere on the
Internet. This library is intended to address this problem.

Library interface is the same as of `LiquidCrystal_PCF8574` which in
its turn repeats API of `LiquidCrystal` library, so it should be easy
to use in an application if you used these LCD libraries. Personally I
tested it with Surenoo SLC4004B which is compatible with HD44780
wiring.

# Wiring

Here I'll try to explain a bit about wiring since I had a hard time
understanding this myself. 4004 displays have the same parallel
interface as all other LCD displays with one exception. That's
it. It is necessary to connect a second EN pin to PCF8574 parallel
output. For this I chose to use RW pin which was assigned to pin 3 in
original `LiquidCrystal_PCF8574`. This pin potentially could be used
for reading from display, but original `LiquidCrystal_PCF8574` didn't
implement this functionality so this pin is basically unused. To make
LCD work for writing only I connected RW PIN to ground permanently.

Here are pins on my Surenoo SLC4004B LCD display:

| pin number | meaning                              | connection |
|------------|--------------------------------------|------------|
| 1          | databus bit 7                        | PCF8574 P7 |
| 2          | databus bit 6                        | PCF8574 P6 |
| 3          | databus bit 5                        | PCF8574 P5 |
| 4          | databus bit 4                        | PCF8574 P4 |
| 5          | databus bit 3                        | |
| 6          | databus bit 2                        | |
| 7          | databus bit 1                        | |
| 8          | databus bit 0                        | |
| 9          | EN1 (enable pin for top part of LCD) | PCF8574 P1 |
| 10         | RW PIN                               | Grounded   |
| 11         | RS                                   | PCF8574 P0 |
| 12         | V0 | connected to ground through 4.7 KOhm potenciomenter |
| 13         | Vss                                  | Grounded   |
| 14         | Vdd                                  | +5V power  |
| 15         | EN2 (enable pin for bottom part of LCD) | PCF8574 P1 |

My LCD doesn't have a backlight control by software, instead it has
separate pins 17 and 18 which are used to supply power to backlight. I
connected them to +5V power through another 4.7 KOhm
potenciomenter. If your LCD has backlight, you can use P3 of PCF8574
to connect it.

Databus connection found experimentally. I suppose some LCDs may use
databus bigs 0-3 instead of 4-7. There is no standard for it.
