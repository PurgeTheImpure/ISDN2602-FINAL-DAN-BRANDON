#include "LineTracking.hpp"

void LineTracking::FollowingLine( uint8_t Case, uint16_t LeftSpeed, uint16_t RightSpeed, int Task, int &CornerNumApproach){
  //Serial.println(getTagUID());


  //LIGHT CONTROL FOR TASK 2!!!!
  if (CornerNumApproach == -1) {
    Motor::Stop();
    delay(30);
  }
  
  switch(CornerNumApproach) {

    case 100:
      Motion::Boost(350, 365);
      delay(100);
      Motion::Boost(650, 666);
      delay(800);
      Motor::Stop();
      delay(500);
      CornerNumApproach = 0;
      break;
    
    case 2: 
      Motion::Rightwards(LeftSpeed, RightSpeed);
      delay(700);
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

    default:
      break;

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
  

  //String x_fuckyou = 20D96691;
  /*
  if (tempIdTag == "20d96691" || tempIdTag == "f0de6691" || tempIdTag == "a0db6691") {
    Motor::Stop();
    delay(4500);
  }
  */



};