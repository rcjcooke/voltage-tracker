#include "SinglePageInputMenu.hpp"

/*******************************
 * Constructors
 *******************************/

SinglePageInputMenu::SinglePageInputMenu() 
  : SerialDisplayMenu(new SerialDisplayMenuConfiguration(SerialDisplayType::ansi_vt100, false, 1000, 100000), nullptr, (int8_t) 2, (int8_t) 4) {
    Serial.begin(115200);
}

/*******************************
 * Actions
 *******************************/

void SinglePageInputMenu::controllerUpdate() {}
String SinglePageInputMenu::constructStatusLine() {
  return "No Status Line Defined";
}

void SinglePageInputMenu::printMenu() {
  Serial.print("  Please type in the new value: ");
}

SerialDisplayMenu* SinglePageInputMenu::processUserInput(String userInput) {
  // long newSetPoint = userInput.toInt();
  // if (newSetPoint < 0 || newSetPoint > 5000) {
  //   displayError("Only whole numbers between 0 and 5000 are accepted, rec: " + String(userInput));
  //   return this;
  // }
  return this;
}