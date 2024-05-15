#ifndef IRSensing_H_
#define IRSensing_H_
#include "Pinout.hpp"
#include <Arduino.h>

#define OutOfTrack      0 
#define OnTrack         1
#define IR_ROnTrack     2
#define IR_LOnTrack     3 
#define IR_LandROnTrack 4
#define IR_Finish       5

#define AllOnTrack      6

namespace IR {
  void Init();
  bool OnStartingLine();
  uint8_t Tracking();


};

#endif