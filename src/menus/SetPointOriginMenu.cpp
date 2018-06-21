#include "SetPointOriginMenu.hpp"
#include "SetPointMenu.hpp"

/*******************************
 * Constructors
 *******************************/

SetPointOriginMenu::SetPointOriginMenu(SerialDisplayMenuConfiguration* configuration, SerialDisplayMenu* origin, VoltageTrackerController* vtController) 
  : VTSDM(configuration, vtController, origin, (int8_t) 7, (int8_t) 8) {}

/*******************************
 * Actions
 *******************************/

void SetPointOriginMenu::printMenu() {
  Serial.print("  Please choose target voltage origin by number.\n\n");
  Serial.print("\t[1] Set digitally from terminal menu\n");
  Serial.print("\t[2] Set using Potentiometer\n\n");
  Serial.print("  Choice (number followed by enter): ");
}

SerialDisplayMenu* SetPointOriginMenu::processUserInput(String userInput) {
  if (userInput.equals("1")) {
    mVoltageTrackerController->setInputMode(VTInputMode::DigitalInput);
  } else if (userInput.equals("2")) {
    mVoltageTrackerController->setInputMode(VTInputMode::ExternalInput);
  } else {
    displayError("Please pick either 1 or 2, you entered " + userInput);
    return this;
  }
  return mOrigin;
}