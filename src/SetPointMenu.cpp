#include "SetPointMenu.hpp"

/*******************************
 * Constructors
 *******************************/

SetPointMenu::SetPointMenu(SerialDisplayMenuConfiguration* configuration, SerialDisplayMenu* origin, VoltageTrackerController* vtController) : SerialDisplayMenu(configuration, origin, (int8_t) 2, (int8_t) 4) {
  mVoltageTrackerController = vtController;
}

/*******************************
 * Actions
 *******************************/

void SetPointMenu::printMenu() {
  Serial << "  Please type in the new voltage in millivolts, e.g. 1000 for 1 Volt: ";
}

SerialDisplayMenu* SetPointMenu::processUserInput(long userInput) {
  if ((int) userInput < 0 || (int) userInput > 5000) {
    displayError("Only whole numbers between 0 and 5000 are accepted, rec: " + String(userInput));
    return this;
  }
  int success = mVoltageTrackerController->setSetPoint((int) userInput);
  switch (success) {
    case -1:
      displayError("Invalid target voltage, please try again.");
      return this;
    case -2:
      displayError("Target voltage is already set to " + String((int) userInput));
      break;
    default:
      break;
  }
  return mOrigin;
}