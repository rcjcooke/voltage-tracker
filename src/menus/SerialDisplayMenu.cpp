#include "SerialDisplayMenu.hpp"

/*******************************
 * Constructors
 *******************************/

SerialDisplayMenu::SerialDisplayMenu(SerialDisplayMenuConfiguration* configuration, SerialDisplayMenu* origin, int8_t statusLineNumber, int8_t errorLineNumber) {
  mOrigin = origin;
  mConfigurationPtr = configuration;
  mStatusLineNumber = statusLineNumber;
  mErrorLineNumber = errorLineNumber;
  mErrorDisplayed = false;
  mErrorCleared = false;
  mShowError = false;
}

SerialDisplayMenu::SerialDisplayMenu(SerialDisplayMenuConfiguration* configuration, int8_t statusLineNumber, int8_t errorLineNumber) 
  : SerialDisplayMenu(configuration, this, statusLineNumber, errorLineNumber) {}

/*******************************
 * Accessors
 *******************************/

int8_t SerialDisplayMenu::getStatusLineNumber() const {
  return mStatusLineNumber;
}

int8_t SerialDisplayMenu::getErrorLineNumber() const {
  return mErrorLineNumber;
}

SerialDisplayMenuConfiguration* SerialDisplayMenu::getSerialDisplayMenuConfiguration() const {
  return mConfigurationPtr;
}

/*******************************
 * Actions
 *******************************/
void SerialDisplayMenu::clearSerialDisplay() {
  switch(mConfigurationPtr->getSerialDisplayType()) {
    case SerialDisplayType::ansi_vt100:
      Serial.print("\e[2J");
      Serial.print("\e[H");
      break;
    default:
      break;
  }
}

void SerialDisplayMenu::display() {
  clearSerialDisplay();
  printMenu();
  showError();
}

void SerialDisplayMenu::showError() {
  if (mShowError) {
    if (millis() < mLastErrorTimeMillis + mConfigurationPtr->getErrorDisplayTimeMillis()) {
      if (!mErrorDisplayed) {
        displayError(mCurrentError);
        // We only want to write it to screen once
        mErrorDisplayed = true;
      }
    } else {
      if (!mErrorCleared) {
        displayError("                                                                                ");
        mErrorCleared = true;
      }
      mErrorDisplayed = false;
      mShowError = false;
    }
  }
}

void SerialDisplayMenu::displayError(String errorMessage) {
  
  mCurrentError = errorMessage;
  mLastErrorTimeMillis = millis();
  mShowError = true;
  mErrorCleared = false;

  if (mConfigurationPtr->getSerialDisplayType() == SerialDisplayType::ansi_vt100) {
    // Save Cursor location
    Serial.print("\e7");
    // Move the cursor to the correct location
    Serial.print("\e[H");
    Serial.print("\e[" + String(getErrorLineNumber()) + "B");
  }

  if (mConfigurationPtr->getSerialDisplayType() == SerialDisplayType::ansi_vt100) {
    // Set characters to display in red
    Serial.print("\e[31m");
  }

  Serial.println("  " + errorMessage);

  if (mConfigurationPtr->getSerialDisplayType() == SerialDisplayType::ansi_vt100) {
    // Set characters back to default
    Serial.print("\e[0m");
    // Restore Cursor location
    Serial.print("\e8");
  }
}

void SerialDisplayMenu::updateStatusLine(String statusLine) {

  if (mConfigurationPtr->getSerialDisplayType() == SerialDisplayType::ansi_vt100) {
    // Save Cursor location
    Serial.print("\e7");
  }
  
  if (mConfigurationPtr->getSerialDisplayType() == SerialDisplayType::ansi_vt100) {
    // Move the cursor to the correct location
    Serial.print("\e[H");
    Serial.print("\e[" + String(getStatusLineNumber()) + "B");
  }

  Serial.print(statusLine);

  if (mConfigurationPtr->getSerialDisplayType() == SerialDisplayType::ansi_vt100) {
    // Restore Cursor location
    Serial.print("\e8");
  }
}

// Processes some raw input and updates the value
bool handleRawSerialInput(String &inputValue) {

  bool terminated = false;
  // Process input - waiting for the terminator (enter key)
  while (!terminated && Serial.available() > 0) {
    int input=0;
    input = Serial.read();
    
    // Correct for terminals that pad out 7-bit ASCII to 8 bits with an extra high bit 
    // (like Putty - pretty sure it's because it's translating but I don't care at this point!) 
    if (input > 127) {
      input = input - 128;
    }

    if (input == '\r') {
      terminated = true;
    } else if (input == '\b') {
      inputValue.remove(inputValue.length()-1, 1);
    } else {
      inputValue.concat((char) input);
    }
  }
  return terminated;
}

// Get user input while letting the menu take care of display updates etc.
String SerialDisplayMenu::getUserInputWhileKeepingStatusUpdated() {
  String inputValue="";
  bool terminated = false;

  while (!terminated) {

    // Update display while waiting for user input
    uint16_t i=0;
    while (Serial.available()==0) {
      // Update the current Voltage (updates from external source if required, gets feedback and adjusts for error)
      controllerUpdate();

      // Update the status every now and then
      if (i % (mConfigurationPtr->getUserStatusUpdateFrequencyModulus()) == 0) {
        if (mConfigurationPtr->getDisplayStatusLine()) {
          // Update the user
          String statusLine = constructStatusLine();
          updateStatusLine(statusLine);
        }
        showError();
      }
      i++;
    }

    // Process the new buffer content and update the inputValue with it
    terminated = handleRawSerialInput(inputValue);

  }
  return inputValue;
}

// Note: This is a static method as defined in the class header
void SerialDisplayMenu::userInputLoop(SerialDisplayMenu* startingMenu) {
  SerialDisplayMenu* currentMenu = startingMenu;
  while(true) {
    // Show the current menu to the user
    currentMenu->display();

    // Get user input while letting the menu take care of display updates etc.
    String inputValue = currentMenu->getUserInputWhileKeepingStatusUpdated();

    // Process the fully formed user input
    SerialDisplayMenu* newMenu = currentMenu->processUserInput(inputValue);
    // TODO: SORT OUT CLEANING UP USED MENUS - the below doesn't work properly
    //if (newMenu != currentMenu) delete currentMenu;
    currentMenu = newMenu;
  }
}