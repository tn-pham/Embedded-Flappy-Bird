all:
		arm-linux-gnueabihf-gcc -pthread -Wall -Werror -g -std=c99 -D _POSIX_C_SOURCE=200809L -lpthread ledMatrix.c screenGameplay.c microphone.c main.c i2c_display.c joystick_control.c welcomeScreen.c screenText.c endScreen.c udp_listener.c preGameScreen.c setupScreen.c potentiometer.c -o FlappyBird
		cp FlappyBird $(HOME)/Desktop/cmpt433/public/myApps/
