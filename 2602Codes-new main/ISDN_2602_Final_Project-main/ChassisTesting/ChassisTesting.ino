#include "Pinout.hpp"
#include "esp32-hal-ledc.h"
// Include Arduino FreeRTOS library
// #include "Arduino_FreeRTOS.h"

/*Include the FreeRTOS library*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include <esp_task_wdt.h>

/*Pre-Set Data*/
volatile float LeftSpeed = 190.0f; // 0.0f - 1024.0f
volatile float RightSpeed = 190.0f; // 0.0f - 1024.0f

unsigned long ms = 0;

/*Exam Firebase Data*/
volatile int Exam_State = 0; // 0 = Stopped || 1 = Started || 2 = Paused
volatile int Demo_time = 0;  // Time in seconds
volatile int Start_point = 0;
volatile int End_point = 0;
volatile int Task = 0;
/*Exam Firebase Data*/

struct IntersectionLights {
  struct Light {
    int state; // 0 = red || 1 = green || 2 = yellow
    int timeleft;
    int intersection;
    int light_number;
  };
  Light intersectionLights[10]; // Array of Light objects
};
IntersectionLights intersectionLights;

/*-------------------------------------------------------------------------------------------------------------------------*/
/*Firebase Objects*/
DefaultNetwork network; // initilize with boolean parameter to enable/disable network reconnection
UserAuth user_auth(API_KEY, USER_EMAIL, USER_PASSWORD);
FirebaseApp app;
WiFiClientSecure ssl_client1, ssl_client2;
using AsyncClient = AsyncClientClass;
AsyncClient aClient(ssl_client1, getNetwork(network));
AsyncClient aClient2(ssl_client2, getNetwork(network));
RealtimeDatabase Database;
AsyncResult aResult_no_callback;

void handleTrafficLight(const String &path, const String &data) {
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, data);
  JsonObject obj = doc.as<JsonObject>();

  String currentState = obj["current_state"];
  Serial.println(currentState);
  int timeRemain = obj["time_remain"];
  Serial.println(timeRemain);

  // Parse the path to get the intersection and light number
  int startPos = path.indexOf("intersection_") + strlen("intersection_");
  int endPos = path.indexOf("/", startPos);
  String intersection = path.substring(startPos, endPos);

  startPos = path.indexOf("light_") + strlen("light_");
  String light = path.substring(startPos);

  // Update the intersectionLights object
  int index = (intersection.toInt() - 1) * 2 + (light.toInt()-1);
  if (currentState == "red")
  {
    intersectionLights.intersectionLights[index].state = 0;
  }
  else if (currentState == "green")
  {
    intersectionLights.intersectionLights[index].state = 1;
  }
  else if (currentState == "yellow")
  {
    intersectionLights.intersectionLights[index].state = 2;
  }
  intersectionLights.intersectionLights[index].timeleft = timeRemain;
}

void asyncCB(AsyncResult &aResult)
{
  printResult(aResult);
}

