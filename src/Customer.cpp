//
// Created by chen on 11/3/18.
//
#include "Customer.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <Customer.h>


using namespace std;

//ctors for all sub-classes

Customer::Customer(std::string c_name, int c_id): name(c_name),id(c_id){}

VegetarianCustomer::VegetarianCustomer(string name, int id): Customer(name,id), type("veg") {}
CheapCustomer::CheapCustomer(string name, int id): Customer(name,id),canOrder(true), type("chp") {}
SpicyCustomer::SpicyCustomer(string name, int id): Customer(name,id),firstOrder(true), type("spc") {}
AlchoholicCustomer::AlchoholicCustomer(string name, int id): Customer(name,id),orderedMostExpensive(false),curAlcDrinkId(-1), type("alc") {}

//START--------------------COPY CONSTRUCTORS-------------------

Customer::Customer(const Customer &otherCustomer):name(otherCustomer.getName()), id(otherCustomer.getId()) {}

VegetarianCustomer::VegetarianCustomer(const VegetarianCustomer &vegCustomer):Customer(vegCustomer), type("veg") {}

CheapCustomer::CheapCustomer(const CheapCustomer &chpCustomer):Customer(chpCustomer), type("chp"), canOrder(chpCustomer.isCanOrder()) {}

SpicyCustomer::SpicyCustomer(const SpicyCustomer &spcCustomer):Customer(spcCustomer),firstOrder(spcCustomer.isFirstOrder()) ,type("spc") {}

AlchoholicCustomer::AlchoholicCustomer(const AlchoholicCustomer &alcCustomer):Customer(alcCustomer), orderedMostExpensive(alcCustomer.isorderedMostExpensive()), curAlcDrinkId(alcCustomer.getCurDrinkId()) ,type("alc") {}

//END--------------------COPY CONSTRUCTORS-------------------

//getId and getName

string Customer::getName() const {return name;}

int Customer::getId() const  {return id;}

//START-----------------------CLONE-----------------------

VegetarianCustomer* VegetarianCustomer::clone() const {return new VegetarianCustomer(*this);}

CheapCustomer* CheapCustomer::clone() const {return new CheapCustomer(*this);}

SpicyCustomer* SpicyCustomer::clone() const {return new SpicyCustomer(*this);}

AlchoholicCustomer* AlchoholicCustomer::clone() const {return new AlchoholicCustomer(*this);}

//END-----------------------CLONE-----------------------

bool CheapCustomer::isCanOrder() const { return canOrder; }
bool SpicyCustomer::isFirstOrder() const { return firstOrder;}
bool AlchoholicCustomer::isorderedMostExpensive() const { return orderedMostExpensive;}
int AlchoholicCustomer::getCurDrinkId() const { return curAlcDrinkId;}


//START---------------------TO STRING-------------------

string VegetarianCustomer::toString() const{
    return to_string(getId())+" "+getName();
}

string CheapCustomer::toString() const{
    return to_string(getId())+" "+getName();
}

string SpicyCustomer::toString() const{
    return to_string(getId())+" "+getName();
}

//END---------------------TOS STRING-------------------


string AlchoholicCustomer::toString() const{
    return to_string(getId())+" "+getName();
}

//implementation of 'getType' for each customer type

string VegetarianCustomer::getType() const {return "veg";}

string CheapCustomer::getType() const {return "chp";}

string SpicyCustomer::getType() const {return "spc";}

string AlchoholicCustomer::getType() const {return "alc";}

