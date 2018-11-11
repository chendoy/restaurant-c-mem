#All Targets
all: Main

#Tool invocations
#Ececutable "main" depends on the files Dish.o, main.o, Restaurant.o, Table.o
Main: bin/Main.o bin/Dish.o bin/Restaurant.o bin/Table.o
	@echo 'Building target: main'
	@echo 'Invoking: C++ Linker'
	g++ -o bin/main bin/main.o bin/Dish.o bin/Restaurant.o bin/Table.o
	@echo 'Finished building target: hello'
	@echo ' '

# Depends on the source and header files
bin/main.o: src/Main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Main.o src/Main.cpp

# Depends on the source and header files 
bin/Restaurant.o: src/Restaurant.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Restaurant.o src/Restaurant.cpp

# Depends on the source and header files
bin/main.o: src/Dish.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Dish.o src/Dish.cpp

# Depends on the source and header files
bin/main.o: src/Table.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Table.o src/Table.cpp

#Clean the build directory
clean: 
	rm -f bin/*
