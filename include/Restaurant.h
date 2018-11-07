#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <vector>
#include "Dish.h"
#include "Table.h"
#include "Action.h"
#include <string>
using namespace std;

class Restaurant{
public:
	Restaurant();
    Restaurant(const std::string &configFilePath);
    void start();
    int getNumOfTables() const;
    Table* getTable(int ind);
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Dish>& getMenu();
	virtual ~Restaurant();
private:
    bool open;
    std::vector<Table*> tables;
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;
    int numOfTables;
    std::vector<string> splitStringBytoken(string s,string token);
};

#endif