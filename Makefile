all: lights

lights: main.cpp
	g++ -O3 -Wall -Wextra -pedantic -o lights main.cpp

.PHONY: clean
clean:
	rm -f lights
