#include "VoltageTrackerController.hpp"

// The reference ground voltage
static const float MIN_V = 0.0;
// The arduino supply voltage (and therefore PWM output voltage) in millivolts
static const float MAX_V = 4954.0;
// Pre-calculate the number of duty cycle points per millivolt
// TODO: TEST THIS!! beware of floating point math errors
static const float CYCLES_PER_MILLIVOLT = 256.0/(MAX_V - MIN_V);
// Pre-calculate conversion factor for analogue readings to millivolts
// TODO: TEST THIS!! beware of floating point math errors
static const float AD_POINT_MILLIVOLT_RATIO = (MAX_V / 1023.0);
// The error tolerance threshold in mV 
static const int TOLERANCE = 20;

/**
 * Divides a given PWM pin frequency by a divisor.
 *
 * The resulting frequency is equal to the base frequency divided by
 * the given divisor:
 *   - Base frequencies:
 *      o The base frequency for pins 3, 9, 10, and 11 is 31250 Hz.
 *      o The base frequency for pins 5 and 6 is 62500 Hz.
 *   - Divisors:
 *      o The divisors available on pins 5, 6, 9 and 10 are: 1, 8, 64,
 *        256, and 1024.
 *      o The divisors available on pins 3 and 11 are: 1, 8, 32, 64,
 *        128, 256, and 1024.
 *
 * PWM frequencies are tied together in pairs of pins. If one in a
 * pair is changed, the other is also changed to match:
 *   - Pins 5 and 6 are paired on timer0
 *   - Pins 9 and 10 are paired on timer1
 *   - Pins 3 and 11 are paired on timer2
 *
 * Note that this function will have side effects on anything else
 * that uses timers:
 *   - Changes on pins 3, 5, 6, or 11 may cause the delay() and
 *     millis() functions to stop working. Other timing-related
 *     functions may also be affected.
--------------------------------------------------------------------------
 *   - Changes on pins 9 or 10 will cause the _Servo library_ to function
 *     incorrectly.
--------------------------------------------------------------------------
 *
 * Thanks to macegr of the Arduino forums for his documentation of the
 * PWM frequency divisors. His post can be viewed at:
 *   http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1235060559/0#4
 */
void setUnoPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = (TCCR0B & 0b11111000) | mode;
    } else {
      TCCR1B = (TCCR1B & 0b11111000) | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x7; break;
      default: return;
    }
    TCCR2B = (TCCR2B & 0b11111000) | mode;
  }
}

/**
 * The Mega has 5 timers:
 * 
 * timer 0: pins 4 and 13
 * timer 1: pins 11 and 12
 * timer 2: pins 9 and 10
 * timer 3: pins 2, 3 and 5
 * timer 4: pins 6, 7 and 8
 * 
 * Base frequencies:
 *      o The base frequency for pins 2, 3, 5, 6, 7, 8, 9, 10, 11 and 12 is 31250 Hz.
 *      o The base frequency for pins 4 and 13 is 62500 Hz.
 *
 * Prescalers:
 *      o The divisors available on pins 2, 3, 5, 6, 7, 8, 9, 10, 11, 12 are: 
 *        1, 8, 64, 256, 1024 and 2048
 *      o The divisors available on pins 4 and 13 are:
 *        1, 8, 64, 256, 1024 and 4096
 * 
 * Note: Divisors determined based on https://forum.arduino.cc/index.php?topic=72092.0
 * Haven't verified this yet and the datasheet seems to suggest Timer 2 may
 * operate differently (pg 185)
 * 
 * Note from datasheet (pg 157):
 * 
 * mode 0x06: External Clock Source on Tn pin. Clock on falling edge.
 * mode 0x07: External Clock Source on Tn pin. Clock on rising edge.
 * 
 * Note: Timer 0 controls delay and millis functions.
 */ 
void setMegaPWMFrequency(int pin, int divisor) {
  byte mode;
  if (pin == 2 || pin == 3 || pin == 5 || pin == 6 || pin == 7 || pin == 8 || pin == 9 || pin == 10 || pin == 11 || pin == 12) {
    switch (divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      case 2048: mode = 0x06; break;
      default: return;
    }
    if (pin == 11 || pin == 12) {
      TCCR1B = (TCCR1B & 0b11111000) | mode;
    } else if (pin == 9 || pin == 10) {
      TCCR2B = (TCCR2B & 0b11111000) | mode;
    } else if (pin == 2 || pin == 3 || pin == 5) {
      TCCR3B = (TCCR3B & 0b11111000) | mode;
    } else {
      TCCR4B = (TCCR4B & 0b11111000) | mode;      
    }
  } else if (pin == 4 || pin == 13) {
    switch (divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      case 2048: mode = 0x06; break;
      default: return;
    }
    TCCR0B = (TCCR0B & 0b11111000) | mode;
  }
}

