//
// Created by chendoy on 11/5/18.
//
#include "Action.h"
#include "Restaurant.h"
#include "Customer.h"
#include "Table.h"
#include <vector>
#include "Dish.h"
#include <string>
#include <Action.h>


using namespace std;
extern Restaurant* backup;

//check if needed here
string dishtypeToString(const DishType dishtype)
{
    if(dishtype == VEG) return "VEG";
    else if(dishtype == SPC) return "SPC";
    else if(dishtype == BVG) return "BVG";
    else if(dishtype == ALC) return "ALC";
    return "";
}

string actionStatusToString(const ActionStatus actionStatus)
{
    if(actionStatus==PENDING) return " Pending";
    else if(actionStatus==COMPLETED) return " Completed";
    else if(actionStatus==ERROR) return " Error: ";
    return "";
}

BaseAction::BaseAction():status(PENDING) {}

ActionStatus BaseAction::getStatus() const {return this->status;}

void BaseAction::complete()
{
    this->status=COMPLETED;
}

void BaseAction::error(std::string errorMsg)
{
    this->status=ERROR;
    this->errorMsg=errorMsg;
}


string BaseAction::getErrorMsg() const {return this->errorMsg;}


//START---------------------------DESTRUCTORS-----------------------------

BaseAction::~BaseAction() {}

OpenTable::~OpenTable()
{
    for(size_t i=0;i<customers.size();i++)
    {
        if(customers[i]!=nullptr)
        {
        delete customers[i];
        customers[i]=nullptr;
        }
    }
    customers.clear();
}

Order::~Order() {}

MoveCustomer::~MoveCustomer() {}

Close::~Close() {}

CloseAll::~CloseAll() {}

PrintMenu::~PrintMenu() {}

PrintTableStatus::~PrintTableStatus() {}

PrintActionsLog::~PrintActionsLog() {}

BackupRestaurant::~BackupRestaurant() {}

RestoreResturant::~RestoreResturant() {}

//END-----------------------------DESTRUCTORS-----------------------------

//START----------------------------COPY CONSTRUCTORS----------------------

BaseAction::BaseAction(const BaseAction &other):errorMsg(other.errorMsg), status(other.status) {}

OpenTable::OpenTable(const OpenTable &other):BaseAction(other),tableId(other.tableId)
{
    for(size_t i=0;i<other.customers.size();i++)
    {
        if(other.customers[i]!=nullptr)
            customers.push_back(other.customers[i]->clone());
    }
}

Order::Order(const Order &other):BaseAction(other),tableId(other.tableId) {}

MoveCustomer::MoveCustomer(const MoveCustomer &other):BaseAction(other),srcTable(other.srcTable), dstTable(other.dstTable), id(other.id) {}

Close::Close(const Close &other):BaseAction(other),tableId(other.tableId) {}

CloseAll::CloseAll(const CloseAll &other):BaseAction(other) {}

PrintMenu::PrintMenu(const PrintMenu &other):BaseAction(other) {}

PrintTableStatus::PrintTableStatus(const PrintTableStatus &other):BaseAction(other), tableId(other.tableId) {}

PrintActionsLog::PrintActionsLog(const PrintActionsLog &other):BaseAction(other) {}

BackupRestaurant::BackupRestaurant(const BackupRestaurant &other):BaseAction(other) {}

RestoreResturant::RestoreResturant(const RestoreResturant &other):BaseAction(other) {}

//END ----------------------------COPY CONSTRUCTORS----------------------

//START---------------------------MOVE CONSTRUCTORS-----------------------------

BaseAction::BaseAction(BaseAction &&baseAction): errorMsg(baseAction.getErrorMsg()),status(baseAction.getStatus()) {}

OpenTable::OpenTable(OpenTable &&openTable):BaseAction(openTable),tableId(openTable.getTableId()),   customers(openTable.getCustomers()) {}

Order::Order(Order &&order):BaseAction(order), tableId(order.getTableId()) {}