void printResult(AsyncResult &aResult)
{
  if (aResult.appEvent().code() > 0)
  {
    Firebase.printf("Event task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.appEvent().message().c_str(), aResult.appEvent().code());
  }

  if (aResult.isDebug())
  {
    Firebase.printf("Debug task: %s, msg: %s\n", aResult.uid().c_str(), aResult.debug().c_str());
  }

  if (aResult.isError())
  {
    Firebase.printf("Error task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.error().message().c_str(), aResult.error().code());
  }


  if (aResult.available()) {

    RealtimeDatabaseResult &RTDB = aResult.to<RealtimeDatabaseResult>();
    if (RTDB.isStream()) {
      if (strcmp(RTDB.dataPath().c_str(), "/") == 0) {

        DynamicJsonDocument doc(1024);
        deserializeJson(doc, RTDB.to<const char *>());
        JsonObject obj = doc.as<JsonObject>();
        String state = obj["demo"]["state"];
        if (state == "stopped")
        {
          Exam_State = 0;
        }
        else if (state == "started")
        {
          Exam_State = 1;
        }
        else if (state == "paused")
        {
          Exam_State = 2;
        }
        String time = obj["demo"]["time_left"];
        if (obj["demo"]["time_left"])
        {
        Demo_time = time.toInt();
        }
        String start = obj["demo"]["start_point"];
        if (obj["demo"]["start_point"])
        {
        Start_point = start.toInt();
        }
        String end = obj["demo"]["end_point"];
        if (obj["demo"]["end_point"])
        {
        End_point = end.toInt();
        }
        String task = obj["demo"]["task"];
        if (obj["demo"]["task"])
        {
        Task = task.toInt();
        }
        // Update the intersectionLights struct with the new data
        intersectionLights.intersectionLights[0].state = (obj["intersections"]["intersection_1"]["traffic_lights"]["light_1"]["current_state"] == "red") ? 0 :
                                                         (obj["intersections"]["intersection_1"]["traffic_lights"]["light_1"]["current_state"] == "yellow") ? 2 : 1;
        intersectionLights.intersectionLights[0].timeleft = obj["intersections"]["intersection_1"]["traffic_lights"]["light_1"]["time_remain"];
        intersectionLights.intersectionLights[1].state = (obj["intersections"]["intersection_1"]["traffic_lights"]["light_2"]["current_state"] == "red") ? 0 : 1;
                                                         (obj["intersections"]["intersection_1"]["traffic_lights"]["light_2"]["current_state"] == "yellow") ? 2 : 1;
        intersectionLights.intersectionLights[1].timeleft = obj["intersections"]["intersection_1"]["traffic_lights"]["light_2"]["time_remain"];

        intersectionLights.intersectionLights[2].state = (obj["intersections"]["intersection_2"]["traffic_lights"]["light_1"]["current_state"] == "red") ? 0 : 1;
                                                         (obj["intersections"]["intersection_2"]["traffic_lights"]["light_1"]["current_state"] == "yellow") ? 2 : 1;
        intersectionLights.intersectionLights[2].timeleft = obj["intersections"]["intersection_2"]["traffic_lights"]["light_1"]["time_remain"];
        intersectionLights.intersectionLights[3].state = (obj["intersections"]["intersection_2"]["traffic_lights"]["light_2"]["current_state"] == "red") ? 0 : 1;
                                                         (obj["intersections"]["intersection_2"]["traffic_lights"]["light_2"]["current_state"] == "yellow") ? 2 : 1;
        intersectionLights.intersectionLights[3].timeleft = obj["intersections"]["intersection_2"]["traffic_lights"]["light_2"]["time_remain"];

        intersectionLights.intersectionLights[4].state = (obj["intersections"]["intersection_3"]["traffic_lights"]["light_1"]["current_state"] == "red") ? 0 : 1;
                                                         (obj["intersections"]["intersection_3"]["traffic_lights"]["light_1"]["current_state"] == "yellow") ? 2 : 1;
        intersectionLights.intersectionLights[4].timeleft = obj["intersections"]["intersection_3"]["traffic_lights"]["light_1"]["time_remain"];
        intersectionLights.intersectionLights[5].state = (obj["intersections"]["intersection_3"]["traffic_lights"]["light_2"]["current_state"] == "red") ? 0 : 1;
                                                         (obj["intersections"]["intersection_3"]["traffic_lights"]["light_2"]["current_state"] == "yellow") ? 2 : 1;
        intersectionLights.intersectionLights[5].timeleft = obj["intersections"]["intersection_3"]["traffic_lights"]["light_2"]["time_remain"];

        intersectionLights.intersectionLights[6].state = (obj["intersections"]["intersection_4"]["traffic_lights"]["light_1"]["current_state"] == "red") ? 0 : 1;
                                                         (obj["intersections"]["intersection_4"]["traffic_lights"]["light_1"]["current_state"] == "yellow") ? 2 : 1;
        intersectionLights.intersectionLights[6].timeleft = obj["intersections"]["intersection_4"]["traffic_lights"]["light_1"]["time_remain"];
        intersectionLights.intersectionLights[7].state = (obj["intersections"]["intersection_4"]["traffic_lights"]["light_2"]["current_state"] == "red") ? 0 : 1;
                                                         (obj["intersections"]["intersection_4"]["traffic_lights"]["light_2"]["current_state"] == "yellow") ? 2 : 1;
        intersectionLights.intersectionLights[7].timeleft = obj["intersections"]["intersection_4"]["traffic_lights"]["light_2"]["time_remain"];

        intersectionLights.intersectionLights[8].state = (obj["intersections"]["intersection_5"]["traffic_lights"]["light_1"]["current_state"] == "red") ? 0 : 1;
                                                         (obj["intersections"]["intersection_5"]["traffic_lights"]["light_1"]["current_state"] == "yellow") ? 2 : 1;
        intersectionLights.intersectionLights[8].timeleft = obj["intersections"]["intersection_5"]["traffic_lights"]["light_1"]["time_remain"];
        intersectionLights.intersectionLights[9].state = (obj["intersections"]["intersection_5"]["traffic_lights"]["light_2"]["current_state"] == "red") ? 0 : 1;
                                                         (obj["intersections"]["intersection_5"]["traffic_lights"]["light_2"]["current_state"] == "yellow") ? 2 : 1;
        intersectionLights.intersectionLights[9].timeleft = obj["intersections"]["intersection_5"]["traffic_lights"]["light_2"]["time_remain"];
      }
      if (strcmp(RTDB.dataPath().c_str(), "/demo") == 0)
      {
        // Parse the obtained data using Json
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, RTDB.to<const char *>());
        JsonObject obj = doc.as<JsonObject>();
        String state = obj["state"];
        if (state == "stopped")
        {
          Exam_State = 0;
        }
        else if (state == "started")
        {
          Exam_State = 1;
        }
        else if (state == "paused")
        {
          Exam_State = 2;
        }
        String time = obj["time_left"];
        if (obj["time_left"])
        {
        Demo_time = time.toInt();
        }
        String start = obj["start_point"];
        if (obj["start_point"])
        {
        Start_point = start.toInt();
        }
        String end = obj["end_point"];
        if (obj["end_point"])
        {
        End_point = end.toInt();
        }
        String task = obj["task"];
        if (obj["task"])
        {
        Task = task.toInt();
        }
      }
      if (RTDB.dataPath().startsWith("/intersections/")) {
        handleTrafficLight(RTDB.dataPath().c_str(), RTDB.to<const char *>());
      }

      // The stream event from RealtimeDatabaseResult can be converted to the values as following.
      bool v1 = RTDB.to<bool>();
      int v2 = RTDB.to<int>();
      float v3 = RTDB.to<float>();
      double v4 = RTDB.to<double>();
      string v5 = RTDB.to<string>();
    }

    else {
      Serial.println("----------------------------");
      Firebase.printf("task: %s, payload: %s\n", aResult.uid().c_str(), aResult.c_str());
    }

#if defined(ESP32) || defined(ESP8266)
    Firebase.printf("Free Heap: %d\n", ESP.getFreeHeap());
#elif defined(ARDUINO_RASPBERRY_PI_PICO_W)
    Firebase.printf("Free Heap: %d\n", rp2040.getFreeHeap());
#endif
  }
}

/* PATH FINDING ALGOOOOOOOOOOOOOOOOOOOOO */
/*Exam Firebase Data
volatile int Exam_State = 0; // 0 = Stopped || 1 = Started || 2 = Paused
volatile int Demo_time = 0;  // Time in seconds
volatile int Start_point = 0;
volatile int End_point = 0;
volatile int Task = 0;
Exam Firebase Data*/
/*----------------------------------------------------*/
StackType_t uxComputeAlgoTaskStack[configMINIMAL_STACK_SIZE];
StaticTask_t xComputeAlgoTaskTCB;
TaskHandle_t ComputeAlgoTaskTCB;

Graph g(14); // Create a graph with 14 nodes
int path[14];
int pathLength = 0;
int startpt = 11; // Example start node
int endpt = 5;   // Example end node

void ComputeAlgoTask(void *pvPara) {
  /*Setup for the task*/

  g.setupGraph();
  // ZERO INDEXED!!
  if (Task == 3 || Task == 4) {
    startpt = Start_point - 1;
    endpt = End_point - 1 ;
  }
  else {
    startpt--;
    endpt--;
  }

  g.dijkstra(startpt, endpt, path, pathLength);
  // Add back to 1 indexed
  startpt++;
  endpt++;
  for (int i = 0; i < pathLength; ++i) {
        path[i]++;
  }
  //Back to recognizable index nodes

  vTaskDelay(1000);

  Serial.print("Shortest path from ");
    Serial.print(startpt);
    Serial.print(" to ");
    Serial.print(endpt);
    Serial.print("(Pathlength is: ");
    Serial.print(pathLength);
    Serial.println("):");

    for (int i = 0; i < pathLength; ++i) {
        Serial.print(path[i]);
        Serial.print(" ");
    }
    Serial.println();
    
    /* Terminate the task */
    // vTaskDelete(NULL);

  /*DO*/
  while(true){

    while (Task == 0) {
      Serial.println("SETTING UP FIREBASE...");
      vTaskDelay(2000);
    }

    // DEBUG PRINT FOR FIREBASE
    Serial.print("Exam_State = ");
    Serial.println(Exam_State); // 0 = Stopped || 1 = Started || 2 = Paused
    Serial.print("Demo_time = ");
    Serial.println(Demo_time);  // Time in seconds
    Serial.print("Start_point = ");
    Serial.println(Start_point);
    Serial.print("End_Point = ");
    Serial.println(End_point);
    Serial.print("Task = ");
    Serial.println(Task);
    // DEBUG PRINT FOR FIREBASE

    while (startpt == Start_point && endpt == End_point ) {
      Serial.println("No Change In Path");
      vTaskDelay(4000);
    }

  // MAKING IT ZERO INDEXED!!
  if (Task == 3 || Task == 4) {
    startpt = Start_point - 1;
    endpt = End_point - 1 ;
  }
  else {
    startpt--;
    endpt--;
  }
  // ---------

  // Running the algo
  g.dijkstra(startpt, endpt, path, pathLength);
  //-----------------

  // Add back to 1 indexed
  startpt++;
  endpt++;
  for (int i = 0; i < pathLength; ++i) {
        path[i]++;
  }
  //Back to recognizable index nodes

  vTaskDelay(1000);

  Serial.print("Shortest path from ");
    Serial.print(startpt);
    Serial.print(" to ");
    Serial.print(endpt);
    Serial.print("(Pathlength is: ");
    Serial.print(pathLength);
    Serial.println("):");

    for (int i = 0; i < pathLength; ++i) {
        Serial.print(path[i]);
        Serial.print(" ");
    }
    Serial.println();
  
    vTaskDelay(1000);
  }
}


/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------RFID Reader Task-------------*/
/*--------Stack and Handle Settings---------*/

StackType_t uxRFIDTagReader[configMINIMAL_STACK_SIZE];
StaticTask_t xRFIDTagReaderTCB;
TaskHandle_t RFIDTagReaderTCB;

/*Creating the Class for RFID Reader*/
MFRC522 mfrc522(0x28, RFID_RST);

/*Function for getting the RFID Tag ID Number*/
String getTagUID() {
    String tagUID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    tagUID += mfrc522.uid.uidByte[i] < 0x10 ? "0" : "";
    tagUID += String(mfrc522.uid.uidByte[i], HEX);
  }
  return tagUID;
};

