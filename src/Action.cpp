//
// Created by chendoy on 11/5/18.
//
#include "Action.h"
#include "Restaurant.h"
#include "Customer.h"
#include "Table.h"
#include "Dish.h"
#include <vector>


using namespace std;

BaseAction::BaseAction() {}

ActionStatus BaseAction::getStatus() const {return this->status;}

void BaseAction::error(std::string errorMsg)
{
    this->status=ERROR;
    cout<<"Error: "+errorMsg<<endl;
}

void BaseAction::complete()
{
    this->status=COMPLETED;
}

OpenTable::OpenTable (int id, vector<Customer *> &customersList):tableId(id), BaseAction(), customers(customersList) {}

void OpenTable::act(Restaurant &restaurant)
{

    //table does not exist                   //table is already open
    if(restaurant.getTable(tableId)==nullptr || restaurant.getTable(tableId)->isOpen())
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

string BaseAction::getErrorMsg() const {return this->errorMsg;}


//Order START
Order::Order(int id):BaseAction(),tableId(id){}

void Order::act(Restaurant &restaurant) {

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
string MoveCustomer::toString() const {}


Close::Close(int id): tableId(id){}

void Close::act(Restaurant &restaurant)
{

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

string Close::toString() const {return stringLog;}
//CloseAll::CloseAll() {}
//void CloseAll::act(Restaurant &restaurant)
//{
//    for(int i=0;i<restaurant.getNumOfTables();i++) {
//        if(restaurant.getTable(i)->isOpen()==false)
//            restaurant.getTable(i)->closeTable();
//    }
//    //NEED TO 'CLOSE THE RESTAURANT' ---HOW TO DO THIS?
//}



