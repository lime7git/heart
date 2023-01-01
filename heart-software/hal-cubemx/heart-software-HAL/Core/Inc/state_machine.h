#ifndef __STATE_MACHINE_H__
#define __STATE_MACHINE_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tca6416a.h"
#include "accelerometer.h"
#include "rtc.h"
#include "sequencies.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "stdbool.h"

typedef enum 
{
	STATE_NULL,
	STATE_INITIALIZATION,
	STATE_IDLE,
	STATE_SEQUENCY_RUNNING,
	STATE_SLEEP,
	STATE_LOW_BATTERY,
	STATE_FAULT
	
} eHEART_STATE;

void State_Machine_Update(eHEART_STATE *state);
void State_Machine_Set_Next_State(eHEART_STATE *current_state, eHEART_STATE new_state);

#endif /* __STATE_MACHINE_H__ */
