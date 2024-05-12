#ifndef _SCHEDULAR_CFG_H_
#define _SCHEDULAR_CFG_H_


#define TasksNo                 9U
#define BinarySemaphoreNo       2U

#define MCAL_Peripherals_Used   2U

#define Send_KeepAliveTask             (&Tasks_Configuration.Tasks[0])
#define Receive_KeepAliveTask          (&Tasks_Configuration.Tasks[1])
#define switchStatesTask               (&Tasks_Configuration.Tasks[2])
#define Process_ADC_ReadingTask        (&Tasks_Configuration.Tasks[3])
#define DTCTask                        (&Tasks_Configuration.Tasks[4])
#define No_CommunicationTask           (&Tasks_Configuration.Tasks[5])
#define OverheatTask                   (&Tasks_Configuration.Tasks[6])
#define SendToPCTask                   (&Tasks_Configuration.Tasks[7])





#define SwitchStatesSempahore         (BinarySem.BinarySemaphores[0])
#define SendStatesSempahore         (BinarySem.BinarySemaphores[1])




#endif