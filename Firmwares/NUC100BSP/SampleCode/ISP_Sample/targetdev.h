
#include "NUC100Series.h"
#include "ISP_HID\hid_transfer.h"
#include "ISP_UART\uart_transfer.h"
#include "ISP_I2C\i2c_transfer.h"
#include "isp_user.h"
#include <stdint.h>

/* rename for uart_transfer.c */
#define UART_N							UART1
#define UART_N_IRQHandler		UART1_IRQHandler
#define UART_N_IRQn					UART1_IRQn

/* rename for i2c_transfer.c */
#define I2C_N								I2C0
#define I2C_N_IRQHandler		I2C0_IRQHandler

#define DetectPin   				PB15
#define ReadyPin   					PB15

#define CONFIG_SIZE 8 // in bytes

