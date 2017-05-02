/* A 'word set' is defined as a set of strings stored
 * in a hashtable that uses chaining to resolve collisions.
 *
 * Peter Jensen
 * January 24, 2017
 */

#include "wordset.h"
#include "node.h"
#include <iostream>  // For debugging, if needed.
#include <string>

namespace cs3505
{
  /*******************************************************
   * wordset member function definitions
   ***************************************************** */
  
  /** Constructor:  The parameter indicates the
    *   size of the hashtable that should be used
    *   to keep this set.
    */
  wordset::wordset(int capacity)
  {
    // Do not change or remove the following line.
    //   It must execute for our auditing to be correct.

    wordset::constructor_calls++;

    // Normal constructor tasks below.
    
    this->capacity = capacity;
    this->count = 0;
    this->table = new node*[capacity];  // Create table (array of node pointers)

    // Make sure the table is clear.  (Not normally cleared on new.)
    
    for (int i = 0; i < capacity; i++)
      table[i] = NULL;
  }

  
  /** Copy constructor:  Initialize this set
    *   to contain exactly the same elements as
    *   another set.
    */
  wordset::wordset(const wordset & other)
  {
    // Do not change or remove the following line.
    //   It must execute for our auditing to be correct.

    wordset::constructor_calls++;

    // Normal constructor tasks below.

    // Initialize this object to appear cleaned (unallocated).
    
    this->capacity = 0;
    this->count = 0;
    this->table = NULL;

    // Use our overloaded assignment operator to do the work
    //   of copying the parameter's elements into this object.
    
    *this = other;
  }

  
  /** Destructor:  release any memory allocated
    *   for this object.
    */
  wordset::~wordset()
  {
    // Do not change or remove the following line.
    //   It must execute for our auditing to be correct.

    wordset::destructor_calls++;

    // Normal destructor tasks below.
    
    clean();  // This does all the work.
  }

  
  /** Adds the specified element to this set.  If the element
    *   is already in this set, no action is taken.
    */
  void wordset::add (const std::string & element)
  {
    // This has been revised slightly from class.  If the
    //   element is not in the linked list (at the correct
    //   table position), I add it to the END of the linked
    //   list.  (I do this because you will use similar
    //   variables and looping in your remove method.)
    
    // Locate the table entry that would contain it.
    
    int index = hash(element);
    
    // Each table entry is the head of a linked list.
    //   Follow it, looking for the element.  Also,
    //   keep track of the last node visited.

    node *current = table[index], *last = NULL;
    
    while (current != NULL)
    {
      if (current->data == element)  // Does a char-by-char comparison
	return;  // Found it - do nothing - it's already in the set.
      last = current;           // Keep track of current as last visited node
      current = current->next;  // Advance to next node
    }

    // Element does not exist in the set.  Create a
    //   node object to contain it.  (New returns
    //   a pointer to it.)  We pass in the string
    //   as part of this operation.
    
    current = new node(element);

    // Add the node to our linked list.  Either
    //   add it to the end of the existing list,
    //   or start a new list.

    if (last == NULL)  // No existing list
      table[index] = current;  // The node becomes the head of a list, store its
                               //   pointer in our table.
    else
      last->next = current;  // Add to end of existing list.

    // Keep track of the number of elements successfully added.
    
    count++;
  }

  
  /** Removes the specified element from this set.  If the
    *   element is not in the set, no action is taken.
    */
  void wordset::remove (const std::string & element)
  {
    // Requirement:  When an element is removed, its
    //   enclosing node must be unlinked from that
    //   linked list, and then the node must be deleted
    //   to free up its memory.  The linked list and/or
    //   table must be appropriately updated.
    
    /*** To be completed by students. ***/
    
    // Locate the table entry that would contain it.
    int index = hash(element);
    
    // Create current and last 'runners'
    node* current = table[index];
    node* last = NULL;

    
    while (current != NULL)
    {
      if (current->data == element)  // found target element to remove
      {

	node* next = current->next; //make note of next node
	delete current; //delete it

	if(last == NULL) //if removed was the end of the linked nodes
	  {
	    table[index] = next; //set to next element hash
	  }
	else
	  {
	    last->next = next; //else just link it
	  }

	count--; //decrement count post-element removal
	return;  // break out post-removal because a set won't have duplicates
      }
	
      last = current;           // Keep track of current as last visited node
      current = current->next;  // Advance to next node
    }
  }

  
  /** Returns true if the specified element in in this set,
    *   false otherwise.
    */
  bool wordset::contains (const std::string & element) const
  {
    // Locate the table entry that would contain it.
    
    int index = hash(element);

    // Each table entry is the head of a linked list.
    //   Follow it, looking for the element.

    node *current = table[index];  // Get the head of the list

    while (current != NULL)
    {
      if (current->data == element)  // Does a char-by-char comparison
	return true;  // Found it.
      
      current = current->next;  // Advance to next node
    }
    
    return false;  // Did not find it.
  }

  
  /** Returns a count of the number of elements
    *   in this set.
    */
  int wordset::size () const
  {
    return count;
  }




