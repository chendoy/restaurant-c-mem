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
}

string actionStatusToString(const ActionStatus actionStatus)
{
    if(actionStatus==PENDING) return "Pending";
    else if(actionStatus==COMPLETED) return "Completed";
    else if(actionStatus==ERROR) return "Error: ";
}

BaseAction::BaseAction() {}

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



//START----------------------------COPY CONSTRUCTORS----------------------

BaseAction::BaseAction(const BaseAction &other):errorMsg(other.errorMsg), status(other.status) {}

OpenTable::OpenTable(const OpenTable &other):BaseAction(other),tableId(other.tableId), customers(other.customers) {}

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

OpenTable::OpenTable (int id, vector<Customer *> &customersList):tableId(id), BaseAction(), customers(customersList) {}

void OpenTable::act(Restaurant &restaurant)
{
    restaurant.addToActionsLog(this);

    //table does not exist                   //table is already open
    if(restaurant.getNumOfTables()<=tableId || restaurant.getTable(tableId)->isOpen()==true) {
        error("Table does not exist or is already open");
        cout<<getErrorMsg()<<endl;
        return ;}


        //mark the table as open
        restaurant.getTable(tableId)->openTable();

        //assigns the customers to it
        for (int i = 0; i < customers.size(); i++)
            restaurant.getTable(tableId)->addCustomer(customers[i]);
        complete();

}

string OpenTable::toString() const
{
    string toReturn="open ";
    toReturn.append(to_string(tableId));
    toReturn.append(" ");

    //appends the customers and their types
    for(int i=0;i<customers.size();i++) {
        toReturn.append(customers[i]->getName());
        toReturn.append(",");
        toReturn.append(customers[i]->getType());
        toReturn.append(": ");
    }
        //toReturn=toReturn.substr(0,toReturn.length()-1);

        toReturn.append(actionStatusToString(getStatus()));
        toReturn.append(getErrorMsg());
        toReturn.append("\n");
        return toReturn;
}

OpenTable* OpenTable::clone() {return new OpenTable(*this);}

//Order START
Order::Order(int id):BaseAction(),tableId(id){}

void Order::act(Restaurant &restaurant) {


    restaurant.addToActionsLog(this);

    Table* table=restaurant.getTable(tableId);
    if(table== nullptr || !table->isOpen())
    {
        cout<<"Table does not exist or is not open"<<endl;
    } else{
        table->order(restaurant.getMenu());
    }
    complete();
}
std::string Order::toString() const {
    string toReturn;
    toReturn.append("order ");
    toReturn.append(to_string(tableId)+": ");
    toReturn.append(actionStatusToString(getStatus()));
    toReturn.append(getErrorMsg());
    toReturn.append("\n");
    return toReturn;
}

Order* Order::clone() {return new Order(*this);}

//Order END

////MoveCustomer START
MoveCustomer::MoveCustomer(int src, int dst, int customerId):BaseAction(),srcTable(src),dstTable(dst),id(customerId) {}

