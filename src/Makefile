all: compile flash

compile:
	arduino-builder -fqbn archlinux-arduino:avr:uno -hardware /usr/share/arduino/hardware -tools /usr/bin main.ino
	# arduino-builder -build-options-file buildopts.json main.ino
	# arduino-builder compile --fqbn arduino:avr:uno 1031_main.ino
	# arduino-cli compile --fqbn arduino:avr:uno -utp /dev/ttyACM0 .

flash:
	arduino-cli upload --fqbn arduino:avr:uno -tp /dev/ttyACM0 .
