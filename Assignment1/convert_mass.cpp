// CS3505 - Assignment 1
// convert_mass.cpp
// 01/11/2017 | SP2017
// Skylar Shyu | u1039726

// Libraries
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <math.h>

using namespace std;

// rounds a double to 2 places right of the decimal (nearest hundredths)
// if the tenths and hundredths places are both 0s, omit them (i.e. 54.00 becomes 54)

void roundCalculationAndPrint(double calculated)
{
  // breaks the double into the integer portion and the decimal portion. 
  double integerPortion;
  double rightOfDecimal = (modf(calculated, &integerPortion));
  
  // Prints the result of the conversion. 
  // if the tenths and hundredths places are both 0s, omit them (i.e. 54.00 becomes 54)
  if(rightOfDecimal < 0.005 || rightOfDecimal > 0.995) 
    {
      cout << "Weight in kilograms is " << nearbyint(calculated) << " kg." << endl; 
    }
  else
    {
      cout << "Weight in kilograms is " << fixed << setprecision(2) << calculated << " kg." << endl;
    }

}

int main(int argc, char* argv[])
{
  // prompts the user to enter a weight in pounds and stores the user input as a double 
  double lbsweight;
  cout << "Please enter a weight in pounds: ";
  cin >> lbsweight;
  
  // performs the lbs to kg calculation and passes the result into the rounding and printing function
  double kgweight = lbsweight * 0.45359237;
  roundCalculationAndPrint(kgweight);
}

