#include <stdint.h>

#define START_FRAME         0xABCD     	// [-] Start frme definition for reliable serial communication

int batLevel_1;
int batLevel_2;

int16_t wheelspeed;
int32_t speedKmh;
uint8_t mode = 0;
uint8_t turbo = 0;

uint8_t idx = 0;                        // Index for new data pointer
uint16_t bufStartFrame;                 // Buffer Start Frame
byte *p;                                // Pointer declaration for the new received data
byte incomingByte;
byte incomingBytePrev;

typedef struct{
   uint16_t start;
   uint8_t  cmd1;
   uint8_t  cmd2;
   int16_t  steer;
   int16_t  speed;
   uint8_t  sensors;
   uint16_t checksum;
} SerialCommand;
SerialCommand Command;

typedef struct{
   uint16_t start;
   int16_t  cmd1;
   int16_t  cmd2;
   int16_t  speedR_meas;
   int16_t  speedL_meas;
   int16_t  batVoltage;
   int16_t  boardTemp;
   uint16_t cmdLed;
   uint16_t checksum;

} SerialFeedback;
SerialFeedback Feedback_driver_1;
SerialFeedback Feedback_driver_2;
SerialFeedback NewFeedback;


/// ESP variables etc. ///

// Replace with your network credentials (STATION)
const char* ssid = "ESP-Dashboard";
const char* password = "12345";

// Data stucture for  received/transmited packet 
// Must match the sender structure
typedef struct incoming_msg_struct {
    int16_t  uFrontLeft;
    int16_t  uFrontRight;
    int16_t  uRearLeft;
    int16_t  uRearRight;
    uint8_t  uMode;
    uint8_t  uPowerState;
} incoming_msg_struct;

// Create a struct_message to hold incoming sensor readings
incoming_msg_struct incomingMsg;

typedef struct output_msg_struct {
    int16_t     uBat1;
    int16_t     uBat2;
    uint8_t     uStatus;
} output_msg_struct;

// Create a struct_message to hold incoming sensor readings
output_msg_struct feedbackMsg;
