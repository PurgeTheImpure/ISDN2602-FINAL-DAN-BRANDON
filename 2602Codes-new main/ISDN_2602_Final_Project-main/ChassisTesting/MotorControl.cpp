#include "esp32-hal-ledc.h"

#include "MotorControl.hpp"

//PWM Setting 
struct PWM_t { 
uint32_t freq ;
uint8_t resolution; 
uint32_t dutycycle;
} PWM;

PWM_t PWM_Channel_0 = { 2000, 10, 0 };
PWM_t PWM_Channel_1 = { 2000, 10, 0 };
PWM_t PWM_Channel_2 = { 2000, 10, 0 };
PWM_t PWM_Channel_3 = { 2000, 10, 0 };
PWM_t PWM_Channel_4 = { 50, 10, 0 };


void Motor::Init(){
  /*Initialize the PWM Channels*/ 
  ledcSetup(PWM_CH0, PWM_Channel_0.freq, PWM_Channel_0.resolution);
  ledcSetup(PWM_CH1, PWM_Channel_1.freq, PWM_Channel_1.resolution);
  ledcSetup(PWM_CH2, PWM_Channel_2.freq, PWM_Channel_2.resolution);
  ledcSetup(PWM_CH3, PWM_Channel_3.freq, PWM_Channel_3.resolution);

  /*Attach the Pins to the PWM Channels*/
  ledcAttachPin(Motor_R_IN1, PWM_CH0);
  ledcAttachPin(Motor_R_IN2, PWM_CH1);
  ledcAttachPin(Motor_L_IN1, PWM_CH2);
  ledcAttachPin(Motor_L_IN2, PWM_CH3);

  /*Initialize the PWM DutyCycle to 0% */
  ledcWrite(PWM_CH0, 0);
  ledcWrite(PWM_CH1, 0);
  ledcWrite(PWM_CH2, 0);
  ledcWrite(PWM_CH3, 0);
  
  /*Debug Message*/
  Serial.println("PWM Channel and DutyCycle for Motors Initialized.");

};

void Servo::Init(){
  /*Initialize the PWM Channel*/ 
  ledcSetup(PWM_CH4, PWM_Channel_4.freq, PWM_Channel_4.resolution);

  /*Attach the Pins to the PWM Channel*/
  ledcAttachPin(Servo_Pin, PWM_CH4);

  /*Initialize the PWM DutyCycle to 0% */
  ledcWrite(PWM_CH4, 0);

  /*Debug Message*/
  Serial.println("Servo Motor Initialized.");
}
/*For SG90 Servo Motor
  PWM         --> 50Hz  (20ms)
  Dutycycle   --> 1-2ms (5-10%)*/
void Servo::TrunDeg(uint16_t Degree){
  uint16_t Dutycycle = (float(Degree) / 90.0f) * 51.2f + 25.0f ;
  ledcWrite(PWM_CH4, Dutycycle);
  /*For Debug*/
  // Serial.print("Servo Degree: ");
  // Serial.println(Degree);
  // Serial.print("Dutycycle: ");
  // Serial.println(Dutycycle);
};

void Motor::Moving_Clockwise(uint16_t Speed, uint8_t Wheel){
  if(Wheel == 1){
    ledcWrite(PWM_CH0, Speed);
    ledcWrite(PWM_CH1, 0);
    delay(1);
  }
  if(Wheel == 2){
    ledcWrite(PWM_CH2, Speed);
    ledcWrite(PWM_CH3, 0);
    delay(1);
  }
};

void Motor::Moving_AntiClockwise(uint16_t Speed, uint8_t Wheel){
  if(Wheel == 1){
    ledcWrite(PWM_CH0, 0);
    ledcWrite(PWM_CH1, Speed);
    delay(1);
  }

  if(Wheel == 2){
    ledcWrite(PWM_CH2, 0);
    ledcWrite(PWM_CH3, Speed);
    delay(1);
  }
};

