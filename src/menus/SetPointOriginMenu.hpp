#ifndef __SETPOINTORIGINMENU_H_INCLUDED__
#define __SETPOINTORIGINMENU_H_INCLUDED__

#include "VTSDM.hpp"

class SetPointOriginMenu : public VTSDM {

public:
  /*******************************
   * Constructors
   *******************************/
  SetPointOriginMenu(SerialDisplayMenuConfiguration* configuration, SerialDisplayMenu* origin, VoltageTrackerController* vtController);

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
  /*******************************
   * Member variables
   *******************************/
  // The controller for the voltage tracker this menu controls
  VoltageTrackerController* mVoltageTrackerController;
  
};

#endif // __SETPOINTORIGINMENU_H_INCLUDED__