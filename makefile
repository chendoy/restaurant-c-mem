# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = g++
CFLAGS  = -g -Wall -std=c++11
LFLAGS  = -L/usr/lib

# All Targets
all: rest

# Tool invocations
rest: bin/Restaurant.o bin/Table.o bin/Dish.o bin/Customer.o bin/Action.o bin/Main.o
	@echo 'Building target: rest'
	@echo 'Invoking: C++ Linker'
	$(CC) -o bin/rest bin/Restaurant.o bin/Table.o bin/Dish.o bin/Customer.o bin/Action.o bin/Main.o $(LFLAGS)
	@echo 'Done'
	@echo ' '

# Depends on the source and header files
bin/Restaurant.o: src/Restaurant.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Restaurant.o src/Restaurant.cpp
	

# Depends on the source and header files 
bin/Main.o: src/Main.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Main.o src/Main.cpp

bin/Action.o: src/Action.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Action.o src/Action.cpp
	
bin/Table.o: src/Table.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Table.o src/Table.cpp
	
bin/Dish.o: src/Dish.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Dish.o src/Dish.cpp
	
bin/Customer.o: src/Customer.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Customer.o src/Customer.cpp


#Clean the build directory
clean: 
	rm -f bin/*