void Motor::Stop(){
  //Enable Both IN1 and IN2 to HIGH to Stop the Wheel 
    ledcWrite(PWM_CH0, 1024);
    ledcWrite(PWM_CH1, 1024);
    ledcWrite(PWM_CH2, 1024);
    ledcWrite(PWM_CH3, 1024);
    delay(5);
  
};
/*WE DO NOT USE THE CODE BELOW*/
/*WE DO NOT USE THE CODE BELOW*/
/*WE DO NOT USE THE CODE BELOW*/
/*To Find the Relationship between RPM and PWM to adjust the PWM using Target RPM*/
float Motor::RPMtoPWM(float TargetRPM, uint8_t Wheel){
    float TargetPWM = 0.0f;
    /*Be Awared of 2 Motor may have a different PWM and RPM ratio*/
    switch (Wheel)
    {
    case LeftWheel:
    /*Find the math relationship
      it's not a linear relationship 
      But can make the estimate value by 2 - 3 range and apply linear estimation*/
    TargetPWM = ((TargetRPM - 400.0f)/20.0f) * 60.0f ;

    if(TargetPWM > 1024.0f)
    TargetPWM = 1024.0f;

    return TargetPWM;


    case RightWheel:
    TargetPWM = TargetRPM;

    if(TargetPWM > 1024.0f)
    TargetPWM = 1024.0f;

    return TargetPWM;
    }
}
/*WE DO NOT USE THE CODE ABOVE*/
/*WE DO NOT USE THE CODE ABOVE*/
/*WE DO NOT USE THE CODE ABOVE*/

// More Speeds for fine tuning for me //
//Forards
volatile float FW_LeftSpeed = 310.0f; // 0.0f - 1024.0f
volatile float FW_RightSpeed = 318.0f; // 0.0f - 1024.0f
// Backwards
volatile float BW_LeftSpeed = 290.0f; // 0.0f - 1024.0f
volatile float BW_RightSpeed = 290.0f; // 0.0f - 1024.0f
//Turn Right
volatile float TR_LeftSpeed = 720.0f; // 0.0f - 1024.0f
volatile float TR_RightSpeed = 350.0f; // 0.0f - 1024.0f
//Turn Left
volatile float TL_LeftSpeed = 350.0f; // 0.0f - 1024.0f
volatile float TL_RightSpeed = 680.0f; // 0.0f - 1024.0f

// Follow line code, small turns only
volatile float FolRT_LeftSpeed = 580.0f; // 0.0f - 1024.0f
volatile float FolRT_RightSpeed = 320.0f; // 0.0f - 1024.0f

volatile float FolLT_LeftSpeed = 320.0f; // 0.0f - 1024.0f
volatile float FolLT_RightSpeed = 560.0f; // 0.0f - 1024.0f


void Motion::Forwards(uint16_t LeftSpeed, uint16_t RightSpeed){
  Motor::Moving_AntiClockwise(FW_LeftSpeed , LeftWheel);
  Motor::Moving_AntiClockwise(FW_RightSpeed, RightWheel);
  Servo::TrunDeg(92);
};

void Motion::Boost(uint16_t B_LeftSpeed, uint16_t B_RightSpeed){
  Servo::TrunDeg(92);
  Motor::Moving_AntiClockwise(B_LeftSpeed, LeftWheel);
  Motor::Moving_AntiClockwise(B_RightSpeed, RightWheel);
  Servo::TrunDeg(92);
};

void Motion::Backwards(uint16_t LeftSpeed, uint16_t RightSpeed){
  Motor::Moving_Clockwise(BW_LeftSpeed, LeftWheel);
  Motor::Moving_Clockwise(BW_RightSpeed, RightWheel);
  Servo::TrunDeg(92);
};  


// Code I planned to use instead of PID
void Motion::Rightwards(uint16_t LeftSpeed, uint16_t RightSpeed){
  Servo::TrunDeg(135);
  Motor::Moving_Clockwise(TR_RightSpeed, RightWheel);
  Motor::Moving_AntiClockwise(TR_LeftSpeed, LeftWheel);
  
};

void Motion::Leftwards(uint16_t LeftSpeed, uint16_t RightSpeed){
  Servo::TrunDeg(45);
  Motor::Moving_AntiClockwise(TL_RightSpeed, RightWheel);
  Motor::Moving_Clockwise(TL_LeftSpeed, LeftWheel);
};


void Motion::Fol_Rightwards(uint16_t LeftSpeed, uint16_t RightSpeed){
  Servo::TrunDeg(125);
  Motor::Moving_Clockwise(FolRT_RightSpeed, RightWheel);
  Motor::Moving_AntiClockwise(FolRT_LeftSpeed, LeftWheel);
  
};

void Motion::Fol_Leftwards(uint16_t LeftSpeed, uint16_t RightSpeed){
  Servo::TrunDeg(55);
  Motor::Moving_AntiClockwise(FolLT_RightSpeed, RightWheel);
  Motor::Moving_Clockwise(FolLT_LeftSpeed, LeftWheel);
};

