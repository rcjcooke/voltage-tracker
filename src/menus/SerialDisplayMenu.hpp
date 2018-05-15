#ifndef __SERIALDISPLAYMENU_H_INCLUDED__
#define __SERIALDISPLAYMENU_H_INCLUDED__

#include <Arduino.h>
#include <stdint.h>
#include <stdarg.h>
#include <Streaming.h>

#include "SerialDisplayMenuConfiguration.hpp"

// Abstract class to be extended by all Menu implementations
class SerialDisplayMenu {

public:
  /*******************************
   * Constructors
   *******************************/
  SerialDisplayMenu(SerialDisplayMenuConfiguration* configuration, int8_t statusLineNumber, int8_t errorLineNumber);
  SerialDisplayMenu(SerialDisplayMenuConfiguration* configuration, SerialDisplayMenu* origin, int8_t statusLineNumber, int8_t errorLineNumber);

  /*******************************
   * Accessors
   *******************************/
  // Return the line number that holds the status line on this menu
  int8_t getStatusLineNumber() const;
  // Return the line number that holds the error line on this menu
  int8_t getErrorLineNumber() const;
  // Return the display configuration object
  SerialDisplayMenuConfiguration* getSerialDisplayMenuConfiguration() const;

  /*******************************
   * Actions
   *******************************/
  // Clears down the menu
  void clearSerialDisplay();
  // Prints the menu to the display over the serial connection
  void display();
  // Displays an error message to the user - additional arguments will be appended to the error message
  void displayError(String error);
  // Update the status line
  void updateStatusLine(String statusLine);
  // Acts on user input - returns the menu the user should see next
  virtual SerialDisplayMenu* processUserInput(long userInput) = 0;

protected:
  /*******************************
   * Member variables
   *******************************/
  // Pointer to the menu that this came from - for handing back control
  SerialDisplayMenu* mOrigin;

  /*******************************
   * Actions
   *******************************/
  // Must be implemented by concrete classes - this prints the menu over the serial connection
  virtual void printMenu() = 0;

private:
  /*******************************
   * Member variables
   *******************************/
  // The display line number on which to print the status
  int8_t mStatusLineNumber;
  // The display line number on which to print any errors
  int8_t mErrorLineNumber;
  // The current error to display to the user
  String mCurrentError;
  // The display and menu configuration
  SerialDisplayMenuConfiguration* mConfigurationPtr;

};

#endif // __SERIALDISPLAYMENU_H_INCLUDED__
