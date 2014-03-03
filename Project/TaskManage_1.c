#include "TaskManage.h"

struct TaskPrioNode{
  unsigned char Priority;
  struct TaskPrioNode *next;
 };
struct TaskTimeList{
  unsigned int Time;
  struct TaskPrioNode *tasknode;
  struct TaskTimeList *next;
};

void (*TaskArray[TaskNum])(void);
unsigned char TaskAlreadySchedule[TaskNum]={0};
unsigned char TaskAlreadyAdd[TaskNum]={0};
unsigned char TaskTime[TaskNum]={0};
struct TaskPrioNode TaskNode[TaskNum];
unsigned char CurrentPriority=TaskNum;

unsigned char firstaddtask=0;
int AddTask(void (*p_Task)(void),unsigned char TaskPriority)
{
  //unsigned char TaskPriorityTmp;
  if (TaskArray[TaskPriority]==0)
  {
    TaskArray[TaskPriority]=p_Task;
    TaskAlreadyAdd[TaskPriority]=1;
    TaskTime[TaskPriority]=1;
    CurrentPriority=(CurrentPriority>TaskPriority)?TaskPriority:CurrentPriority;
  }
  else
    return -1;
  
  return 0;
}

int DeleteTask(unsigned char TaskPriority)
{
  TaskArray[TaskPriority]=0;
  TaskAlreadySchedule[TaskPriority]=0;
  return 0;
}

int TaskDelay(unsigned int DelayTime)
{
  TaskTime[CurrentPriority]=DelayTime;
  return 0;
}

int TaskSchedule()
{
  unsigned char TaskPriorityTmp,PrioritySearch;
  
  TaskPriorityTmp=CurrentPriority;
  PrioritySearch=0;
  while(PrioritySearch<TaskNum)
  {
    if(TaskArray[PrioritySearch]!=0 && TaskAlreadySchedule[PrioritySearch]==0)
      TaskTime[PrioritySearch]--;
    PrioritySearch++;
  }
  for (PrioritySearch=0;PrioritySearch<TaskNum;PrioritySearch++)
  {
    if(TaskArray[PrioritySearch]!=0)
    {
      if(TaskAlreadySchedule[PrioritySearch]==0)
      {
        if(TaskTime[PrioritySearch]==0)
        {
          TaskAlreadySchedule[PrioritySearch]++;
          CurrentPriority=PrioritySearch;
          (*TaskArray[CurrentPriority])();
          CurrentPriority=TaskPriorityTmp;
          TaskAlreadySchedule[PrioritySearch]--;
        }
      }
      else
        break;
    }
    else
      continue;

  }
  
  return 0;
}








