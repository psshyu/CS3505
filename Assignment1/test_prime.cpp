
// CS3505 - Assignment 1
// test_prime.cpp
// 01/11/2017 | SP2017
// Skylar Shyu | u1039726

// Libraries
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <string>
#include <math.h>

using namespace std;

// Prints either 'prime' or 'composite'
void printOutput(string primeOrNot)
{
  cout << primeOrNot << endl;
}

// Checks the primality of the int argument 
void primeChecker(int number)
{
  string result = "prime"; //let the default result be 'prime'

  // begin checking primality
  // if the integer is not prime, we assume it's composite

  if (number < 2) //cases where the integer is less than 2
    {
      result = "composite";
    }
  else if (number == 2 || number == 3) //special prime cases: 2 and 3
    {
      //pass, keep result as "prime"
    }
  else
    {
      int currentfactor = 3; //set the lowest factor to 3
      int maxfactor = (int)floor(sqrt(number));  // the largest factor we ought to check is square root of int; factors larger than maxfactor would simply be complements of factors we have already checked. 
      
      if(number % 2  == 0) // we check if int is even; if it is at this point, it's definitely NOT prime
	{
	  result = "composite";
	}
      else  //if the int is odd...  
	{
	  while(currentfactor <=  maxfactor) // ...we can check the odd factors starting from 3 
	    {
	      if(number % currentfactor == 0) // if the currentfactor divides the integer without producing a remainder
		{                             // the integer is composite AND we can break out of loop as checking the   
		  result = "composite";       // remaining factors up to and including maxfactor is unnecessary.
		  break;
		}
	      else                            // incrementally increase the current factor by 2 
		{
		  currentfactor = currentfactor + 2; 
		}
	    }
	}
    }
  // call the function to print whether the integer is prime or composite
  printOutput(result);

}

int main(int argc, char* argv[])
{
  // these if/else statements handle the user input regarding the number of arguments
  if(argc == 1)       // no number given
    {
      cout << "Please relaunch the program with an number to evaluate." << endl;
    }
  else if (argc == 2) // 1 number given
    {
      int num = atoi(argv[1]); // converts the char* into int
      primeChecker(num);       // passes the user-definied number to be evaluated into primeChecker()
    }
  else                // more than 1 number given
    {
      cout << "Please relaunch the program with a single number to evaluate. You have inputted " << argc - 1 << " numbers." << endl;
    }
}
