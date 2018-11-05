//
// Created by chen on 11/2/18.
//
#include "Restaurant.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Dish.h"
using namespace std;

//empty ctor
Restaurant::Restaurant():open(false), tables(),menu(),actionsLog() {}

//converts a string to fruit Enum defined in the header file
DishType fruit_convert(const string& str)
{
    if(str == "VEG") return VEG;
    else if(str == "SPC") return SPC;
    else if(str == "BVG") return BVG;
    else if(str == "ALC") return ALC;
}


Restaurant::Restaurant(const std::string &configFilePath) {
ifstream inFile (configFilePath.c_str());

//will hold current line in config file
string line;



/*
 * 'region' variable meaning:
 * zero means haven't read 'number of table' section yet
 * one means already read 'number of table' section , but not description
 * two means already read number and description, reading menu items is in progress
 */
int region=0;
int numOfTables=0;
int tableCapacity=0;
int runningId=0; //sequential number that will go from 0 to ["number of tables"]-1
vector<string> currDish; //will hold the dishes when we will parse this section

try{

    while(getline(inFile,line))
    {
        if(line[0]!='#')
        {

            switch (region)
            {
                //reading numbers
                case 0: {
                    numOfTables = stoul(line);
                    region++;
                }
                    break;
                //reading description
                case 1: {

                    string delimiter=",";
                    size_t pos=0;
                    string token;
                    while((pos=line.find(delimiter))!=string::npos){
                        token=line.substr(0,pos);
                        tableCapacity=stoul(token);
                        //!!!delete this pointers!!
                        tables.push_back(new Table(tableCapacity));
                        line.erase(0,pos+delimiter.length());
                    }
                    //!!!delete this pointers!!
                    tables.push_back(new Table(tableCapacity));
                    }
                    region++;


                    break;

                //reading menu
                case 2: {
                    string delimiter=",";
                    size_t pos=0;
                    string token;
                    while((pos=line.find(delimiter))!=string::npos){
                        token=line.substr(0,pos);
                        currDish.push_back(token);
                        line.erase(0,pos+delimiter.length());
                    }
                    token=line.substr(0,pos);
                    currDish.push_back(token);
                    Dish newDish(runningId,currDish[0],stoi(currDish[2]),fruit_convert(currDish[1]));
                    menu.push_back(newDish);
                    runningId++;
                    currDish.clear(); //clearing the vector for the next line (=dish) parsing
                }
                    break;
                default:
                    break;
            }

        }

    }
    inFile.close();
    this->numOfTables=numOfTables;

}catch(const string & msg){
    if(inFile.is_open()) inFile.close();
    cout<<"Error !!";
}
}

void Restaurant::start() {
    this->open=true;
    cout<<"Restaurant Is Now Open!"<<endl;

}


int Restaurant::getNumOfTables() const {return numOfTables;}

//the function returns a pointer to table in the table vector
//if the table doesn't exist in the vector the function will return a pointer to nullptr
Table* Restaurant::getTable(int ind) {

    if(ind>=tables.size())//not valid index
    {
        //return a pointer to nullptr
        return nullptr;
    } else {
        return tables[ind];
    }

}

Restaurant::~Restaurant () {
//delete pointers to tables
for(int i=0;i<tables.size();i=i+1)
{
    delete(tables[i]);
    tables[i]= nullptr;
}
//clear the tables vector
tables.clear();
}

const vector<BaseAction*>& Restaurant::getActionsLog() const {return actionsLog;}

vector<Dish>& Restaurant::getMenu() {return menu;}

