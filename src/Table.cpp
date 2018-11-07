//
// Created by chen on 11/2/18.
//
#include <Table.h>
#include <vector>
#include <iostream>

using namespace std;


Table::Table(int t_capacity): capacity(t_capacity){
    customersList=vector<Customer*>();
    orderList=vector<OrderPair>();
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

void Table::closeTable() {this->open=false;}

bool Table::isOpen() { return open;}

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
            addCustomerOrders(customerDishes,menu);
        }
        //printing the order list to the screen
        for(int i=0;i<orderList.size();i=i+1)
        {
            cout<<"customer "<<getCustomerById(orderList[i].first)->getName()<<" ordered "<<orderList[i].second.getName();

        }
    }

}
void Table::addCustomerOrders(std::vector<int> csutomerDishes,const std::vector<Dish> &menu) {
    //add customer dishes to the pairs array
    for(int i=0;i<csutomerDishes.size();i=i+1)
    {
        const OrderPair op(customersList[i]->getId(), getDishById(menu, csutomerDishes[i]));
        orderList.push_back(op);
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
    //if the table doesn't exist any more we can delete the the object of Customers that allocated for this table at the 'customerList'
    //note that if this table closed because the customers moved to another table we will *copy* the list of customers and allocate them
    // *new* space in the memory. thus, the deletion of the customer list is safe
    for(int i=0;i<customersList.size();i=i+1)
    {
        delete customersList[i];
        customersList[i]= nullptr;
    }
    //clean the list
    customersList.clear();
}




