# Instructions

## Building

Use [ArduinoIDE](https://www.arduino.cc/en/software) or [arduino-cli](https://arduino.github.io/arduino-cli/1.0/).

### Prerequisites

The *src* directory containts symbolic links on ext4 to files in `../Ardymo`. 
This means the application **won't compile on Windows or Mac as is**. 
For the compilation to work, either replace the symbolic links with links
relevant to your platform or copy the files over from the `../Ardymo` directory.

I have found no other way to include the shared code. The *ArduinoIDE* and
*arduino-cli* don't seem to provide another way to instruct the compiler to 
include a source directory other than `src` directly inside the sketch.

If there is a better way, please open an issue or a pull request with the
solution.

### Gameplay

This is not a game but an accessory for the Ardymo application. It displays the
game map and participates in the calculation of sensor values.

The Application can zoom in and out on the map. The zoomed window can be panned
left, right, up and down. The view can follow the vehicle or not. Vehicle
following and display of coordinates can be configured in the menu.

### Controls

| Key    | Action                                            |
|--------|---------------------------------------------------|
| UP     | Move the view up/Menu up                          |
| DOWN   | Move the view down/Menu down                      |
| LEFT   | Move the view left, Previous option in menu item  |
| RIGHT  | Move the view right, Next option in menu item     |
| A      | Menu Select                                       |
| B      | Open/Close menu                                   |
| A+DOWN | Zoom in                                           |
| A+UP   | Zoom out                                          |
