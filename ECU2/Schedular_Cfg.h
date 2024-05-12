#ifndef _SCHEDULAR_CFG_H_
#define _SCHEDULAR_CFG_H_


#define TasksNo                 10U
#define BinarySemaphoreNo       1U

#define MCAL_Peripherals_Used   2U

#define Send_KeepAliveTask           (&Tasks_Configuration.Tasks[0])
#define Receive_KeepAliveTask          (&Tasks_Configuration.Tasks[1])
#define switchStatesTask             (&Tasks_Configuration.Tasks[2])
#define Send_ADC_ReadingTask          (&Tasks_Configuration.Tasks[3])
#define sendVoltageTask           (&Tasks_Configuration.Tasks[4])
#define DTCTask          (&Tasks_Configuration.Tasks[5])
#define No_CommunicationTask           (&Tasks_Configuration.Tasks[6])
#define OverheatTask          (&Tasks_Configuration.Tasks[7])
#define CheckStateTask          (&Tasks_Configuration.Tasks[8])





#define SwitchStatesSempahore         (BinarySem.BinarySemaphores[0])



#endif