#include "Scheduler.h"

extern uint32 g_tick;

task tasks[NUMBER_OF_TASKS];
uint32 task_num=0;

uint8 initialTime = 0;
uint8 nextTime = 0;
uint8 timeSlice = 0;

void Task_Create(void (*pointer)(void), uint32 period)
{
task mytask;
mytask.p=pointer;
mytask.period= period;

tasks[task_num]=mytask;
task_num++;
}

void task_scheduler(void)
{
  initialTime = g_tick;
  uint32 i=0;
  while(1)
  {
   // __asm("CPSID i");
    if (i == NUMBER_OF_TASKS)
    {
      i=0;
    }
    if (g_tick % tasks[i].period == 0 && g_tick !=0 )
    {
      (*tasks[i].p)();
    }
    i++;
   // __asm("CPSIE i");
  }
}