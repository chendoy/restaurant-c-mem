#ifndef DISH_H_
#define DISH_H_

#include <string>

enum DishType{
    VEG, SPC, BVG, ALC
};

class Dish{
public:
    Dish(int d_id, std::string d_name, int d_price, DishType d_type);
	Dish(const Dish &otherDish);
	Dish(Dish &&dish);
    int getId() const;
    std::string getName() const;
    int getPrice() const;
    DishType getType() const;
    Dish clone() const;
    virtual ~Dish();
private:
	const int id;
    const std::string name;
    const int price;
    const DishType type;
};


#endif