  /** populates and returns a vector of strings 
   *    using elements from this set
   */ 
  std::vector<std::string> wordset::get_elements () const
  {
    std::vector<std::string> setofelements (0); //initilize a vector of strings to populate
    
    //using a node runner, go through each index of table and append all elements to the setofelements vector
    for(int i = 0; i < capacity; i++)
      {
	node* current = table[i];

	while (current != NULL)
	  {
	    setofelements.push_back(current->data);
	    current = current->next;
	  }
      }
    return setofelements;
  }







  /*** Assignment operator ***/
  
  /** This function overloads the assignment operator.  It
    *   clears out this set, builds an empty table, and copies
    *   the entries from the right hand side (rhs) set into
    *   this set.
    */
  wordset & wordset::operator= (const wordset & rhs)
  {
    // If we are assigning this object to this object,
    //   do nothing.  (This is important!)

    if (this == &rhs)  // Compare addresses (not object contents)
      return *this;  // Do nothing if identical

    // Wipe away anything that is stored in this object.
    
    clean();
    
    // Create a new set (new table) and populate it with the entries
    //   from the set in rhs.  Use the capacity from rhs.  Hint:
    //   see the first constructor above (but you cannot call it).
    
    // Requirement:  Do not permanently point to arrays or nodes in rhs.  
    //   When rhs is destroyed, it will delete its array and nodes, 
    //   and we cannot count on their existence.  Instead, you will
    //   create a new array for this object, traverse rhs,
    //   and add one entry to this set for every entry in rhs.





    /*** Student work goes here ***/
    
    //set up table array and declare its size; pretty much a partial constructor call
    this->capacity = rhs.capacity;
    this->table = new node*[capacity];
    this->count = 0;

    // Clear the table by iterating thru it and setting the element at each index to NULL
    for(int i = 0; i < capacity; i++)
      {
	table[i] = NULL;
      }

    // Iterate thru rhs and copy the entries in rhs into this->table array
    
    for(int k = 0; k < capacity; k++)
      {
	// set up node pointer to be the 1st node of the index
	node* rhs_current = rhs.table[k]; 
	
	while(rhs_current != NULL) //so as long as we haven't reached the end of the rhs table
	  {
	    add(rhs_current->data); //add element of current rhs node to the this->table
	    rhs_current = rhs_current->next; //set the current node to be the next node before repeating the process
	  }

      }

    // Done with assignment operator.

    return *this;
  }


  /*** Private helper functions ***/
  
  /** Computes a table index for a given string.
    *   If two strings have the same contents, their
    *   hash code (table index) will be identical.
    * The hash code is guaranteed to be in the
    *   range [0..capacity).
    */  
  int wordset::hash (const std::string & s) const
  {
    // A well-known algorithm for computing string hash codes.
    // Students should not change this algorithm in any way.
    
    long long hash = 0;
    for (int i = 0; i < s.length(); i++)
      hash = ((hash * 1117) + s[i]) % capacity;

    // Use modern cast syntax for all casts!
    //   (int) hash   would be OK, but wrong.

    return static_cast<int>(hash);
  }


  /** Releases any memory that was allocated by
    *   this object.  This effectively destroys the
    *   set, so it should only be called if this object
    *   is destructing, or is being assigned.
    */
  void wordset::clean ()
  {
    // If the object is already clean, do nothing.
    //   (Our representation for a clean object is that
    //   the table pointer is NULL).
    
    if (table == NULL)  
      return;

    // Delete all the nodes in the table.

    for (int i = 0; i < capacity; i++)
    {
      // Each entry in the table is the head of a linked list.
      // Follow the head to each node and delete the nodes.

      node *current = table[i];
      table[i] = NULL;  // Invalidate the table entry.

      while (current != NULL)
      {
	// Keep a copy of the next pointer.  When we delete the
	//  node, we can no longer access 'next'.  (Common mistake.)

	node *temp_next = current->next;
	
	// Delete the node.  All its storage should be
	//   considered illegal.  (Afterwards, we cannot access
	//   anything inside of it, not even .next)

	delete current;

	// Advance to the next node.

	current = temp_next;	
      }      
    }

    // The nodes are all deleted.  Just delete the table.

    delete [] table;  // Notice the syntax for deleting an array.
    
    // We're done freeing up memory, but we should set
    //   our pointer to the array to NULL to indicate to
    //   ourself that it has been deleted.  (This also prevents
    //   us from accidentally using it.)

    table = NULL;
    capacity = 0;
    count = 0;

    // Done cleaning up.
  }

  
  /*******************************************************
   * wordset static definitions:
   *
   *     These are for debugging purposes only.  They help 
   * the programmer audit their memory usage.
   *
   *     Do not change anything below this point.
   ***************************************************** */

  // Since static variables are not in objects, we need to define
  // storage for them.  These variables are -here-, not in any object.
  // (This is the ONLY copy of these variables that will exist.)
  // I've changed the names from the version I did in class.
  // Also, I've qualified the access to the static variables to
  // make their scope and usage clear.
  
  long long wordset::constructor_calls = 0;
  long long wordset::destructor_calls = 0;


  /** Returns the number of times any wordset
    *   constructor has been called.
    */
  long long wordset::constructor_count ()
  {
    return wordset::constructor_calls;
  }

  
  /** Returns the number of times the wordset
    *   destructor has been called.
    */
  long long wordset::destructor_count ()
  {
    return wordset::destructor_calls;
  }

}
