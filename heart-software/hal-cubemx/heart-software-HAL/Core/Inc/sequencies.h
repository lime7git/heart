#ifndef __SEQUENCIES_H__
#define __SEQUENCIES_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tca6416a.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "stdbool.h"

typedef enum 
{ 
    SEQUENCY_NONE, 
    SEQUENCY_1, 
    SEQUENCY_2,
		SEQUENCY_3,
		SEQUENCY_4,
		SEQUENCY_5,
		SEQUENCY_6,
		SEQUENCY_7,
		SEQUENCY_8,
		SEQUENCY_9,
		SEQUENCY_10,
		SEQUENCY_LAST,
		SEQUENCY_INIT
	
} SEQUENCIES;

typedef struct{
	
	bool is_sequency_running;
	SEQUENCIES current_sequency;
  SEQUENCIES previous_sequency;
	uint8_t number_of_sequencies;
	
} sSEQUENCY;

/**
  * @brief  Sequency structure initialization
  * @param  None
  * @retval None
  */
void Sequency_Initialization(void);

/**
  * @brief  Set new sequency
  * @param  Sequency
  * @retval None
  */
void Set_Sequency(SEQUENCIES sequency);

/**
  * @brief  Get current sequency
  * @param  None
  * @retval Current sequency
  */
SEQUENCIES Get_Current_Sequency(void);

/**
  * @brief  Get number of sequencies
  * @param  None
  * @retval Current sequency
  */
int Get_Number_of_Sequencies(void);

/**
  * @brief  Is sequency running
  * @param  None
  * @retval True/false sequency running
  */
bool Is_Sequency_Running(void);

/**
  * @brief  Run current sequency
  * @param  None
  * @retval None
  */
void Run_Current_Sequency(void);



#endif /* __SEQUENCIES_H__ */