/*Define a Storage to store the previous tagUID*/
String prevRFIDTag;
int CornerNumApproach = 0;
uint8_t MapCase_LineTrack = 0;

/*User Task for RFID Tag Reader*/
void RFIDTagReader(void *pvPara) {
  /*Setup for the task*/
  mfrc522.PCD_SetAntennaGain(0x70);

    /*Do*/
    while(true) {
    
    /*DONT CHANGE THE FOLLOWING 2 LINE OF CODE*/
    if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) 
    vTaskDelay(40);

    /*You may want to setup a check new UID to save procress power
      Comparing the PrevUID and CurrentUID...*/
  
    String currenttagUID = getTagUID();
    // prevRFIDTag = currenttagUID;

    //When change in RFID
    if(currenttagUID != prevRFIDTag){
      Serial.print("RFID Tag CHANGED: ");
      Serial.println(currenttagUID);
      prevRFIDTag = currenttagUID;

      if (currenttagUID == "c0cd6791" || currenttagUID == "e0cd6691" || currenttagUID == "40ce6791" || currenttagUID == "d0ce6791")
        CornerNumApproach = 1;
      else if (currenttagUID == "404b6791" || currenttagUID == "a0406791" || currenttagUID == "10d26691" || currenttagUID == "30406791" )
        CornerNumApproach = 2;
      else if (currenttagUID == "20476891" || currenttagUID == "60d66691" || currenttagUID == "60d26691" || currenttagUID == "50d86691")
        CornerNumApproach = 3;
      else if (currenttagUID == "204e6791" || currenttagUID == "904e6791" || currenttagUID == "404d6791" || currenttagUID == "80506791")
        CornerNumApproach = 4;
      else if (currenttagUID == "50c96791" || currenttagUID == "00506791" || currenttagUID == "d0c86791" || currenttagUID == "004f6791")
        CornerNumApproach = 5;
      else if (currenttagUID == "30336891" || currenttagUID == "b0326891" || currenttagUID == "d0386891")
        CornerNumApproach = 6;
      else if (currenttagUID == "e03d6891" || currenttagUID == "901e6991" || currenttagUID == "50346891")
        CornerNumApproach = 7;
      else if (currenttagUID == "20266991" || currenttagUID == "200d6891")
        CornerNumApproach = 8;
      else if (currenttagUID == "d03c6891" || currenttagUID == "10166991" || currenttagUID == "f0316891")
        CornerNumApproach = 9;
      else if (currenttagUID == "90176991" || currenttagUID == "101e6991")
        CornerNumApproach = 10;
      else if (currenttagUID == "b02b6891" || currenttagUID == "c0226891" || currenttagUID == "b0236891")
        CornerNumApproach = 11;
      else if (currenttagUID == "00d76791" || currenttagUID == "20da6791" || currenttagUID == "a0d96791")
        CornerNumApproach = 12;
      else if (currenttagUID == "e0246891" || currenttagUID == "c0066991" || currenttagUID == "60246891" || currenttagUID == "e02d6891")
        CornerNumApproach = 13;
      else if (currenttagUID == "50466791" || currenttagUID == "403c6791" || currenttagUID == "c03c6791")
        CornerNumApproach = 14;
      // Boost code for bridge
      else if (currenttagUID == "20db6791" || currenttagUID == "a0da6791" || currenttagUID == "702b6891" || currenttagUID == "802a6891" || currenttagUID == "f0296891") 
        CornerNumApproach = 100;
      // Task 1 light junction
      else if (currenttagUID == "a0cb6791" || currenttagUID == "30d16791")
        CornerNumApproach = 90;
      // Default
      else
        CornerNumApproach = 0;

    }

    /*----------------------------------------------------*/
    // FOR DEBUG USAGE
    // Serial.print("RFID Tag: ");
    // Serial.println(currenttagUID);
    /*----------------------------------------------------*/
    /*A delay must be added inside each User Task*/
    vTaskDelay(2);
      
    }
}

