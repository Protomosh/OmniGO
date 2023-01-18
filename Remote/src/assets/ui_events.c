#include "ui.h"
#include "data_struct.h"

extern outgoing_msg_struct outputMsg;

void colorPick(lv_event_t * e)
{
	// Your code here
}

void audio_test(lv_event_t * e)
{
	// Your code here
}

void motorPowerOn(lv_event_t * e)
{
	outputMsg.uPowerState   = 0;
	LV_LOG_USER("Motor power OFF");
}

void powerOff(lv_event_t * e)
{
	outputMsg.uPowerState   = 1;
	LV_LOG_USER("Motor power 0N");
}

void ExternalControl(lv_event_t * e)
{
	LV_LOG_USER("Control mode: EXTERNAL");
}

void LocalControl(lv_event_t * e)
{
	LV_LOG_USER("Control mode: LOCAL");
}