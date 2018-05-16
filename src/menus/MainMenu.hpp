#ifndef __MAINMENU_H_INCLUDED__
#define __MAINMENU_H_INCLUDED__

#include "VTSDM.hpp"

class MainMenu : public VTSDM {

public:
  /*******************************
   * Constructors
   *******************************/
  MainMenu(SerialDisplayMenuConfiguration* configuration, VoltageTrackerController* vtController);

  /*******************************
   * Actions
   *******************************/
  // Processes user input
  SerialDisplayMenu* processUserInput(String userInput);

protected:
  /*******************************
   * Actions
   *******************************/
  
  // Prints the menu out to the serial interface
  void printMenu();

private:
};

#endif // __MAINMENU_H_INCLUDED__