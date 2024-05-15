#include "esp32-hal-ledc.h"

#include "MotorControl.hpp"

/*For fine tune the motor bias*/
uint16_t RightFwBias      = 70;
uint16_t LeftFwBias      = 0;

uint16_t RightBwBias      = 30;
uint16_t LeftBwBias       = 70; 

/*For extra power to turning Left or Right*/
uint16_t TurnRightExtraSpeed = 60;
uint16_t TurnLeftExtraSpeed  = 100;
/*For extra power to turning Left or Right while following line forwards*/
uint16_t FLRightExtraSpeed = 20;
uint16_t FLLeftExtraSpeed  = 20;

//PWM Setting 
struct PWM_t{ 
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
  if(Wheel == 1 ){
    ledcWrite(PWM_CH0, Speed);
    ledcWrite(PWM_CH1, 0);
    //delay(1);
  }
  if(Wheel == 2){
    ledcWrite(PWM_CH2, Speed);
    ledcWrite(PWM_CH3, 0);
    //delay(1);
  }
};

void Motor::Moving_AntiClockwise(uint16_t Speed, uint8_t Wheel){
  if(Wheel == 1 ){
    ledcWrite(PWM_CH0, 0);
    ledcWrite(PWM_CH1, Speed);
    //delay(1);
  }

  if(Wheel == 2){
    ledcWrite(PWM_CH2, 0);
    ledcWrite(PWM_CH3, Speed);
    //delay(1);
  }
};

void Motor::Stop(){
  //Enable Both IN1 and IN2 to HIGH to Stop the Wheel 
    ledcWrite(PWM_CH0, 1024);
    ledcWrite(PWM_CH1, 1024);
    ledcWrite(PWM_CH2, 1024);
    ledcWrite(PWM_CH3, 1024);
    delay(1);
  
};


void Motion::Forwards(uint16_t Speed){
  /*If the two Wheels have bias, adjust one of the speed of the Wheel*/
  Motor::Moving_AntiClockwise(Speed + LeftFwBias, LeftWheel);
  Motor::Moving_AntiClockwise(Speed + RightFwBias, RightWheel); 

  //Fix the Servo Motor to 90 deg all the time
  Servo::TrunDeg(95);
};

void Motion::Backwards(uint16_t Speed){
  /*If the two Wheels have bias, adjust one of the speed of the Wheel*/
  Motor::Moving_Clockwise(Speed + LeftBwBias, LeftWheel);
  Motor::Moving_Clockwise(Speed + RightBwBias, RightWheel);

  //Fix the Servo Motor to 90 deg all the time
  Servo::TrunDeg(95);
};

void Motion::Rightwards(uint16_t Speed){
  /*If the two Wheels have bias, adjust one of the speed of the Wheel*/
  Motor::Moving_Clockwise(Speed + RightBwBias + TurnRightExtraSpeed, RightWheel );
  Motor::Moving_AntiClockwise(Speed + LeftFwBias + TurnRightExtraSpeed, LeftWheel);

  //Fix the Servo Motor to 135 deg all the time
  Servo::TrunDeg(135);
  
};

void Motion::Leftwards(uint16_t Speed){
  /*If the two Wheels have bias, adjust one of the speed of the Wheel*/
  Motor::Moving_AntiClockwise(Speed + RightFwBias + TurnLeftExtraSpeed, RightWheel);
  Motor::Moving_Clockwise(Speed + LeftBwBias + TurnLeftExtraSpeed, LeftWheel);

  //Fix the Servo Motor to 45 deg all the time
  Servo::TrunDeg(45);
};

void Motion::FL_Rightwards(uint16_t Speed){
  /*If the two Wheels have bias, adjust one of the speed of the Wheel*/
  Motor::Moving_AntiClockwise(Speed + RightFwBias, RightWheel );
  Motor::Moving_AntiClockwise(Speed + LeftFwBias + FLRightExtraSpeed, LeftWheel);

  //Fix the Servo Motor to 135 deg all the time
  Servo::TrunDeg(105);
  
};

void Motion::FL_Leftwards(uint16_t Speed){
  /*If the two Wheels have bias, adjust one of the speed of the Wheel*/
  Motor::Moving_AntiClockwise(Speed + RightFwBias + FLLeftExtraSpeed, RightWheel);
  Motor::Moving_AntiClockwise(Speed + LeftFwBias, LeftWheel);

  //Fix the Servo Motor to 45 deg all the time
  Servo::TrunDeg(85);
};
