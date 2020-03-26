all: compile flash

compile:
	arduino-cli compile --fqbn arduino:avr:uno 1031_bikelights.ino machinestates.c buttons.c
	# arduino-cli compile --fqbn arduino:avr:uno -utp /dev/ttyACM0 .

flash:
	arduino-cli upload --fqbn arduino:avr:uno -tp /dev/ttyACM0 .
