#include "accelerometer.h"

	stmdev_ctx_t dev_ctx;
	lis3dh_ctrl_reg3_t ctrl_reg3;
  lis3dh_click_cfg_t click_cfg;
	
	int16_t data_raw_acceleration[3];
	float acceleration_mg[3];
  uint8_t whoamI;

	uint8_t count = 0;
	float meanX = 0.0f;
	float meanY = 0.0f;
	float meanZ = 0.0f;
	float total_accelerometer;

	float roll_threshold = 750;
	uint8_t shake_detected = 0;

int32_t platform_write(void *handle, uint8_t reg, const uint8_t *bufp, uint16_t len)
{
	reg |= 0x80;
  HAL_I2C_Mem_Write(handle, LIS3DH_I2C_ADD_L, reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*) bufp, len, 1000);
	
	return 0;
}
int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len)
{
	reg |= 0x80;
  HAL_I2C_Mem_Read(handle, LIS3DH_I2C_ADD_L, reg,
                   I2C_MEMADD_SIZE_8BIT, bufp, len, 1000);
	
	return 0;
}

bool accelerometer_init(void)
{
	dev_ctx.write_reg = platform_write;
  dev_ctx.read_reg = platform_read;
  dev_ctx.handle = &hi2c1;
	
	lis3dh_device_id_get(&dev_ctx, &whoamI);

  if (whoamI != LIS3DH_ID) {
    return false;
  }
	
	 /* Enable Block Data Update. */
  lis3dh_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);
  /* Set Output Data Rate to 1Hz. */
  lis3dh_data_rate_set(&dev_ctx, LIS3DH_ODR_100Hz);
  /* Set full scale to 2g. */
  lis3dh_full_scale_set(&dev_ctx, LIS3DH_2g);
  /* Set device in continuous mode with 12 bit resol. */
  lis3dh_operating_mode_set(&dev_ctx, LIS3DH_HR_12bit);
	
  lis3dh_pin_sdo_sa0_mode_set(&dev_ctx, LIS3DH_PULL_UP_CONNECT);
	
	return true;
}

bool accelerometer_enter_low_power(void)
{
	lis3dh_device_id_get(&dev_ctx, &whoamI);

  if (whoamI != LIS3DH_ID) {
    return false;
  }
	
	lis3dh_block_data_update_set(&dev_ctx, LIS3DH_POWER_DOWN);
	lis3dh_aux_adc_set(&dev_ctx, LIS3DH_AUX_DISABLE);
	lis3dh_pin_sdo_sa0_mode_set(&dev_ctx, LIS3DH_PULL_UP_CONNECT);
	
	return true;
}

bool accelerometer_shake_detection(void)
{
		bool ret = false;
		lis3dh_reg_t reg;
    /* Read output only if new value available */
    lis3dh_xl_data_ready_get(&dev_ctx, &reg.byte);

    if (reg.byte) {
      /* Read accelerometer data */
      memset(data_raw_acceleration, 0x00, 3 * sizeof(int16_t));
      lis3dh_acceleration_raw_get(&dev_ctx, data_raw_acceleration);
      acceleration_mg[0] =
        lis3dh_from_fs2_hr_to_mg(data_raw_acceleration[0]);
      acceleration_mg[1] =
        lis3dh_from_fs2_hr_to_mg(data_raw_acceleration[1]);
      acceleration_mg[2] =
        lis3dh_from_fs2_hr_to_mg(data_raw_acceleration[2]);
    }

    lis3dh_temp_data_ready_get(&dev_ctx, &reg.byte);

		if(count >= 4)
		{
			meanX /= count;
			meanY /= count;
			meanZ /= count;
			
			total_accelerometer = (fabsf(meanX) + fabsf(meanY) + fabsf(meanZ)) / 3.0f;
			
			if(total_accelerometer > roll_threshold) 
			{
				total_accelerometer = 0;
				ret = true;
			}
			else 
			{
				total_accelerometer = 0;
				ret = false;
			}
			
			count = 0;		
		}
		else
		{
			meanX += acceleration_mg[0];
			meanY += acceleration_mg[1];
			meanZ += acceleration_mg[2];
			
			count++;
		}
		
		return ret;
}

float accelerometer_get_total(void)
{
	return total_accelerometer;
}
