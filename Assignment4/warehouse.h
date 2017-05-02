/* 
 * CS3505 - Assignment 4 - Resubmit
 * Skylar Shyu 
 * warehouse.h
 */

/*
 * This class represents a warehouse that houses food objects. 
 * Food can be added, removed from the warehouse. Each warehouse 
 * possesses a unique name.
 */


#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include "food.h" 
#include <string>
#include <map>
#include <queue>
#include <vector>

class warehouse
{
 private:
  std::string name; // name of warehouse

 public:
  /*
   * The constructors & destructor
   */
  warehouse(); // default constructor
  warehouse(const std::string, const std::map<std::string, std::string>); // Initialized constructor
  ~warehouse(); // Destructor. Because C++ is a special snowflake
  
  std::map<std::string, std::vector<Food> > FoodBins;            // Containers to hold food objects
  void InitializeBins(const std::map<std::string, std::string>); // creates a food container to hold shipments of 1 food-type (i.e. all Apples) 
  void ReceiveFood(const std::string, const int, const int);     // performs receive food actions
  void RequestFood(const std::string, const int);                // performs request food actions
  void AgeAndToss();                                             // decrease shelf life by of all foods by 1, 
                                                                 // then 'tosses' (deletes) those with a shelf life of 0 
};

#endif
