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
  Serial.print("  Please choose a menu item by number.\n\n");
  if (mVoltageTrackerController->getInputMode() == VTInputMode::DigitalInput) {
    Serial.println("\t[1] Define new Set Point (SP)");
  } else {
    Serial.println("\t[1] Define new Set Point (SP) (changes back to digital input)");
  }
  Serial.println("\t[2] Choose Set Point origin (Pot/PC)");
  if (mVoltageTrackerController->getUseFeedback()) {
    Serial.print("\t[3] Stop using feedback in output regulation\n\n");
  } else {
    Serial.print("\t[3] Start using feedback in output regulation\n\n");
  }
  Serial.print("  Choice (number followed by enter): ");
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
  } else if (userInput.equals("3")) {
    mVoltageTrackerController->setUseFeedback(!mVoltageTrackerController->getUseFeedback());
  } else {
    displayError("Please pick either 1, 2 or 3. You entered " + String(userInput));
  }
  return returnMenu;
}