# Keyboard firmware for Blink ESP32

This firmware is a fork of @Galzai's MK32 keyboard firmware that was rewriten to use jumptables as keyboard matrix layers. It also contains some fixes for the matrix scanning logic.

https://reddit.com/r/blink_keebs

# Building instructions
* Connect the keyboard
* Run docker container with build environment using `./run.sh`
* Inside the container run `make flash`

use `ESPBAUD= make monitor` to monitor debug output over the serial connection.
