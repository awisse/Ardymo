# Description

A game for the Arduboy Mini with the ATmega32U4. For the jam due October 
31st, 2024.

## Remarks

At this stage, this is a proof of concept rather than a fully featured game. 
Bells and whistles like a menu, preferences, levels, high scores etc. are 
notably absent. However, it is a fully playable game which demonstrates the 
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

Surely this can be optimized for speed. 


### Space

Also, the code is not optimized for space but for readability. Both the Map and
Ardymo sketches takeup about 90% of the program memory. There is space left for
about 50 more obstacles on the map. With some work, this can surely be improved upon.
