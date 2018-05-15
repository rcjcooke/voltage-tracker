#ifndef __MAINMENU_H_INCLUDED__
#define __MAINMENU_H_INCLUDED__

#include "SerialDisplayMenu.hpp"
#include "../controller/VoltageTrackerController.hpp"

class MainMenu : public SerialDisplayMenu {

public:
  /*******************************
   * Constructors
   *******************************/
  MainMenu(SerialDisplayMenuConfiguration* configuration, VoltageTrackerController* vtController);

  /*******************************
   * Actions
   *******************************/
  // Processes user input
  SerialDisplayMenu* processUserInput(long userInput);

protected:
  /*******************************
   * Actions
   *******************************/
  
  // Prints the menu out to the serial interface
  void printMenu();

private:
  /*******************************
   * Member variables
   *******************************/
  // Pointer to the controller for passing to the set point menu
  VoltageTrackerController* mVoltageTrackerController;
  
};

#endif // __MAINMENU_H_INCLUDED__