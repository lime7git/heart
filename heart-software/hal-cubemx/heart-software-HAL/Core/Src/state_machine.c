#include "state_machine.h"

extern bool button_was_pressed;
extern uint32_t button_pressed_time;
uint32_t timer = 0;

void State_Machine_Update(eHEART_STATE *state)
{
	switch(*state)
		{
			case STATE_INITIALIZATION:
			{
			
				if((TCA6416A_Initialization() == HAL_OK) && (accelerometer_init() == true))	// initialize gpio expadner and accelerometer
				{
					TCA6416A_Disable_All_LEDs();
					State_Machine_Set_Next_State(state, STATE_IDLE);
				}
				else
				{
					State_Machine_Set_Next_State(state, STATE_FAULT);
				}
				
			
			break;
			}
			
			case STATE_IDLE:
			{
				//static uint32_t timer = 0;
				
				if(timer >= 100)
				{
					timer = 0;
					State_Machine_Set_Next_State(state, STATE_SLEEP);
					return;
				}
				
				if(button_was_pressed || accelerometer_shake_detection())
				{
					timer = 0;
					button_was_pressed = false;
					State_Machine_Set_Next_State(state, STATE_SEQUENCY_RUNNING);
				}
				else
				{
					timer++;
				}
			break;
			}
			case STATE_SEQUENCY_RUNNING:
			{
					static int previous_random = 0;
					static int random_number = 0;
				
					srand(button_pressed_time);
					do{
							random_number = rand() % ((SEQUENCY_LAST - 1) - SEQUENCY_1 + 1) + SEQUENCY_1;
					}while(random_number == previous_random);
					
					previous_random = random_number;
					
					Set_Sequency((SEQUENCIES)random_number);
					Run_Current_Sequency();
					
					State_Machine_Set_Next_State(state, STATE_IDLE);
			break;
			}
			
			case STATE_SLEEP:
			{
				TCA6416A_Disable_All_LEDs();
				accelerometer_enter_low_power();
				
				HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
				HAL_SuspendTick();
				HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON,PWR_STOPENTRY_WFI); // 25uA
				
				HAL_ResumeTick();
				
				if(*state != STATE_SLEEP) break;
				
				if((TCA6416A_Initialization() == HAL_OK) && (accelerometer_init() == true))	// initialize gpio expadner and accelerometer
				{
					TCA6416A_Disable_All_LEDs();
					State_Machine_Set_Next_State(state, STATE_IDLE);
				}
				else
				{
					State_Machine_Set_Next_State(state, STATE_FAULT);
				}
			
			break;
			}
			
			case STATE_LOW_BATTERY:
			{
				static bool doOnce = true;
				
				if(doOnce) 
				{
					TCA6416A_Disable_All_LEDs();
					accelerometer_enter_low_power();
					doOnce = false;
				}
				
				HAL_PWR_EnablePVD();
				
				
				if(__HAL_PWR_GET_FLAG(PWR_FLAG_PVDO) == RESET)
				{
					State_Machine_Set_Next_State(state, STATE_INITIALIZATION);	
					doOnce = true;
					return;
				}
				
				TCA6416A_Write(TCA6416A_OUTPUT_PORT_0_ADDRESS, 127);
				HAL_Delay(500);
				TCA6416A_Disable_All_LEDs();
				HAL_Delay(500);
				TCA6416A_Write(TCA6416A_OUTPUT_PORT_0_ADDRESS, 127);
				HAL_Delay(500);
				TCA6416A_Disable_All_LEDs();
				
				
				HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 0x61A8, RTC_WAKEUPCLOCK_RTCCLK_DIV16);
				HAL_SuspendTick();
				
				//HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON,PWR_STOPENTRY_WFI); // w sleep modzie na takiej konfiguracji bylo zuzycie na poziomie 50uA
				HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON,PWR_STOPENTRY_WFI); // 25uA
				
				HAL_ResumeTick();
				HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
				
			break;
			}
			
			default: 
				State_Machine_Set_Next_State(state, STATE_IDLE);		
			break;
		}
}

void State_Machine_Set_Next_State(eHEART_STATE *current_state, eHEART_STATE new_state)
{
	*current_state = new_state;
}
