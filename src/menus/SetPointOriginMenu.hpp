#ifndef __SETPOINTORIGINMENU_H_INCLUDED__
#define __SETPOINTORIGINMENU_H_INCLUDED__

#include "SerialDisplayMenu.hpp"
#include "../controller/VoltageTrackerController.hpp"

class SetPointOriginMenu : public SerialDisplayMenu {

public:
  /*******************************
   * Constructors
   *******************************/
  SetPointOriginMenu(SerialDisplayMenuConfiguration* configuration, SerialDisplayMenu* origin, VoltageTrackerController* vtController);

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
  // The controller for the voltage tracker this menu controls
  VoltageTrackerController* mVoltageTrackerController;
  
};

#endif // __SETPOINTORIGINMENU_H_INCLUDED__