all: compile flash

compile:
	arduino-builder -fqbn archlinux-arduino:avr:uno -hardware /usr/share/arduino/hardware -tools /usr/bin bikelights.ino
	# arduino-builder -build-options-file buildopts.json bikelights.ino
	# arduino-builder compile --fqbn arduino:avr:uno 1031_bikelights.ino
	# arduino-cli compile --fqbn arduino:avr:uno -utp /dev/ttyACM0 .

flash:
	arduino-cli upload --fqbn arduino:avr:uno -tp /dev/ttyACM0 .
