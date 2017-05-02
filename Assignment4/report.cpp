/* 
 * CS3505 - Assignment 4 - Resubmit
 * Skylar Shyu 
 * food.cpp
 */

/*
 * This class represents a food object. Each food will have a UPC Code (int),
 * the shelf life before expiry (int), and the name of the food (string). 
 */

#include "report.h" 
#include <sstream>
#include <iostream>
#include <string>
#include <list>
#include <set>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

/**********************************
 * BEGIN FOOD CLASS IMPLEMENTATION
 **********************************/

/*
 * Default Constructor
 */
Report::Report()
{
    
}

/*
 * Initialized Constructor; we initialize the object's private members with
 * the arguments passed in. Upon examination of each line, appropriate actions
 * are then further taken via Parse_xxxxxx() methods.
 */
Report::Report(const std::string input)
{
    std::ifstream infile(input.c_str());    // gets the file and 'opens' it
    std::string aLine, tempUPC;             // some temp variables
    
    while (std::getline(infile, aLine))     // 'reads' every line in the file
    {
        std::stringstream ss(aLine);        //  line as a stringstream

        switch (aLine[0])   // The line is...
        {
            case 'F':       // ... a FoodItem
                Parse_FoodItem(aLine);
                break;
            case 'W':       // ... a Warehouse
                Parse_Warehouse(ss);
                break;
            case 'S':       // ... denoting the start of Request/Receives
                break;
            case 'N':       // ... denoting the start of a new day
                Parse_BeginNewDay();
                break;
            case 'R':       // ... either a request or a receive
                if(aLine[2] == 'c') // ... it's a receive:
                {
                    Parse_Receive(ss);
                }
                else                //... it's a request
                {
                    Parse_Request(ss);
                }
                break;
        }

    }
}

/*
 * Destructor
 */
Report::~Report()
{

}

/*
 * Registeration of Food and Warehouses for quick retrieval
 */ 
void Report::RegisterFood(const std::string UPC, const std::string name, const int shelflife)
{
    foods.insert(std::pair<std::string, std::string>(UPC, name)); // register UPC to name
    foodLife.insert(std::pair<std::string, int>(UPC, shelflife)); // register UPC to shelf life

    /*
     * The 3 inserts below are priming the report generation
     */
    FoodUPCs.insert(UPC);   // for UnstockedProducts() use
    CountLog.insert(std::pair<std::string, int>(UPC, 0));   // for WellStocked() use
    RequestLog.insert(std::pair<std::string, int>(UPC, 0)); // for PopularProduct() use
}

void Report::RegisterWarehouse(const std::string name)
{
    warehouse AWarehouse(name, foods);  // creates a warehouse that 'knows' all the foods
    warehouses.insert(std::pair<std::string, warehouse>(name, AWarehouse)); // 'Registers'' the warehouse
}

/*
 * Where the report generation and printing to console happens
 */
void Report::printReport() 
{
    
    std::cout << "Report by Skylar Shyu" << std::endl;
    std::cout << std::endl;
    UnstockedProducts();    // Figure out and prints the Unstocked Products
    WellStocked();          // Figure out and prints the Well-Stocked Products
    PopularProduct();       // Figure out and prints the Popular Products
}


/******************* 
 * PRIVATE HELPERS *
 *******************/

// Executes all the actions when a Food's existence is named'
void Report::Parse_FoodItem(std::string line)
{
    std::string foodname;               // temp variable
    std::vector<std::string> tokens;    // temp vector to hold all words in the line

    boost::split(tokens, line, boost::is_space());  // splits the line and inserts each word
                                                    // into the temp vector as individual elements
    
    for(int i = 11 ; i < tokens.size(); i++)    // iterate thru all the elements in the vector
    {
        if(i == tokens.size() - 1)      // this if/else block concats the name 
        {                               // of the food together, paying attention
            foodname += tokens[i];      // to the spacing when necessary
        }
        else
        {
            foodname += tokens[i] + " ";
        }
        
    }

    //parameters are: (UPC, name, shelflife)
    RegisterFood(tokens.at(4), foodname, boost::lexical_cast< int >(tokens.at(8)));
}

// Executes all the actions when a Warehouse's existence is named
void Report::Parse_Warehouse(std::stringstream &ss)
{
    std::string blank1, blank2, warehouse;  // set up some temp string variables
    ss >> blank1 >> blank2 >> warehouse;    // parse the stringstream, and assign the parsed 
                                            // information to their respective temp variables

    RegisterWarehouse(warehouse);           // Primes the Warehouse for future functionalities

    ss.str(""); //empty the stringstream
}

// Executes all the actions when a shipment is Received
void Report::Parse_Receive(std::stringstream &ss)
{
    std::string UPC, warehouse, blank;      // set up some temp string variables
    int qty;                                // set up the temp quantity variable
    ss >> blank >> UPC >> qty >> warehouse; // parse the stringstream, and assign the parsed 
                                            // information to their respective temp variables

    warehouses[warehouse].ReceiveFood(UPC, qty, foodLife[UPC]); // locate the proper warehouse
                                                                // and have it recieve the food shipment
    ss.str(""); // empty the stringstream
}

// Executes all the actions when a Request is made
void Report::Parse_Request(std::stringstream &ss)
{
    std::string UPC, warehouse, blank;      // set up some temp string variables
    int qty;                                // set up the temp quantity variable
    ss >> blank >> UPC >> qty >> warehouse; // parse the stringstream, and assign the parsed
                                            // information to their respective temp variables

    RequestLog[UPC] += qty; // make note of the quantity of items that was requested
                            // (used for the Most Popular Product Report)

    warehouses[warehouse].RequestFood(UPC, qty);    // locate the proper warehouse
                                                    // and have it fulfill the request
    ss.str(""); // empty the stringstream
}

