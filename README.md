# PGSensorHub - A Power Glove input collector

This project exists to supplement the ESP8266 firmware ([esp-ar-drone](https://github.com/nullhan/esp-ar-drone)) used in my [Power Glove UHID](https://hackaday.io/project/8455-power-glove-uhid) project. The program collects the keypad state, flex sensors and IMU data from my modified Power Glove and sends it serially to the ESP8266. It is largely based on examples and can still be improved upon immensely to better fit this very specific application.

## Requirements

 - [LSM303 library](https://github.com/pololu/lsm303-arduino) by Pololu (assuming usage of the AltIMU-v4 10-DoF IMU sensor)
 - [Keypad library](https://github.com/Chris--A/Keypad) by Christopher Andrews

## Usage

The state of the keypad, flex sensors and IMU are collected and processed. The flex sensors are matched to relative states (at rest or flexed) based on their position. The data is then sent serially formatted (in ASCII) as follows:

```
K,G,(-)###,(-)###<LF>
```

Where **K** is a single `char` representing the pressed button on the keypad, **G** is a single `char` representing the current gesture being performed using the Power Glove and **(-)###** are pitch and roll values (`int` type), respectively, with optional negative signs.

Currently the following Power Glove buttons and gestures are processed:
###Buttons
- 'S' \- The Select button, used to send the emergency command
- 'P' \- The Start button, used to send takeoff/land command when landed/flying, respectively
- '0' (zero) \- The 0 or Center buttons, used to calibrate trim (use only when grounded)

###Gestures
- 'R' \- Relaxed gesture, when flying, puts the drone in hover mode.
- 'F' \- Fist gesture, when flying, causes the drone to move forward/back and left/right, according to the level of tilt in the Power Glove
- 'P' \- Pointing gesture, when flying, causes the drone to move up/down and yaw Cw/CCW, according to the level of tilt in the Power Glove

## Contributing

1. Fork it!
2. Create your feature branch: `git checkout -b my-new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request :D

## History

Version 0.1: initial upload

## License

Refer to LICENSE file
