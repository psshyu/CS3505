/*
 * vector - A class that represents x,y vector.
 */

#include <iostream>
#include <string>
#include "vector.h"

using namespace std;

vector::vector()
{
  x = 0;
  y = 0;
  cout << "Constructor called." << endl;
}

vector::vector(const vector & other)
{
  x = other.x;
  y = other.y;
  cout << "Copy Constructor called." << endl;
}

vector::vector(int _x, int _y)
{
  x = _x;
  y = _y;
  cout << "Value constructor called." << endl;
}

vector::~vector()
{
  cout << "Destructor called." << endl;
}

void vector::debug()
{
  
  cout << "Debug called." << x << " " << y << endl;
}

vector vector::operator+ (const vector & rhs) const
{
  vector result (x + rhs.x, y + rhs.y);
  return result;

}

vector & vector::operator= (const vector & rhs)
{
  x = rhs.x;
  y = rhs.y;
  return *this;
}
