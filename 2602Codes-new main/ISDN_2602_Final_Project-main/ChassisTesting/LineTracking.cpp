#include "LineTracking.hpp"

#define turnTime90 510
#define turnTime45 260

void LineTracking::FollowingLine( uint8_t Case, uint16_t LeftSpeed, uint16_t RightSpeed, int Task, int &CornerNumApproach){
  //Serial.println(getTagUID());

  //LIGHT CONTROL FOR TASK 2 3 4!!!!
  if (CornerNumApproach == -1) {
    Motor::Stop();
    delay(150);
    return;
  }

  if (Task == 1 || Task == 2) {
    switch(CornerNumApproach) {

      case 6: 
        
        Motion::Forwards(LeftSpeed, RightSpeed);
        delay(200);
        CornerNumApproach = 0;
        return;
      
      case 2: 
        Motion::Rightwards(LeftSpeed, RightSpeed);
        delay(turnTime45);
        CornerNumApproach = 0;
        return;
      
      case 3: 
        Motion::Forwards(LeftSpeed, RightSpeed);
        delay(turnTime90);
        CornerNumApproach = 0;
        return;
      
      case 4: 
        Motion::Forwards(LeftSpeed, RightSpeed);
        delay(turnTime45);
        CornerNumApproach = 0;
        return;
      
      case 5:
        Motor::Stop();
        delay(6000);
        CornerNumApproach = 0;
        return;
        break;

      case 100:
        Motion::Boost(350, 365);
        delay(100);
        Motion::Boost(750, 766);
        delay(800);
        Motor::Stop();
        delay(500);
        CornerNumApproach = 0;
        return;
        break;

      default:
        break;

    }
  }

  else if (Task == 3) {

    if (startpt == 12 || startpt == 11 ) {
      if(CornerNumApproach == 11 && startpt == 12) {
        Motion::Rightwards(LeftSpeed, RightSpeed);
        delay(turnTime90);
        CornerNumApproach = 0;
        return;
      }

      switch (CornerNumApproach) {
        
        case 6:
          
          Motion::Forwards(LeftSpeed, RightSpeed);
          delay(250);
        
        //Motion::Forwards(LeftSpeed, RightSpeed);
        //delay(250);
          CornerNumApproach = 0;
          return;

        case 2:
          Motion::Forwards(LeftSpeed, RightSpeed);
          delay(turnTime45);
          CornerNumApproach = 0;
          return;

        case 1:
          Motor::Stop();
          delay(6000);
          CornerNumApproach = 0;
          return;

        case 100:
          Motion::Boost(350, 365);
          delay(100);
          Motion::Boost(750, 766);
          delay(800);
          Motor::Stop();
          delay(500);
          CornerNumApproach = 0;
          return;

        default:
          break;

      }
    }

    else if (startpt == 13 || startpt == 14) {

      switch (CornerNumApproach) {
        case 13:
          Motion::Forwards(LeftSpeed, RightSpeed);
          delay(turnTime90);
          CornerNumApproach = 0;
          return;
        
        case 5:
          Motion::Forwards(LeftSpeed, RightSpeed);
          delay(turnTime90);
          CornerNumApproach = 0;
          return;

        case 90:
          Motion::Leftwards(LeftSpeed, RightSpeed);
          delay(turnTime90);
          CornerNumApproach = 0;
          return;

        case 1:
          Motor::Stop();
          delay(10000);
          CornerNumApproach = 0;
          return;

        case 100:
          Motion::Boost(350, 365);
          delay(100);
          Motion::Boost(750, 766);
          delay(800);
          Motor::Stop();
          delay(500);
          CornerNumApproach = 0;
          return;
        default:
          break;
      }
    }
  }

  if (Task == 4) {
    
    if(startpt == 12) { // to 5
      switch (CornerNumApproach) {
        case 13:
          Motion::Rightwards(LeftSpeed, RightSpeed);
          delay(turnTime90);
          CornerNumApproach = 0;
          return;
        
        case 5:
          Motor::Stop();
          delay(10000);
          CornerNumApproach = 0;
          return;

        case 100:
          Motion::Boost(350, 365);
          delay(100);
          Motion::Boost(750, 766);
          delay(800);
          Motor::Stop();
          delay(500);
          CornerNumApproach = 0;
          return;

        default:
          break;
      }
    }

    else if (startpt == 13) { // to 6
      switch (CornerNumApproach) {
        case 11:
          Motion::Rightwards(LeftSpeed, RightSpeed);
          delay(turnTime90);
          CornerNumApproach = 0;
          return;

        case 12:
          Motion::Forwards(LeftSpeed, RightSpeed);
          delay(turnTime45);
          CornerNumApproach = 0;
          return;
        
        case 6:
          Motor::Stop();
          delay(10000);
          CornerNumApproach = 0;
          return;

        case 100:
          Motion::Boost(350, 365);
          delay(100);
          Motion::Boost(750, 766);
          delay(800);
          Motor::Stop();
          delay(500);
          CornerNumApproach = 0;
          return;

        default:
          break;
      }

    }

    else if (startpt == 11) { // jump the gap

      switch (CornerNumApproach) {

        case 12:
          Motion::Forwards(LeftSpeed, RightSpeed);
          delay(turnTime45);
          CornerNumApproach = 0;
          return;
        
        case 13:
          Motor::Stop();
          delay(10000);
          CornerNumApproach = 0;
          return;

        case 100:
          Motion::Boost(350, 365);
          delay(100);
          Motion::Boost(750, 766);
          delay(800);
          Motor::Stop();
          delay(500);
          CornerNumApproach = 0;
          return;

        default:
          break;
      }

    }

    else if (startpt == 10 || startpt == 8) {

      if (startpt == 10) {
        if (CornerNumApproach == 9) {
          Motion::Rightwards(LeftSpeed, RightSpeed);
          delay(turnTime90);
          CornerNumApproach = 0;
          return;
        }
        else if (CornerNumApproach == 7) {
          Motion::Forwards(LeftSpeed, RightSpeed);
          delay(turnTime45);
          CornerNumApproach = 0;
          return;
        }
      }
      else if (startpt == 8) {
        if (CornerNumApproach == 7) {
          Motion::Rightwards(LeftSpeed, RightSpeed);
          delay(turnTime90);
          CornerNumApproach = 0;
          return;
        }
      }

      switch (CornerNumApproach) {
        case 4:
          Motion::Leftwards(LeftSpeed, RightSpeed);
          delay(850);
          CornerNumApproach = 0;
          return;

        case 3:
          Motion::Rightwards(LeftSpeed, RightSpeed);
          delay(turnTime45);
          CornerNumApproach = 0;
          return;
        
        case 1:
          Motor::Stop();
          delay(10000);
          CornerNumApproach = 0;
          return;

        default:
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
        delay(5);
        }
        delay(1);
        Motion::Fol_Rightwards(LeftSpeed, RightSpeed);
        delay(10);
        
      break;

      case IR_ROnTrack:
        //Motion::Fol_Rightwards(LeftSpeed, RightSpeed);
        //delay(1);
          
        while(IR::Tracking() != OnTrack) {
        Motion::Fol_Rightwards(LeftSpeed, RightSpeed);
        delay(5);
        }
        delay(1);
        Motion::Fol_Leftwards(LeftSpeed, RightSpeed);
        delay(10);
        
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