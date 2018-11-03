//
// Created by chen on 11/2/18.
//
#include <Table.h>
#include <vector>

using namespace std;


Table::Table(int t_capacity): capacity(t_capacity){
    customersList=vector<Customer*>();
    orderList=vector<OrderPair>();
}

int Table::getCapacity() const {return this->capacity;}

void Table::addCustomer(Customer *customer) {this->customersList.push_back(customer);}

void Table::removeCustomer(int id) {customersList.erase(customersList.begin()+id);}

Customer* Table::getCustomer(int id) {return this->customersList[id];}

vector<Customer*>& Table::getCustomers() {return this->customersList;}

vector<OrderPair>& Table::getOrders() {return this->orderList;}

void Table::order(const std::vector<Dish> &menu) {

}