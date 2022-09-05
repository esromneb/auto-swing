# Trying vector
* https://andybrown.me.uk/2011/01/15/the-standard-template-library-stl-for-avr-with-c-streams/

can't find out what .ino vs .cpp does. Can't figure out how to have more than 1 file



# 3.3 <-> 5
* a few options in the comments https://hackaday.com/2016/12/05/taking-it-to-another-level-making-3-3v-and-5v-logic-communicate-with-level-shifters/#comments
* According to this, our board does not need the level shift https://steemit.com/utopian-io/@drencolha/mpu6050-3-axis-gyroscope-and-accelerometer-module-setup-arduino-library-use-shown-with-an-example-project-tutorial


# make library
* how to setup keywords.txt etc http://mc-computing.com/Hardware_Platforms/Arduino/Libraries.html


# MPU
* Examples - https://github.com/ElectronicCats/mpu6050/tree/master/examples
* https://www.arduino.cc/reference/en/libraries/mpu6050/
* https://github.com/electroniccats/mpu6050


# Downloads
* https://www.arduinolibraries.info/libraries/arduino-stl



# Include weirdness
I can't get things inside the src directory to include proerly. Either it's:
* use the library (annoying because of keywords)
* put all files in root (annoying because of cleanliness)
* use blob, best of both but lumps all recompiles into 1

I think I will use blob. If things get out of hand I can go back to library or whatever else I want