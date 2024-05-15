#ifndef LineTracking_H_
#define LineTracking_H_
#include "Pinout.hpp"
#include "IRSensing.hpp"
#include <Arduino.h>

#define OnLine 0
#define NearBend 1
#define NearCorner 2



namespace LineTracking {
  void FollowingLine( uint8_t Case, uint16_t LeftSpeed, uint16_t RightSpeed, uint8_t MapCase, uint8_t &CornerNumApproach);
  //String tempIdTag;
}

#endif