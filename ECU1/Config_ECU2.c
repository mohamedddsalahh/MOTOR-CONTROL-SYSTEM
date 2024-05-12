#include "SSI.h"
#include "uart.h"
#include "GPTM.h"

SSI_Config_t SSI_Configuration = {SSI3_ID, SSI_SLAVE_OUTPUT_Disabled, SSI_SysClk, SSI_Transmission_FREQ_1MHz,SSI_Clk_Prescaler_2,
DATA_CAP_FIRST_EDGE, CLK_SS_LOW, FreeScale_SPI, SSI_DATA_SIZE_8Bits};

UART_ConfigType Uart5_Config = { BIT_8 , NO_PARITY , ONE_BIT , ENABLE_FIFO , 115200 };


Timer_ConfigType Timer0A = { 0xFFFF , ENABLE_INTERRUPT , COUNT_UP , ONE_SHOT , BIT_32_MODE , PRESCALE_1 };


