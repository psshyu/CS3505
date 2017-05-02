/* 
 * CS3505 - Assignment 4 - Resubmit
 * Skylar Shyu 
 * food.h
 */

/*
 * This class represents a Food package/shipment. Each Food object will 
 * have a UPC Code (int), the shelf life before expiry (int), the quantity
 * contained within the package (int), and the name of the Food (string). 
 */


#ifndef FOOD_H
#define FOOD_H

#include <string>

class Food
{
 private:
  std::string UPC;  // the unique UPC code each Food possesses
  std::string name; // what the Food is called
  int quantity;     // amount of Food
  int life;         // how many more days the shipment of food is good for

 public:
  /*
   * The constructors & destructor
   */
  Food(); // default constructor
  Food(std::string, int, int); // Initialized constructor
  ~Food(); // Destructor. Because C++ is a special snowflake

  /*
   * The lone setters
   */
  void setQTY(int const&);
  void age(int const&);

  /*
   * The getters for the UPC, name, and shelf life of 
   * the Food, respectively
   */
  std::string get_UPC() const;
  std::string get_name() const;
  int get_quantity() const;
  int get_life() const;
};

#endif
