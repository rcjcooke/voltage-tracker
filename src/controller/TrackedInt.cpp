#include "TrackedInt.hpp"

TrackedInt::TrackedInt() {
  reset(0);
}

TrackedInt::TrackedInt(int value) {
  reset(value);
}

int TrackedInt::getValue() const {
  return mValue;
}

int TrackedInt::getMax() const {
  return mMax;
}

int TrackedInt::getMin() const {
  return mMin;
}

void TrackedInt::reset(int value) {
  mValue = value;
  mMax = value;
  mMin = value;
}

void TrackedInt::setValue(int value) {
  mValue = value;
  if (value > mMax) mMax = value;
  if (value < mMin) mMin = value;
}