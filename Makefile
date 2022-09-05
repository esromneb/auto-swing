# Arduino Make file. Refer to https://github.com/sudar/Arduino-Makefile


ARDUINO_DIR=/usr/share/arduino
ARDMK_DIR=/usr/share/arduino
#     AVR_TOOLS_DIR – Directory where avr tools are installed

AVRDUDE_CONF=/usr/share/arduino/hardware/tools/avrdude.conf

BOARD_TAG    = uno

ARDUINO_SKETCHBOOK=.

# You can specify space separated list of libraries that are needed for your sketch in the variable ARDUINO_LIBS.
ARDUINO_LIBS=ArduinoSTL-1.3.3 Wire


include $(ARDMK_DIR)/Arduino.mk




# I ran make download, and it couldn't find the correct path
# So I patched it and this works
# I suspect this will break if the board name or whatever else changes
# flash:
# 	/usr/share/arduino/hardware/tools/avr/bin/avrdude -q -V -p atmega328p -C /usr/share/arduino/hardware/tools/avrdude.conf -D -c arduino -b 115200 -P /dev/ttyACM0 \
# 		-U flash:w:build-uno/auto-swing_.hex:i











# --- leonardo (or pro micro w/leo bootloader)
#BOARD_TAG    = leonardo
#MONITOR_PORT = /dev/ttyACM0
#include /usr/share/arduino/Arduino.mk

# --- mega2560 ide 1.0
#BOARD_TAG    = mega2560
#ARDUINO_PORT = /dev/ttyACM0
#include /usr/share/arduino/Arduino.mk

# --- mega2560 ide 1.6
#BOARD_TAG    = mega
#BOARD_SUB    = atmega2560
#MONITOR_PORT = /dev/ttyACM0
#ARDUINO_DIR  = /where/you/installed/arduino-1.6.5
#include /usr/share/arduino/Arduino.mk

# --- nano ide 1.0
#BOARD_TAG    = nano328
#MONITOR_PORT = /dev/ttyUSB0
#include /usr/share/arduino/Arduino.mk

# --- nano ide 1.6
#BOARD_TAG   = nano
#BOARD_SUB   = atmega328
#ARDUINO_DIR = /where/you/installed/arduino-1.6.5
#include /usr/share/arduino/Arduino.mk

# --- pro mini
#BOARD_TAG    = pro5v328
#MONITOR_PORT = /dev/ttyUSB0
#include /usr/share/arduino/Arduino.mk

# --- sparkfun pro micro
#BOARD_TAG         = promicro16
#ALTERNATE_CORE    = promicro
#BOARDS_TXT        = $(HOME)/arduino/hardware/promicro/boards.txt
#BOOTLOADER_PARENT = $(HOME)/arduino/hardware/promicro/bootloaders
#BOOTLOADER_PATH   = caterina
#BOOTLOADER_FILE   = Caterina-promicro16.hex
#ISP_PROG          = usbasp
#AVRDUDE_OPTS      = -v
#include /usr/share/arduino/Arduino.mk

# --- chipkit
#BOARD_TAG = mega_pic32
#MPIDE_DIR = /where/you/installed/mpide-0023-linux64-20130817-test
#include /usr/share/arduino/chipKIT.mk

# --- pinoccio
#BOARD_TAG         = pinoccio256
#ALTERNATE_CORE    = pinoccio
#BOOTLOADER_PARENT = $(HOME)/arduino/hardware/pinoccio/bootloaders
#BOOTLOADER_PATH   = STK500RFR2/release_0.51
#BOOTLOADER_FILE   = boot_pinoccio.hex
#CFLAGS_STD        = -std=gnu99
#CXXFLAGS_STD      = -std=gnu++11
#include /usr/share/arduino/Arduino.mk

# --- fio
#BOARD_TAG = fio
#include /usr/share/arduino/Arduino.mk

# --- atmega-ng ide 1.6
#BOARD_TAG    = atmegang
#BOARD_SUB    = atmega168
#MONITOR_PORT = /dev/ttyACM0
#ARDUINO_DIR  = /where/you/installed/arduino-1.6.5
#include /usr/share/arduino/Arduino.mk

# --- teensy3
#BOARD_TAG   = teensy31
#ARDUINO_DIR = /where/you/installed/the/patched/teensy/arduino-1.0.6
#include /usr/share/arduino/Teensy.mk

# --- mighty 1284p
#BOARD_TAG         = mighty_opt
#BOARDS_TXT        = $(HOME)/arduino/hardware/mighty-1284p/boards.txt
#BOOTLOADER_PARENT = $(HOME)/arduino/hardware/mighty-1284p/bootloaders
#BOOTLOADER_PATH   = optiboot
#BOOTLOADER_FILE   = optiboot_atmega1284p.hex
#ISP_PROG          = usbasp
#AVRDUDE_OPTS      = -v
#include /usr/share/arduino/Arduino.mk

# --- mightycore
#ALTERNATE_CORE = MightyCore
#BOARD_TAG = 1284
#MCU = atmega1284p
#BOARD_TAG = 644
#MCU = atmega644p
#VARIANT = standard
#F_CPU = 16000000L
#include /usr/share/arduino/Arduino.mk

# ---  megacore
#BOARD_TAG      = 128
#ALTERNATE_CORE = megacore
#F_CPU          = 16000000L
#include /usr/share/arduino/Arduino.mk

# --- sanguino 644p
#BOARD_TAG      = atmega644p
#ALTERNATE_CORE = sanguino
#ISP_PROG       = usbasp
#AVRDUDE_OPTS   = -v
#include /usr/share/arduino/Arduino.mk

# --- atmega328p on breadboard
#BOARD_TAG    = atmega328bb
#ISP_PROG     = usbasp
#AVRDUDE_OPTS = -v
#BOARDS_TXT   = $(HOME)/arduino/hardware/breadboard/boards.txt
#include /usr/share/arduino/Arduino.mk
