# Description

This is the main game. It compiles by default with I2C communication enabled
(`#define USE_I2C` in *defines.h*).

If the *Map* I2C client is not available, the display will show *NACK 
(Address)* for five seconds and after that the game can be played without the 
*Map* application. No more attempts are made to establish I2C communication.

## Gameplay

### Objective

The player is an AI driving a vehicle and has to rely exclusively on the sensor
values to reach the target. The objective is to reach the target without
crashing into obstacles or into the target. The run is successful if the
vehicle gets within 5m of the target at zero speed.

If the player crashes into an obstacle the vehicle is replaced at the starting
position. The coordinates of the crash are displayed. If the player crashes
into the target, the target is destroyed and the game is lost.

### Screen
![Screen](../doc/odg/screen.pdf)

The sensor values available are:
1. The distances to the nearest obstacle to the left, the right, rearward and 
   in front of the vehicle.

2. The distance to the target: the box in the top left corner.

3. The direction of the target with respect to the vehicle axis: the dot on the
   circle in the center of the screen.

4. The north with respect to the vehicle axis: the line inside the circle.
   hea

5. The heading of the vehicle with respect to the north: the number in degrees
   inside the circle.

6. The coordinates of the vehicle in the lower left corner.

### Movement

The vehicle is accelerated forward with the UP button and rearward with the
DOWN button. The vehicle turns left or right around the midpoint of its rear 
side by 2 degrees when the LEFT or RIGHT buttons are pressed respectively.

### Controls

A long A press restarts the game. I2C communication is attempted again. is can also be used to try to reestablish
I2C communication.

