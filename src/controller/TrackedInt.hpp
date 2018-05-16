#ifndef __TRACKEDINT_H_INCLUDED__
#define __TRACKEDINT_H_INCLUDED__

// A replacement for int that maintains a historical maximum and minimum.
class TrackedInt {

public:

  TrackedInt();
  TrackedInt(int value);

  int getValue() const;
  int getMax() const;
  int getMin() const;

  void setValue(int value);
  void reset(int value);

private:
  int mValue;
  int mMax;
  int mMin;
};

#endif // __TRACKEDINT_H_INCLUDED__
