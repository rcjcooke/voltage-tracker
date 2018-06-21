#ifndef __SINGLEPAGEINPUTMENU_H_INCLUDED__
#define __SINGLEPAGEINPUTMENU_H_INCLUDED__

#include "SerialDisplayMenu.hpp"

class SinglePageInputMenu : public SerialDisplayMenu {

public:
  /*******************************
   * Constructors
   *******************************/
  SinglePageInputMenu();
  
  /*******************************
   * Actions
   *******************************/
  // Processes user input
  SerialDisplayMenu* processUserInput(String userInput);

protected:
  /*******************************
   * Actions
   *******************************/
  // This method is called on every loop while the menu is waiting for user input. It is here in case 
  // the system needs to keep something up to date while waiting.
  void controllerUpdate();
  // Puts together the one line status update string to show users
  String constructStatusLine();
  // Prints the menu out to the serial interface
  void printMenu();

private:

};

#endif // __SINGLEPAGEINPUTMENU_H_INCLUDED__