/*-------------------------------------------------------------------------------------------------------------------------*/
/*------------Line Tracking Task------------*/
/*--------Stack and Handle Settings---------*/

StackType_t uxLineTrackingTaskStack[configMINIMAL_STACK_SIZE];
StaticTask_t xLineTrackingTaskTCB;
TaskHandle_t LineTrackingTaskTCB;
int pathNodePast = 0;
/*User Task for Line Tracking*/
void LineTrackingTask(void *pvPara) {
  /*Setup for the task*/

  /*DO*/
  while(true){
    //if (Task == 2 || Task == 3 || Task == 4) {
      while (Exam_State == 0) {
        //CornerNumApproach = -1;
        Serial.println("not started stopped. -1");
        int gay = -1;
        LineTracking::FollowingLine(IR::Tracking(), LeftSpeed, RightSpeed, Task, gay);
        vTaskDelay(300);
      }
    //}
    LineTracking::FollowingLine(IR::Tracking(), LeftSpeed, RightSpeed, Task, CornerNumApproach);
    // delay(1);

    /*A delay must be added inside each User Task*/    
    vTaskDelay(2);
  }
}

/*-------------------------------------------------------------------------------------------------------------------------*/
/*------------FireBase Task------------*/
/*--------Stack and Handle Settings---------*/
StackType_t uxFireBaseTaskStack[configMINIMAL_STACK_SIZE];
StaticTask_t xFireBaseTaskTCB;
TaskHandle_t FireBaseTaskTCB;

