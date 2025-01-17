#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Dish.h"

class Customer{
public:
    Customer(std::string c_name, int c_id);
    Customer(const Customer &otherCustomer);
    virtual std::vector<int> order(const std::vector<Dish> &menu)=0;
    virtual std::string toString() const = 0;
    std::string getName() const;
    virtual std::string getType() const=0;
    int getId() const;
    virtual Customer* clone() const=0;
    virtual ~Customer();
private:
    const std::string name;
    const int id;

};


class VegetarianCustomer : public Customer {
public:
	VegetarianCustomer(std::string name, int id);
    VegetarianCustomer(const VegetarianCustomer &vegCustomer);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    std::string getType() const;
    virtual VegetarianCustomer* clone() const;
    virtual ~VegetarianCustomer();
private:
    const std::string type;
};


class CheapCustomer : public Customer {
public:
	CheapCustomer(std::string name, int id);
    CheapCustomer(const CheapCustomer &chpCustomer);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    std::string getType() const;
    virtual CheapCustomer* clone() const;
    virtual ~CheapCustomer();
	bool isCanOrder() const;
private:
	bool canOrder;
    const std::string type;
};


class SpicyCustomer : public Customer {
public:
	SpicyCustomer(std::string name, int id);
    SpicyCustomer(const SpicyCustomer &spcCustomer);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    std::string getType() const;
    virtual SpicyCustomer* clone() const;
    virtual ~SpicyCustomer();
	bool isFirstOrder() const ;
private:
	bool firstOrder;
    const std::string type;
};


class AlchoholicCustomer : public Customer { public:
	AlchoholicCustomer(std::string name, int id);
    AlchoholicCustomer(const AlchoholicCustomer &alcCustomer);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    std::string getType() const;
    virtual AlchoholicCustomer* clone() const;
    virtual ~AlchoholicCustomer();
	bool isorderedMostExpensive() const ;
	int getCurDrinkId()const;
private:

	void setNextExpensiveDrinkId(const std::vector<Dish> &menu);
	Dish getDishById(int dishId,const std::vector<Dish> &menu);
	bool orderedMostExpensive;
	int curAlcDrinkId;
    const std::string type;
};


#endif