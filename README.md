Travis [![Build Status](https://travis-ci.org/trikset/trik-desktop-gamepad.svg?branch=master)](https://travis-ci.org/trikset/trik-desktop-gamepad)

Appveyor [![Build status](https://ci.appveyor.com/api/projects/status/asrrwcndmcfnxeww/branch/master?svg=true)](https://ci.appveyor.com/project/iakov/trik-desktop-gamepad/branch/master)

# trik-desktop-gamepad

Simple desktop gamepad for TRIK on C++/Qt.
Gamepad can be thought as a set of additional sensors for robot, which can be accessed from scripts, so there is no
intrinsic behavior associated with gamepad commands. To see that gamepad actually does something it is needed
to write a script that uses it or to use already written program, like "remoteControl" from TRIK Studio examples.

TRIK gamepad is active when TRIK GUI is launched (and gamepad is enabled in config files, which is true by default).
It listens port 4444 (by default, can be changed in configs) and accepts following commands:
* pad <id> <x> <y>  --- pad with given id is pressed in (x, y) point. Valid ids are 1 for left pad and 2 for right
      pad, coordinates are from -100 to 100, where -100 is on top (or to the left) and 100 is bottom or right
      of a pad.
* pad <id> up  --- pad with given id is released. Valid ids are 1 for left pad and 2 for right pad.
* btn <id>  --- "magic button" with given id is pressed. Valid ids are 1, 2, 3, 4 and 5.
* wheel <percent>  --- tilt of a gamepad reported by gyroscope. Valid percents are in range [-100, 100], where -100
      corresponds to maximum left tilt, 100 --- maximum right. Wheel commands are not shown in this example to keep
      it simple.
All commands are separated by '\n' symbol. So example of a data packet sent to a robot for "pad" command is
"pad 1 0 -100\n", excluding quotes.