MoveCustomer::MoveCustomer(MoveCustomer &&moveCustomer):BaseAction(moveCustomer),srcTable(moveCustomer.getSrcTable()), dstTable(moveCustomer.getDstTable()), id(moveCustomer.getId())  {}

Close::Close(Close &&close):BaseAction(close), tableId(close.getTableId()) {}

CloseAll::CloseAll(CloseAll &&closeAll):BaseAction(closeAll) {}

PrintMenu::PrintMenu(PrintMenu &&printMenu):BaseAction(printMenu) {}

PrintTableStatus::PrintTableStatus(PrintTableStatus &&printTableStatus):BaseAction(printTableStatus), tableId(printTableStatus.getTableId()) {}

PrintActionsLog::PrintActionsLog(PrintActionsLog &&printActionsLog):BaseAction(printActionsLog) {}

BackupRestaurant::BackupRestaurant(BackupRestaurant &&backupRestaurant):BaseAction(backupRestaurant) {}

RestoreResturant::RestoreResturant(RestoreResturant &&restoreResturant):BaseAction(restoreResturant) {}

//END-----------------------------MOVE CONSTRUCTORS-----------------------------

//START-----------------------------MOVE ASSIGNMENT OPERATORS-----------------------------

BaseAction &BaseAction::operator=(BaseAction &&baseAction)
{
    errorMsg=baseAction.getErrorMsg();
    status=baseAction.getStatus();

    return *this;
}


//END-------------------------------MOVE ASSIGNMENT OPERATORS-----------------------------

//START----------------------------COPY ASSIGNMENT OPERATORS----------------------

BaseAction &BaseAction::operator=(const BaseAction &baseAction) {

    //check for "self assignment" and do nothing in that case
    if(this==&baseAction)
        return *this;

    errorMsg.clear(); //clears whatever errorMsg *this holds
    errorMsg=baseAction.getErrorMsg();
    status=baseAction.getStatus();

    return *this;
}


CloseAll &CloseAll::operator=(const CloseAll &closeAll)
{
    BaseAction::operator=(closeAll);
    return *this;
}

PrintMenu &PrintMenu::operator=(const PrintMenu &printMenu)
{
    BaseAction::operator=(printMenu);
    return *this;
}

PrintActionsLog &PrintActionsLog::operator=(const PrintActionsLog &printActionsLog)
{
    BaseAction::operator=(printActionsLog);
    return *this;
}

BackupRestaurant &BackupRestaurant::operator=(const BackupRestaurant &backupRestaurant)
{
    BaseAction::operator=(backupRestaurant);
    return *this;
}

RestoreResturant &RestoreResturant::operator=(const RestoreResturant &restoreResturant)
{
    BaseAction::operator=(restoreResturant);
    return *this;
}

//END------------------------------COPY ASSIGNMENT OPERATORS----------------------


OpenTable::OpenTable (int id, vector<Customer *> &customersList):BaseAction(),tableId(id),customers(customersList)
{

}

void OpenTable::act(Restaurant &restaurant)
{
    //table does not exist       --OR--        //table is already open
    if(restaurant.getNumOfTables()<=tableId || restaurant.getTable(tableId)->isOpen()) {
        string toPrint;
        for(size_t i=0;i<customers.size();i++) {
            toPrint.append(customers[i]->getName());
            toPrint.append(",");
            toPrint.append(customers[i]->getType()+" ");
            customers[i]=nullptr;
        }
        string errorMsgToPrint;
        errorMsgToPrint.append("Table does not exist or is already open");
        error(errorMsgToPrint);
        toPrint.append(actionStatusToString(getStatus()));
        toPrint.append(getErrorMsg());
        toPrint.append("\n");
        errorMsgToPrint.insert(0,actionStatusToString(getStatus()));
        errorMsgToPrint=errorMsgToPrint.substr(1,errorMsgToPrint.length()-1);
        cout<<errorMsgToPrint<<endl;
        return ;}

    //checks if the user try to assign more customer to table than the table capacity
    if((size_t)customers.size()>(size_t)restaurant.getTable(tableId)->getCapacity()) {
        error("you cant assign more customer to a table then the table capacity");
        cout<<getErrorMsg()<<endl;
        return;}

        //mark the table as open
        restaurant.getTable(tableId)->openTable();

        //assigns the customers to it
        for (size_t i = 0; i < customers.size(); i++)
            restaurant.getTable(tableId)->addCustomer(customers[i]->clone());
        complete();

}


