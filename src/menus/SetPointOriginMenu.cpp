#include "SetPointOriginMenu.hpp"
#include "SetPointMenu.hpp"

/*******************************
 * Constructors
 *******************************/

SetPointOriginMenu::SetPointOriginMenu(SerialDisplayMenuConfiguration* configuration, SerialDisplayMenu* origin, VoltageTrackerController* vtController) : SerialDisplayMenu(configuration, origin, (int8_t) 7, (int8_t) 8) {
  mVoltageTrackerController = vtController;
}

/*******************************
 * Actions
 *******************************/

void SetPointOriginMenu::printMenu() {
  Serial << "  Please choose target voltage origin by number." << endl << endl;
  Serial << "\t[1] Set digitally from terminal menu" << endl;
  Serial << "\t[2] Set using Potentiometer" << endl << endl;
  Serial << "  Choice (number followed by enter): ";
}

SerialDisplayMenu* SetPointOriginMenu::processUserInput(long userInput) {
  
  if (userInput == 1) {
    mVoltageTrackerController->setInputMode(VTInputMode::DigitalInput);
  } else if (userInput == 2) {
    mVoltageTrackerController->setInputMode(VTInputMode::ExternalInput);
  } else {
    displayError("Please pick either 1 or 2, you entered " + String(userInput));
    return this;
  }
  return mOrigin;
}