/* 
 * CS3505 - Assignment 4 - Resubmit
 * Skylar Shyu 
 * warehouse.cpp
 */

/*
 * This class represents a warehouse object. Each warehouse has a unique
 * name (string) for identification. 
 */

#include "warehouse.h" 
#include <iostream>
#include <algorithm>


/**********************************
 * BEGIN Food CLASS IMPLEMENTATION
 **********************************/


/*
 * Default Constructor
 */
warehouse::warehouse()
{

}

/*
 * Initialized Constructor; we initialize the object's private members with
 * the arguments passed in
 */
warehouse::warehouse(const std::string warehousename, const std::map<std::string, std::string> AllUPCs)
{
  name = warehousename;     // sets the name of the warehouse object
  InitializeBins(AllUPCs);  // Set-up food bins to 'store' the foods
}

/*
 * Destructor; again, because C++ is like a helpless baby and can't pick up 
 * after itself :/
 */
warehouse::~warehouse()
{

}

/*
 * Sets up empty bins that the warehouse can store and retrieve food shipments
 */ 
void warehouse::InitializeBins(const std::map<std::string, std::string> AllUPCs)
{
  // iterates through all the UPCS
  for(auto it = AllUPCs.begin(); it != AllUPCs.end(); it++)
  {
    std::string UPC = it->first;  // actual UPC
    std::vector <Food> shipments; // vector 'bin' to hold all the shipments of a food

    //insert into list of bins
    FoodBins.insert(std::pair<std::string, std::vector<Food> > (UPC, shipments)); 
  }
}

/*
 * creates foodshipment and places it in the appropriate warehouse bin
 */
void warehouse::ReceiveFood(const std::string _UPC, const int _qty, const int _life)
{
  Food shipment(_UPC, _qty, _life); // create food object to represent shipment
  std::vector <Food> &bin = FoodBins.find(_UPC)->second; // locate the appropriate bin
  bin.insert(bin.end(), shipment);  // place shipment inside the bin
}


/*
 * Removes requested food. If the request is larger, removes whatever it can.
 * A shipment (nor a bin) will have 'negative' quantities
 */
void warehouse::RequestFood(const std::string _UPC, const int _qty)
{ 
  int need = _qty;  // create a non-const 'copy' of quantity requested
  std::vector <Food> &bin = FoodBins.find(_UPC)->second; //retrieve the bin

  if(bin.size() > 0) //if there are some food shipments left....
  {
    // go thru the bin
    for(int i = 0; i < bin.size(); i++)
    {
       if(bin[i].get_quantity() < need) // if we need more than what this shipment has
       {
         need -= bin[i].get_quantity(); // reduce what we need
         bin[i].setQTY(0);              // remove all items in shipment
       }
       else                             // if not...
       {
         bin[i].setQTY(bin[i].get_quantity() - need); // take what we need from shipment
         need = 0; // and we don't need anymore items
         break;
       }
    }

    // now we deal with all those emptied packages in the bins...
    for(auto it = bin.begin(); it != bin.end(); ) // go thru all the bins (again) 
    {
      if(it->get_quantity() == 0) // if the shipment is empty
      {
        it = bin.erase(it); // delete it.
      }
      else 
      {
        ++it; // this is to prevent iteration errors
      }
    }

  }
  else // if there were no food shipments in the bin to start
  {
    need = 0; // sorry, we couldn't fulfill the request... :(
  }
}

/*
 * This method goes through every single shipment in every single food bin in the warehouse 
 * object and ages the shipments. It deletes all the shipments where the shelf-life
 * has been decremented to 0. 
 */ 
void warehouse::AgeAndToss()
{
  for(auto bin_it = FoodBins.begin(); bin_it != FoodBins.end(); bin_it++) //iterate thru all Foodbins
        {
            std::vector<Food> &A_Bin = bin_it->second; // accesses a bin
           
            // iterates thru all the shipments inside the bin
            for(auto it = A_Bin.begin(); it != A_Bin.end(); ) 
            {
              it->age(1); //decrement the age of a shipment by 1
              if(it->get_life() == 0) // if the shelf-life is 0
              {
                it = A_Bin.erase(it); // erase it from the bin
              }
              else 
              {
                ++it; // this is to prevent iteration errors
              }
            }   
        }
  
}