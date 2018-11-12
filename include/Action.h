#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "Customer.h"
#include "Table.h"

using namespace std;

enum ActionStatus{
    PENDING, COMPLETED, ERROR
};

//Forward declaration
class Restaurant;

class BaseAction{
public:
    BaseAction();
    BaseAction(const BaseAction &other);
    BaseAction(BaseAction &&baseAction);
	virtual ~BaseAction();
    ActionStatus getStatus() const;
    virtual void act(Restaurant& restaurant)=0;
    virtual std::string toString() const=0;
	virtual BaseAction* clone()=0;
	virtual BaseAction& operator=(const BaseAction &baseAction);
	BaseAction &operator=(BaseAction &&baseAction);
protected:
    void complete();
    void error(std::string errorMsg);
    std::string getErrorMsg() const;
private:
    std::string errorMsg;
    ActionStatus status;
};


class OpenTable : public BaseAction {
public:
    OpenTable(int id, std::vector<Customer *> &customersList);
    OpenTable(const OpenTable &openTable);
	OpenTable(OpenTable &&openTable);
	virtual ~OpenTable();
	void act(Restaurant &restaurant);
    std::string toString() const;
    int getTableId() const;
	std::vector<Customer *> getCustomers();
	OpenTable* clone();
private:
	const int tableId;
	std::vector<Customer *> customers;
};


class Order : public BaseAction {
public:
    Order(int id);
	Order(const Order &other);
	Order(Order &&order);
	virtual ~Order();
	void act(Restaurant &restaurant);
    std::string toString() const;
	Order* clone();
	int getTableId() const;
private:
    const int tableId;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
	MoveCustomer(const MoveCustomer &other);
	MoveCustomer(MoveCustomer &&moveCustomer);
	virtual ~MoveCustomer();
	void act(Restaurant &restaurant);
    std::string toString() const;
	MoveCustomer* clone();
	int getSrcTable() const;
	int getDstTable() const;
	int getId() const;
private:
    
    const int srcTable;
    const int dstTable;
    const int id;
};


class Close : public BaseAction {
public:
	Close(int id);
	Close(const Close &other);
	Close(Close &&close);
	virtual ~Close();
	void act(Restaurant &restaurant);
	std::string toString() const;
	Close* clone();
	int getTableId() const;
private:
	const int tableId;
};


class CloseAll : public BaseAction {
public:
	CloseAll();
	CloseAll(const CloseAll &other);
	CloseAll(CloseAll &&closeAll);
	virtual ~CloseAll();
	void act(Restaurant &restaurant);
	std::string toString() const;
	CloseAll* clone();
	virtual CloseAll& operator=(const CloseAll &closeAll);
private:
};


class PrintMenu : public BaseAction {
public:
    PrintMenu();
	PrintMenu(const PrintMenu &other);
	PrintMenu(PrintMenu &&printMenu);
	virtual ~PrintMenu();
    void act(Restaurant &restaurant);
    std::string toString() const;
	PrintMenu* clone();
	virtual PrintMenu& operator=(const PrintMenu &printMenu);
private:
};


class PrintTableStatus : public BaseAction {
public:
    PrintTableStatus(int id);
	PrintTableStatus(const PrintTableStatus &other);
	PrintTableStatus(PrintTableStatus &&printTableStatus);
	virtual ~PrintTableStatus();
    void act(Restaurant &restaurant);
    std::string toString() const;
	PrintTableStatus* clone();
	int getTableId() const;
private:
    const int tableId;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
	PrintActionsLog(const PrintActionsLog &other);
	PrintActionsLog(PrintActionsLog &&printActionsLog);
	virtual ~PrintActionsLog();
    void act(Restaurant &restaurant);
    std::string toString() const;
	PrintActionsLog* clone();
	virtual PrintActionsLog& operator=(const PrintActionsLog &printActionsLog);
private:
};


class BackupRestaurant : public BaseAction {
public:
    BackupRestaurant();
	BackupRestaurant(const BackupRestaurant &other);
	BackupRestaurant(BackupRestaurant &&backupRestaurant);
	virtual ~BackupRestaurant();
    void act(Restaurant &restaurant);
    std::string toString() const;
	BackupRestaurant* clone();
	virtual BackupRestaurant& operator=(const BackupRestaurant &backupRestaurant);
private:
};


class RestoreResturant : public BaseAction {
public:
    RestoreResturant();
	RestoreResturant(const RestoreResturant &other);
	RestoreResturant(RestoreResturant &&restoreResturant);
	virtual ~RestoreResturant();
    void act(Restaurant &restaurant);
    std::string toString() const;
	RestoreResturant* clone();
	virtual RestoreResturant& operator=(const RestoreResturant &restoreResturant);
};


#endif