# c128kbd
A USB HID controller for a C128 keyboard, using a frickin' Teensy 4.1

This uses Teensyduino. See the sketch for pin numbers and info on the key mapping.

At this point, the included VICE keymap is not complete.


References:
https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf
https://github.com/PaulStoffregen/cores/blob/master/teensy4/keylayouts.h


If you want to use this code for another keyboard, you need to assign I/O pins
to the rows and columns of your keyboard matrix. The pin numbers go into
the lists "colpins" and "rowpins".

After this, I suggest you fill in the commented-out matrix "keyname" with the
actual key names for each position in the matrix. When this is done, just update the
"keycode" matrix with all the corresponding keycodes you want to send for each key.
If you are using a Teensy, look at the "keylayouts.h" file linked above for all
the key codes available. If you are using an Arduino Micro, Arduino Leonardo or
Sparkfun Pro Micro or similar, you should be able to find the key codes in the
Arduino documentation.

A suggestion is to get rid of the "keycode2" array, rename "keycode1" to "keycode",
eliminate the current "keycode" variable and the code in checkForChanges() that
switches between "keycode1" and "keycode2".
