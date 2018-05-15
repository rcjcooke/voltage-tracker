#include "SerialDisplayMenu.hpp"

/*******************************
 * Constructors
 *******************************/

SerialDisplayMenu::SerialDisplayMenu(SerialDisplayMenuConfiguration* configuration, SerialDisplayMenu* origin, int8_t statusLineNumber, int8_t errorLineNumber) {
  mOrigin = origin;
  mConfigurationPtr = configuration;
  mStatusLineNumber = statusLineNumber;
  mErrorLineNumber = errorLineNumber;
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
      Serial << _BYTE(27) << "[2J";
      Serial << _BYTE(27) << "[H";
      break;
    default:
      break;
  }
}

void SerialDisplayMenu::display() {
  clearSerialDisplay();
  printMenu();

  if (mCurrentError != NULL) displayError(mCurrentError);
  
}

void SerialDisplayMenu::displayError(String errorMessage) {
  
  mCurrentError = errorMessage;

  if (mConfigurationPtr->getSerialDisplayType() == SerialDisplayType::ansi_vt100) {
    // Save Cursor location
    Serial << _BYTE(27) << "7";
    // Move the cursor to the correct location
    Serial << _BYTE(27) << "[H";
    Serial << _BYTE(27) << "[" << _DEC(getErrorLineNumber()) << "B";
  }

  if (mConfigurationPtr->getSerialDisplayType() == SerialDisplayType::ansi_vt100) {
    // Set characters to display in red
    Serial << _BYTE(27) << "[31m";
  }

  Serial << "  " << errorMessage << endl;

  // TODO: Sort out error printing for a duration - instantly disappears at the moment because the screen is cleared
  
  if (mConfigurationPtr->getSerialDisplayType() == SerialDisplayType::ansi_vt100) {
    // Set characters back to default
    Serial << _BYTE(27) << "[0m";
    // Restore Cursor location
    Serial << _BYTE(27) << "8";
  }
}

void SerialDisplayMenu::updateStatusLine(String statusLine) {

  if (mConfigurationPtr->getSerialDisplayType() == SerialDisplayType::ansi_vt100) {
    // Save Cursor location
    Serial << _BYTE(27) << "7";
  }
  
  if (mConfigurationPtr->getSerialDisplayType() == SerialDisplayType::ansi_vt100) {
    // Move the cursor to the correct location
    Serial << _BYTE(27) << "[H";
    Serial << _BYTE(27) << "[" << _DEC(getStatusLineNumber()) << "B";
  }

  Serial.print(statusLine);

  if (mConfigurationPtr->getSerialDisplayType() == SerialDisplayType::ansi_vt100) {
    // Restore Cursor location
    Serial << _BYTE(27) << "8";
  }
}