//implementations of 'order' for each customer type
vector<int> VegetarianCustomer::order(const std::vector<Dish> &menu) {
    vector<int> orderedDishes;
    int orderedVeg=-1; //-1 means not found yet in iteration
    int orderedBvgPrice=-1; //'dummy' beverage
    int orderedBvg=-1;
    //will iterate the @menu looking for the first veg dish and most expensive non alc beverage
    for(size_t i=0;i<menu.size();i++)
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
    vector<int> orderedDishes;
    if(canOrder)
    {

        int orderedDishId=0;
        int orderedDishPrice=menu[0].getPrice();
        for(size_t i=1;i<menu.size();i++)
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
    return orderedDishes;
}

vector<int> SpicyCustomer::order(const std::vector<Dish> &menu) {
    vector<int> orderedDishes;
    if (firstOrder) //means this spicy customer hasn't order yet, spicy dish will be picked
    {
        int orderedSpcPrice = -1; //'dummy' beverage
        int orderedSpc = -1;
        for (size_t i = 0; i < menu.size(); i++) {
            if (menu[i].getType() == SPC & menu[i].getPrice() > orderedSpcPrice) {
                orderedSpcPrice = menu[i].getPrice();
                orderedSpc = i;
            }
        }
        orderedDishes.push_back(orderedSpc);
        firstOrder = false;

    } else //already made an order, now will order cheapest non-alc bvg
    {
        int orderedBvgMinPriceIndex = 0;

        for (size_t i = 1; i < menu.size(); i++) {
            if (menu[i].getType() == BVG & menu[i].getPrice() < menu[orderedBvgMinPriceIndex].getPrice()) {
                orderedBvgMinPriceIndex = i;
            }
        }
        orderedDishes.push_back(menu[orderedBvgMinPriceIndex].getId());
    }
    return orderedDishes;
}

//START----------------------DESTRUCTORS------------------

Customer::~Customer() {}

SpicyCustomer::~SpicyCustomer() {}

VegetarianCustomer::~VegetarianCustomer() {}

CheapCustomer::~CheapCustomer() {}

AlchoholicCustomer::~AlchoholicCustomer() {}

//END----------------------DESTRUCTORS------------------

vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu) {
    vector<int> nextAlcDish;
    setNextExpensiveDrinkId(menu);
    if(!orderedMostExpensive)
        nextAlcDish.push_back(curAlcDrinkId);

    return nextAlcDish;


}

//this function return Dish from the menu by the dish id
Dish AlchoholicCustomer::getDishById(int dishId,const std::vector<Dish> &menu) {

    for(size_t i=0;i<menu.size();i=i+1)
    {
        if(menu[i].getId()==dishId)
            return menu[i];
    }
}

// the function updates 'curAlcDrinkId' field to the next expensive Alcoholic drink from the menu
//if the customer already ordered the most expensive or there are not ALC drink in the menu the function will change 'orderMostExpensive' field to be true
void AlchoholicCustomer::setNextExpensiveDrinkId(const std::vector<Dish> &menu) {

    if(!orderedMostExpensive) {
        std::vector<Dish> vector_nextAlcDrink; //the vector will contains the next available alcDrinks from the menu

        if (curAlcDrinkId == -1) //the alcoholic customer didn't ordered yet
        {
            //lets add the alcoholic drinks to the vector
            for (size_t i = 0; i < menu.size(); i = i + 1) {
                Dish nextDish = menu[i];
                if (nextDish.getType() == ALC) {
                    vector_nextAlcDrink.push_back(nextDish);
                }

            }

        } else { //the alcoholic customer had ordered drink before
            Dish curAlcDrink = getDishById(curAlcDrinkId, menu);

            for (size_t i = 0; i < menu.size(); i = i + 1) {
                Dish nextDish = menu[i];
                if (nextDish.getType() == ALC) {
                    if (nextDish.getPrice() > curAlcDrink.getPrice() ||
                        nextDish.getPrice() == curAlcDrink.getPrice() & nextDish.getId() > curAlcDrink.getId()) {
                        vector_nextAlcDrink.push_back(nextDish);
                    }
                }

            }

        }

        if (vector_nextAlcDrink.size() == 0) //there are not Alc dishes on the menu or the customer ordered the most expensive alcoholic drink on the menu
        {
            orderedMostExpensive = true; //the customer can't order any more
            return;
        } else {
            int minPriceIndex = 0;
            for (size_t i = 1; i < vector_nextAlcDrink.size(); i = i + 1) {

                if (vector_nextAlcDrink[i].getPrice() < vector_nextAlcDrink[minPriceIndex].getPrice() ||
                    vector_nextAlcDrink[i].getPrice() == vector_nextAlcDrink[minPriceIndex].getPrice() &
                    vector_nextAlcDrink[i].getId() < vector_nextAlcDrink[minPriceIndex].getId()) {
                    minPriceIndex = i;
                }
            }
            curAlcDrinkId = vector_nextAlcDrink[minPriceIndex].getId(); //updating the new curAlcDrink id
            return;
        }

    }
    return;

}