void MoveCustomer::act(Restaurant &restaurant) {

    restaurant.addToActionsLog(this);

    Table *tblSrc=restaurant.getTable(srcTable);
    Table *tblDest=restaurant.getTable(dstTable);

    if(tblSrc== nullptr||tblDest==nullptr||!tblSrc->isOpen()|!tblDest->isOpen()|!tblSrc->isCustomerAtTable(id)|
                                           tblDest->getCapacity()==tblDest->getCustomers().size())
    {
        cout<<"Cannot Move Customer"<<endl;
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
    toReturn.append(to_string(id)+": ");
    toReturn.append(actionStatusToString(getStatus()));
    toReturn.append(getErrorMsg());
    toReturn.append("\n");
    return toReturn;
}


MoveCustomer* MoveCustomer::clone() {return new MoveCustomer(*this);}

Close::Close(int id): tableId(id){}

void Close::act(Restaurant &restaurant)
{

    restaurant.addToActionsLog(this);


    //table does not exist                             //table is not open
    if(restaurant.getNumOfTables()<=tableId || restaurant.getTable(tableId)->isOpen()==false)
        error("Table does not exist or is not open");

    string toPrint="Table ";
    toPrint.append(to_string(tableId));
    toPrint.append(" was closed. Bill ");
    toPrint.append(to_string(restaurant.getTable(tableId)->getBill()));
    toPrint.append(" NIS.");
    cout<<toPrint<<endl;

    restaurant.getTable(tableId)->closeTable();
    complete();
}

string Close::toString() const {
    string toReturn;
    toReturn.append("Closed table ");
    toReturn.append(to_string(tableId));
    toReturn.append(": ");
    toReturn.append(actionStatusToString(getStatus()));
    toReturn.append(getErrorMsg());
    toReturn.append("\n");
    return toReturn;

}

Close* Close::clone() {return new Close(*this);}


CloseAll::CloseAll():BaseAction() {}


void CloseAll::act(Restaurant &restaurant)
{

    restaurant.addToActionsLog(this);

    for(int i=0;i<restaurant.getNumOfTables();i++) {
        if(restaurant.getTable(i)->isOpen())
        {
            Close* closeAction=new Close(i);
            closeAction->act(restaurant);
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

    restaurant.addToActionsLog(this);

    string toPrint="";
    for(int i=0;i<restaurant.getMenu().size();i++)
    {
        toPrint.append(restaurant.getMenu()[i].getName()+",");
        toPrint.append(dishtypeToString(restaurant.getMenu()[i].getType())+",");
        toPrint.append(to_string(restaurant.getMenu()[i].getPrice())+"\n");
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

    restaurant.addToActionsLog(this);
    string toPrint;

    //appending table status
    toPrint.append("Table ");
    toPrint.append(to_string(tableId));
    if(!restaurant.getTable(tableId)->isOpen()) {
        toPrint.append(" status: closed");
        cout << toPrint << endl;
        return;
    }

    toPrint.append(" status: open\nCustomers:\n");

    //appending customers list
    for(int i=0;i<restaurant.getTable(tableId)->getCustomers().size();i++)
    {
        toPrint.append(to_string(restaurant.getTable(tableId)->getCustomers()[i]->getId())+" ");
        toPrint.append(restaurant.getTable(tableId)->getCustomers()[i]->getName()+"\n");
    }
    toPrint.append("Orders:\n");
    //appending orders
    for(int i=0;i<restaurant.getTable(tableId)->getOrders().size();i++)
    {
        toPrint.append(restaurant.getTable(tableId)->getOrders()[i].second.getName()+" ");
        toPrint.append(to_string(restaurant.getTable(tableId)->getOrders()[i].second.getPrice()));
        toPrint.append("NIS ");
        toPrint.append(to_string(restaurant.getTable(tableId)->getOrders()[i].first)+"\n");
    }

    //appending bill
    toPrint.append("Current bill: ");
    toPrint.append(to_string(restaurant.getTable(tableId)->getBill()));
    toPrint.append(" NIS");
    cout<<toPrint<<endl;
    complete();
}

string PrintTableStatus::toString() const
{
    string toReturn;
    toReturn.append("status ");
    toReturn.append(to_string(tableId));
    toReturn.append(": ");
    toReturn.append(actionStatusToString(getStatus()));
    toReturn.append(getErrorMsg());
    toReturn.append("\n");
    return toReturn;
}

PrintTableStatus* PrintTableStatus::clone() {return new PrintTableStatus(*this);}

PrintActionsLog::PrintActionsLog():BaseAction() {}

void PrintActionsLog::act(Restaurant &restaurant)
{


    //-1 because we don't want to print the actual "print action log" action (that we just typed)
    for(int i=0;i<restaurant.getActionsLog().size();i++) {
        {
            cout << restaurant.getActionsLog()[i]->toString();
        }
    }

    restaurant.addToActionsLog(this);
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
    restaurant.addToActionsLog(this);
    //using copy assignment operator
    backup=new Restaurant(restaurant);
    complete();
}

string BackupRestaurant::toString() const
{
    string toReturn;
    toReturn.append("Backup: ");
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
    restaurant.addToActionsLog(this);
    //using copy assignment operator
    if (backup != nullptr) {
        restaurant = *backup;
        complete();
    } else {
        error("No backup available");
        cout<<getErrorMsg()<<endl;
    }
}

string RestoreResturant::toString() const
{
    string toReturn;
    toReturn.append("Restore: ");
    toReturn.append(actionStatusToString(getStatus()));
    toReturn.append(getErrorMsg());
    toReturn.append("\n");
    return toReturn;
}

RestoreResturant* RestoreResturant::clone() {return new RestoreResturant(*this);}

