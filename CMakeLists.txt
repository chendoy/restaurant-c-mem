cmake_minimum_required(VERSION 3.12)
project(SPL_ass1)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(SPL_ass1
        include/Action.h
        include/Customer.h
        include/Dish.h
        include/Restaurant.h
        include/Table.h
        config1.txt
        src/Main.cpp
        src/Restaurant.cpp src/Table.cpp src/Dish.cpp src/Customer.cpp src/Action.cpp)
