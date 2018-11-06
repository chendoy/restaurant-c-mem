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
using namespace std;
extern Restaurant* backup;

BaseAction::BaseAction() {}

ActionStatus BaseAction::getStatus() const {return this->status;}

void BaseAction::complete()
{
    this->status=COMPLETED;
}

void BaseAction::error(std::string errorMsg)
{
    this->status=ERROR;
    std::cout<<"Error: "+errorMsg<<endl;
}


string BaseAction::getErrorMsg() const {return this->errorMsg;}




OpenTable::OpenTable (int id, vector<Customer *> &customersList):tableId(id), BaseAction(), customers(customersList) {statusString="";}

void OpenTable::act(Restaurant &restaurant)
{
    restaurant.addToActionsLog(this);

    //table does not exist                   //table is already open
    if(restaurant.getTable(tableId)==nullptr || restaurant.getTable(tableId)->isOpen()==true)
        error("Table does not exist or is already open");


    //mark the table as open
    restaurant.getTable(tableId)->openTable();

    //assigns the customers to it
    for(int i=0;i<customers.size();i++)
        restaurant.getTable(tableId-1)->addCustomer(customers[i]);
}

string OpenTable::toString() const
{
    string toReturn="open "+tableId;

    //appends the customers and their types
    for(int i=0;i<customers.size();i++) {
        toReturn.append(customers[i]->getName()) + ",";
        //toReturn.append(customers[i]->get + ","; //NEED TO COMPLETE THIS-APPEND THE CUSTOMER TYPE
    }

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
}
std::string Order::toString() const {
    return "order "+tableId;
}

Order* Order::clone() {return new Order(*this);}

//Order END

////MoveCustomer START
MoveCustomer::MoveCustomer(int src, int dst, int customerId):BaseAction(),srcTable(src),dstTable(dst),id(customerId) {}

void MoveCustomer ::act(Restaurant &restaurant) {

    Table *tblSrc=restaurant.getTable(srcTable);
    Table *tblDest=restaurant.getTable(dstTable);

    if(tblSrc== nullptr||tblDest==nullptr||!tblSrc->isOpen()|!tblDest->isOpen()|!tblSrc->isCustomerAtTable(id)|
                                           tblDest->getCapacity()==tblDest->getCustomers().size())
    {
        cout<<"Cannt Move Customer"<<endl;
    }
    else {//can move customer
        tblDest->addCustomer(tblSrc->getCustomerById(id));
        //add costumer orders (from src table to dest table)
        std::vector<OrderPair>customerOrders=tblSrc->getCustomerOrders(id);
        tblDest->addNewCustomerOrdersToBill(customerOrders); //add customer orders to 'dest' orderList
        //deleting the customer from the current table
        tblSrc->removeCustomer(id);
    }
}

string MoveCustomer::toString() const {return "";}

MoveCustomer* MoveCustomer::clone() {return new MoveCustomer(*this);}

Close::Close(int id): tableId(id){}

void Close::act(Restaurant &restaurant)
{

    restaurant.addToActionsLog(this);


    //table does not exist                             //table is not open
    if(restaurant.getTable(tableId)==nullptr || restaurant.getTable(tableId)->isOpen()==false)
        error("Table does not exist or is not open");

    string toPrint="Table "+tableId;
    toPrint.append(" was closed. Bill "+ restaurant.getTable(tableId)->getBill());
    toPrint.append("NIS.");
    cout<<toPrint<<endl;

    stringLog=toPrint;
    restaurant.getTable(tableId)->closeTable();
}

string Close::toString() const {return "Closed table: "+tableId;}

Close* Close::clone() {return new Close(*this);}


CloseAll::CloseAll():BaseAction() {}


void CloseAll::act(Restaurant &restaurant)
{

    restaurant.addToActionsLog(this);

    for(int i=0;i<restaurant.getNumOfTables();i++) {
        if(restaurant.getTable(i)->isOpen()==false)
            restaurant.getTable(i)->closeTable();
    }
    //NEED TO 'CLOSE THE RESTAURANT' ---HOW TO DO THIS?
}

string CloseAll::toString() const {return stringLog;}

CloseAll* CloseAll::clone() {return new CloseAll(*this);}

PrintMenu::PrintMenu():BaseAction() {}

void PrintMenu::act(Restaurant &restaurant)
{

    restaurant.addToActionsLog(this);

    string toPrint="";
    for(int i=0;i<restaurant.getMenu().size();i++)
    {
        toPrint.append(restaurant.getMenu()[i].getName()+" ");
        toPrint.append(restaurant.getMenu()[i].getType()+" ");
        toPrint.append(restaurant.getMenu()[i].getPrice()+" ");
    }
}

string PrintMenu::toString() const { return "";}

PrintMenu* PrintMenu::clone() {return new PrintMenu(*this);}

PrintTableStatus::PrintTableStatus(int id):BaseAction(), tableId(id) {}

void PrintTableStatus::act(Restaurant &restaurant)
{

    restaurant.addToActionsLog(this);
    string toPrint;

    //appending table status
    toPrint.append("Table: "+tableId);
    if(!restaurant.getTable(tableId)->isOpen()) {
        toPrint.append(" status: closed");
        cout << toPrint << endl;
        return;
    }

    toPrint.append(" status: open +\n+ Customers:");

    //appending customers list
    for(int i=0;i<restaurant.getTable(tableId)->getCustomers().size();i++)
    {
        toPrint.append(restaurant.getTable(tableId)->getCustomers()[i]->getId()+" ");
        toPrint.append(restaurant.getTable(tableId)->getCustomers()[i]->getName()+"\n");
    }

    //appending orders
    for(int i=0;i<restaurant.getTable(tableId)->getOrders().size();i++)
    {
        toPrint.append(restaurant.getTable(tableId)->getOrders()[i].second.getName()+" ");
        toPrint.append(restaurant.getTable(tableId)->getOrders()[i].second.getPrice()+"\n");
        toPrint.append(restaurant.getTable(tableId)->getOrders()[i].first+"\n");
    }

    //appending bill
    toPrint.append("Current bill: "+restaurant.getTable(tableId)->getBill());
}

string PrintTableStatus::toString() const {return "Print table status: "+tableId;}

PrintTableStatus* PrintTableStatus::clone() {return new PrintTableStatus(*this);}

PrintActionsLog::PrintActionsLog():BaseAction() {}

void PrintActionsLog::act(Restaurant &restaurant)
{
    restaurant.addToActionsLog(this);

    vector<BaseAction*> actionsLog=restaurant.getActionsLog();
    for(int i=0;i<actionsLog.size();i++)
        cout<<actionsLog[i]->toString();
}

string PrintActionsLog::toString() const {return "Actions log printed";}

PrintActionsLog* PrintActionsLog::clone() {return new PrintActionsLog(*this);}

BackupRestaurant::BackupRestaurant():BaseAction() {}

void BackupRestaurant::act(Restaurant &restaurant)
{
    //using copy assignment operator
    *backup=restaurant;
}

string BackupRestaurant::toString() const {return "Backup completed";}

BackupRestaurant* BackupRestaurant::clone()
{
    return new BackupRestaurant(*this);}

RestoreResturant::RestoreResturant():BaseAction() {}

void RestoreResturant::act(Restaurant &restaurant)
{
    //using copy assignment operator
    restaurant=*backup;
}

RestoreResturant* RestoreResturant::clone() {return new RestoreResturant(*this);}