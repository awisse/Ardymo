# Instructions

## Building

Use [ArduinoIDE](https://www.arduino.cc/en/software) or [arduino-cli](https://arduino.github.io/arduino-cli/1.0/).

### Prerequisites

The *src* directory containts symbolic links on ext4 to files in 
`../Ardymo`. This means the application **won't compile on Windows or Mac as is**. 
For the compilation to work, either replace the symbolic links with links
relevant to your platform or copy the files over from the `../Ardymo` directory.

I have found no other way to include the shared code. The *ArduinoIDE* and
*arduino-cli* don't seem to provide another way to instruct the compiler to include a
source directory other than `src` directly inside the sketch.

If there is a better way, please open an issue or a pull request with the
solution.

### Gameplay

This is not a game but an accessory for the Ardymo application. It displays the
game map and participates in the calculation of sensor values.

The Application can zoom in and out on the map. The zoomed window can be panned
left, right, up and down. The view can follow the vehicle or not.

### Controls

| Key    | Action                                  |
|--------|-----------------------------------------|
| UP     | Move the view up                        |
| DOWN   | Move the view down                      |
| LEFT   | Move the view left                      |
| RIGHT  | Move the view right                     |
| Long A | Toggle follow vehicle                   |
| Long B | Toggle display coordinates of vehicle   |