// Executes the actions when a new day begins
void Report::Parse_BeginNewDay()
{
    for(auto wh_it = warehouses.begin(); wh_it != warehouses.end(); wh_it++) //iterate thru all warehouses...
    {
        warehouse &wh = wh_it->second;  // ...access the warehouse object
        wh.AgeAndToss();                // ...and begin aging all foods by a day and 
                                        // tossing all the spoiled foods
    }
}

/*****************************************
 * Begin spitting out the 3 requirements *
 *****************************************/
 /*
 * Generates and prints a list of products (UPC and name) that do not exist 
 * in ANY warehouses
 */
void Report::UnstockedProducts()
{
    std::cout << "Unstocked Products:" << std::endl;
    
    //iterate thru all warehouses
    for(auto wh_it = warehouses.begin(); wh_it != warehouses.end(); wh_it++) 
    {
        warehouse wh = wh_it->second;
        //then iterate thru all the FoodBins inside the warehouse   
        for(auto bin_it = wh.FoodBins.begin(); bin_it != wh.FoodBins.end(); bin_it++) 
        {
            std::vector<Food> A_Bin = bin_it->second; // get the actual bin
            // If there's something inside the bin AND we have not struck it off our list..
            if((A_Bin.size() > 0) && (FoodUPCs.count(bin_it->first) > 0))
            {
                FoodUPCs.erase(bin_it->first);  // ...strike it off our list
            }

            // If we struck off all foods on the list, we don't have anything else to check
            if(FoodUPCs.size() == 0)
            {
                break; // so we break out of the loop
            }  
        }
    }

    if(FoodUPCs.size() != 0) // if we actually have unstocked products
    {
        // iterate thru what's left of the list
        for(auto foodit = FoodUPCs.begin(); foodit != FoodUPCs.end(); ++foodit)
        {
            // print the UPC and actual name of the food item
            std::cout << *foodit << " " << foods[*foodit] << std::endl;
        }
    }

    std::cout << std::endl;
}

/*
 * The most popular products are the most products with the highest total
 * quantities of requests. Generates and prints the top 3 foods (UPC and 
 * name).
 */
void Report::PopularProduct()
{
    std::string First = "";
    std::string Second = "";
    std::string Third = "";
    int FirstQty = 0;
    int SecondQty = 0;
    int ThirdQty = 0;

    std::cout << "Most Popular Products:" << std::endl;
    // go through each food item
    for(auto req_it = RequestLog.begin(); req_it != RequestLog.end(); req_it++)
    {
        // these if/else if statements were placed with transitive logic in mind
        if(req_it->second > FirstQty) // if the current food item is the even more  
        {                             // popular than any previous food item iterated  
            ThirdQty = SecondQty;       // Second most popular now becomes the third...
            Third = Second;
            SecondQty = FirstQty;       // The most popular is the now second... 
            Second = First;
            FirstQty = req_it->second;  // ...And the current food item is now THE MOST 
            First = req_it->first;      // popular (FIRST)
        }
        else if(req_it->second > SecondQty) // The above logic applies here if the current
        {                                   // food item is more popular than the 2nd, but
                                            // isn't more popular than the 1st
            ThirdQty = SecondQty;       // Second most popular now becomes the third...
            Third = Second;
            SecondQty = req_it->second; // ...And the current food item is now THE SECOND
            Second = req_it->first;     // most popular 
        }
        else if (req_it->second > ThirdQty) // Above logic applies again... if the current
        {                                   // food item is more popular than the 3rd, but
                                            // not more popular than the 2nd or 1st...
            ThirdQty = req_it->second;  // ...the current food item is now THE THIRD      
            Third = req_it->first;      // most popular
        }
        else 
        {
            // if the current food item's popularity doesn't crack our current top 3
            // then do nothing.
        }
        
    }

    /*
     * Print the popular items ONLY if there are actually foods
     */
    if(First != "")
        std::cout <<  First << " " << foods[First]  << std::endl;
    if(Second != "")
        std::cout <<  Second << " " << foods[Second]  << std::endl;
    if(Third != "")
        std::cout <<  Third << " " << foods[Third] <<  std::endl;

   std::cout << std::endl;
}
/*
 * Generates and prints a list of products (UPC and name) that have positive 
 * quantities in at least 2 warehouses
 */
void Report::WellStocked()
{
    std::cout << "Well-Stocked Products:" << std::endl;
    
    // iterate thru each warehouse
    for(auto wh_it = warehouses.begin(); wh_it != warehouses.end(); wh_it++)
    {
        warehouse wh = wh_it->second; //actual warehouse object
        // iterate thru each foodbin within the warehouse
        for(auto bin_it = wh.FoodBins.begin(); bin_it != wh.FoodBins.end(); bin_it++)
        {
            std::vector<Food> A_Bin = bin_it->second; // actual bin 
            if(A_Bin.size() > 0) // if the product exists..
            {
                CountLog[bin_it->first]++; // ...that's 1 additional warehouse that stocks the item
            }
        }
    }

    // iterate through the CountLog...
    for(auto count_it = CountLog.begin(); count_it != CountLog.end(); count_it++)
    {
        if(count_it->second >= 2) // ...if a food appeared in more than 2 or more warehouses
        {
            // print the UPC and actual name of the food item
            std::cout << count_it->first << " " << foods[count_it->first] << std::endl;
        }
    }
    std::cout << std::endl;
}
