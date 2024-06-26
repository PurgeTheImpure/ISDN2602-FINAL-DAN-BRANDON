#ifndef LineTracking_H_
#define LineTracking_H_
#include "Pinout.hpp"
#include "IRSensing.hpp"
#include <Arduino.h>

#define OnLine 0
#define NearBend 1
#define NearCorner 2

extern int path[14];
extern int pathLength;
extern int startpt; // Example start node
extern int endpt;   // Example end node

namespace LineTracking {
  void FollowingLine( uint8_t Case, uint16_t LeftSpeed, uint16_t RightSpeed, int Task, int &CornerNumApproach);
  //String tempIdTag;
}

#endif