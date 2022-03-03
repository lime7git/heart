/**
 * @file sequencies.c
 * @author lime7git
 * @date 16.02.2022
 * @brief sequencies source file
 */

#include "sequencies.h"

volatile sSEQUENCY SEQUENCY;

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
		TCA6416A_Disable_All_LEDs();
		
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
				for(int i = 7; i >= 0; i--)
				{
					TCA6416A_Write(TCA6416A_OUTPUT_PORT_0_ADDRESS, (0xFF & ~(1 << i)));
					HAL_Delay(25);
				}
				TCA6416A_Write(TCA6416A_OUTPUT_PORT_0_ADDRESS, 0xFF);
				
				for(int i = 7; i >= 0; i--)
				{
					TCA6416A_Write(TCA6416A_OUTPUT_PORT_1_ADDRESS, (0xFF & ~(1 << i)));
					HAL_Delay(25);
				}
				TCA6416A_Write(TCA6416A_OUTPUT_PORT_1_ADDRESS, 0xFF);
			break;
			}
			
			case SEQUENCY_3 : {
				TCA6416A_Enable_All_LEDs();
				HAL_Delay(250);
			break;
			}
			
			case SEQUENCY_4 : {
				for(uint8_t i = 0; i < 8; i++)
				{
					TCA6416A_Write(TCA6416A_OUTPUT_PORT_0_ADDRESS, (0xFF & ~(1 << i)));
					TCA6416A_Write(TCA6416A_OUTPUT_PORT_1_ADDRESS, (0xFF & ~(1 << i)));
					HAL_Delay(75);
				}
				TCA6416A_Disable_All_LEDs();
				
				for(uint8_t i = 0; i < 8; i++)
				{
					TCA6416A_Write(TCA6416A_OUTPUT_PORT_0_ADDRESS, (0xFF & ~(1 << i)));
					TCA6416A_Write(TCA6416A_OUTPUT_PORT_1_ADDRESS, (0xFF & ~(1 << i)));
					HAL_Delay(75);
				}
				TCA6416A_Disable_All_LEDs();
				
				for(int i = 7; i >= 0; i--)
				{
					TCA6416A_Write(TCA6416A_OUTPUT_PORT_0_ADDRESS, (0xFF & ~(1 << i)));
					TCA6416A_Write(TCA6416A_OUTPUT_PORT_1_ADDRESS, (0xFF & ~(1 << i)));
					HAL_Delay(25);
				}
				TCA6416A_Disable_All_LEDs();
				
				for(int i = 7; i >= 0; i--)
				{
					TCA6416A_Write(TCA6416A_OUTPUT_PORT_0_ADDRESS, (0xFF & ~(1 << i)));
					TCA6416A_Write(TCA6416A_OUTPUT_PORT_1_ADDRESS, (0xFF & ~(1 << i)));
					HAL_Delay(25);
				}
				TCA6416A_Disable_All_LEDs();
			break;
			}
			
			case SEQUENCY_5 : {
				for(int i = 0; i < 3; i++)
				{			
					TCA6416A_Enable_All_LEDs();
					HAL_Delay(500);
					
					TCA6416A_Disable_All_LEDs();
					HAL_Delay(500);
				}	
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
			
			case SEQUENCY_INIT : {
				for(int i = 0; i < 8; i ++)
				{
					TCA6416A_Write(TCA6416A_OUTPUT_PORT_0_ADDRESS, (0xFF & ~(1 << i)));
					TCA6416A_Write(TCA6416A_OUTPUT_PORT_1_ADDRESS, (0xFF & ~(1 << abs((i - 7)))));
					HAL_Delay(150);
				}
				for(int i = 7; i >= 0; i--)
				{
					TCA6416A_Write(TCA6416A_OUTPUT_PORT_0_ADDRESS, (0xFF & ~(1 << i)));
					TCA6416A_Write(TCA6416A_OUTPUT_PORT_1_ADDRESS, (0xFF & ~(1 << abs((i - 7)))));
					HAL_Delay(150);
				}
				
				TCA6416A_Disable_All_LEDs();			
				HAL_Delay(150);
				TCA6416A_Enable_All_LEDs();			
				HAL_Delay(1000);
				
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
