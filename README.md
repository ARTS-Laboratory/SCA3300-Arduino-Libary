# SCA3300-D01 Accelerometer Library for Arduino
An Arduino-compatible driver for the SCA3300-D01 MEMS accelerometer written in
C++.

## Installation
The current recommended installation process is to clone this library to the
Arduino library directory on your system. For Linux/MacOS, this is
'~/.arduino15/libraries'. Windows users can clone to
'Documents\Arduino\libraries'. From there, the 'SCA3300.h' header should be
available to you. You may also choose to download the compressed archive of the
source code and decompress it in the proper directory.

We may add support for installation via Arduino Library Manager in the future.

## Example Code
The SCA3300-D01 is the accelerometer used in our [open-source drone-deployable
structural health monitoring sensing
node](https://github.com/ARTS-Laboratory/Drone-Delivered-Vibration-Sensor), and
this driver was originally made to enable software development for it. As a
result, the best place to find example code for this driver and even an example
hardware configuration for the SCA3300-D01 is on its GitHub repo. The [data
collection
firmware](https://github.com/ARTS-Laboratory/Drone-Delivered-Vibration-Sensor/blob/master/system_design/Bayonne/V0.2/Software/Arduino/dataset-collection/dataset-collection.ino)
provides the simplest application of the SCA3300-D01.

## License
[![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].


[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg






