void FireBaseTask(void *pvPara) {
  /*Setup for the task*/
  Firebase.printf("Firebase Client v%s\n", FIREBASE_CLIENT_VERSION);
  Serial.println("Initializing app...");
  ssl_client1.setInsecure();
  ssl_client2.setInsecure();
  app.setCallback(asyncCB);
  // The async client used for auth task should not use for stream task
  initializeApp(aClient2, app, getAuth(user_auth));
  // Waits for app to be authenticated.
  // For asynchronous operation, this blocking wait can be ignored by calling app.loop() in loop().
  ms = millis();
  while (app.isInitialized() && !app.ready() && millis() - ms < 120 * 1000);
  app.getApp<RealtimeDatabase>(Database);
  Database.url(DATABASE_URL);
  Database.get(aClient, "/", asyncCB, true /* SSE mode (HTTP Streaming) */);

  while (true) {

    app.loop();
    // This required when different AsyncClients than used in FirebaseApp assigned to the Realtime database functions.
    Database.loop();



    // TRAFFIC LIGHT ADJUSTMENTS!
    if (Task == 2) {
      // 0 = red || 1 = green || 2 = yellow
      if (CornerNumApproach == 3) {
        while (intersectionLights.intersectionLights[1].state == 0) {
          CornerNumApproach = -1;
          LineTracking::FollowingLine(IR::Tracking(), LeftSpeed, RightSpeed, Task, CornerNumApproach);
          delay(300);
        }
        CornerNumApproach = 3;
        LineTracking::FollowingLine(IR::Tracking(), LeftSpeed, RightSpeed, Task, CornerNumApproach);
      }
      /*
      else {
        CornerNumApproach = 3;
        LineTracking::FollowingLine(IR::Tracking(), LeftSpeed, RightSpeed, Task, CornerNumApproach);
      }
      */
      
    }

    // Debug for Firebase --------------
    /*
    Serial.print("Exam_State = ");
    Serial.println(Exam_State); // 0 = Stopped || 1 = Started || 2 = Paused
    Serial.print("Demo_time = ");
    Serial.println(Demo_time);  // Time in seconds
    Serial.print("Start_point = ");
    Serial.println(Start_point);
    Serial.print("End_Point = ");
    Serial.println(End_point);
    Serial.print("Task = ");
    Serial.println(Task);
    */
    //---------------------------------
    /*A delay must be added inside each User Task*/
    vTaskDelay(5);
  }
}



