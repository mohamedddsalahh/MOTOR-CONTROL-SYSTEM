#include "Schedular.h"
#include "ECU2.h"

Task_Config Tasks_Configuration = {
 .Tasks=  
  {
    {
      .StackSize = 100,
      .Priority = 4,
      .p_TaskEntry = Send_KeepAlive_Task,
      .TimingWaiting.Blocking = BlockingEnabled,
      .TimingWaiting.Ticks_Count = 100,
      .TaskName = "task1"
    },
    {
      .StackSize = 100,
      .Priority = 4,
      .p_TaskEntry = Receive_KeepAlive_Task,
      .TimingWaiting.Blocking = BlockingEnabled,
      .TimingWaiting.Ticks_Count = 100,
      .TaskName = "task2"
    },
    {
      .StackSize = 100,
      .Priority = 1,
      .p_TaskEntry = switchStates,
      .TimingWaiting.Blocking = BlockingDisabled,
      .TimingWaiting.Ticks_Count = 120,
      .TaskName = "task3"
    },
    {
      .StackSize = 100,
      .Priority = 4,
      .p_TaskEntry = Send_ADC_Reading,
      .TimingWaiting.Blocking = BlockingEnabled,
      .TimingWaiting.Ticks_Count = 500,
      .TaskName = "task4"
    },
    {
      .StackSize = 100,
      .Priority = 4,
      .p_TaskEntry = sendVoltage,
      .TimingWaiting.Blocking = BlockingEnabled,
      .TimingWaiting.Ticks_Count = 105,
      .TaskName = "task5"
    },
    {
      .StackSize = 100,
      .Priority = 3,
      .p_TaskEntry = DTC_Task,
      .TimingWaiting.Blocking = BlockingDisabled,
      .TimingWaiting.Ticks_Count = 100,
      .TaskName = "task6"
    },
    {
      .StackSize = 100,
      .Priority = 1,
      .p_TaskEntry = No_Communication_Task,
      .TimingWaiting.Blocking = BlockingDisabled,
      .TimingWaiting.Ticks_Count = 100,
      .TaskName = "task7"
    },
    {
      .StackSize = 100,
      .Priority = 4,
      .p_TaskEntry = Overheat_Task,
      .TimingWaiting.Blocking = BlockingDisabled,
      .TimingWaiting.Ticks_Count = 100,
      .TaskName = "task8"
    },
    {
      .StackSize = 100,
      .Priority = 4,
      .p_TaskEntry = CheckState_Task,
      .TimingWaiting.Blocking = BlockingEnabled,
      .TimingWaiting.Ticks_Count = 100,
      .TaskName = "task9"
    }
  }
};

Semaphore_Config BinarySem = {
  .BinarySemaphores = {
    [0] = &(BinarySemaphore){NULL_PTR, NULL_PTR, "Task1Semaphore"}
  }
};