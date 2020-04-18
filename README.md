# Kvinlumo

A 5-light lighting system controller for bicycles (and possibly other road
vehicles) made for Arduino boards. Currently only tested on an Arduino UNO
board.

## Features

Controls 5 lights:

1. Headlight
2. Front turning light, left
3. Front turning light, right
4. Rear braking and turning light, left
5. Rear braking and turning light, right

Supports different modes that affect the ilumination pattern of all lights:

* **Day and night modes.** Enables headlight and always-on tail lights to
  increase visibility in dark environments.
* **Blinking and steady ilumination modes.** Select between blinking and steady
  lighting modes.
* **Turning mode.** Indicate whether the vehicle is turning left, right or
  staying on heading.
* **Braking mode.** Indicate whether the vehicle is braking.

The ilumination patterns are defined in the doc/requirements.ods spreadsheet.

Modes are currently selected by using multiple gestures on two buttons: a left
button and a right button. The gestures are defined below:

* Single press toggles turning signals
* Long press on left button toggles between blinking and steady ilumination
  modes.
* Long press on right button toggles between day lights and night lights.

## Building the circuit

The schematics to build this project are currently under development.

## Building the software

The software can be built with command line tools on Linux or with the Arduino
IDE on Windows.

To build on Linux, simply run

        make compile

on the sw/ directory.

To build on Windows, load the main.ino file and upload the sketch to the
Arduino.

## License

See LICENSE.txt.

## Credits

The project idea, circuit design and software have been developed by
**wolterhv**.

The schematics have been developed by **Smudge6446**.

For a list of contributors see AUTHORS.txt.
