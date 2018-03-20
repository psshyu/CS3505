/* 
 * huge_integer: A class representing non-negative ints of an arbitrary 
 */

#include "huge_integer.h"
#include <iostream>
#include <string>

using namespace std;

/* Constructors */
huge_integer::huge_integer(string s)
{
  value = s;
}

huge_integer::huge_integer(const huge_integer & other)
{
  value = other.value;
}

huge_integer::huge_integer()
{
  value = "0";
}

huge_integer::~huge_integer()
{

}

/* Debug */
void huge_integer::debug()
{

}

// returns the value of the huge_integer object
string huge_integer::get_value() const
{
  
  return value;
}

// Strip out any leading 0's from our result (but leave at least one digit)
// Credit goes to prof. Jensen
string huge_integer::stripzeroes(string astring) const
{
  while (astring.length() > 1 && astring[0] == '0')
    astring.erase(0, 1);

  return astring;
}


//
/* HELPERS */
//

// Takes 2 strings, divides them. Depending if quotient is true, will return quotient (true)
// or remainder (false)
string huge_integer::div(string dividend, string divisor, const bool quotient) const
{
  // set up strings for quotient and remainder results
  string quot = ""; 
  string rem = "";

  // strip them of preceding zeroes
  dividend = stripzeroes(dividend);
  divisor = stripzeroes(divisor);

  int beginindex = 0;
  int lastindex = divisor.length();
  int dvisor = stoi(divisor);

  if(dividend.compare(divisor) < 0) //if the divisor > dividend, quotient is zero and remainder is the dividend
    {
      quot = "0";
      rem = dividend;
      
      if(quotient == true)
	{
	  return quot;
        }
      else
        {
	  return rem;
        } 
    }
  else if (dividend.compare(divisor) == 0) //if divisor = dividend, perfect division happens
    {
      quot = "1";
      rem = "0";
      if(quotient == true)
	{
	  return quot;
        }
      else
        {
	  return rem;
        } 
    }
  else //do the division operation
    {
      int tempquot = 0;
      int temprem = 0;
      string dividethisSTR = dividend.substr(beginindex, divisor.length());
      
      while(lastindex < dividend.length())
	{
	  while(dividethisSTR.compare(divisor) < 0)
	    {
	      dividethisSTR+= lastindex;
	      lastindex++;
	    }

	  quot+= to_string(stoi(dividethisSTR) / dvisor);
	  dividethisSTR = to_string(stoi(dividethisSTR) % dvisor);
	}
      rem = dividethisSTR;
      if(quotient == true)
	{
	  return quot;
        }
      else
        {
	  return rem;
        } 
    }
}

// takes in 2 strings, finds the difference
// Code derived from prof. Jensen's add code.
string huge_integer::sub(string left, string right) const
{
  //begin a string to keep track of difference
  string result = "";
  
  //strip them of preceding zeroes
  left = stripzeroes(left);
  right = stripzeroes(right);

  //starting at the last index of each string
  int left_pos = left.length() - 1;
  int right_pos = right.length() - 1;

  if(right_pos > left_pos) //if the difference will be negative, return 0
    {
      result = "0";
      return result;
    }

  // conducts subtraction via 9's complement
  // submits the 2nd string to nine's complement via iterating thru each digit
  while (right_pos >=0 || left_pos >=0) 
    {
      int right_digit = right_pos >= 0 ? right[right_pos--] - '0' : 0;
      int diff = 9 - right_digit;
      result.insert(0, 1, (char)(diff+'0'));
      left_pos--;
    }

  
  result = add(result, left ); //add the result to the 1st string
  result = add(result, "1");   //get rid of the first '1' of the result
  result = result.substr(1);   //then add one on top

  return result;
}


// multiplies 2 strings together
// Code created by prof. Jensen; some variables were renamed.
string huge_integer::mul(string left, string right) const
{
  //begin blank string to formulate product
  string result = "";

  // strip strings of preceding zeroes
  left = stripzeroes(left);
  right = stripzeroes(right);

  //initialize starting index
  int right_pos = 0;

  while (right_pos < right.length())
    {
      result.append("0");  // Multiply the product by 10.

      int right_digit = right[right_pos++] - '0'; //turn the value @ right_pos++ from char to int

      //loop thru and compound add to get the product
      for (int i = 0; i < right_digit; i++)
	{
	  result = add(result, get_value());
	}
    }

  return result;
}


// Adds 2 integers stored in strings, building a string result.
// Code from prof. Jensen; variables were renamed
string huge_integer::add(string string1, string string2) const
{
  // Build up a string to contain the result.
  string result = "";

  //Strip the strings of unnecessary preceding zeroes 
  string1 = stripzeroes(string1);
  string2 = stripzeroes(string2);

  // Work right to left.

  int left_pos  = string1.length() - 1;
  int right_pos = string2.length() - 1;

  int carry = 0;

  // Loop, adding columns until no more digits remain.

  while (left_pos >= 0 || right_pos >= 0 || carry > 0)
  {
    // Get the digit from each string, or 0 if none.

    int left_digit  = left_pos  >= 0 ? string1[left_pos--]   - '0' : 0;
    int right_digit = right_pos >= 0 ? string2[right_pos--] - '0' : 0;

    // Calcuate the sum of the digits.

    int sum = left_digit + right_digit + carry;
    carry = sum / 10;
    sum   = sum % 10;

    // Put the sum into the new string (at the left side)

    result.insert (0, 1, (char)(sum+'0') );    
  }

  return result;
}



/* OVERLOADED OPERATIONS */
// For the following methods, their respective operators are overloaded such that their
// arithmetic functions can be conducted on the huge_integer objects


huge_integer huge_integer::operator+ (const huge_integer & rhs) const
{
  //call the helper function associated with the operator and assign result to a string
  string result = add(get_value(), rhs.value);
  huge_integer sum(result);
  return sum;
}

huge_integer huge_integer::operator- (const huge_integer & rhs) const
{
  //call the helper function associated with the operator and assign result to a string
  string result = sub(get_value(), rhs.value);

  //create the new huge_integer object using result and return it
  huge_integer difference(result);
  return difference;
}

huge_integer huge_integer::operator/ (const huge_integer & rhs) const
{
  //get the quotient
  bool quotient = true;

  //call the helper function associated with the operator and assign result to a string
  string result = div(get_value(), rhs.value, true);

  //create the new huge_integer object using result and return it
  huge_integer quot(result);
  return quot;
}

huge_integer huge_integer::operator% (const huge_integer & rhs) const
{
  //get the remainder
  bool quotient = false;

  //call the helper function associated with the operator and assign result to a string
  string result = div(get_value(), rhs.value, false);

  //create the new huge_integer object using result and return it
  huge_integer remainder(result);
  return remainder;
}

huge_integer huge_integer::operator* (const huge_integer & rhs) const
{
  //call the helper function associated with the operator and assign result to a string
  string result = mul(get_value(), rhs.value);

  //create the new huge_integer object using result and return it
  huge_integer product(result);
  return product;
}

huge_integer & huge_integer::operator= (const huge_integer & rhs)
{
  value = rhs.value;
  return *this;
}
