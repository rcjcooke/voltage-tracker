#include <stdint.h>

#include "SerialDisplayMenuConfiguration.hpp"

SerialDisplayMenuConfiguration::SerialDisplayMenuConfiguration(SerialDisplayType displayType, bool displayStatusLine, int userStatusUpdateFrequencyModulus, long errorDisplayTimeMillis) {
  mSerialDisplay = displayType;
  mDisplayStatusLine = displayStatusLine;
  mUserStatusUpdateFrequencyModulus = userStatusUpdateFrequencyModulus;
  mErrorDisplayTimeMillis = errorDisplayTimeMillis;
}

SerialDisplayType SerialDisplayMenuConfiguration::getSerialDisplayType() const {
  return mSerialDisplay;
}

bool SerialDisplayMenuConfiguration::getDisplayStatusLine() const {
  return mDisplayStatusLine;
}

int SerialDisplayMenuConfiguration::getUserStatusUpdateFrequencyModulus() const {
  return mUserStatusUpdateFrequencyModulus;
}

unsigned long SerialDisplayMenuConfiguration::getErrorDisplayTimeMillis() const {
  return mErrorDisplayTimeMillis;
}