/*WE DO NOT USE THE CODE BELOW*/
/*WE DO NOT USE THE CODE BELOW*/
/*WE DO NOT USE THE CODE BELOW*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*PID Control for Motor
  PID Function and Settings
  Creating PID_t for Mult. PID Setting
  PID 1 for LeftWheel
  PID 2 for RightWheel */
struct PID_t
{
  /*Creating the parameters for PID*/
  volatile float Kp;
  volatile float Ki;
  volatile float Kd;

  volatile float target_val; // The target RPM
  float actual_val;          // Actual RPM Reading
  float err;                 // Error
  float err_last;
  float integral;

  /*General PID Function*/
  float PID_realize(float temp_val)
  {
    this->err = this->target_val - temp_val;

    this->integral += this->err;

    this->actual_val = this->Kp * this->err + this->Ki * this->integral + this->Kd * (this->err - this->err_last);

    this->err_last = this->err;

    return this->actual_val;
  }

} PID;

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------RPM Measure Task-------------*/
/*--------Stack and Handle Settings---------*/
StackType_t uxcalculateRPMTaskStack[configMINIMAL_STACK_SIZE];
StaticTask_t xcalculateRPMTaskTCB;
TaskHandle_t calculateRPMTaskTCB;

/*Constants for Encoder
  Find out the encoder resolution by yourself */
const int encoderResolution = 320;  // Number of pulses per revolution
const unsigned long interval = 100; // Time interval in milliseconds 100ms

/*Encoder to RPM Function and Settings
  Creating RPMCounter_t for 2 Wheel Setting
  */
typedef struct RPMCounter_t
{

  volatile int encoderPulses;
  unsigned long previousMillis;
  volatile float rpm;

  float RPMCounter()
  {
    unsigned long currentMillis = millis();

    // Check if the time interval has elapsed
    if (currentMillis - previousMillis >= interval)
    {
      // Calculate RPM
      float rotations = float(encoderPulses) / ((float)encoderResolution);
      float time = (currentMillis - previousMillis) / 100.0f; // Convert to seconds
      float rpm = (rotations / time) * 60.0f;

      // Reset encoder pulse count and update previousMillis
      encoderPulses = 0;
      previousMillis = currentMillis;

      // Print RPM
      // Serial.println(rpm);
      vTaskDelay(100 / portTICK_PERIOD_MS); // Delay for 0.1 second
      return rpm;
    }
  }
} RPM;

/*Define 2 Sets of Variables using RPMCounter_t for 2 Wheel
  Init the RPM related Variables before the task starts   */
RPMCounter_t LeftMotor = {0, 0, 0};
RPMCounter_t RightMotor = {0, 0, 0};

// Interrupt Service Routine
// Create a struct to handle 2 motors encoder
struct Encoder_t
{
  int pinAState;
  int pinBState;
  int Encoder_A;
  int Encoder_B;

} Encoder;

/*Init the Enocoder related Variables before the task starts*/
Encoder_t EncoderLeft = {0, 0, Motor_L_Encoder_A, Motor_L_Encoder_B};
Encoder_t EncoderRight = {0, 0, Motor_R_Encoder_A, Motor_R_Encoder_B};

/*-------------------------------------------------------------------------------------------------------------------------*/
/*Interrupt Service Routine Function
  Since attachInterrupt() cannot using non Static function
  Below are 2 IRAM_ATTR function for handle the interrupts for the encoder*/
