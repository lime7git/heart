/**
 * @file tca6416a.h
 * @author lime7git
 * @date 16.02.2022
 * @brief GPIO expander header file
 */

/* File tca6416a  */
#ifndef _TCA6416A_H
#define _TCA6416A_H

#include "stm32l031xx.h"
#include "stm32l0xx.h"

#include "i2c.h"

#define TCA6416A_WRITE_ADDRESS 0x40
#define TCA6416A_READ_ADDRESS 0x41

#define TCA6416A_INPUT_PORT_0_ADDRESS 0x00
#define TCA6416A_INPUT_PORT_1_ADDRESS 0x01

#define TCA6416A_OUTPUT_PORT_0_ADDRESS 0x02
#define TCA6416A_OUTPUT_PORT_1_ADDRESS 0x03

#define TCA6416A_POLARITY_PORT_0_ADDRESS 0x04
#define TCA6416A_POLARITY_PORT_1_ADDRESS 0x05

#define TCA6416A_CONFIGURATION_0_ADDRESS 0x06
#define TCA6416A_CONFIGURATION_1_ADDRESS 0x07

void TCA6416A_Initialization(void);

void TCA6416A_Write(uint8_t register_address, uint8_t *data_to_transfer);
void TCA6416A_Read(uint8_t register_address, uint8_t *data_received);

void TCA6416A_Set_Port_0_As_Output(void);
void TCA6416A_Set_Port_1_As_Output(void);

#endif /* !TCA6416A_H */