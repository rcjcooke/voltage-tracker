#ifndef __SETPOINTMENU_H_INCLUDED__
#define __SETPOINTMENU_H_INCLUDED__

#include "VTSDM.hpp"

class SetPointMenu : public VTSDM {

public:
  /*******************************
   * Constructors
   *******************************/
  SetPointMenu(SerialDisplayMenuConfiguration* configuration, SerialDisplayMenu* origin, VoltageTrackerController* vtController);
  
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

#endif // __SETPOINTMENU_H_INCLUDED__