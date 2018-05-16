#include "MainMenu.hpp"
#include "SetPointMenu.hpp"
#include "SetPointOriginMenu.hpp"

/*******************************
 * Constructors
 *******************************/

MainMenu::MainMenu(SerialDisplayMenuConfiguration* configuration, VoltageTrackerController* vtController) : VTSDM(configuration, vtController, nullptr, (int8_t) 7, (int8_t) 8) {
}

/*******************************
 * Actions
 *******************************/

void MainMenu::printMenu() {
  Serial << "  Please choose a menu item by number." << endl << endl;
  if (mVoltageTrackerController->getInputMode() == VTInputMode::DigitalInput) {
    Serial << "\t[1] Define new Set Point (SP)" << endl;
  } else {
    Serial << "\t[1] Define new Set Point (SP) (changes back to digital input)" << endl;
  }
  Serial << "\t[2] Choose Set Point origin (Pot/PC)" << endl << endl;
  Serial << "  Choice (number followed by enter): ";
}

SerialDisplayMenu* MainMenu::processUserInput(String userInput) {
  
  SerialDisplayMenu* returnMenu = this;
  if (userInput.equals("1")) {
    if (mVoltageTrackerController->getInputMode() == VTInputMode::ExternalInput) {
      mVoltageTrackerController->setInputMode(VTInputMode::DigitalInput);
    }
    returnMenu = new SetPointMenu(getSerialDisplayMenuConfiguration(), this, mVoltageTrackerController);
  } else if (userInput.equals("2")) {
    returnMenu = new SetPointOriginMenu(getSerialDisplayMenuConfiguration(), this, mVoltageTrackerController);
  } else {
    displayError("Please pick either 1 or 2, you entered " + String(userInput));
  }
  return returnMenu;
}