#include <stdio.h>
#include "targetdev.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
uint8_t i2c_rcvbuf[64];
volatile uint8_t bI2cDataReady;

volatile uint8_t g_u8DeviceAddr = 0x60;
volatile uint8_t g_u8SlvDataLen;

__STATIC_INLINE void I2C_SlaveTRx(I2C_T *i2c, uint32_t u32Status);

#define GET_PCLK0_DIV()     ((CLK->CLKSEL0 & CLK_CLKSEL0_PCLK0SEL_Msk)? 1 : 0)
#define GET_PCLK1_DIV()     ((CLK->CLKSEL0 & CLK_CLKSEL0_PCLK1SEL_Msk)? 1 : 0)

void I2C_Init(void)
{
    uint32_t u32Div, u32Pclk;
    uint32_t u32BusClock = 100000;
    /* Reset I2C0 */
    SYS->IPRST1 |=  SYS_IPRST1_I2C0RST_Msk;
    SYS->IPRST1 &= ~SYS_IPRST1_I2C0RST_Msk;
    /* Enable I2C0 Controller */
    I2C0->CTL |= I2C_CTL_I2CEN_Msk;
    /* Get I2C0 PCLK0 Clock */
    u32Pclk = SystemCoreClock / (1 << (GET_PCLK0_DIV()));
    /* Calculate */
    u32Div = (uint32_t)(((u32Pclk * 10u) / (u32BusClock * 4u) + 5u) / 10u - 1u); /* Compute proper divider for I2C clock */
    I2C0->CLKDIV = u32Div;
    /* Set I2C0 Slave Addresses */
    I2C0->ADDR0 = (I2C0->ADDR0 & ~I2C_ADDR0_ADDR_Msk) | (g_u8DeviceAddr << I2C_ADDR0_ADDR_Pos);
    /* Enable I2C0 interrupt and set corresponding NVIC bit */
    I2C0->CTL |= I2C_CTL_INTEN_Msk;
    NVIC_EnableIRQ(I2C0_IRQn);
    /* I2C enter no address SLV mode */
    I2C_SET_CONTROL_REG(I2C0, I2C_CTL_SI_AA);
}

//#define STATUS							I2CSTATUS
//#define TOCTL								I2CTOC
//#define I2C_TOCTL_TOIF_Msk	I2C_I2CTOC_TIF_Msk
//#define I2C_CTL_SI_AA				I2C_I2CON_SI_AA
//#define I2C_CTL_SI					I2C_I2CON_SI

/*---------------------------------------------------------------------------------------------------------*/
/*  I2C IRQ Handler                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void I2C_N_IRQHandler(void)
{
    uint32_t u32Status;
    u32Status = I2C_N->STATUS;

    if (I2C_N->TOCTL & I2C_TOCTL_TOIF_Msk) {
        /* Clear I2C0 Timeout Flag */
        I2C_N->TOCTL |= I2C_TOCTL_TOIF_Msk;
    } else {
        I2C_SlaveTRx(I2C_N, u32Status);
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/*  I2C TRx Callback Function                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
void I2C_SlaveTRx(I2C_T *i2c, uint32_t u32Status)
{
    uint8_t u8data;

    if (u32Status == 0x60) {                    /* Own SLA+W has been receive; ACK has been return */
        bI2cDataReady = 0;
        bISPDataReady = 0;
        g_u8SlvDataLen = 0;
        I2C_SET_CONTROL_REG(i2c, I2C_CTL_SI_AA);
    } else if (u32Status == 0x80)                 /* Previously address with own SLA address
                                                   Data has been received; ACK has been returned*/
    {
        i2c_rcvbuf[g_u8SlvDataLen] = I2C_GET_DATA(i2c);
        g_u8SlvDataLen++;
        g_u8SlvDataLen &= 0x3F;
        bI2cDataReady = (g_u8SlvDataLen == 0);

        if (bI2cDataReady) {
            ReadyPin = 1;
        }

        if (g_u8SlvDataLen == 0x3F) {
            I2C_SET_CONTROL_REG(i2c, I2C_CTL_SI);
        } else {
            I2C_SET_CONTROL_REG(i2c, I2C_CTL_SI_AA);
        }
    } else if (u32Status == 0xA8) {             /* Own SLA+R has been receive; ACK has been return */
        g_u8SlvDataLen = 0;

        if (bISPDataReady == 0) {
            u8data = 0xCC;
        } else {
            u8data = response_buff[g_u8SlvDataLen];
        }

        I2C_SET_DATA(i2c, u8data);
        g_u8SlvDataLen++;
        I2C_SET_CONTROL_REG(i2c, I2C_CTL_SI_AA);
    } else if (u32Status == 0xB8) {
        if (bISPDataReady == 0) {
            u8data = 0xDD;
        } else {
            u8data = response_buff[g_u8SlvDataLen];
        }

        I2C_SET_DATA(i2c, u8data);
        g_u8SlvDataLen++;
        g_u8SlvDataLen &= 0x3F;

        if (g_u8SlvDataLen == 0x00) {
            I2C_SET_CONTROL_REG(i2c, I2C_CTL_SI);
        } else {
            I2C_SET_CONTROL_REG(i2c, I2C_CTL_SI_AA);
        }
    } else if (u32Status == 0xC8) {
        I2C_SET_CONTROL_REG(i2c, I2C_CTL_SI_AA);
    } else if (u32Status == 0xC0)                 /* Data byte or last data in I2CDAT has been transmitted
                                                   Not ACK has been received */
    {
        I2C_SET_CONTROL_REG(i2c, I2C_CTL_SI_AA);
    } else if (u32Status == 0x88)                 /* Previously addressed with own SLA address; NOT ACK has
                                                   been returned */
    {
        i2c_rcvbuf[g_u8SlvDataLen] = I2C_GET_DATA(i2c);
        g_u8SlvDataLen++;
        bI2cDataReady = (g_u8SlvDataLen == 64);

        if (bI2cDataReady) {
            ReadyPin = 1;
        }

        g_u8SlvDataLen = 0;
        I2C_SET_CONTROL_REG(i2c, I2C_CTL_SI_AA);
    } else if (u32Status == 0xA0)                 /* A STOP or repeated START has been received while still
                                                   addressed as Slave/Receiver*/
    {
        g_u8SlvDataLen = 0;
        I2C_SET_CONTROL_REG(i2c, I2C_CTL_SI_AA);
    } else {
        /* TO DO */
        // printf("Status 0x%x is NOT processed\n", u32Status);
    }
}
