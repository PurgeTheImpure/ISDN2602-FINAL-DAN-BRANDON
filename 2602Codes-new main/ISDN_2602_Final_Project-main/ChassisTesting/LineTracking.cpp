#include "LineTracking.hpp"

#define turnTime90 700
#define turnTime45 400

void LineTracking::FollowingLine( uint8_t Case, uint16_t LeftSpeed, uint16_t RightSpeed, int Task, int &CornerNumApproach){
  //Serial.println(getTagUID());


  //LIGHT CONTROL FOR TASK 2!!!!
  if (CornerNumApproach == -1) {
    Motor::Stop();
    delay(30);
  }

  if (Task == 1 || ) {
    switch(CornerNumApproach) {
      
      case 2: 
        Motion::Rightwards(LeftSpeed, RightSpeed);
        delay(turnTime45);
        CornerNumApproach = 0;
        break;
      
      case 3: 
        Motion::Forwards(LeftSpeed, RightSpeed);
        delay(500);
        CornerNumApproach = 0;
        break;
      
      case 4: 
        Motion::Forwards(LeftSpeed, RightSpeed);
        delay(500);
        CornerNumApproach = 0;
        break;
      
      case 5:
        Motor::Stop();
        delay(3000);
        CornerNumApproach = 0;
        break;

      case 100:
        Motion::Boost(350, 365);
        delay(100);
        Motion::Boost(650, 666);
        delay(800);
        Motor::Stop();
        delay(500);
        CornerNumApproach = 0;
        break;

      default:
        break;

    }
  }

  else if (Task == 3) {
    if (startpt == 11 || startpt == 12) {
      switch (CornerNumApproach) {
        case 11:
          Motion::Rightwards(LeftSpeed, RightSpeed);
          delay(turnTime90);
          CornerNumApproach = 0;
          break;
        
        case 6:
          Motion::Forwards(LeftSpeed, RightSpeed);
          delay(turnTime90);
          CornerNumApproach = 0;
          break;

        case 2:
          Motion::Forwards(LeftSpeed, RightSpeed);
          delay(turnTime90);
          CornerNumApproach = 0;
          break;

        case 1:
          Motor::Stop();
          delay(3000);
          CornerNumApproach = 0;
          break;

        case 100:
          Motion::Boost(350, 365);
          delay(100);
          Motion::Boost(650, 666);
          delay(800);
          Motor::Stop();
          delay(500);
          CornerNumApproach = 0;
          break;

      }
    }

    else if (startpt == 13 || startpt == 14) {

      switch (CornerNumApproach) {
        case 13:
          Motion::Forwards(LeftSpeed, RightSpeed);
          delay(turnTime90);
          CornerNumApproach = 0;
          break;
        
        case 5:
          Motion::Forwards(LeftSpeed, RightSpeed);
          delay(turnTime90);
          CornerNumApproach = 0;
          break;

        case 1:
          Motor::Stop();
          delay(3000);
          CornerNumApproach = 0;
          break;

        case 100:
          Motion::Boost(350, 365);
          delay(100);
          Motion::Boost(650, 666);
          delay(800);
          Motor::Stop();
          delay(500);
          CornerNumApproach = 0;
          break;

      }
    }
  }

  switch (Case) {
    case OnTrack:
      Motion::Forwards(LeftSpeed, RightSpeed);
      delay(1);
    break;

    case IR_LOnTrack:
      //Motion::Fol_Leftwards(LeftSpeed, RightSpeed);
      //delay(1);
      
      while(IR::Tracking() != OnTrack) {
      Motion::Fol_Leftwards(LeftSpeed, RightSpeed);
      delay(1);
      }
      delay(5);
      Motion::Fol_Rightwards(LeftSpeed, RightSpeed);
      delay(5);
      
    break;

    case IR_ROnTrack:
      //Motion::Fol_Rightwards(LeftSpeed, RightSpeed);
      //delay(1);
        
      while(IR::Tracking() != OnTrack) {
      Motion::Fol_Rightwards(LeftSpeed, RightSpeed);
      delay(1);
      }
      delay(5);
      Motion::Fol_Leftwards(LeftSpeed, RightSpeed);
      delay(5);
      
    break;

    case AllOnTrack:
      Motor::Stop();
      delay(1500);
      Motion::Forwards(LeftSpeed, RightSpeed);
      delay(300);
    break;
    
    case OutOfTrack:
      Motion::Forwards(LeftSpeed, RightSpeed);
      delay(2);

    break;
  }
  


};