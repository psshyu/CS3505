/* This node class is used to build linked lists for the
 * wordset class.
 *
 * Peter Jensen
 * January 24, 2017
 */

#include "node.h"
#include <string>

namespace cs3505
{
  /*******************************************************
   * node member function definitions
   ***************************************************** */

  /** Constructor:  Creates a node containing
    *   an element.  It is initialized to
    *   not point to any other node.
    */
  node::node(const std::string & d)
    : data(d)  // This syntax is used to call member variable constructors
               // This is the copy constructor - we are making a copy of the string.
  {
    // Do not change or remove the following line.
    //   It must execute for our auditing to be correct.

    node::constructor_calls++;

    // Normal constructor tasks below.

    this->next = NULL;  // This node points to no other node.
  }

  
  /** Destructor:  release any memory allocated
    *   for this object.
    */
  node::~node()
  {
    // Do not change or remove the following line.
    //   It must execute for our auditing to be correct.

    node::destructor_calls++;

    // Normal destructor tasks below.

    next = NULL;  // Invalidate the entry.
    
    // I have removed the recursive destruction that
    //   I used in class (bad idea).

    // Note:  The string object in 'this' is now going
    //   out of scope.  it was allocated as part of this
    //   node, so its destructor will automatically
    //   be called after the node destructor completes.
    // (We didn't directly allocate it, so we don't
    // directly delete it.)
  }

  
  /*******************************************************
   * node static definitions:
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
  
  long long node::constructor_calls = 0;
  long long node::destructor_calls = 0;

  /** Returns the number of times any node
    *   constructor has been called.
    */
  long long node::constructor_count ()
  {
    return node::constructor_calls;
  }

  
  /** Returns the number of times the node
    *   destructor has been called.
    */
  long long node::destructor_count ()
  {
    return node::destructor_calls;
  }
}
