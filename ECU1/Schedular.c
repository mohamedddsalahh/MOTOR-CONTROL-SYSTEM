#include "Schedular.h"
#include "MY_RTOS_FIFO.h"
#include "string.h"

Task_ref Idletask;
Task_ref* temp;

extern uint32_t __INITIAL_SP;
uint32* StackFrame;
uint32 SVC_Number;

uint32 g_tick;

FIFO_Buf_t Ready_QUEUE;
Task_ref* Ready_QUEUE_FIFO[10];

struct{
  
  Task_ref*      Tasks[TasksNo];
  uint32*        _S_MSP_Task;
  uint32*        _E_MSP_Task;
  uint32*        _PSP_TaskLocator;
  uint32        ActiveTasksNo;
  Task_ref*     PreviousTask;
  Task_ref*     CurrentTask;
  Task_ref*     NextTask;
  
  enum{
    OS_Suspended,
    OS_Running
  }OS_STATE;
  
}OS_Control;

void Update_SchedularTable(void){
  
  Task_ref* pTask;
  Task_ref* pNextTask;
  Task_ref* temp;
  Task_ref* dummy = NULL;
  uint32 k = 0;
  for(unsigned char i = 0; i < OS_Control.ActiveTasksNo - 1; i++ ){
    for(unsigned char j = 0; j < OS_Control.ActiveTasksNo - i - 1; j++){
      if(OS_Control.Tasks[j]->Priority > OS_Control.Tasks[j+1]->Priority ){
        temp = OS_Control.Tasks[j];
        OS_Control.Tasks[j] = OS_Control.Tasks[j+1];
        OS_Control.Tasks[j+1] = temp;
      }
    }
  }
  
  while(FIFO_dequeue(&Ready_QUEUE, &dummy) != FIFO_EMPTY);
  while(k < OS_Control.ActiveTasksNo){
    
    pTask = OS_Control.Tasks[k];
    pNextTask = OS_Control.Tasks[k+1];
    
    if(pTask->TaskState != Suspended){
      
      if(pNextTask->TaskState == Suspended){
        FIFO_enqueue(&Ready_QUEUE, pTask);
        pTask->TaskState = Ready;
        break;
      }
      if(pTask->Priority < pNextTask->Priority){
        FIFO_enqueue(&Ready_QUEUE, pTask);
        pTask->TaskState = Ready;
        break;
      }
      else if(pTask->Priority > pNextTask->Priority){
        FIFO_enqueue(&Ready_QUEUE, pTask);
        pTask->TaskState = Ready;
        break;
      }
      else if (pTask->Priority == pNextTask->Priority){
        FIFO_enqueue(&Ready_QUEUE, pTask);
        pTask->TaskState = Ready;
      }
    }
    k++;
  }
  
}

void SVC_Handler(void){
  
  __asm("tst lr, #4   \n"
        "ITE EQ         \n"
          "mrseq r0, MSP  \n"
            "mrsne r0, PSP  \n"
              "mov %0, r0" : "=r" (StackFrame));
  SVC_Number = *((uint8*)StackFrame[6] - 2);
  
  switch(SVC_Number){
  case 0:
  case 1:
    Update_SchedularTable();
    if(OS_Control.OS_STATE == Running){
      if(strcmp(OS_Control.CurrentTask->TaskName, "IDLETASK") != 0){
        //Decide what next
        FIFO_dequeue(&Ready_QUEUE, &OS_Control.NextTask);
        OS_Control.NextTask->TaskState = Running;
        //Update ReadyQueue
        if((OS_Control.CurrentTask->Priority == OS_Control.NextTask->Priority)&&(OS_Control.CurrentTask->TaskState != Suspended)){
          FIFO_enqueue(&Ready_QUEUE, OS_Control.CurrentTask);
          OS_Control.CurrentTask->TaskState = Ready;
        }
        //Trigger OS_PendSV
        SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
      }
    }
  case 2:
    Update_SchedularTable();
    break;
  }
}