/*******************************
 * Constructors
 *******************************/

VoltageTrackerController::VoltageTrackerController(uint8_t controlPin, uint8_t feedbackPin, uint8_t externalSourcePin, bool useFeedback) {
  mControlPin = controlPin;
  mFeedbackPin = feedbackPin;
  mExternalSourcePin = externalSourcePin;
  mInputMode = VTInputMode::DigitalInput;
  mUseFeedback = useFeedback;
  mBaseDutyCycle = 0;
  mErrorDutyCycle = TrackedInt(0);
  setMegaPWMFrequency(controlPin, 1);
  setSetPoint(0, true);
  update(); // Make sure the current voltage reading is up to date
}

/*******************************
 * Accessors
 *******************************/

int VoltageTrackerController::getControlPin() const {
  return mControlPin;
}

int VoltageTrackerController::getFeedbackPin() const {
  return mFeedbackPin;
}

int VoltageTrackerController::getSetPoint() const {
  return mSetPoint;
}

int VoltageTrackerController::getCurrentVoltage() const {
  return mCurrentVoltage;
}

int VoltageTrackerController::getExternalSourcePin() const {
  return mExternalSourcePin;
}

VTInputMode VoltageTrackerController::getInputMode() const {
  return mInputMode;
}

TrackedInt VoltageTrackerController::getErrorDutyCycle() const {
  return mErrorDutyCycle;
}

int VoltageTrackerController::getDutyCycle() const {
  return mBaseDutyCycle;
}

int VoltageTrackerController::getRawFeedbackValue() const {
  return mFeedbackVoltageADValue;
}

int VoltageTrackerController::getRawExternalSetPointValue() const {
  return mSetPointVoltageADValue;
}

/*******************************
 * Actions
 *******************************/

int VoltageTrackerController::setSetPoint(int newSetPoint) {
  return setSetPoint(newSetPoint, false);
}

int VoltageTrackerController::setSetPoint(int newSetPoint, bool overrideExisting) {
  if (newSetPoint < 0 || newSetPoint > MAX_V) {
    return -1;
  }
  if (overrideExisting || mSetPoint != newSetPoint) {
    mSetPoint = newSetPoint;
    return mSetPoint;
  } else {
    return -2;
  } 
}

int VoltageTrackerController::readExternalSetPoint() {
  // Read current target voltage
  mSetPointVoltageADValue = analogRead(getExternalSourcePin());
  int newSetPoint = mSetPointVoltageADValue * AD_POINT_MILLIVOLT_RATIO;
  return newSetPoint;
}

void VoltageTrackerController::update() {
  // Read current voltage
  mFeedbackVoltageADValue = analogRead(getFeedbackPin());
  mCurrentVoltage = mFeedbackVoltageADValue * AD_POINT_MILLIVOLT_RATIO;

  // Update set point from external source if required
  if (getInputMode() == VTInputMode::ExternalInput) {
    int newSetPoint = readExternalSetPoint();
    setSetPoint(newSetPoint);
  }

  // Adjust if required
  if (mUseFeedback) {
    int error = mSetPoint - mCurrentVoltage;  
    if (abs(error) > TOLERANCE) {
  
      mErrorDutyCycle.setValue(CYCLES_PER_MILLIVOLT * error);
      mBaseDutyCycle = mBaseDutyCycle + mErrorDutyCycle.getValue();
      if (mBaseDutyCycle < 0) mBaseDutyCycle = 0;
      if (mBaseDutyCycle > 255) mBaseDutyCycle = 255;

      analogWrite(getControlPin(), mBaseDutyCycle);
    }
  }
}

void VoltageTrackerController::setInputMode(VTInputMode mode) {
  if (mInputMode != mode) {
    mInputMode = mode;
    /** 
     * If we're now sourcing externally, we need to make sure 
     * we've got the new set point. If we're switching back
     * to digital thendon't bother because the set point
     * will just remain as whatever it was from the external
     * input previously.
     */
    if (mInputMode == VTInputMode::ExternalInput) {
      update();
    }
  }
}
