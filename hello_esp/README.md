# Hello World Example

Starts a FreeRTOS task to print "Hello World"

See the README.md file in the upper level 'examples' directory for more information about examples.
https://github.com/OtherCrashOverride/esp-idf/tree/release/v3.2-odroid
https://forum.odroid.com/viewtopic.php?t=31750

#serial port /dev/cu.SLAB_USBtoUART
or
#/dev/cu.usbserial-14330
#BaudRate 115200

Build .bin
#either  make -j4 flash monitor
#make -j4 all

#copy bin to tools/mkfw


$(IDF_PATH)/hello_esp/build/
Build .fw
./mkfw test tile.raw 0 16 1048576 app $(IDF_PATH)/hello_esp/build/hello-world.bin
