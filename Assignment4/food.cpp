/* 
 * CS3505 - Assignment 4 - Resubmission
 * Skylar Shyu 
 * Food.cpp
 */

/*
 * This class represents a Food object. Each Food will have a UPC Code (int),
 * the shelf life before expiry (int), and the name of the Food (string). 
 */

#include "food.h" 
#include <iostream>
/**********************************
 * BEGIN Food CLASS IMPLEMENTATION
 **********************************/

/*
 * Default Constructor
 */
Food::Food()
{

}

/*
 * Initialized Constructor; we initialize the object's private members with
 * the arguments passed in, namely the UPC, name, and starting shelf-life
 * of the food 
 */
Food::Food(std::string _UPC, int _qty, int _life)
{
  UPC = _UPC;
  quantity = _qty;
  life = _life;
}

/*
 * Destructor; because C++ is like a helpless baby and can't pick up after
 * itself :/
 */
Food::~Food()
{

}

/*
 * GETTERS 
 */
std::string Food::get_UPC() const   //gets the UPC code of the food
{
  return UPC;
}

std::string Food::get_name() const  //gets the actual name of the food
{
  return name;
}

int Food::get_quantity() const      // gets the quantity of the food associated
{                                   // with the "Recieve" parse
  return quantity;
}

int Food::get_life() const          // gets the shelf life of the food;
{                                   // essentially, how many days until 
  return life;                      // this particular shipment of food spoils
}

/*
 * SETTERS
 */
void Food::setQTY(int const& newqty) // Sets the quantity of a food shipment
{
  quantity = newqty;
}

void Food::age(int const& daydec)    // Age the shipment of food by daydec days 
{
  life = life - daydec;
}