string OpenTable::toString() const
{
    string toReturn="open ";
    toReturn.append(to_string(tableId));
    toReturn.append(" ");


    //appends the customers and their types
    for(size_t i=0;i<customers.size();i++) {
        if(customers[i]!=nullptr) {
        toReturn.append(customers[i]->getName());
        toReturn.append(",");
        toReturn.append(customers[i]->getType());
        toReturn.append(" ");
        }
    }
        toReturn=toReturn.substr(0,toReturn.length()-1);
        toReturn.append(actionStatusToString(getStatus()));
        toReturn.append(getErrorMsg());
        toReturn.append("\n");
        return toReturn;
}

OpenTable* OpenTable::clone() {return new OpenTable(*this);}

int OpenTable::getTableId() const {
    return tableId;
}

std::vector<Customer *> OpenTable::getCustomers() {
    return customers;
}


//Order START
Order::Order(int id):BaseAction(),tableId(id){}

void Order::act(Restaurant &restaurant) {


    //restaurant.addToActionsLog(this);

    Table* table=restaurant.getTable(tableId);
    if(table== nullptr || !table->isOpen())
    {
        error("Table does not exist or is not open");
        string toPrint;
        toPrint.append(actionStatusToString(getStatus()));
        toPrint.append(getErrorMsg());
        toPrint=toPrint.substr(1,toPrint.length()-1);
        cout<<toPrint<<endl;

    } else{
        table->order(restaurant.getMenu());
        complete();
    }

}

std::string Order::toString() const {
    string toReturn;
    toReturn.append("order ");
    toReturn.append(to_string(tableId));
    toReturn.append(actionStatusToString(getStatus()));
    toReturn.append(getErrorMsg());
    toReturn.append("\n");
    return toReturn;
}

Order* Order::clone() {return new Order(*this);}

int Order::getTableId() const {
    return tableId;
}

//Order END

////MoveCustomer START
MoveCustomer::MoveCustomer(int src, int dst, int customerId):BaseAction(),srcTable(src),dstTable(dst),id(customerId) {}

void MoveCustomer::act(Restaurant &restaurant) {

    //restaurant.addToActionsLog(this);

    Table *tblSrc=restaurant.getTable(srcTable);
    Table *tblDest=restaurant.getTable(dstTable);

    if((tblSrc== nullptr)||(tblDest==nullptr)||(!tblSrc->isOpen())|(!tblDest->isOpen())|(!tblSrc->isCustomerAtTable(id))|
                                                       ((size_t)tblDest->getCapacity()==(size_t)tblDest->getCustomers().size()))
    {
        error("Cannot move customer");
        string toPrint;
        toPrint.append(actionStatusToString(getStatus()));
        toPrint.append(getErrorMsg());
        toPrint=toPrint.substr(1,toPrint.length()-1);
        cout<<toPrint<<endl;
    }
    else {//can move customer
        tblDest->addCustomer(tblSrc->getCustomerById(id));
        //add costumer orders (from src table to dest table)
        std::vector<OrderPair>customerOrders=tblSrc->getCustomerOrders(id);
        tblDest->addNewCustomerOrdersToBill(customerOrders); //add customer orders to 'dest' orderList
        //deleting the customer from the current table
        tblSrc->removeCustomer(id);
    }
    complete();

}

