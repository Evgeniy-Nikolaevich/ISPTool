#ifndef TARGETDEV_H
#define TARGETDEV_H

#include "NUC122.h"
#include "ISP_HID\hid_transfer.h"
#include "ISP_UART\uart_transfer.h"
#include "ISP_I2C\i2c_transfer.h"
#include "isp_user.h"

/* rename for uart_transfer.c */
#define UART_N							UART1
#define UART_N_IRQHandler		UART1_IRQHandler
#define UART_N_IRQn					UART1_IRQn

/* rename for i2c_transfer.c */
#define I2C_N								I2C1
#define I2C_N_IRQHandler		I2C1_IRQHandler

#define DetectPin   				PA10
#define ReadyPin   					PA12

/* rename for isp_user.c & fmc_user.c */
#define RSTSTS		RSTSRC
#define ISPCTL		ISPCON
#define ISPADDR		ISPADR
#define FMC_ISPCTL_ISPFF_Msk		FMC_ISPCON_ISPFF_Msk

#define CONFIG_SIZE 8 // in bytes

#define STATUS							I2CSTATUS
#define TOCTL								I2CTOC
#define I2C_TOCTL_TOIF_Msk	I2C_I2CTOC_TIF_Msk
#define I2C_CTL_SI_AA				I2C_I2CON_SI_AA
#define I2C_CTL_SI					I2C_I2CON_SI


#endif // #define TARGETDEV_H
