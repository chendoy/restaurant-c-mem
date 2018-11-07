#ifndef TABLE_H_
#define TABLE_H_

#include <vector>
#include "Customer.h"
#include "Dish.h"

typedef std::pair<int, Dish> OrderPair;

class Table{
public:
    Table(int t_capacity);
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomerById(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    std::vector<OrderPair> getCustomerOrders(int customerId);
    void addNewCustomerOrdersToBill(std::vector<OrderPair>&customerOrders);
    void order(const std::vector<Dish> &menu);
    bool isCustomerAtTable(int customerId);
    Table* clone();

    void openTable();
    void closeTable();
    int getBill();
    bool isOpen();


    //Destructor
    virtual ~Table();
private:
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order in a table - (dcustomer_id, Dish)
    Dish  getDishById(const std::vector<Dish> &menu, const int dishId) const ;
};


#endif
