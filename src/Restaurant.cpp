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

int Restaurant::getNumOfTables() const {return numOfTables;}


void Restaurant::start() {
    this->open=true;
    int curCustomerId=0;
    cout<<"Restaurant Is Now Open!"<<endl;
    string nextAction;
    cin>>nextAction;
    while (nextAction!="closeall")
    {
        vector<string> splitBySpace=splitStringBytoken(nextAction," ");

        if(splitBySpace[0]=="open") {
            int tableId = stoul(splitBySpace[1]);
            vector<string> customerNamesAndTypes = splitStringBytoken(splitBySpace[2], ",");
            vector<Customer*>customersList;

            for (int i = 0; i < customerNamesAndTypes.size(); i = i + 2) {
                ///!!!delete this pointers !!!
                Customer *customer;
                if (customerNamesAndTypes[i + 1] == "veg") {
                    customer=new VegetarianCustomer(customerNamesAndTypes[i],curCustomerId);

                } else if (customerNamesAndTypes[i+1]=="chp") {
                    customer=new CheapCustomer(customerNamesAndTypes[i],curCustomerId);

                } else if(customerNamesAndTypes[i+1]=="spc") {
                    customer=new SpicyCustomer(customerNamesAndTypes[i],curCustomerId);
                } else { //it is alcoholic csutomer
                    customer=new AlchoholicCustomer(customerNamesAndTypes[i],curCustomerId);
                }
                customersList.push_back(customer);
                curCustomerId=curCustomerId+1;
            }

            ///!!!delete this pointer!!!
            OpenTable* openTable=new OpenTable(tableId,customersList);



        }
        else if(splitBySpace[0]=="order"){

        }
        else if(splitBySpace[0]=="move") {

        }
        else if (splitBySpace[0]=="close")
        {

        }
    }

    //closeall action chosed


}



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

Restaurant& Restaurant::operator=(const Restaurant &rest)
        {
//check for "self assignment" and do nothing in that case
if(this==&rest)
    return *this;

//assigning status
open=rest.open;

//assigning numOfTables
numOfTables=rest.numOfTables;

//assigning tables
for(int i=0;i<tables.size();i++)
    tables.push_back(rest.tables[i]->clone());

//assigning menu
for(int i=0;i<menu.size();i++)
    menu.push_back(rest.menu[i]);

//assigning actionsLog
for(int i=0;i<actionsLog.size();i++)
    actionsLog.push_back(rest.actionsLog[i]->clone());

return *this;
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

void Restaurant::addToActionsLog(BaseAction * actionToAdd) {actionsLog.push_back(actionToAdd);}

//the function return a vector of split string (by token)
std::vector<string> splitStringBytoken(string myStr,string delimiter)
{
    vector<string>splittedString;
    size_t pos=0;
    string token;
    while((pos=myStr.find(delimiter))!=string::npos){
        token=myStr.substr(0,pos);
        splittedString.push_back(token);
        myStr.erase(0,pos+delimiter.length());
    }
    return splittedString;
}