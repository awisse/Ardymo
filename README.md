# Description

A game for the Arduboy Mini with the ATmega32U4. For the [Mini
Jam](https://community.arduboy.com/t/arduboy-mini-jam-win-an-ipad-mini/12295/61) due October 31st, 2024.

## Remarks

The game in the
[Ardymo](https://github.com/awisse/Ardymo/tree/7c01b704e8a9441d824b4c3ba460043306ef6824/Ardymo) 
directory is a fully featured game which demonstrates the 
use of the I2C capabilities of the 
[Arduboy mini](https://www.arduboy.com/shop/p/arduboy-mini).

In order to fully use the I2C capabilities, the user needs two Arduboy minis,
one with the Ardymo application and the other with the Map application as well
as a QWIIC cable to connect them.

## I2C

The I2C bus is used for two purposes:

1. The Ardymo application transmits information about the vehicle to the Map
   application.

2. The Ardymo application requests certain sensor values from the Map
   application. The Map application computes these and sends them to the Ardymo
   application.

## About the code

### Speed

The code contains many floating point calculations for which there are no 
instructions on the ATmega32U4. 

Without computation sharing between the Ardymo
and Map applications, one cycle takes about 67ms on Ardymo and 47ms on Map for
the display applications. When sharing the computations, the time needed rises
to 77ms on Map and decreases to 47ms on Ardymo. 


### Space

The code is not optimized for space but for readability. Both the Map and
Ardymo sketches take up more than 94% of the program memory. There is space 
left for about 10 more obstacles on the map. With some work, this can surely 
be improved upon.
