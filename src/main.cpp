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

// Serial Display Configuration
SerialDisplayMenuConfiguration* mConfiguration = new SerialDisplayMenuConfiguration(SerialDisplayType::ansi_vt100, true, 500);
// The Voltage Tracker Controller
VoltageTrackerController* mVoltageTrackerController;
// The menu we're currently displaying
MainMenu* mMainMenu;

/*******************************
 * Setup functions
 *******************************/
void setup() {
  // Initialise variables 
  mVoltageTrackerController  = new VoltageTrackerController(VOLTAGE_TRACKER_OUTPUT_PIN, VOLTAGE_TRACKER_FEEDBACK_PIN, VOLTAGE_TRACKER_SOURCE_PIN, true);
  mMainMenu = new MainMenu(mConfiguration, mVoltageTrackerController);
  
  // Serial interface setup
  Serial.begin(115200);
}

/*******************************
 * Main loop
 *******************************/
void loop() {
  // The menu runs the show - this will never exit
  SerialDisplayMenu::userInputLoop(mMainMenu);
}