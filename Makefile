CC := g++
CFLAGS := -O2 -Wall -Wextra
SRC_FOLDER := ./source
SOURCES := $(wildcard ./source/*.cpp)
INCLUDES := $(wildcard ./include/*.hpp)
BIN_FOLDER := ./bin
BINARIES := $(addprefix $(BIN_FOLDER)/lib, $(addsuffix .a, $(basename $(notdir $(SOURCES)))))
IFLAGS := -I./include/
LDFLAGS := -I./include/ -ldatetime
LINKDIR := -L./bin/

# all: $(BIN_FOLDER)/libdatetime.a main
all: $(BIN_FOLDER) $(BINARIES) main

main: ./main.cpp $(INCLUDES) $(BINARIES)
	$(CC) $(CFLAGS) $< -o $@ $(LINKDIR) $(LDFLAGS)

$(BIN_FOLDER):
	mkdir $(BIN_FOLDER)

$(BINARIES): $(SOURCES) $(BIN_FOLDER)
	rm -f $@
	$(CC) $(CFLAGS) $< -c -o $(basename $@).o $(IFLAGS)
	ar rcs $@ $(basename $@).o
	rm -f $(basename $@).o

.PHONY: test clean

test: main
	./main

clean:
	rm -f ./main
	rm -rf $(BIN_FOLDER)