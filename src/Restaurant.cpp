//
// Created by chen on 11/2/18.
//
#include "Restaurant.h"
#include <iostream>
using namespace std;

//chen: empty ctor
Restaurant::Restaurant():open(false), tables(),menu(),actionsLog() {}

Restaurant::Restaurant(const std::string &configFilePath):open(true) {

}