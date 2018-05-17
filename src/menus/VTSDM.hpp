#ifndef __VTSDM_H_INCLUDED__
#define __VTSDM_H_INCLUDED__

#include "SerialDisplayMenu.hpp"
#include "../controller/VoltageTrackerController.hpp"

class VTSDM : public SerialDisplayMenu {

public:
  /*******************************
   * Constructors
   *******************************/
  VTSDM(SerialDisplayMenuConfiguration* configuration, VoltageTrackerController* VoltageTrackerController, SerialDisplayMenu* origin, int8_t statusLineNumber, int8_t errorLineNumber);

  /*******************************
   * Static helper methods
   *******************************/
  // Converts a VTInputMode into a human readable string
  static String vtInputMode2Str(VTInputMode mode);

protected:
  /*******************************
   * Actions
   *******************************/
  
  // This method is called on every loop while the menu is waiting for user input. It is here in case 
  // the system needs to keep something up to date while waiting.
  void controllerUpdate();
  // Puts together the one line status update string to show users
  String constructStatusLine();

  /*******************************
   * Member variables
   *******************************/
  // Pointer to the controller these menus interface with
  VoltageTrackerController* mVoltageTrackerController;
  
private:
  // Used for a visible user status update heartbeat
  int mHrtBeatCharNum;
};

#endif // __VTSDM_H_INCLUDED__