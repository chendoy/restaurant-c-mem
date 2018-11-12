#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <vector>
#include <string>
#include "Dish.h"
#include "Table.h"
#include "Action.h"

using namespace std;

class Restaurant{		
public:

    Table* getTable(int ind);Restaurant();
	Restaurant(const std::string &configFilePath);
	Restaurant(const Restaurant &rest);
    Restaurant(Restaurant&& otherRest); //move constructor
	void start();
	int getNumOfTables() const;
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Dish>& getMenu();
    void addToActionsLog(BaseAction* actionToAdd);
    Restaurant& operator=(const Restaurant &rest);
    Restaurant &operator=(Restaurant&& otherRest); //move assignment operator
    bool isOpen() const;
    vector<Dish> getDishes() const;
    vector<Table*> getAllTables() const;
    vector<BaseAction*>getAllBaseActions() const;
    void setTablePointer(Table* tablePtr,int index);
    void setActionLogsPointer(BaseAction* baseAction,int index);
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