string MoveCustomer::toString() const
{
    string toReturn;
    toReturn.append("move ");
    toReturn.append(to_string(srcTable)+" ");
    toReturn.append(to_string(dstTable)+" ");
    toReturn.append(to_string(id));
    toReturn.append(actionStatusToString(getStatus()));
    toReturn.append(getErrorMsg());
    toReturn.append("\n");
    return toReturn;
}


MoveCustomer* MoveCustomer::clone() {return new MoveCustomer(*this);}

int MoveCustomer::getSrcTable() const {
    return srcTable;
}

int MoveCustomer::getDstTable() const {
    return dstTable;
}

int MoveCustomer::getId() const {
    return id;
}

Close::Close(int id): tableId(id){}

void Close::act(Restaurant &restaurant)
{

    //restaurant.addToActionsLog(this);


    //table does not exist                             //table is not open
    if(restaurant.getNumOfTables()<=tableId || restaurant.getTable(tableId)->isOpen()==false) {
        error("Table does not exist or is not open");
        string toPrint;
        toPrint.append(actionStatusToString(getStatus()));
        toPrint.append(getErrorMsg());
        toPrint=toPrint.substr(1,toPrint.length()-1);
        cout<<toPrint<<endl;

    }
    else {

        string toPrint = "Table ";
        toPrint.append(to_string(tableId));
        toPrint.append(" was closed. Bill ");
        toPrint.append(to_string(restaurant.getTable(tableId)->getBill()));
        toPrint.append("NIS");
        cout << toPrint << endl;

        restaurant.getTable(tableId)->closeTable();
        complete();
    }
}

string Close::toString() const {
    string toReturn;
    toReturn.append("Closed table ");
    toReturn.append(to_string(tableId));
    toReturn.append(" ");
    toReturn.append(actionStatusToString(getStatus()));
    toReturn.append(getErrorMsg());
    toReturn.append("\n");
    return toReturn;

}

Close* Close::clone() {return new Close(*this);}

int Close::getTableId() const {
    return tableId;
}


CloseAll::CloseAll():BaseAction() {}


void CloseAll::act(Restaurant &restaurant)
{

    //restaurant.addToActionsLog(this);

    for(size_t i=0;i<restaurant.getAllTables().size();i++) {
        if(restaurant.getTable(i)->isOpen())
        {
            Close* closeAction=new Close(i);
            closeAction->act(restaurant);
            delete closeAction;

        }

    }
    //NEED TO 'CLOSE THE RESTAURANT' ---HOW TO DO THIS?
    complete();
}

string CloseAll::toString() const {return "Close all tables";}

CloseAll* CloseAll::clone() {return new CloseAll(*this);}

PrintMenu::PrintMenu():BaseAction() {}

void PrintMenu::act(Restaurant &restaurant)
{

    //restaurant.addToActionsLog(this);

    string toPrint="";
    for(size_t i=0;i<restaurant.getMenu().size();i++)
    {
        toPrint.append(restaurant.getMenu()[i].getName()+" ");
        toPrint.append(dishtypeToString(restaurant.getMenu()[i].getType())+" ");
        toPrint.append(to_string(restaurant.getMenu()[i].getPrice()));
        toPrint.append("NIS+\n");
    }
    toPrint=toPrint.substr(0,toPrint.length()-1); //removes last /n
    cout<<toPrint<<endl;
    complete();
}

string PrintMenu::toString() const {

    string toReturn;
    toReturn.append("Menu printed ");
    toReturn.append(actionStatusToString(getStatus()));
    toReturn.append(getErrorMsg());
    toReturn.append("\n");
    return toReturn;

}

PrintMenu* PrintMenu::clone() {return new PrintMenu(*this);}

PrintTableStatus::PrintTableStatus(int id):BaseAction(), tableId(id) {}

