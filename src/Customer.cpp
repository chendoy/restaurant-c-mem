//
// Created by chen on 11/3/18.
//
#include "Customer.h"
#include <string>
#include <iostream>

using namespace std;

//ctors for all sub-classes

Customer::Customer(std::string c_name, int c_id): name(c_name),id(c_id){}

VegetarianCustomer::VegetarianCustomer(string name, int id): Customer(name,id) {}
CheapCustomer::CheapCustomer(string name, int id): Customer(name,id),canOrder(true) {}
SpicyCustomer::SpicyCustomer(string name, int id): Customer(name,id),firstOrder(true) {}
AlchoholicCustomer::AlchoholicCustomer(string name, int id): Customer(name,id),threshold(0) {}

//getId and getName

string Customer::getName() const {return name;}

int Customer::getId() const  {return id;}

//implementations of 'toString' for each customer type

string VegetarianCustomer::toString() const{
    return to_string(getId())+" "+getName();
}

string CheapCustomer::toString() const{
    return to_string(getId())+" "+getName();
}

string SpicyCustomer::toString() const{
    return to_string(getId())+" "+getName();
}

string AlchoholicCustomer::toString() const{
    return to_string(getId())+" "+getName();
}

//implementations of 'order' for each customer type

vector<int> VegetarianCustomer::order(const std::vector<Dish> &menu) {
    vector<int> orderedDishes;
    int orderedVeg=-1; //-1 means not found yet in iteration
    int orderedBvgPrice=-1; //'dummy' beverage
    int orderedBvg=-1;
    //will iterate the @menu looking for the first veg dish and most expensive non alc beverage
    for(int i=0;i<menu.size();i++)
    {
        if(orderedVeg==-1 & menu[i].getType()==VEG)
            orderedVeg=i;

        if(menu[i].getType()==BVG & menu[i].getPrice()>orderedBvgPrice) {
            orderedBvgPrice = menu[i].getPrice();
            orderedBvg = i;
        }
    }
    if(orderedVeg==-1 | orderedBvg==-1)
        cout<<"Error: customer "<<getName()<<" couldn't complete his order"<<endl;
    orderedDishes.push_back(orderedVeg);
    orderedDishes.push_back(orderedBvg);
    return orderedDishes;
}

vector<int> CheapCustomer::order(const std::vector<Dish> &menu) {
    if(canOrder)
    {
        vector<int> orderedDishes;
        int orderedDishId=0;
        int orderedDishPrice=menu[0].getPrice();
        for(int i=1;i<menu.size();i++)
        {
            if(menu[i].getPrice()<orderedDishPrice) {
                orderedDishId = i;
                orderedDishPrice=menu[i].getPrice();
            }
            this->canOrder=false;
        }
        orderedDishes.push_back(orderedDishId);
        return orderedDishes;
    }
}

vector<int> SpicyCustomer::order(const std::vector<Dish> &menu) {
    vector<int> orderedDishes;
    if (firstOrder) //means this spicy customer hasn't order yet, spicy dish will be picked
    {
        int orderedSpcPrice = -1; //'dummy' beverage
        int orderedSpc = -1;
        for (int i = 0; i < menu.size(); i++) {
            if (menu[i].getType() == SPC & menu[i].getPrice() > orderedSpcPrice) {
                orderedSpcPrice = menu[i].getPrice();
                orderedSpc = i;
            }
        }
        orderedDishes.push_back(orderedSpc);
        firstOrder = false;
    } else //already made an order, now will order cheapest non-alc bvg
    {
        int orderedBvgPrice = 0;
        int orderedBvg = 0;
        for (int i = 1; i < menu.size(); i++) {
            if (menu[i].getType() == BVG & menu[i].getPrice() < orderedBvgPrice) {
                orderedBvgPrice = menu[i].getPrice();
                orderedBvg = i;
            }
        }
        orderedDishes.push_back(orderedBvg);
    }
    return orderedDishes;
}

vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu) {
    return vector<int>(); //not completed
}