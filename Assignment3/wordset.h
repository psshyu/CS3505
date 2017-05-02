/* A 'word set' is defined as a set of strings stored
 * in a hashtable that uses chaining to resolve collisions.
 *
 * Peter Jensen
 * January 24, 2017
 */

#ifndef WORDSET_H
#define WORDSET_H

#include<vector>
#include<string>

namespace cs3505
{
  class node;  // A forward declaration of the class.  We can now
               // create pointer variables involving the node class.
  
  class wordset
  {
    public:
      wordset(int capacity = 100);     // Constructor.  Notice the default parameter value.
      wordset(const wordset & other);  // Copy constructor
      ~wordset();                      // Destructor

      void add      (const std::string & element);  // Not const - modifies the object
      void remove   (const std::string & element);  // Not const - modifies the object
      bool contains (const std::string & element) const;  // const - does not change the object
      int  size     () const;               // Const - does not change object
      std::vector<std::string> get_elements() const; // Const - populates and returns a vector of strings that draw from the set
      wordset & operator= (const wordset & rhs);  // Modifies this object
     
    private:
      int hash (const std::string & s) const;  // Const - does not change this object
      void clean ();                           // Not const - modifies the object
    
      int capacity;  // The size of the hashtable
      int count;     // The number of elements in the set
      node** table;  // The hashtable, a pointer to a node pointer
                   //   (which will really be an array of node pointers)

      // Below are a few definitions that I use to make sure I am cleaning
      //   up memory properly.  (Notice that I can switch back to public / private
      //   as many times as I like.)  These are not part of a normal
      //   hashtable, but they are here because they help with debugging.
      // These functions and variables are declared static - they exist in the
      //   class, but not in any particular object.  (The objects can access
      //   them, of course.)
    
    public:
      static long long constructor_count ();  // A debugging function for Peter
      static long long destructor_count ();   // A debugging function for Peter

    private:
      static long long constructor_calls;  // A debugging variable for Peter
      static long long destructor_calls;   // A debugging variable for Peter    
  };

 
}

#endif
