/* 
 * CS3505 - Assignment 4 - Resubmit
 * Skylar Shyu 
 * tester.cpp
 */

 /*
  * Where main() lives. 
  */

  #include "report.h"
  
  int main(int argc, char* argv[])
  {
      Report A_Report = Report(std::string(argv[1]));   // process the file inputted
      A_Report.printReport();   // generates and prints a report
  }