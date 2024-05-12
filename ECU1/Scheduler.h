#ifndef __Scheduler__
#define __Scheduler__
#include "types.h"

#define NUMBER_OF_TASKS 4


typedef struct{
  void (*p)(void);
  int period;
}task;

void Task_Create(void (*pointer)(void),uint32 period);
void task_scheduler(void);

#endif
