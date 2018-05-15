#ifndef __VOLTAGETRACKERCONTROLLER_H_INCLUDED__
#define __VOLTAGETRACKERCONTROLLER_H_INCLUDED__

#include <Arduino.h>
#include <stdint.h>

#include "TrackedInt.hpp"

enum class VTInputMode {
  DigitalInput,
  ExternalInput
};

class VoltageTrackerController {

public:

  /*******************************
   * Constructors
   *******************************/
  VoltageTrackerController(uint8_t controlPin, uint8_t feedbackPin, uint8_t externalSourcePin, bool mUseFeedback);

  /*******************************
   * Accessors
   *******************************/
  // Returns the target voltage for the Voltage Tracker
  int getSetPoint() const;
  // Returns the current voltage in millivolts
  int getCurrentVoltage() const;
  // Returns the Arduino Pin number that this controller is changing
  int getControlPin() const;
  // Returns the Arduino Pin number for getting feedback
  int getFeedbackPin() const;
  // Returns the raw feedback A/D output value
  int getRawFeedbackValue() const;
  // Returns the Arduino Pin number for getting the set point from
  int getExternalSourcePin() const;
  // Returns the raw set point A/D output value (if external)
  int getRawExternalSetPointValue() const;  
  // Returns the additional duty cycle count applied as a result of compensating for error
  TrackedInt getErrorDutyCycle() const;
  // Returns the current duty cycle / 255
  int getDutyCycle() const;
  // The input mode for the controller
  VTInputMode getInputMode() const;

  /*******************************
   * Actions
   *******************************/
  /** 
   * Sets a new target voltage - returns the new target voltage if set 
   * successfully, otherwise an error code as follows:
   * -1 : Invalid new target voltage
   * -2 : New target voltage same as existing, doing nothing
   */
  int setSetPoint(int newSetPoint);
  // Updates the current voltage (processes feedback)
  void update();
  // Determines the input mode
  void setInputMode(VTInputMode mode);

private:
  /*******************************
   * Member variables
   *******************************/
  // The Set Point Voltage in millivolts we're aiming for
  int mSetPoint;
  // The current voltage in millivolts
  int mCurrentVoltage;
  // The Arduino output pin this controller is changing 
  uint8_t mControlPin;
  // The Arduino analogue input pin this controller is using for feedback 
  uint8_t mFeedbackPin;
  // The calculated duty cycle for the required output voltage 
  int mBaseDutyCycle;
  // The additional duty cycle applied based on feedback
  TrackedInt mErrorDutyCycle;
  // The input mode (menu driven, or external signal driven)
  VTInputMode mInputMode;
  // The Arduino analogue input pin this controller is using to determine the set point
  uint8_t mExternalSourcePin;
  // If true then feedback is used to correct for errors
  bool mUseFeedback;
  // The raw A/D output from the feedback
  int mFeedbackVoltageADValue;
  // The raw A/D output from the set point source (if external)
  int mSetPointVoltageADValue;

  /*******************************
   * Actions
   *******************************/
  // Determine current set point from an external source
  int readExternalSetPoint();
  // Internal version to allow force set
  int setSetPoint(int newSetPoint, bool overrideExisting);
};

#endif // __VOLTAGETRACKERCONTROLLER_H_INCLUDED__