__attribute((naked))void PendSV_Handler(void){
  
  __asm("CPSID  I");
  __asm volatile("mrs %0, PSP" :  "=r"(OS_Control.CurrentTask->Current_PSP));
  
  (OS_Control.CurrentTask->Current_PSP)--;
  __asm volatile("mov %0, r4" : "=r" (*(OS_Control.CurrentTask->Current_PSP)));
  (OS_Control.CurrentTask->Current_PSP)--;
  __asm volatile("mov %0, r5" : "=r" (*(OS_Control.CurrentTask->Current_PSP)));
  OS_Control.CurrentTask->Current_PSP--;
  __asm volatile("mov %0, r6" : "=r" (*(OS_Control.CurrentTask->Current_PSP)));
  OS_Control.CurrentTask->Current_PSP--;
  __asm volatile("mov %0, r7" : "=r" (*(OS_Control.CurrentTask->Current_PSP)));
  OS_Control.CurrentTask->Current_PSP--;
  __asm volatile("mov %0, r8" : "=r" (*(OS_Control.CurrentTask->Current_PSP)));
  OS_Control.CurrentTask->Current_PSP--;
  __asm volatile("mov %0, r9" : "=r" (*(OS_Control.CurrentTask->Current_PSP)));
  OS_Control.CurrentTask->Current_PSP--;
  __asm volatile("mov %0, r10" : "=r" (*(OS_Control.CurrentTask->Current_PSP)));
  OS_Control.CurrentTask->Current_PSP--;
  __asm volatile("mov %0, r11" : "=r" (*(OS_Control.CurrentTask->Current_PSP)));
  
  OS_Control.CurrentTask = OS_Control.NextTask;
  OS_Control.NextTask = NULL;
  
  __asm volatile("mov r11, %0" : : "r" (*(OS_Control.CurrentTask->Current_PSP)));
  OS_Control.CurrentTask->Current_PSP++;
  __asm volatile("mov r10, %0" : : "r" (*(OS_Control.CurrentTask->Current_PSP)));
  OS_Control.CurrentTask->Current_PSP++;
  __asm volatile("mov r9, %0" : : "r" (*(OS_Control.CurrentTask->Current_PSP)));
  OS_Control.CurrentTask->Current_PSP++;
  __asm volatile("mov r8, %0" : : "r" (*(OS_Control.CurrentTask->Current_PSP)));
  OS_Control.CurrentTask->Current_PSP++;
  __asm volatile("mov r7, %0" : : "r" (*(OS_Control.CurrentTask->Current_PSP)));
  OS_Control.CurrentTask->Current_PSP++;
  __asm volatile("mov r6, %0" : : "r" (*(OS_Control.CurrentTask->Current_PSP)));
  OS_Control.CurrentTask->Current_PSP++;
  __asm volatile("mov r5, %0" : : "r" (*(OS_Control.CurrentTask->Current_PSP)));
  OS_Control.CurrentTask->Current_PSP++;
  __asm volatile("mov r4, %0" : : "r" (*(OS_Control.CurrentTask->Current_PSP)));
  OS_Control.CurrentTask->Current_PSP++;
  
  
  OS_SET_PSP(OS_Control.CurrentTask->Current_PSP);
  __asm("CPSIE  I");
  __asm volatile ("BX LR");
  
}


void IDLETASK(void){
  
  while(1){
    __asm("wfe");
  }
}



void OS_Init(void){
  
  OS_Control._S_MSP_Task = &__INITIAL_SP;
  OS_Control._E_MSP_Task = (uint32*)((((uint32)OS_Control._S_MSP_Task - 1000)/8)*8);
  OS_Control._PSP_TaskLocator = (uint32*)((((uint32)OS_Control._E_MSP_Task - 8)/8)*8);
  
  FIFO_init(&Ready_QUEUE, Ready_QUEUE_FIFO, 10);
  
  Idletask.StackSize = 300;
  Idletask.Priority = 20;
  Idletask.p_TaskEntry = IDLETASK;
  Idletask.TimingWaiting.Blocking = BlockingDisabled;
  Idletask.TimingWaiting.Ticks_Count = 0;
  strcpy(Idletask.TaskName, "IDLETASK");
  OS_CreateTask(&Idletask);
  
  //NVIC_SetPriority(SysTick_IRQn, 0U);
  *(uint32_t volatile *)0xE000ED20 |= (0xFFU << 16);
}


