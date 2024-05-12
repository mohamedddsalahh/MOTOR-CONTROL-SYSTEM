#include "SSI.h"
#include "uart.h"


SSI_Config_t  SSI_Configuration = {SSI3_ID, SSI_MASTER, SSI_SysClk, SSI_Transmission_FREQ_1MHz,
                                    SSI_Clk_Prescaler_2,  DATA_CAP_FIRST_EDGE, CLK_SS_LOW, FreeScale_SPI, SSI_DATA_SIZE_8Bits};


UART_ConfigType Uart0_Config = { BIT_8 , NO_PARITY , ONE_BIT , ENABLE_FIFO , 12006 };


UART_ConfigType Uart5_Config = { BIT_8 , NO_PARITY , ONE_BIT , ENABLE_FIFO , 115200 };
