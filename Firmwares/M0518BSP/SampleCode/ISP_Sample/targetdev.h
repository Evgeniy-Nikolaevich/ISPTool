
#include "M0518.h"
#include "ISP_UART\uart_transfer.h"
#include "ISP_I2C\i2c_transfer.h"
#include "isp_user.h"

#define ReadyPin	   				PA10
/* rename for uart_transfer.c */
#define UART_T					UART0
#define UART_T_IRQHandler		UART02_IRQHandler
#define UART_T_IRQn				UART02_IRQn

/* rename for i2c_transfer.c */
#define I2C_N								I2C0
#define I2C_N_IRQHandler		I2C0_IRQHandler

#define CONFIG_SIZE 8 // in bytes
