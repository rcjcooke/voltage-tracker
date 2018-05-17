#include <Arduino.h>

#include "VTSDM.hpp"

// Shorthand included in status string when the set point source is digital
static const String DIGITAL_SOURCE_STRING = "Dig";
// Shorthand included in status string when the set point source is external potentiometer
static const String EXTERNAL_SOURCE_STRING = "Pot";

static const String HEARTBEAT_ARRAY[] = {"|","/","-","\\"};

/*******************************
 * Constructors
 *******************************/

VTSDM::VTSDM(SerialDisplayMenuConfiguration* configuration, VoltageTrackerController* vtController, SerialDisplayMenu* origin, int8_t statusLineNumber, int8_t errorLineNumber) : SerialDisplayMenu(configuration, origin, (int8_t) 8, (int8_t) 9) {
  mVoltageTrackerController = vtController;
  mHrtBeatCharNum=0;
}

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

/*******************************
 * Actions
 *******************************/
void VTSDM::controllerUpdate() {
  // Update the current Voltage (updates from external source if required, gets feedback and adjusts for error)
  mVoltageTrackerController->update();
}

// static helper function
String VTSDM::vtInputMode2Str(VTInputMode mode) {
  String sourceStr;
  switch (mode) {
    case VTInputMode::DigitalInput:
      sourceStr = DIGITAL_SOURCE_STRING;
      break;
    case VTInputMode::ExternalInput:
      sourceStr = EXTERNAL_SOURCE_STRING;
      break;
  }
  return sourceStr;
}

String VTSDM::constructStatusLine() {
  String sourceStr = VTSDM::vtInputMode2Str(mVoltageTrackerController->getInputMode());
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
  mHrtBeatCharNum++;
  if (mHrtBeatCharNum==4) mHrtBeatCharNum=0;  
  String statusLine = HEARTBEAT_ARRAY[mHrtBeatCharNum] + " SP (" + sourceStr  + "): " + spStr + "mV (s:" + rsStr + "/f:" + rfStr + ")\tV: " + cvStr + "mV (" + pvStr + "% err DC: " + dcStr + ")";
  return statusLine;
}