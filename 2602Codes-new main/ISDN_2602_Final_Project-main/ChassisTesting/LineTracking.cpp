#include "LineTracking.hpp"

#define turnTime90 700
#define turnTime45 370

void LineTracking::FollowingLine( uint8_t Case, uint16_t LeftSpeed, uint16_t RightSpeed, int Task, int &CornerNumApproach){
  //Serial.println(getTagUID());
  while(1) {
    Motion::Rightwards(LeftSpeed, RightSpeed);
    delay(15);
  }

  //LIGHT CONTROL FOR TASK 2 3 4!!!!
  if (CornerNumApproach == -1) {
    Motor::Stop();
    delay(100);
    return;
  }

  if (Task == 1 || Task == 2) {
    switch(CornerNumApproach) {
      
      case 2: 
        Motion::Rightwards(LeftSpeed, RightSpeed);
        delay(turnTime45);
        CornerNumApproach = 0;
        break;
      
      case 3: 
        Motion::Forwards(LeftSpeed, RightSpeed);
        delay(turnTime45);
        CornerNumApproach = 0;
        break;
      
      case 4: 
        Motion::Forwards(LeftSpeed, RightSpeed);
        delay(turnTime45);
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

    if (startpt == 12 || startpt == 11) {
      if(CornerNumApproach == 11 && startpt == 12) {
        Motion::Rightwards(LeftSpeed, RightSpeed);
        delay(turnTime90);
        CornerNumApproach = 0;
          
      }

      switch (CornerNumApproach) {
        
        case 6:
          Motion::Forwards(LeftSpeed, RightSpeed);
          delay(turnTime45);
          CornerNumApproach = 0;
          break;

        case 2:
          Motion::Forwards(LeftSpeed, RightSpeed);
          delay(turnTime45);
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
          delay(turnTime45);
          CornerNumApproach = 0;
          break;
        
        case 5:
          Motion::Forwards(LeftSpeed, RightSpeed);
          delay(turnTime45);
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

  //else if ()
    switch (Case) {
      case OnTrack:
        Motion::Forwards(LeftSpeed, RightSpeed);
        //delay (100);
        //Motor::Stop();
        delay(10);
      break;

      case IR_LOnTrack:
        //Motion::Fol_Leftwards(LeftSpeed, RightSpeed);
        //delay(1);
        
        while(IR::Tracking() != OnTrack) {
        Motion::Fol_Leftwards(LeftSpeed, RightSpeed);
        delay(50);
        }
        delay(1);
        Motion::Fol_Rightwards(LeftSpeed, RightSpeed);
        delay(5);
        
      break;

      case IR_ROnTrack:
        //Motion::Fol_Rightwards(LeftSpeed, RightSpeed);
        //delay(1);
          
        while(IR::Tracking() != OnTrack) {
        Motion::Fol_Rightwards(LeftSpeed, RightSpeed);
        delay(50);
        }
        delay(1);
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