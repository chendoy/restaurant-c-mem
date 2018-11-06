//
// Created by chen on 11/2/18.
//
#include <Dish.h>
using namespace std;

Dish::Dish(int d_id, string d_name, int d_price, DishType d_type):id(d_id),name(d_name),price(d_price),type(d_type){}

//copy constructor
Dish::Dish(const Dish &otherDish):id(otherDish.getId()),name(otherDish.getName()),price(otherDish.getPrice()),type(otherDish.getType()){};

int Dish::getId() const {return id;}

std::string Dish::getName() const {return name;}

int Dish::getPrice() const {return price;}

DishType Dish::getType() const {return type;}

Dish* Dish::clone() {return new Dish(*this);}