void PrintTableStatus::act(Restaurant &restaurant)
{

    //restaurant.addToActionsLog(this);
    string toPrint;

    //appending table status
    toPrint.append("Table ");
    toPrint.append(to_string(tableId));
    if(!restaurant.getTable(tableId)->isOpen()) {
        toPrint.append(" status: closed");
        cout << toPrint << endl;
        complete();
        return;
    }

    toPrint.append(" status: open\nCustomers:\n");

    //appending customers list
    for(size_t i=0;i<restaurant.getTable(tableId)->getCustomers().size();i++)
    {
        toPrint.append(to_string(restaurant.getTable(tableId)->getCustomers()[i]->getId())+" ");
        toPrint.append(restaurant.getTable(tableId)->getCustomers()[i]->getName()+"\n");
    }
    toPrint.append("Orders:\n");
    //appending orders
    for(size_t i=0;i<restaurant.getTable(tableId)->getOrders().size();i++)
    {
        toPrint.append(restaurant.getTable(tableId)->getOrders()[i].second.getName()+" ");
        toPrint.append(to_string(restaurant.getTable(tableId)->getOrders()[i].second.getPrice()));
        toPrint.append("NIS ");
        toPrint.append(to_string(restaurant.getTable(tableId)->getOrders()[i].first)+"\n");
    }

    //appending bill
    toPrint.append("Current Bill: ");
    toPrint.append(to_string(restaurant.getTable(tableId)->getBill()));
    toPrint.append("NIS");
    cout<<toPrint<<endl;
    complete();
}

string PrintTableStatus::toString() const
{
    string toReturn;
    toReturn.append("status ");
    toReturn.append(to_string(tableId));
    toReturn.append(actionStatusToString(getStatus()));
    toReturn.append(getErrorMsg());
    toReturn.append("\n");
    return toReturn;
}

PrintTableStatus* PrintTableStatus::clone() {return new PrintTableStatus(*this);}

int PrintTableStatus::getTableId() const {
    return tableId;
}

PrintActionsLog::PrintActionsLog():BaseAction() {}

void PrintActionsLog::act(Restaurant &restaurant)
{


    //-1 because we don't want to print the actual "print action log" action (that we just typed)
    for(size_t i=0;i<restaurant.getActionsLog().size();i++) {
        {
            cout << restaurant.getActionsLog()[i]->toString();
        }
    }

    //restaurant.addToActionsLog(this);
    complete();
}

string PrintActionsLog::toString() const
{
    string toReturn;
    toReturn.append("Print actions log: ");
    toReturn.append(actionStatusToString(getStatus()));
    toReturn.append(getErrorMsg());
    toReturn.append("\n");
    return toReturn;
}

PrintActionsLog* PrintActionsLog::clone() {return new PrintActionsLog(*this);}

BackupRestaurant::BackupRestaurant():BaseAction() {}

void BackupRestaurant::act(Restaurant &restaurant)
{
    //deleting older backup (if exists)
    if(backup!=nullptr){
        delete backup;
        backup = nullptr;
    }
    //using copy assignment operator
    backup=new Restaurant(restaurant);
    //restaurant.addToActionsLog(this);
    complete();
}

string BackupRestaurant::toString() const
{
    string toReturn;
    toReturn.append("backup");
    toReturn.append(actionStatusToString(getStatus()));
    toReturn.append(getErrorMsg());
    toReturn.append("\n");
    return toReturn;
}

BackupRestaurant* BackupRestaurant::clone()
{
    return new BackupRestaurant(*this);}

RestoreResturant::RestoreResturant():BaseAction() {}

void RestoreResturant::act(Restaurant &restaurant) {


    //using copy assignment operator
    if (backup != nullptr) {
        restaurant = *backup;
        //restaurant.addToActionsLog(this);
        complete();
    } else {
        error("No backup available");
        cout<<getErrorMsg()<<endl;
    }
}

string RestoreResturant::toString() const
{
    string toReturn;
    toReturn.append("restore");
    toReturn.append(actionStatusToString(getStatus()));
    toReturn.append(getErrorMsg());
    toReturn.append("\n");
    return toReturn;
}

RestoreResturant* RestoreResturant::clone() {return new RestoreResturant(*this);}

