#ifndef __ACCELEROMETER_H__
#define __ACCELEROMETER_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "lis3dh_reg.h"
#include "stdbool.h"
#include <string.h>

int32_t platform_write(void *handle, uint8_t reg, const uint8_t *bufp,	uint16_t len);
int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len);
bool accelerometer_init(void);
bool accelerometer_enter_low_power(void);
bool accelerometer_shake_detection(void);
float accelerometer_get_total(void);

#endif /* __ACCELEROMETER_H__ */
