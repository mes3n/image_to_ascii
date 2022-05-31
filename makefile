# Variables to control Makefile operation
CC = g++
MYDIR = .
SRC_DIR = $(MYDIR)/src
OBJ_DIR = $(MYDIR)/obj
CFLAGS = -Wall -g
LDFLAGS = -Wall -g

# create list of object files from source files but replace ".cpp" and "src"
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp))


main: $(OBJ_FILES)
	$(CC) $(LDFLAGS) -o bin/$@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

all: clean main


# header: spec_file main
#
# spec_file:
# 	$(CC) $(CFLAGS) -o $(OBJ_FILE) -c $(SRC_FILE)


.PHONY: clean
clean:
	rm -rf bin obj
	mkdir bin obj obj/entities
