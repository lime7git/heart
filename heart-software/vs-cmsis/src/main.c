/**
 * @file main.c
 * @author lime7git
 * @date 16.02.2022
 * @brief Main source file
 */

#include "main.h"

//uint8_t read_buffer;
uint32_t tick_debug;
extern uint32_t clock_after;

int main(void)
{
    
    Clock_Initialization();
   // I2C_Initialization();
   // TCA6416A_Initialization();
    
    // I2C_Read(LIS3DH_READ_ADDRESS, LIS3DH_WHO_AM_I_REGISTER, &read_buffer, 1);

    while(true)
    {
        tick_debug++;
        Delay_Miliseconds(2000);
    }
}
