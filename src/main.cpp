#include <Arduino.h>
#include <Streaming.h>
#include <stdlib.h>

#include "controller/VoltageTrackerController.hpp"
#include "menus/MainMenu.hpp"

/*******************************
 * Global variables definitions
 *******************************/
// Arduino Pin the voltage is being controlled on
// Note: http://www.righto.com/2009/07/secrets-of-arduino-pwm.html
static const uint8_t VOLTAGE_TRACKER_OUTPUT_PIN = 4;
// The Arduino analogue input pin this controller is using for feedback
static const uint8_t VOLTAGE_TRACKER_FEEDBACK_PIN = A0;
// The Arduino analogue input pin this controller is using to determine the set point
static const uint8_t VOLTAGE_TRACKER_SOURCE_PIN = A1;
// Shorthand included in status string when the set point source is digital
static const String DIGITAL_SOURCE_STRING = "Dig";
// Shorthand included in status string when the set point source is external potentiometer
static const String EXTERNAL_SOURCE_STRING = "Pot";

// Serial Display Configuration
SerialDisplayMenuConfiguration* mConfiguration = new SerialDisplayMenuConfiguration(SerialDisplayType::ansi_vt100, true);
// The Voltage Tracker Controller
VoltageTrackerController* mVoltageTrackerController;
// The menu we're currently displaying
SerialDisplayMenu* mCurrentMenu;

/*******************************
 * Processing functions
 *******************************/
// Create a padded string from an integer
String padString(int value, unsigned int paddedLength, const String padChar) {
  String retStr = String(value);
  while (retStr.length() < paddedLength) {
    retStr = padChar + retStr;
  }
  return retStr;
}

// Divides a by b and returns a rounded integer result
int roundedFloatDivide(float a, float b) {
  float c = a/b;
  // Sort out the rounding prior to truncation
  c = c < 0 ? c - 0.5 : c + 0.5;
  return c;
}

// Processes some raw input and updates the value
bool handleRawSerialInput(long &inputValue) {

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
    } else {
      // Cheat convert ASCII to decimal
      input = input - 48;
      inputValue = inputValue*10 + input;
    }
  }
  return terminated;
}

String constructStatusLine() {
  String sourceStr;
  switch (mVoltageTrackerController->getInputMode()) {
    case VTInputMode::DigitalInput:
      sourceStr = DIGITAL_SOURCE_STRING;
      break;
    case VTInputMode::ExternalInput:
      sourceStr = EXTERNAL_SOURCE_STRING;
      break;
  }
  int sp = mVoltageTrackerController->getSetPoint();
  int cv = mVoltageTrackerController->getCurrentVoltage();
  int pv = sp == 0 ? 0 : abs(roundedFloatDivide((sp-cv)*100, sp));
  int dc = mVoltageTrackerController->getDutyCycle();
  int rf = mVoltageTrackerController->getRawFeedbackValue();
  int rs = mVoltageTrackerController->getRawExternalSetPointValue();
  //TrackedInt edc = mVoltageTrackerController->getErrorDutyCycle();
  String spStr = padString(sp, 4, "0");
  String cvStr = padString(cv, 4, "0");
  String pvStr = padString(pv, 3, "0");
  String dcStr = padString(dc, 3, "0");
  String rfStr = padString(rf, 4, "0");
  String rsStr = mVoltageTrackerController->getInputMode() == VTInputMode::DigitalInput ? " -  " : padString(rs, 4, "0");
  
  // String edcMaxStr = padString(edc.getMax(), 3, " ");
  // String edcStr = padString(edc.getValue(), 3, " ");
  // String edcMinStr = padString(edc.getMin(), 3, " ");

  // String statusLine = "  SP (" + sourceStr  + "): " + spStr + "mV\tV: " + cvStr + "mV (" + pvStr + "% err EDC: " + edcMinStr + "/" + edcStr + "/" + edcMaxStr + ")";
  String statusLine = "  SP (" + sourceStr  + "): " + spStr + "mV (s:" + rsStr + "/f:" + rfStr + ")\tV: " + cvStr + "mV (" + pvStr + "% err DC: " + dcStr + ")";
  return statusLine;
}

// Get user input while letting the menu take care of display updates etc.
long inputLoop() {
  long inputValue=0;
  bool terminated = false;

  while (!terminated) {

    // Update display while waiting for user input
    uint16_t i=0;
    while (Serial.available()==0) {
      // Update the current Voltage (updates from external source if required, gets feedback and adjusts for error)
      mVoltageTrackerController->update();

      // Update the status every now and then
      if (i%500 == 0) {
        // Update the user
        String statusLine = constructStatusLine();
        mCurrentMenu->updateStatusLine(statusLine);
      }
      i++;
    }

    // Process the new buffer content and update the inputValue with it
    terminated = handleRawSerialInput(inputValue);

  }
  return inputValue;
}

/*******************************
 * Setup functions
 *******************************/
void setup() {
  // Initialise variables 
  mVoltageTrackerController  = new VoltageTrackerController(VOLTAGE_TRACKER_OUTPUT_PIN, VOLTAGE_TRACKER_FEEDBACK_PIN, VOLTAGE_TRACKER_SOURCE_PIN, true);
  mCurrentMenu = new MainMenu(mConfiguration, mVoltageTrackerController);
  
  // Serial interface setup
  Serial.begin(115200);
}

/*******************************
 * Main loop
 *******************************/
void loop() {
  // Show the current menu to the user
  mCurrentMenu->display();

  // Get user input while letting the menu take care of display updates etc.
  long inputValue = inputLoop();

  // Process the fully formed user input
  mCurrentMenu = mCurrentMenu->processUserInput(inputValue);
}