void IRAM_ATTR handleLeftEncoderInterrupt()
{
  // init the local variable
  int change = 0;

  // Read the current state of the encoder pins
  EncoderLeft.pinAState = digitalRead(EncoderLeft.Encoder_A);
  EncoderLeft.pinBState = digitalRead(EncoderLeft.Encoder_B);

  // Determine the direction of rotation based on the phase change
  if (EncoderLeft.pinAState != EncoderLeft.pinBState)
  {
    change = (EncoderLeft.pinAState == HIGH) ? 1 : 0;
  }
  else
  {
    change = (EncoderLeft.pinAState == HIGH) ? 0 : 1;
  }

  // Update the encoder count
  LeftMotor.encoderPulses += change;
}

void IRAM_ATTR handleRightEncoderInterrupt()
{
  // init the local variable
  int change = 0;

  // Read the current state of the encoder pins
  EncoderRight.pinAState = digitalRead(EncoderRight.Encoder_A);
  EncoderRight.pinBState = digitalRead(EncoderRight.Encoder_B);

  // Determine the direction of rotation based on the phase change
  if (EncoderRight.pinAState != EncoderRight.pinBState)
  {
    change = (EncoderRight.pinAState == HIGH) ? 1 : 0;
  }
  else
  {
    change = (EncoderRight.pinAState == HIGH) ? 0 : 1;
  }

  // Update the encoder count
  RightMotor.encoderPulses += change;
}

/*-------------------------------------------------------------------------------------------------------------------------*/
/*-------------RPM Measure User Task-------------*/

void calculateRPMTask(void *pvPara)
{
  /*Setup for the Task*/
  /*----------------------------------------------------*/
  /*Define 2 sets PID for 2 Motors*/
  RPMCounter_t TargetRPM;
  /*Change the PID Para. here
    LeftMotor PID*/
  PID_t pid1 = {10.0f // Kp
                ,
                0.3f // Ki
                ,
                0.0f}; // Kd

  /*RightMotor PID*/
  PID_t pid2 = {8.0f // Kp
                ,
                0.5f // Ki
                ,
                0.0f}; // Kd

  /*Set the initial Target RPM Here*/
  pid1.target_val = 250.0f;
  pid2.target_val = 250.0f;
  /*----------------------------------------------------*/
  while (true)
  {
    /*----------------------------------------------------*/
    /*FOR DEBUG USAGE*/
    // Serial.print("RPM Left: ");
    // Serial.println(LeftMotor.RPMCounter());

    // Serial.print("RPM Right: ");
    // Serial.println(RightMotor.RPMCounter());
    /*----------------------------------------------------*/
    /*Setting the actual value to PID function*/
    pid1.actual_val = LeftMotor.RPMCounter();
    pid2.actual_val = RightMotor.RPMCounter();

    /*Compute the PID and Write the Result to Speed of the Wheel*/
    LeftSpeed = Motor::RPMtoPWM(pid1.PID_realize(LeftMotor.RPMCounter()), LeftWheel);
    RightSpeed = Motor::RPMtoPWM(pid1.PID_realize(RightMotor.RPMCounter()), RightWheel);

    /*----------------------------------------------------*/
    /*FOR DEBUG USAGE*/
    // Serial.print("Speed Left: ");
    // Serial.println(LeftSpeed);

    // Serial.print("Speed Right: ");
    // Serial.println(RightSpeed);
    /*----------------------------------------------------*/
    /*A delay must be added inside each User Task*/
    vTaskDelay(200);
  }
}
/*WE DO NOT USE THE CODE ABOVE*/
/*WE DO NOT USE THE CODE ABOVE*/
/*WE DO NOT USE THE CODE ABOVE*/



/*DO NOT CHANGE THE CODE BELOW*/
/*----------------------------------------------------*/
/*------------LED Blinking Task-------------
  --------Stack and Handle Settings---------
To ensure there is visualization that the program is running*/
StackType_t uxBlinkTaskStack[configMINIMAL_STACK_SIZE];
StaticTask_t xBlinkTaskTCB;
TaskHandle_t BlinkTaskTCB;

