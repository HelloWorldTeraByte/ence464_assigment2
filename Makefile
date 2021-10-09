CC = gcc
CFLAGS=-pg -g -pthread
BUILD_DIR=build
SRC_DIR=src

all: Ox O1 O2 O3

Ox : poisson1 poisson2 poisson3 poisson4 poisson5 poisson6 

O1: poisson1O1  poisson2O1  poisson3O1  poisson4O1  poisson5O1  poisson6O1 
O2: poisson1O2  poisson2O2  poisson3O2  poisson4O2  poisson5O2  poisson6O2 
O3: poisson1O3  poisson2O3  poisson3O3  poisson4O3  poisson5O3  poisson6O3 

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

poisson5: $(SRC_DIR)/poisson5.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $^

poisson6: $(SRC_DIR)/poisson6.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $^



poisson1O1: $(SRC_DIR)/poisson1.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -O1 -o $(BUILD_DIR)/$@ $^

poisson2O1: $(SRC_DIR)/poisson2.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -O1 -o $(BUILD_DIR)/$@ $^

poisson3O1: $(SRC_DIR)/poisson3.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -O1 -o $(BUILD_DIR)/$@ $^

poisson4O1: $(SRC_DIR)/poisson4.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -O1 -o $(BUILD_DIR)/$@ $^

poisson5O1: $(SRC_DIR)/poisson5.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -O1 -o $(BUILD_DIR)/$@ $^

poisson6O1: $(SRC_DIR)/poisson6.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -O1 -o $(BUILD_DIR)/$@ $^



poisson1O2: $(SRC_DIR)/poisson1.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -O2 -o $(BUILD_DIR)/$@ $^

poisson2O2: $(SRC_DIR)/poisson2.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -O2 -o $(BUILD_DIR)/$@ $^

poisson3O2: $(SRC_DIR)/poisson3.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -O2 -o $(BUILD_DIR)/$@ $^

poisson4O2: $(SRC_DIR)/poisson4.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -O2 -o $(BUILD_DIR)/$@ $^

poisson5O2: $(SRC_DIR)/poisson5.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -O2 -o $(BUILD_DIR)/$@ $^

poisson6O2: $(SRC_DIR)/poisson6.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -O2 -o $(BUILD_DIR)/$@ $^


poisson1O3: $(SRC_DIR)/poisson1.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -O3 -o $(BUILD_DIR)/$@ $^

poisson2O3: $(SRC_DIR)/poisson2.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -O3 -o $(BUILD_DIR)/$@ $^

poisson3O3: $(SRC_DIR)/poisson3.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -O3 -o $(BUILD_DIR)/$@ $^

poisson4O3: $(SRC_DIR)/poisson4.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -O3 -o $(BUILD_DIR)/$@ $^

poisson5O3: $(SRC_DIR)/poisson5.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -O3 -o $(BUILD_DIR)/$@ $^

poisson6O3: $(SRC_DIR)/poisson6.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -O3 -o $(BUILD_DIR)/$@ $^

clean:
	rm -rf $(BUILD_DIR)/