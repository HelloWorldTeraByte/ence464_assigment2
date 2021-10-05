CC = gcc
CFLAGS=-pg -g
BUILD_DIR=build
SRC_DIR=src

all: poisson1 poisson2 poisson3 poisson4

poisson1: $(SRC_DIR)/poisson1.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $^

poisson2: $(SRC_DIR)/poisson2.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $^

poisson3: $(SRC_DIR)/poisson3.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $^

poisson4: $(SRC_DIR)/poisson4.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $^

clean:
	rm -rf $(BUILD_DIR)/