/**
 * @file sequencies.c
 * @author lime7git
 * @date 16.02.2022
 * @brief sequencies source file
 */

#include "sequencies.h"

sSEQUENCY SEQUENCY;

void Sequency_Initialization(void)
{
	SEQUENCY.is_sequency_running = false;
	SEQUENCY.current_sequency = SEQUENCY_NONE;
	SEQUENCY.previous_sequency = SEQUENCY_NONE;
	SEQUENCY.number_of_sequencies = SEQUENCY_LAST - 1;
}

void Set_Sequency(SEQUENCIES sequency)
{
	SEQUENCY.previous_sequency = Get_Current_Sequency();
	SEQUENCY.current_sequency = sequency;
}

SEQUENCIES Get_Current_Sequency(void)
{
	return SEQUENCY.current_sequency;
}

void Run_Current_Sequency(void)
{
	if(!SEQUENCY.is_sequency_running)
	{		
		SEQUENCY.is_sequency_running = true;
		SEQUENCY.previous_sequency = Get_Current_Sequency();
		
		switch (Get_Current_Sequency())
		{
			case SEQUENCY_NONE : {
				TCA6416A_Disable_All_LEDs();
			break;
			}
			
			case SEQUENCY_1 : {
				for(uint8_t i = 0; i < 8; i++)
				{
					TCA6416A_Write(TCA6416A_OUTPUT_PORT_1_ADDRESS, (0xFF & ~(1 << i)));
					HAL_Delay(25);
				}
				TCA6416A_Write(TCA6416A_OUTPUT_PORT_1_ADDRESS, 0xFF);
				
				for(uint8_t i = 0; i < 8; i++)
				{
					TCA6416A_Write(TCA6416A_OUTPUT_PORT_0_ADDRESS, (0xFF & ~(1 << i)));
					HAL_Delay(25);
				}
				TCA6416A_Write(TCA6416A_OUTPUT_PORT_0_ADDRESS, 0xFF);
			break;
			}
			
			case SEQUENCY_2 : {
				
				
			break;
			}
			
			case SEQUENCY_3 : {
				
				
			break;
			}
			
			case SEQUENCY_4 : {
				
				
			break;
			}
			
			case SEQUENCY_5 : {
				
				
			break;
			}
			
			case SEQUENCY_6 : {
				
				
			break;
			}
			
			case SEQUENCY_7 : {
				
				
			break;
			}
			
			case SEQUENCY_8 : {
				
				
			break;
			}
			
			case SEQUENCY_9 : {
				
				
			break;
			}
			
			case SEQUENCY_10 : {
				
				
			break;
			}
			
			case SEQUENCY_LAST : {
				
				
			break;
			}
			default : {
				
			}
		}
		
		Set_Sequency(SEQUENCY_NONE);
		SEQUENCY.is_sequency_running = false;
		TCA6416A_Disable_All_LEDs();
	}
}
