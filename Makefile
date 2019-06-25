all: lights

lights: main.c
	gcc -O3 -Wall -Wextra -pedantic -o lights main.c

.PHONY: clean
clean:
	rm -f lights
