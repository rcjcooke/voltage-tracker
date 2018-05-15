#include <stdint.h>

#include "SerialDisplayMenuConfiguration.hpp"

SerialDisplayMenuConfiguration::SerialDisplayMenuConfiguration(SerialDisplayType displayType, bool displayStatusLine) {
  mSerialDisplay = displayType;
  mDisplayStatusLine = displayStatusLine;
}

SerialDisplayType SerialDisplayMenuConfiguration::getSerialDisplayType() const {
  return mSerialDisplay;
}

bool SerialDisplayMenuConfiguration::getDisplayStatusLine() const {
  return mDisplayStatusLine;
}
