#ifndef __I2C_TRANS_H__
#define __I2C_TRANS_H__
#include <stdint.h>
#include "targetdev.h"

extern volatile uint8_t bI2cDataReady;
extern uint8_t i2c_rcvbuf[];

/*-------------------------------------------------------------*/
void I2C_Init(void);
void I2C_N_IRQHandler(void);

#endif  /* __I2C_TRANS_H__ */
