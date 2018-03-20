/* Peter Jensen
 * January 13, 2017
 *
 * An example of addition and multiplication algorithms
 * for large integers stored in strings.  The algorithms below
 * may be used as a starting point for your code, but it is up to
 * you to make sure they work in all cases.  Note:  This is not robust.
 * Care should be taken to ensure that the strings only have numeric
 * digits within them.
 *
 * If you'd like to see how the algorithms work, add some cout statements
 * to them.  (That is the primary reason for using strings for integer
 * storage, they're easy to print out in the middle of an algorithm.  You'd
 * normally use arrays of ints and base 2^32 digits for big integers.)
 */

#include <iostream>  // If you don't print out anything, don't bother with this
#include <string>    // Sometimes all you need to import is this one

// The following is a convenience.  Otherwise, we'd have to qualify all
//   of the std library identifiers like this:  std::string or std::cout
// Note:  Never say 'using namespace' in a .h file.  Qualify your
//   identifiers instead.  Ask about this in class.

using namespace std;

// Forward declarations - everything must be declared before it is used.

string add (string left, string right);
string multiply (string left, string right);

// Main - the application entry point
int main ()
{
  // A quick test or two
  
  cout << "Hello" << endl;
  cout << add("234", "432") << endl;
  cout << multiply("9389475933475987349857983453", "837948759837459879387983749875983487") << endl;

  cout << multiply("214", "987") << endl;

  // Inform the shell that the program terminated without errors by
  //   returning a 0.  (We don't make use of this error code.)
  
  return 0;
}

/*
 * Adds two integers stored in strings, building a string result.
 */
string add (string left, string right)
{
  // Build up a string to contain the result.
  
  string result = "";

  // Work right to left.

  int left_pos  = left.length()  - 1;
  int right_pos = right.length() - 1;

  int carry = 0;

  // Loop, adding columns until no more digits remain.

  while (left_pos >= 0 || right_pos >= 0 || carry > 0)
  {
    // Get the digit from each string, or 0 if none.

    int left_digit  = left_pos  >= 0 ? left[left_pos--]   - '0' : 0;
    int right_digit = right_pos >= 0 ? right[right_pos--] - '0' : 0;

    // Calcuate the sum of the digits.

    int sum = left_digit + right_digit + carry;
    carry = sum / 10;
    sum   = sum % 10;

    // Put the sum into the new string (at the left side)

    result.insert (0, 1, (char)(sum+'0') );    
  }

  // Strip out any leading 0's from our result (but leave at least one digit).
  //   (Only useful for subtraction, but I'm giving it to you here.)

  while (result.length() > 1 && result[0] == '0')
    result.erase(0, 1);
  
  return result;
}

/*
 * Multiplies two integers stored in strings, building a string result.
 * The algorithm is long multiplication, starting with the most significant
 * digit of the multiplier.
 */
string multiply (string left, string right)
{
  string result = "0";

  int right_pos = 0;

  while (right_pos < right.length())
  {
    result.append("0");  // Multiply the product by 10.

    int right_digit = right[right_pos++] - '0';

    for (int i = 0; i < right_digit; i++)
      result = add(result, left);
  }

  return result;
}

// Other really useful helper functions you may want to write

//is_bigger (a, b)
//subtract (a, b)
//divide (a, b)
//mod (a, b)

