
#include "M451Series.h"
#include "isp_user.h"
#include "ISP_HID\hid_transfer.h"
#include "ISP_I2C\i2c_transfer.h"

/* rename for i2c_transfer.c */
#define I2C_N								I2C0
#define I2C_N_IRQHandler		I2C0_IRQHandler

#define DetectPin   				PB6
#define ReadyPin	   				PA1
#define CONFIG_SIZE 8 // in bytes

