#ifndef _DATA_STRUCT_H
#define _DATA_STRUCT_H

#include <stdint.h>

//// Variables ////
bool boot_loading_ready;
uint8_t left_btn_prev;                       // last state of left button
uint8_t right_btn_prev;                      // last state of right button
uint8_t speed_mode;                          // platform speed mode


//Structure example to send data
//Must match the receiver structure
typedef struct incoming_msg_struct {
    int16_t  uBat1;
    int16_t  uBat2;
    int16_t  uSpeed;
    uint8_t  uStatus;
    //uint16_t uSonar;

} incoming_msg_struct;

// Create a struct_message to hold outgoing sensor readings
// incoming_msg_struct outputMsg;

typedef struct outgoing_msg_struct {
    int16_t  uFrontLeft;
    int16_t  uFrontRight;
    int16_t  uRearLeft;
    int16_t  uRearRight;
    uint8_t  uMode;
    uint8_t  uPowerState;
    //uint16_t uLight;

//    uint16_t uLight;
} outgoing_msg_struct;

// Create a struct_message to hold out going sensor readings
// outgoing_msg_struct incomingMsg;
#endif
