CC := g++
CFLAGS := -O2 -Wall -Wextra
SOURCES := ./source/*.cpp
LDFLAGS := -I./include/

main: ./main.cpp
	$(CC) $(CFLAGS) $< $(SOURCES) -o $@ $(LDFLAGS)

.PHONY: test clean

test: ./main
	./main

clean:
	rm -r ./main