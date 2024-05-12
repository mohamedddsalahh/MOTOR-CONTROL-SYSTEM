#ifndef _Schedular_H_
#define _Schedular_H_

#include "ARMCM4.h"
#include "tm4c123gh6pm.h"
#include "systick.h"
#include "Schedular_Cfg.h"
#include "types.h"


#define OS_SET_PSP(add)        __asm volatile("MOV r0, %0 \n\t MSR PSP, r0" : : "r"(add))
#define OS_GET_PSP(add)        __asm volatile("MRS r0, PSP \n\t MOV %0, r0"  :  "=r"(add))


#define OS_SET_SP_TO_PSP       __asm volatile( "MRS r0, CONTROL   \n\t"\
"MOV r1, #0x2      \n\t"\
  "ORR r0, r1, r0    \n\t"\
    "MSR CONTROL, r1");

#define OS_SET_SP_TO_MSP       __asm volatile( "mrs r0, CONTROL   \n\t"\
"mov r1, #0b101    \n\t"\
  "and r0, r0, r1    \n\t"\
    "msr CONTROL, r0");

#define CPUAccess_Privileged	__asm("mrs r3, CONTROL	\n\t" \
"lsr r3, r3, #0x1	\n\t" \
  "lsl r3, r3, #0x1	\n\t" \
    "msr CONTROL, r3");

#define CPUAccess_Unprivileged	__asm( "mrs r3, CONTROL  \n\t" \
"orr r3, r3, #0x1 \n\t" \
  "msr CONTROL, r3");

typedef struct{
  
  uint32 StackSize;
  uint8  Priority;
  void(*p_TaskEntry)(void);
  
  uint32* _S_PSP_Task;
  uint32* _E_PSP_Task;
  uint32* Current_PSP;
  
  uint8 TaskName[30];
  enum{
    Suspended,
    Waiting,
    Ready,
    Running
  }TaskState;
  
  struct{
    enum{
      BlockingEnabled,
      BlockingDisabled
    }Blocking;
    unsigned int Ticks_Count;
  }TimingWaiting;
  
}Task_ref;


typedef struct{
  
  Task_ref* CurrentTask;
  Task_ref* NextTask;
  uint8    BinarySemaphoreName[30];
  
}BinarySemaphore;


typedef struct
{
  Task_ref Tasks[TasksNo];
}Task_Config;

typedef struct{
    BinarySemaphore*       BinarySemaphores[BinarySemaphoreNo]; 
}Semaphore_Config;

void OS_Init(void);
void OS_CreateTask(Task_ref* Task);
void OS_ActivateTask(Task_ref* Task);
void OS_TerminateTask(Task_ref* Task);
void OS_HoldTask(Task_ref* Task);
uint32 OS_GetTime(void);
void OS_Start(void);

void SemaphoreTake(BinarySemaphore* Semaphore, Task_ref* task);
void SemaphoreGive(BinarySemaphore* Semaphore);

extern Task_Config Tasks_Configuration;
extern Semaphore_Config BinarySem;
#endif