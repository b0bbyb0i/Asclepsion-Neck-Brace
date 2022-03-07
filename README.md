# Asclepsion-Neck-Brace
A collection of designs and software for Eight Block EDD with Mr. Hulsey. Takes a lot of libraries and mashes them together to create a device that detects concussion-level 
acceleration (using the MPU 6050 and Elegoo Nano) on a football helmet and locks the helmet to a neck brace using motor-clamped pistons and MOSFETs. Includes some CAD models 
(Better Geabox a modification of "Worm gearbox 1:60" by daGHIZmo on Thingiverse, https://www.thingiverse.com/thing:3079719) and some accelerometer/motor software (Uses Arduino 
library i2cdev byjrowberg on Github, https://github.com/jrowberg/i2cdevlib#installation).
# Circuitry
https://drive.google.com/file/d/1oQKKA-zEA0MykPX0acs9WuGdiq6JGyO7/view?usp=sharing
Link leads to a circuit diagram with four mosfets controlling four motors with the Nano, powered by a nine volt battery.
# Software
The default outputs for the 6 axis gyro display as x,y,z acceleration and x,y,z rotation. Gravity in my location reads at about 16300.
