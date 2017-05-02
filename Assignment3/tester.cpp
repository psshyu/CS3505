/*
 * This is a tester similar to the tester written in class.  It reads
 * words from a text file, then adds the words to two sets: A built-in
 * set class, and our wordset class.  After reading the file, it
 * prints out all the words stored in the STL set object.  At the end
 * of the test, it prints out the sizes of both sets to see that they
 * are the same.
 *
 * After the test completes, I make sure the local variabls are properly
 * cleaned up.
 *
 * If the comments wrap lines, widen your emacs window.
 *
 * Peter Jensen
 * January 24, 2017
 */

#include <iostream>
#include <fstream>
#include <set>
#include <iterator>
#include <cassert>
#include "wordset.h"
#include "node.h"

using namespace std;

// Note:  Our classes were declared in a cs3505 namepsace.
//        Instead of 'using namespace cs3505', I qualify the class names below with cs3505::

int main ()
{
  int testcounter = 0;
  bool passed = true;

  // Open up another block.  This way, when the block ends,
  // variables local to the block will be destroyed, but main
  // will still be running.  (Did you know that you can open
  // up a block at any time to control local variable scope and
  // lifetime?)
  
  {
    // Create the two sets.  Declaring the local variables constructs the objects.
  
    set<string>      stl_set_of_words;  // The built-in set class - no constructor parameters.

    cs3505::wordset  our_set_of_words(1000);  // Our set class, with a hashtable of 1000 slots.
  
    // Open the file stream for reading.  (We'll be able to use it just like
    //   the keyboard stream 'cin'.)

    ifstream in("zarathustra.txt");

    // Loop for reading the file.  Note that it is controlled
    //   from within the loop (see the 'break').
    
    while (true)
    {
      // Read a word (don't worry about punctuation)
      
      string word;
      in >> word;

      // If the read failed, we're probably at end of file
      //   (or else the disk went bad).  Exit the loop.
      
      if (in.fail())
	break;

      // Word successfully read.  Add it to both sets.
      
      stl_set_of_words.insert(word);
      our_set_of_words.add(word);
    }

    // Close the file.

    in.close();

    // Print out the words found in the STL set.  I do this to show
    //   students how to get at the entries in an STL object.  For
    //   more examples, look up 'STL iterator examples'.
    // Notice that the iterator object 'it' acts like a pointer, but
    //   it is not a pointer.  (Bad, bad, bad design choice, but not mine!)
    // Feel free to comment this out.
  
    for (set<string>::iterator it = stl_set_of_words.begin(); it != stl_set_of_words.end(); it++)
    {
      string s = *it;
      cout << s << endl;
    }

    // Print out the number of words found in each set.

    cout << endl;
    cout << "STL set contains " << stl_set_of_words.size() << " unique words.\n";
    cout << "Our set contains " << our_set_of_words.size() << " unique words.\n";
    cout << endl;

    // Done.  Notice that this code block ends below.  Any local
    // variables declared within this block will be automatically
    // destroyed.  Local objects will have their destructors
    // called.  (Blocks are great for controlling scope/lifetime.)

  }




  // PART 3 TESTS //

  /*
   * Test 1: Proper set count (and by extension, adding words to the set)
   */
  {
    cs3505::wordset word_set(1000);
    
    word_set.add("A");
    word_set.add("B");
    word_set.add("C");
    word_set.add("D");
    word_set.add("E");
    
    if(word_set.size() == 5)
      { 

      }
    else
      {
	cout << "Test 1 failed." << endl;
	passed = false;
      }
  }

  /*
   * Test 2: Adding redundant elements; should not add an element that already exists in the wordset to the wordset. 
   */
  {
    cs3505::wordset word_set(1000);
    
    word_set.add("A");
    word_set.add("B");
    word_set.add("C");
    word_set.add("D");
    word_set.add("A");

    if(word_set.size() == 4)
      { 

      }
    else
      {
	cout << "Test 2 failed." << endl;
	passed = false;
      }
  }


  /*
   * Test 3: Contains method works?
   */
  {
    cs3505::wordset word_set(1000);
    
    word_set.add("A");
    word_set.add("B");
    word_set.add("C");
    word_set.add("D");
    word_set.add("E");
    
    if((word_set.contains("A") == true) && (word_set.contains("Z") == false))
      { 

      }
    else
      {
	cout << "Test 3 failed." << endl;
	passed = false;
      }

   
  }

  /*
   * Test 4: Proper set count & remove method
   */
  {
    cs3505::wordset word_set(1000);
    
    word_set.add("A");
    word_set.add("B");
    word_set.add("C");
    word_set.add("D");
    word_set.add("E");
    
    word_set.remove("A");
    
    if((word_set.contains("A") == false) && (word_set.contains("B") == true) && (word_set.contains("C") == true) && (word_set.contains("D") == true)&& (word_set.contains("E") == true) && (word_set.size() == 4))
      { 

      }
    else
      {
	cout << "Test 4 failed." << endl;
	passed = false;
      }
  }


  /*
   * Test 5: Attempting to remove an element that does not exist in the set
   */
  {
    cs3505::wordset word_set(1000);
    
    word_set.add("A");
    word_set.add("B");
    word_set.add("C");
    word_set.add("D");
    word_set.add("E");
    
    word_set.remove("Z");

    if((word_set.contains("A") == true) && (word_set.contains("B") == true) && (word_set.contains("C") == true) && (word_set.contains("D") == true)&& (word_set.contains("E") == true) && (word_set.size() == 5))
      { 
      }
    else
      {
	cout << "Test 5 failed." << endl;
	passed = false;
      }
    
  }

  /*
   * Test 6: Empty set testing 
   */
  {
    cs3505::wordset word_set(1000);

    if(word_set.size() == 0)
      { 

      }
    else
      {
	cout << "Test 6 failed." << endl;
	passed = false;
      }
    
  }


  /*
   * Test 7: Empty set testing - removing from empty set
   */
  {
    cs3505::wordset word_set(1000);

    word_set.remove("A");
    if((word_set.size() == 0) && (word_set.contains("A") != true))
      { 

      }
    else
      {
	cout << "Test 7 failed." << endl;
	passed = false;
      }
    
  }


  /*
   * Test 8: Empty set testing - getelements should be empty, too
   */
  {
    cs3505::wordset word_set(1000);

    vector<string> vectorized = word_set.get_elements();
    if(vectorized.capacity() == 0)
      { 

      }
    else
      {
	cout << "Test 8 failed." << endl;
	passed = false;
      }
  }


  /*
   * Test 9: Empty set testing - get_elements should be empty, too
   */
  {
    cs3505::wordset word_set(1000);

    vector<string> vectorized = word_set.get_elements();
    if(vectorized.capacity() == 0)
      { 

      }
    else
      {
	cout << "Test 9 failed." << endl;
	passed = false;
      }
  }

  /*
   * Test 10: Get_elements testing - size
   */
  {
    cs3505::wordset word_set(5);
    word_set.add("A");
    word_set.add("B");
    word_set.add("C");
    word_set.add("D");
    word_set.add("E");

    vector<string> vectorized = word_set.get_elements();
    if(vectorized.size() == 5)
      { 

      }
    else
      {
	cout << "Test 10 failed." << endl;
	passed = false;
      }
  }

  /*
   * Test 11: Get_elements testing - contents
   */
  {
    cs3505::wordset word_set(5);
    word_set.add("A");
    word_set.add("B");
    word_set.add("C");
    word_set.add("D");
    word_set.add("E");

    vector<string> vectorized = word_set.get_elements();

    //make sure contents of the string vector is in order of original wordset
    if((vectorized[0] == "A") && (vectorized[1] == "B") && (vectorized[2] == "C") && (vectorized[3] == "D") && (vectorized[4] == "E"))
      { 

      }
    else
      {
	cout << "Test 11 failed." << endl;
	passed = false;
      }

  }
  /*
   * Test 12: Copy Constructor
   */
  {
    cs3505::wordset word_set(1000);

    ifstream in("kanye.txt");


    while (true)
    {
      // Read a word (don't worry about punctuation)
      
      string word;
      in >> word;

      // If the read failed, we're probably at end of file
      //   (or else the disk went bad).  Exit the loop.
      
      if (in.fail())
	break;

      // Word successfully read.  Add it to both sets.
      
      word_set.add(word);
    }

    in.close();
    cs3505::wordset wordset_copy(word_set);

    if(wordset_copy.size() == word_set.size())
      { 

      }
    else
      {
	cout << "Test 12 failed." << endl;
	passed = false;
      }
  }

  /*
   * Test 13: Copy Constructor Contents
   */
  {
    cs3505::wordset word_set(1000);

    ifstream in("kanye.txt");


    while (true)
    {
      // Read a word (don't worry about punctuation)
      
      string word;
      in >> word;

      // If the read failed, we're probably at end of file
      //   (or else the disk went bad).  Exit the loop.
      
      if (in.fail())
	break;

      // Word successfully read.  Add it to both sets.
      
      word_set.add(word);
    }

    in.close();

    cs3505::wordset wordset_copy(word_set);
    vector<string> iterablecopy = wordset_copy.get_elements();

    // make sure the words in the copy can all be found in the original word set
    for(int i = 0; i < wordset_copy.size();i++)
      {

	string word = iterablecopy[i];
        if(word_set.contains(word) == true)
	  { 

	  }
	else
	  {

	    cout << "Test 13 failed."  << endl;
	    passed = false;
	    break;
	    }
      }
    
  }


  /*
   * Test 14: Assignment Operator
   */
  {
    cs3505::wordset word_set(1000);
    
    word_set.add("A");
    word_set.add("B");
    word_set.add("C");
    word_set.add("D");
    word_set.add("E");
    
    cs3505::wordset word_set_dup(1000);
    word_set_dup = word_set;

    vector<string> vectorized = word_set_dup.get_elements();

    //make sure contents of the string vector is in order of original wordset
    if((vectorized[0] == "A") && (vectorized[1] == "B") && (vectorized[2] == "C") && (vectorized[3] == "D") && (vectorized[4] == "E"))
      { 

      }
    else
      {
	cout << "Test 14 failed." << endl;
	passed = false;
      }
    
  }

  // END PART 3 TESTS //





  // Now that the objects have been destroyed, I will simply call my auditing
  // code to print out how many times constructors have been called, and
  // how many times destructors have been called.  They should exactly match.
  // If not, we have a memory problem.

  cout << "Class cs3505::wordset:" << endl;
  cout << "    Objects created:  " << cs3505::wordset::constructor_count() << endl;
  cout << "    Objects deleted:  " << cs3505::wordset::destructor_count() << endl;
  cout << endl;

  cout << "Class cs3505::node:" << endl;
  cout << "    Objects created:  " << cs3505::node::constructor_count() << endl;
  cout << "    Objects deleted:  " << cs3505::node::destructor_count() << endl;
  cout << endl;

  if(passed == false)
    cout << "Tests failed" << endl;
  else
    cout << "Tests passed" << endl;

  // Now we're really done.  End main.
  
  return 0;
}
