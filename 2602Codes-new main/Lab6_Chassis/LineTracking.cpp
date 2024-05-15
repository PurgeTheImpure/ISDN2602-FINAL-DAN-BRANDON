#include "LineTracking.hpp"

bool followOnly = 1;

void LineTracking::FollowingLine( uint8_t Case, uint16_t Speed ){
  if (followOnly) {
    switch (Case) {
    /*Change the Motion::ACTION for each case */
    case OnTrack:
      Motion::Forwards(Speed);
      delay(1); //You may change the delay duration 
    break;

    case IR_LOnTrack:
      Motion::FL_Leftwards(Speed);
      delay(150); //You may change the delay duration 
    break;

    case IR_ROnTrack:
      Motion::FL_Rightwards(Speed);
      delay(150); //You may change the delay duration 
    break;

    case AllOnTrack:
      Motor::Stop();
      delay(1500);
      Motion::Forwards(Speed);
      //delay(1500); //You may change the delay duration 
    break;
    
    case IR_LandROnTrack:
      Motion::Forwards(Speed);
      delay(10); //You may change the delay duration 
    break;

    case OutOfTrack:
      Motion::Forwards(Speed);
      delay(10); //You may change the delay duration 

    break;
    }
  }


};