void Blink(void *pvPara) {
  /*Setup for the task*/
  pinMode(LED1, OUTPUT);
  /*DO*/
  while(true){
  digitalWrite(LED1,HIGH);
  vTaskDelay(100);
  digitalWrite(LED1, LOW);
  vTaskDelay(200);

  Serial.print("Current Task: ");
  Serial.println(Task);
  Serial.print("Current CornerNumApproach: ");
  Serial.println(CornerNumApproach);
  vTaskDelay(20);


  /* Some cheeky code for ComputeAlgo
    Serial.print("Shortest path from ");
    Serial.print(startpt);
    Serial.print(" to ");
    Serial.print(endpt);
    Serial.print("(Pathlength is: ");
    Serial.print(pathLength);
    Serial.println("):");

    for (int i = 0; i < pathLength; ++i) {
        Serial.print(path[i]);
        Serial.print(" ");
    }
    Serial.println();
    */

  }
}
/*----------------------------------------------------*/

/*------------Connect to Wifi-------------
  --------Stack and Handle Settings---------
To ensure there is visualization that the program is running*/
StackType_t uxWifitaskTaskStack[configMINIMAL_STACK_SIZE];
StaticTask_t xWifitaskTCB;
TaskHandle_t WifitaskTCB;

void Wifitask(void *pvPara) {
  esp_task_wdt_init(30, false);
  /*Setup for the task*/
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    vTaskDelay(100);
  }
  Serial.println("\nConnected to Wi-Fi");

  while (true)
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
      Serial.print("Reconnecting");
    }
    vTaskDelay(10);
  }
}



void setup() {
// put your setup code here, to run once:
Serial.begin(115200);
Serial.println("---------Initializing...---------");
//Set up PWM Channel for Motor
Motor::Init();
Serial.println("Wheel Motors Initialized");

//Set up PWM Channel for Servo Motor
Servo::Init();
Serial.println("Servo Motor Initialized");

//Initialize IR Sensor
IR::Init();
Serial.println("IR Sensor Initialized");

//Initialize RFID Reader 
Wire.begin(RFID_SDA, RFID_SCL);
mfrc522.PCD_Init();

Serial.println("RFID Initialized");
//Initialize IMU

//Initialize the FireBase Connection
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    vTaskDelay(100);
  }
  Serial.println("\nConnected to Wi-Fi");

// Serial.println("FireBase Initialized");


// Init the PinMode for the Encoder Pins 
pinMode(Motor_L_Encoder_A, INPUT_PULLUP);
pinMode(Motor_L_Encoder_B, INPUT_PULLUP);

pinMode(Motor_R_Encoder_A, INPUT_PULLUP);
pinMode(Motor_R_Encoder_B, INPUT_PULLUP);

// Attach the interrupt service routine to the encoder pins
//attachInterrupt(digitalPinToInterrupt(Motor_L_Encoder_A), handleLeftEncoderInterrupt, CHANGE);
//attachInterrupt(digitalPinToInterrupt(Motor_R_Encoder_A), handleRightEncoderInterrupt, CHANGE);
Serial.println("Interrupt Pins Initialized");

Serial.println("---------Initialized---------");

/*FreeRTOS Task Pinned to core*/
/*Do not change the config of the core
  Events Run on Core:   Core 0 (For FreeRTOS)
  Arduino Runs on Core: Core 1 (As Default)

  Run Core Tasks Config:
  Core 0: local  task  (Control)
  Core 1: online task (Firebase)*/
  /*xTaskCreatePinnedtoCore: pin the specific task to desired core (esp32 is a dual cores MCU)
  xTaskCreatePinnedToCore(  void((void *pvPara)), Text for the task, Stack (Min. is 1024), const para. , &TaskTCB, uxPriority, Core )*/
  xTaskCreatePinnedToCore(FireBaseTask, "FireBase", 10000, NULL, 3, &FireBaseTaskTCB, 0);
  // xTaskCreatePinnedToCore(Wifitask, "Connect to Wifi", 10000, NULL, 3, &WifitaskTCB, 0);
  xTaskCreatePinnedToCore(Blink, "Blink", 2048, NULL, 1, &BlinkTaskTCB, 1);
  xTaskCreatePinnedToCore(RFIDTagReader, "RFIDReader", 2048, NULL, 3, &RFIDTagReaderTCB, 1);
  xTaskCreatePinnedToCore(LineTrackingTask, "LineTracking", 10000, NULL, 3, &LineTrackingTaskTCB, 1);
  //xTaskCreatePinnedToCore(calculateRPMTask, "calculteRPM", 10000, NULL, 1, &calculateRPMTaskTCB, 1);
  xTaskCreatePinnedToCore(ComputeAlgoTask, "ComputeAlgo", 10000, NULL, 1, &ComputeAlgoTaskTCB, 1);
/*Adding a small delay for the setup()*/

vTaskDelay(5);
}


/*Nothing will run in loop()
  Please do not write any code inside the loop()*/
void loop() {
}

