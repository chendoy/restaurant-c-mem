//
// Created by chen on 11/2/18.
//
#include "Table.h"
#include "Dish.h"
#include <vector>
#include <iostream>

using namespace std;


Table::Table(int t_capacity): capacity(t_capacity){
    customersList=vector<Customer*>();
    orderList=vector<OrderPair>();
}

//copy ctor
Table::Table(const Table &table)
{
    customersList=vector<Customer*>();
    orderList=vector<OrderPair>();

   capacity=table.capacity;
   open=table.open;

    for(int i=0;i<table.customersList.size();i++)
        customersList.push_back(table.customersList[i]->clone());

     for(int i=0;i<table.orderList.size();i++) {
         orderList.push_back(OrderPair(table.orderList[i].first,table.orderList[i].second.clone()));
     }
}

int Table::getCapacity() const {return this->capacity;}

void Table::addCustomer(Customer *customer) { this->customersList.push_back(customer);}


void Table::removeCustomer(int id) {
    //remove customer from customer list
    bool isDelted=false;
    for(int i=0;i<customersList.size()&!isDelted;i=i+1)
    {
        if(customersList[i]->getId()==id)
        {
            customersList.erase(customersList.begin()+i);
            isDelted= true;
        }
    }

    //remove customer orders from orderList
    vector<OrderPair>newOrders;
    for(int i=0;i<orderList.size();i=i+1)
    {
        if(orderList[i].first!=id)
        {
            newOrders.push_back(orderList[i]);
        }
    }
    orderList.clear();
    for(int i=0;i<newOrders.size();i=i+1)
    {
        orderList.push_back(newOrders[i]);
    }
}

Customer* Table::getCustomerById(int id) {
    for(int i=0;i<customersList.size();i=i+1)
    {
        if(customersList[i]->getId()==id)
            return customersList[i];
    }
}

void Table::addNewCustomerOrdersToBill(std::vector<OrderPair>&customerOrders) {
    for(int i=0;i<customerOrders.size();i=i+1)
    {
        orderList.push_back(customerOrders[i]);
    }
}


vector<Customer*>& Table::getCustomers() {return this->customersList;}

vector<OrderPair>& Table::getOrders() {return this->orderList;}

vector<OrderPair>Table::getCustomerOrders(int customerId) {
    vector<OrderPair> customerOrders;
    for(int i=0;i<orderList.size();i=i+1)
    {
        if(orderList[i].first==customerId)
        {
            customerOrders.push_back(orderList[i]);
        }
    }
    return customerOrders;
}


void Table:: openTable() {this->open= true;}

void Table::closeTable() {
    this->open=false;

    //we have to clean now the customers vector (and delete its customers)
    for(int i=0;i<customersList.size();i=i+1) {
        delete customersList[i];
        customersList[i]= nullptr;
    }
    customersList.clear();
    //cleaning the orderList
    orderList.clear();
}

bool Table::isOpen() const { return open;}

//the function returns the sum of all dishes order at this table
int Table::getBill(){
    if(!open){
        cout<<"can't get bill from closed table"<<endl;
    }
    else {
        //iterating thourgh orderlist and sum the dishes by their price
        int sum=0;
        for(int i=0;i<orderList.size();i=i+1)
        {
            sum=sum+orderList[i].second.getPrice();
        }
        return sum;
    }

}


//this function will preform order from each customer in the table (according his strategy). list of the orders will be printed to the screen
void Table::order(const std::vector<Dish> &menu) {
    if(!open)
    {
        cout<<"the table is not open yet, can't order"<<endl;
    }
    else

    {
        //make the orders
        for(int i=0;i<customersList.size();i=i+1)
        {
            std::vector<int> customerDishes=customersList[i]->order(menu);
            //add the customer dishes to the pair list
            for(int j=0;j<customerDishes.size();j=j+1)
            {
                const OrderPair op(customersList[i]->getId(), getDishById(menu, customerDishes[j]));
                orderList.push_back(op);
                //print the orders to the screnn
                cout<<customersList[i]->getName()<<" ordered "<<op.second.getName()<<endl;
            }

        }
    }

}



// the function return an object (new one pass by value) of a dish from the menu by the dish Id
Dish Table::getDishById(const std::vector<Dish> &menu, const int dishId) const {
    for (int i = 0; i < menu.size(); i = i + 1) {
        if (menu[i].getId() == dishId) {
            return menu[i];
        }
    }
}
bool Table::isCustomerAtTable(int customerId) {
    vector<Customer*>customers=getCustomers();
    bool exist=false;
    for(int i=0;i<customers.size()&!exist;i=i+1)
    {
        if(customers[i]->getId()==customerId)
            exist=true;
    }
    return exist;
}

Table* Table::clone() {return new Table(*this);}

//implementing the class Destructor
Table::~Table () {
    //close the current table (if not already closed)
    closeTable();
}



