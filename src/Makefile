all: compile flash

setup:
	arduino-cli board attach arduino:avr:uno

compile: ctags
	arduino-cli compile -b arduino:avr:uno -utp /dev/ttyACM0 ../main
	# arduino-builder is deprecated!
	# arduino-builder -fqbn archlinux-arduino:avr:uno -hardware /usr/share/arduino/hardware -tools /usr/bin -compile main.ino
	# arduino-builder -fqbn archlinux-arduino:avr:uno -hardware /usr/share/arduino/hardware -tools /usr/bin -compile main.ino
	# arduino-builder -build-options-file buildopts.json main.ino
	# arduino-builder compile --fqbn arduino:avr:uno 1031_main.ino

flash:
	arduino-cli upload -b arduino:avr:uno -tp /dev/ttyACM0 ../main

ctags:
	ctags --language-force=c++ *.h *.ino
