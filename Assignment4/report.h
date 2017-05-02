/* 
 * CS3505 - Assignment 4 - Resubmit
 * Skylar Shyu 
 * Report.h
 */

/*
 * This class generates a Report according to the aspects listed in the assignment,
 * simply because trying to generate all the output while adhering to the requested
 * formatting is way too cluttersome. Separation of concerns at work!
 */


#ifndef REPORT_H
#define REPORT_H

#include "food.h" 
#include "warehouse.h"
#include <string>
#include <fstream>
#include <set>


class Report
{
 private:
  std::set<std::string> FoodUPCs;             // set of all food UPCs
  std::map<std::string, std::string> foods;   // UPC to food name map
  std::map<std::string, int> foodLife;        // UPC to string shelf-life map
  std::map<std::string, warehouse> warehouses;// warehouse name to warehouse object map
  std::map<std::string, int> CountLog;        // for keeping track of well-stocked products
  std::map<std::string, int> RequestLog;      // for keeping track of most popular products

  /*
   * Helpers for parsing the inputfile
   */  
  void Parse_FoodItem(std::string);         // Parses food item
  void Parse_Warehouse(std::stringstream&); // Parses warehouse 
  void Parse_Receive(std::stringstream&);   // Parses Receives
  void Parse_Request(std::stringstream&);   // Parses Requests
  void Parse_BeginNewDay();                 // Parses New day

 public:
  /*
   * The constructors & destructor
   */
  Report(); // default constructor
  Report(const std::string); // Initialized constructor
  ~Report(); // Destructor. Because C++ is a special snowflake
  
  // Does all necessary actions for registering a food for future use 
  void RegisterFood(const std::string, const std::string, const int);
  // Does all necessary actions for registering a warehouse for future use 
  void RegisterWarehouse(const std::string);
  
  void printReport(); // Prints a report

  /*
   * Report helpers
   */
  // Generates a report and prints the unstocked products
  void UnstockedProducts();
  // Generates a report and prints the most popular products
  void PopularProduct();
  // Generates a report and prints the well-stocked products
  void WellStocked();
};

#endif
