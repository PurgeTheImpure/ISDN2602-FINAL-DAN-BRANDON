#include "esp32-hal-gpio.h"
#include "IRSensing.hpp"

/*Initialize all the IR sensor Data Pins*/
void IR::Init(){
  pinMode(IR_L,INPUT);
  pinMode(IR_M,INPUT);
  pinMode(IR_R,INPUT);
};
/*For the Logic
  0: White 
  1: Black*/

bool IR::OnStartingLine(){
  if(digitalRead(IR_M) && (digitalRead(IR_L)) && (digitalRead(IR_R))){
    return true; 
  }
  else{
    return false;
  }
}


uint8_t IR::Tracking(){
  //On track means detecting black,
  //IR LIGHT ON = 0 ; IR LIGHT OFF = 1
  //IR ON BLACK = OFF = 1 == TRUE
  
  //M_IR on Track
  if(digitalRead(IR_M) && !(digitalRead(IR_L)) && !(digitalRead(IR_R)))
    return OnTrack;
  //R_IR on Track 
  if( (!digitalRead(IR_M) && (!digitalRead(IR_L)) && (digitalRead(IR_R))) || (digitalRead(IR_M) && (!digitalRead(IR_L)) && (digitalRead(IR_R))))
    return IR_ROnTrack;  
  //L_IR on Track 
  if(!digitalRead(IR_M) && (digitalRead(IR_L)) && !(digitalRead(IR_R)) || (digitalRead(IR_M) && (digitalRead(IR_L)) && !(digitalRead(IR_R))))
    return IR_LOnTrack;
  //L_IR and R_IR are on Track
  if(!digitalRead(IR_M) && (digitalRead(IR_L)) && !(digitalRead(IR_R)) || (digitalRead(IR_M) && (digitalRead(IR_L)) && !(digitalRead(IR_R))))
    return IR_LandROnTrack;
  //All on Track
  if(digitalRead(IR_M) && (digitalRead(IR_L)) && (digitalRead(IR_R)))
    return AllOnTrack;
  

  return OutOfTrack; 
};

