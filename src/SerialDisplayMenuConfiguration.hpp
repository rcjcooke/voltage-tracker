#ifndef __SERIALDISPLAYMENUCONFIGURATION_H_INCLUDED__
#define __SERIALDISPLAYMENUCONFIGURATION_H_INCLUDED__


//: std::int8_t
enum class SerialDisplayType {
  ansi_vt100,
  serialMonitor
};

class SerialDisplayMenuConfiguration {

public:

  /*******************************
   * Constructors
   *******************************/
  SerialDisplayMenuConfiguration(SerialDisplayType displayType, bool displayStatusLine);

  /*******************************
   * Accessors
   *******************************/
  SerialDisplayType getSerialDisplayType() const;
  bool getDisplayStatusLine() const;

  /*******************************
   * Actions
   *******************************/

private:
  /*******************************
   * Member variables
   *******************************/
  // Defines the type of output interface we're using
  SerialDisplayType mSerialDisplay;
  // Defines whether a status line is included on the menus
  bool mDisplayStatusLine;

};

#endif // __SERIALDISPLAYMENUCONFIGURATION_H_INCLUDED__
