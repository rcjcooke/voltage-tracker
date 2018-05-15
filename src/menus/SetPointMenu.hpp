#ifndef __SETPOINTMENU_H_INCLUDED__
#define __SETPOINTMENU_H_INCLUDED__

#include "SerialDisplayMenu.hpp"
#include "../controller/VoltageTrackerController.hpp"

class SetPointMenu : public SerialDisplayMenu {

public:
  /*******************************
   * Constructors
   *******************************/
  SetPointMenu(SerialDisplayMenuConfiguration* configuration, SerialDisplayMenu* origin, VoltageTrackerController* vtController);
  
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
  // Pointer to the controller for changing the set point
  VoltageTrackerController* mVoltageTrackerController;

};

#endif // __SETPOINTMENU_H_INCLUDED__