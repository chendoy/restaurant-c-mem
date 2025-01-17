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

extern Restaurant* backup;

//empty ctor
Restaurant::Restaurant():open(false), tables(),menu(),actionsLog() {}

//copy ctor
Restaurant::Restaurant(const Restaurant &rest):open(rest.isOpen())
{
    numOfTables=rest.numOfTables;

    //deep copying tables
    for(int i=0;i<rest.numOfTables;i++)
        tables.push_back(rest.tables[i]->clone());

    //deep copying dishes
    for(size_t i=0;i<rest.menu.size();i++)
        menu.push_back(rest.menu[i].clone());

    //deep copying actions log
    for(size_t i=0;i<rest.actionsLog.size();i++)
        actionsLog.push_back(rest.actionsLog[i]->clone());
}

//move ctor
Restaurant::Restaurant(Restaurant &&otherRest):open(otherRest.isOpen()),menu(otherRest.getMenu()),numOfTables(otherRest.getNumOfTables()) {

    //copy vector of pointers to table
    tables=otherRest.getAllTables();
    //changing otherRest table's pointers to point to nullptr
    for(size_t i=0;i<tables.size();i=i+1) {
        otherRest.setTablePointer(nullptr,i);
    }

    //copy vector of pointers to BaseActions to actionsLog
    actionsLog=otherRest.getAllBaseActions();
    //changing otherRest actionLog's pointers to point to nullptr
    for(size_t i=0;i<actionsLog.size();i=i+1) {
        otherRest.setActionLogsPointer(nullptr,i);
    }
}

bool Restaurant::isOpen() const {
    return open;
}
vector<Dish> Restaurant::getDishes() const {
    return menu;
}
vector<Table*> Restaurant::getAllTables() const {
    return tables;
}
void Restaurant::setTablePointer(Table *tablePtr,int index) {
    tables[index]=tablePtr;
}
vector<BaseAction*> Restaurant::getAllBaseActions() const {
    return actionsLog;
}
void Restaurant::setActionLogsPointer(BaseAction *baseAction, int index) {
    actionsLog[index]=baseAction;
}

//converts a string to fruit Enum defined in the header file
DishType fruit_convert(const string& str)
{
    if(str == "VEG") return VEG;
    else if(str == "SPC") return SPC;
    else if(str == "BVG") return BVG;
    else if(str == "ALC") return ALC;
    return VEG; //default value
}

std::vector<string> Restaurant::splitStringBytoken(string myStr,string delimiter)
{
    vector<string>splittedString;
    size_t pos=0;
    string token;
    while((pos=myStr.find(delimiter))!=string::npos){
        token=myStr.substr(0,pos);
        splittedString.push_back(token);
        myStr.erase(0,pos+delimiter.length());
    }
    token=myStr.substr(0,pos);
    splittedString.push_back(token);
    return splittedString;
}


Restaurant::Restaurant(const std::string &configFilePath):open(true)  {
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
        if(line !="" && line!="\r" && line[0]!='#')
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
                        tables.push_back(new Table(tableCapacity));
                        line.erase(0,pos+delimiter.length());
                    }
                    line.erase(0,pos+delimiter.length());
                    token=line.substr(0,pos);
                    tableCapacity=stoul(token);
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
        line.clear();
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
    cout<<"Restaurant is now open!"<<endl;

    string nextLine;
    getline(cin,nextLine);
    vector<string> splitBySpace=splitStringBytoken(nextLine," ");
    string nextAction=splitBySpace[0];

    while (nextAction!="closeall")
    {

        if(nextAction=="open") {
            int tableId = stoul(splitBySpace[1]);
            vector<Customer*>customersList;

            for (size_t i = 2; i < splitBySpace.size(); i = i + 1) {
                vector<string> customerNameAndType = splitStringBytoken(splitBySpace[i], ",");
                Customer *customer;
                if (customerNameAndType[1] == "veg") {
                    customer=new VegetarianCustomer(customerNameAndType[0],curCustomerId);

                } else if (customerNameAndType[1]=="chp") {
                    customer=new CheapCustomer(customerNameAndType[0],curCustomerId);

                } else if(customerNameAndType[1]=="spc") {
                    customer=new SpicyCustomer(customerNameAndType[0],curCustomerId);
                } else { //it is alcoholic csutomer
                    customer=new AlchoholicCustomer(customerNameAndType[0],curCustomerId);
                }
                customersList.push_back(customer);
                curCustomerId=curCustomerId+1;
            }

            if((tableId>=numOfTables) || ((getTable(tableId)!=nullptr) && (getTable(tableId)->isOpen()))) { //openTable is going to fail
                OpenTable *openTableAction = new OpenTable(tableId, customersList);
                openTableAction->act(*this);

                customersList.clear();
                addToActionsLog(openTableAction);


            }
            else { //openTable is going to succeed
                OpenTable *openTableAction = new OpenTable(tableId, customersList);
                openTableAction->act(*this);
                addToActionsLog(openTableAction);
            }



            }

        else if(nextAction=="order"){
            int table_num=stoi(splitBySpace[1]);
            Order* orderAction=new Order(table_num);
            orderAction->act(*this);
            addToActionsLog(orderAction);

        }
        else if(nextAction=="move") {
            int src=stoi(splitBySpace[1]);
            int dst=stoi(splitBySpace[2]);
            int customerId=stoi(splitBySpace[3]);
            MoveCustomer* moveAction=new MoveCustomer(src,dst,customerId);
            moveAction->act(*this);
            addToActionsLog(moveAction);

        }
        else if (nextAction=="close") {
            int table_num=stol(splitBySpace[1]);
            Close* closeAction=new Close(table_num);
            closeAction->act(*this);
            addToActionsLog(closeAction);


        }

        else if(nextAction=="menu") {
            PrintMenu* printMenuAction=new PrintMenu();
            printMenuAction->act(*this);
            addToActionsLog(printMenuAction);
        }

        else if(nextAction=="status")
        {
            int tableId=stol(splitBySpace[1]);
            PrintTableStatus* printTableStatusAction=new PrintTableStatus(tableId);
            printTableStatusAction->act(*this);
            addToActionsLog(printTableStatusAction);

        }
        else if(nextAction=="log"){
            PrintActionsLog* printActionsLogAction=new(PrintActionsLog);
            printActionsLogAction->act(*this);
            addToActionsLog(printActionsLogAction);

        }
        else if(nextAction=="backup"){
            BackupRestaurant* backupAction=new BackupRestaurant();
            backupAction->act(*this);
            addToActionsLog(backupAction);

        }
        else if(nextAction=="restore"){
            RestoreResturant* restoreAction=new RestoreResturant();
            restoreAction->act(*this);
            addToActionsLog(restoreAction);

        }

        getline(cin,nextLine);
        splitBySpace.clear();
        splitBySpace=splitStringBytoken(nextLine," ");
        nextAction=splitBySpace[0];
    }

    CloseAll closeAllAction;
    closeAllAction.act(*this);

    //'closeall' action was chosen


}



