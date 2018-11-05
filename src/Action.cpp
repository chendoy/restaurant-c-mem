//
// Created by chendoy on 11/5/18.
//
#include "Action.h"
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

string BaseAction::getErrorMsg() const {return this->errorMsg;}

OpenTable::OpenTable(int id, vector<Customer *> &customersList):tableId(id)
//body will populate customers vector
{
    for(int i=0;i<customersList.size();i++)
    {
        customers.push_back(customersList[i]);

    }
}
