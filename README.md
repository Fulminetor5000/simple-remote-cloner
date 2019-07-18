# Simple Remote Cloner
This script allow you to clone an IR message, save it in the EEPROM (not volatile memory) and send it back using Arduino.

## Prerequisites
You need to install IRremote library https://github.com/z3t0/Arduino-IRremote on Arduino IDE.

## Most frequent problem
If the script doesn't work properly, probably you have reached the storage limit imposed by IRremote, to fix go to the library directory
(on windows, Documents -> Arduino) and change with a bigger value (I recommend at least 300) the constant "RAWBUF" placed in IRremoteInt around the line 40,
more info googling "irremote rawbuf".

## Arduino physical scheme example
![alt text](https://github.com/Fulminetor5000/simple_remote_cloner/blob/master/example.jpg?raw=true)
