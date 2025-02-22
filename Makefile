CC := g++
CFLAGS := -O2 -Wall -Wextra -Wno-maybe-uninitialized -std=c++20
SRC_FOLDER := ./source
SOURCES := $(wildcard ./source/*.cpp)
INCLUDES := $(wildcard ./include/*.hpp)
BIN_FOLDER := ./bin
BINARIES := $(addprefix $(BIN_FOLDER)/lib, $(addsuffix .a, $(basename $(notdir $(SOURCES)))))
BINARIES_DEBUG := $(addprefix $(BIN_FOLDER)/lib, $(addsuffix dbg.a, $(basename $(notdir $(SOURCES)))))
IFLAGS := -I./include/
# LDFLAGS := -I./include/ -ldatetime
LDFLAGS := -I./include/ $(addprefix -l, $(basename $(notdir $(SOURCES))))
# LDFLAGS_DEBUG := -I./include/ -ldatetimedbg
LDFLAGS_DEBUG := -I./include/ $(addsuffix dbg, $(addprefix -l, $(basename $(notdir $(SOURCES)))))
LINKDIR := -L./bin/

all: $(BIN_FOLDER) main

debug: ./main.cpp $(INCLUDES) $(BINARIES_DEBUG)
	$(CC) $(CFLAGS) -g $< -o $(basename $<) $(LINKDIR) $(LDFLAGS_DEBUG)

main: ./main.cpp $(INCLUDES) $(BINARIES)
	echo $(BINARIES)
	$(CC) $(CFLAGS) $< -o $@ $(LINKDIR) $(LDFLAGS)

$(BIN_FOLDER):
	mkdir $(BIN_FOLDER)

$(BINARIES): $(SOURCES) $(BIN_FOLDER)
	rm -f $@
	$(CC) $(CFLAGS) $< -c -o $(basename $@).o $(IFLAGS)
	ar rcs $@ $(basename $@).o
	rm -f $(basename $@).o

$(BINARIES_DEBUG): $(SOURCES) $(BIN_FOLDER)
	rm -f $@
	$(CC) $(CFLAGS) -g $< -c -o $(basename $@).o $(IFLAGS)
	ar rcs $@ $(basename $@).o
	rm -f $(basename $@).o

.PHONY: run qcPerf qcRead clean

run: main
	./main

qcPerf:
	clang-tidy -checks="-*,boost-*,clang-analyzer-*,modernize-*,performance-*,cppcoreguidelines-*,-cppcoreguidelines-avoid-magic-numbers" main.cpp ./source/* ./include/* -- -I./include/ -std=c++20

qcRead:
	clang-tidy -checks="-*,boost-*,clang-analyzer-*,readability-*,-readability-magic-numbers" main.cpp ./source/* ./include/* -- -I./include/ -std=c++20

clean:
	rm -f ./main
	rm -rf $(BIN_FOLDER)