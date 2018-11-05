//
// Created by chendoy on 11/5/18.
//
#include "Action.h"
#include <vector>
#include <Table.h>

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
    toReturn.append(statusString(getStatus()));
}

string BaseAction::getErrorMsg() const {return this->errorMsg;}

OpenTable::OpenTable(int id, vector<Customer *> &customersList):tableId(id)
//body will populate customers vector
{
    for(int i=0;i<customersList.size();i++)
    {
        customers.push_back(customersList[i]);

    }
}

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

//MoveCustomer START
MoveCustomer::MoveCustomer(int src, int dst, int customerId):BaseAction(),srcTable(src),dstTable(dst),id(customerId) {}

void MoveCustomer ::act(Restaurant &restaurant) {
    Table *srcTable=restaurant.getTable(src);
    Table *dstTable=restaurant.getTable(dst);
    if(srcTable== nullptr||dstTable==nullptr||!srcTable->isOpen()|!dstTable->isOpen()|!isCustomerInTable(*srcTable)|
    dstTable->getCapacity()==dstTable->getCustomers().size())
    {
        cout<<"Cannt Move Customer"<<endl;
    }
    else {

    }
}

bool MoveCustomer::isCustomerInTable(Table &originTable) {
    std::vector<Customer*>& tblCustomer=originTable.getCustomers();
    bool exist=false;
    for(int i=0;i<tblCustomer.size()&!exist;i=i+1)
    {
        if(tblCustomer[i]->getId()==id)
            exist=true;
    }
    return exist;
}
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
CloseAll::CloseAll() {}
void CloseAll::act(Restaurant &restaurant)
{
    for(int i=0;i<restaurant.getNumOfTables();i++) {
        if(restaurant.getTable(i)->isOpen()==false)
            restaurant.getTable(i)->closeTable();
    }
    //NEED TO 'CLOSE THE RESTAURANT' ---HOW TO DO THIS?
}