//the function returns a pointer to table in the table vector
//if the table doesn't exist in the vector the function will return a pointer to nullptr
Table* Restaurant::getTable(int ind) {

    if((size_t)ind>=tables.size())//not a valid index
    {
        //return  nullptr
        return nullptr;
    } else {
        return tables[ind];
    }

}

//assignment operator
Restaurant& Restaurant::operator=(const Restaurant &rest)
        {

//check for "self assignment" and do nothing in that case
if(this==&rest)
    return *this;

//deleting old tables
for(size_t i=0;i<tables.size();i++) {
    delete tables[i];
    tables[i]=nullptr;
}

//deleting old action log
    for(size_t i=0;i<actionsLog.size();i++) {
        delete actionsLog[i];
        actionsLog[i]=nullptr;
    }


tables.clear();menu.clear(); actionsLog.clear();

open=rest.isOpen();

numOfTables=rest.getNumOfTables();

//deep copying tables
for(size_t i=0;i<rest.tables.size();i++)
    tables.push_back(rest.tables[i]->clone());

//deep copying menu
for(size_t i=0;i<rest.menu.size();i++)
    menu.push_back(rest.menu[i].clone());


//deep copying actionLog
for(size_t i=0;i<rest.actionsLog.size();i++)
    actionsLog.push_back(rest.actionsLog[i]->clone());

return *this;

        }

//move assignment operator
Restaurant &Restaurant::operator=(Restaurant&& otherRest) {


    //destroy old resources
    for(size_t i=0;i<tables.size();i=i+1) {
        if(tables[i]!= nullptr)
            delete tables[i];

        tables[i]= nullptr;
    }
    for(size_t i=0;i<actionsLog.size();i=i+1) {
        if(actionsLog[i]!= nullptr)
            delete actionsLog[i];

        actionsLog[i]= nullptr;
    }
    menu.clear();
    //copy other's data fields
    tables=otherRest.getAllTables();
    actionsLog=otherRest.getAllBaseActions();
    menu=otherRest.getDishes();
    numOfTables=otherRest.getNumOfTables();
    //Detach resources from other
    for(size_t i=0;i<(size_t)otherRest.getNumOfTables();i=i+1) {
        otherRest.setTablePointer(nullptr,i);
    }
    for(size_t i=0;i<otherRest.getAllBaseActions().size();i=i+1) {
        otherRest.setActionLogsPointer(nullptr,i);
    }

    return *this;
}


Restaurant::~Restaurant () {
//delete pointers to tables
    for (size_t i = 0; i < tables.size(); i = i + 1) {
        if(tables[i]!=nullptr) {
            delete (tables[i]);
            tables[i] = nullptr;
        }
    }
    tables.clear();

//delete pointer of BaseActions
    for (size_t i = 0; i < actionsLog.size(); i = i + 1) {
        if(actionsLog[i]!=nullptr) {
            delete (actionsLog[i]);
            actionsLog[i] = nullptr;
        }
    }
    actionsLog.clear();
}


const vector<BaseAction*>& Restaurant::getActionsLog() const {return actionsLog;}

vector<Dish>& Restaurant::getMenu() {return menu;}

void Restaurant::addToActionsLog(BaseAction * actionToAdd) {actionsLog.push_back(actionToAdd);}

//the function return a vector of split string (by token)
