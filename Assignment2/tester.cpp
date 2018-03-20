#include <iostream>
#include <string>
#include "huge_integer.h"

using namespace std;

int main()
{
  huge_integer h("211");
  huge_integer a("2");
  huge_integer y;
  huge_integer z;
  // a.get_value();
  z = h / a;
  cout << "quot: " << z.get_value() << endl;
  // y = h % a;
  // cout << "rem: " <<  y.get_value() << endl;

}
/*
string huge_integer::div(string dividend, string divisor, bool quotient) const
{
  string quot = "";
  string rem = "";

  int left_pos = left.length() - 1;
  int right_pos = right.length() - 1;

  if(right_pos > left_pos) //if the difference will be negative, return 0
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
  else
    {

    }

}

*/

