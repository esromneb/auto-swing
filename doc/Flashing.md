# Cmd
```bash
/usr/share/arduino/hardware/tools/avr/bin/avrdude -q -V -p atmega328p -C /usr/share/arduino/hardware/tools/avr/etc/avrdude.conf -D -c arduino -b 115200 -P /dev/ttyACM0 \
		-U flash:w:build-uno/auto-swing_.hex:i

```