void OS_CreateTask(Task_ref* Task){
  
  Task->_S_PSP_Task = OS_Control._PSP_TaskLocator;
  Task->_E_PSP_Task = (uint32*)((((uint32)(Task->_S_PSP_Task) - Task->StackSize)/8)*8);
  OS_Control._PSP_TaskLocator = Task->_E_PSP_Task - 30;
  
  /**Create Task Stack**/
  Task->Current_PSP = Task->_S_PSP_Task;
  
  Task->Current_PSP--;                       //XPSR
  *(Task->Current_PSP) &= 0x0;
  *(Task->Current_PSP) = 0x01000000;                            
  
  Task->Current_PSP--;                       //PC
  *(Task->Current_PSP) &= 0x0;
  *(Task->Current_PSP) = (uint32)(Task->p_TaskEntry);          
  
  Task->Current_PSP--;                       //LR
  *(Task->Current_PSP) = 0x0;
  *(Task->Current_PSP) = 0xFFFFFFFD;         //Return to Thread with PSP  
  
  for(uint8 i = 0; i < 13; i++){              //R0 - R12
    Task->Current_PSP--;                                         
    *(Task->Current_PSP) = 0x0;
  }
  
  OS_Control.Tasks[OS_Control.ActiveTasksNo] = Task;
  OS_Control.ActiveTasksNo++;
  
  Task->TaskState = Suspended;
  OS_ActivateTask(Task);
  
}

void OS_ActivateTask(Task_ref* Task){
  
  Task->TaskState = Waiting;
  /**Trigger SVC**/
  __asm("svc #0x00");
  
}


void OS_TerminateTask(Task_ref* Task){
  
  Task->TaskState = Suspended;
  /**Trigger SVC**/
  __asm("svc #0x01");
}


void OS_HoldTask(Task_ref* Task){
  
  Task->TaskState = Suspended;
  /**Trigger SVC**/
  __asm("svc #0x01");
  
}
uint32 OS_GetTime(void){
  
  return g_tick;
  
}

void OS_Start(void){
  
  OS_Control.OS_STATE = Running;
  
  OS_Control.CurrentTask = &Idletask;
  // OS_Control.NextTask = OS_Control.Tasks[1];
  Systick_Start();
  
  OS_ActivateTask(&Idletask);
  
  OS_SET_PSP(OS_Control.CurrentTask->Current_PSP);
  OS_SET_SP_TO_PSP;
  CPUAccess_Unprivileged;
  OS_Control.CurrentTask->p_TaskEntry();
  
}

void SysTick_Handler(void){
  g_tick++;
  
  for(uint8 i = 0; i < TasksNo; i++){
    if(OS_Control.Tasks[i]->TaskState == Suspended){
      if(OS_Control.Tasks[i]->TimingWaiting.Blocking == BlockingEnabled){
        if(g_tick % OS_Control.Tasks[i]->TimingWaiting.Ticks_Count - 1 == 0){
          OS_Control.Tasks[i]->TaskState = Waiting;
          Update_SchedularTable();
        }   
      }
    }
  }
  if(Ready_QUEUE.counter == 0 && OS_Control.CurrentTask->TaskState != Suspended){
    OS_Control.CurrentTask->TaskState = Running;
    FIFO_enqueue(&Ready_QUEUE, OS_Control.CurrentTask);
    OS_Control.NextTask = OS_Control.CurrentTask;
  }
  
  else{
    FIFO_dequeue(&Ready_QUEUE, &OS_Control.NextTask);
    OS_Control.NextTask->TaskState = Running;
    if((OS_Control.CurrentTask->Priority == OS_Control.NextTask->Priority)&&(OS_Control.CurrentTask->TaskState != Suspended)){
      FIFO_enqueue(&Ready_QUEUE, OS_Control.CurrentTask);
      OS_Control.CurrentTask->TaskState = Ready;
    }
  }
  SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}


void SemaphoreTake(BinarySemaphore* Semaphore, Task_ref* task){
  
  if(Semaphore->NextTask == NULL){
    Semaphore->NextTask = task;
    task->TaskState = Suspended;
    __asm("svc #0x01");
  }
  else{
    /*ytl3 error aw haga 3shan mynf3sh aktar mn task yakhdo nfs el binary semaphore*/
  }
  
}


void SemaphoreGive(BinarySemaphore* Semaphore){
  
  Semaphore->CurrentTask = Semaphore->NextTask ;
  Semaphore->NextTask = NULL;
  Semaphore->CurrentTask->TaskState = Waiting;
  __asm("svc #0x00");
  
}
