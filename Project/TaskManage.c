#include "TaskManage.h"

void (*TaskArray[TaskNum])(void);
unsigned char TaskAlreadySchedule[TaskNum]={0};
unsigned char CurrentPriority=TaskNum;

struct TaskPrioNode{
  unsigned char Priority;
  struct TaskPrioNode *next;
 };
struct TaskTimeList{
  unsigned int Time;
  struct TaskPrioNode *tasknode;
  struct TaskTimeList *next;
}*ListHead;

unsigned char firstaddtask=0;
int AddTask(void (*p_Task)(void),unsigned char TaskPriority)
{
  unsigned char TaskPriorityTmp;
  if (TaskArray[TaskPriority]==0)
  {
    TaskArray[TaskPriority]=p_Task;
    CurrentPriority=(CurrentPriority>TaskPriority)?TaskPriority:CurrentPriority;
  }
  else
    return -1;
  
  if (firstaddtask==0)
  {
    ListHead=(struct TaskTimeList *)malloc(sizeof(struct TaskTimeList));
    ListHead->next=0;
    ListHead->tasknode=(struct TaskPrioNode *)malloc(sizeof(struct TaskPrioNode));
    ListHead->tasknode->next=0;
    
    ListHead->Time=0;
    ListHead->tasknode->Priority=TaskPriority;
    firstaddtask=-1;
  }
  else
  {
    TaskPriorityTmp=CurrentPriority;
    CurrentPriority=TaskPriority;
    TaskDelay(0);
    CurrentPriority=TaskPriorityTmp;
  }
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
  struct TaskTimeList *TaskDelayNode,*TaskDelayP;//,*TaskDelayQ;
  struct TaskPrioNode *TaskPrioNodeNew,*TaskPrioNodeP;
  TaskDelayP=ListHead;
  //TaskDelayQ=TaskDelayP->next;
  if(DelayTime<TaskDelayP->Time)
  {//插入到第一个节点之前
    TaskDelayNode=(struct TaskTimeList *)malloc(sizeof(struct TaskTimeList));
    TaskDelayNode->next=0;
    TaskDelayNode->tasknode=(struct TaskPrioNode *)malloc(sizeof(struct TaskPrioNode));
    TaskDelayNode->tasknode->next=0;
    
    TaskDelayNode->Time=DelayTime;
    TaskDelayNode->tasknode->Priority=CurrentPriority;
    TaskDelayP->Time=TaskDelayP->Time-DelayTime;
    TaskDelayNode->next=TaskDelayP;
    TaskDelayP=TaskDelayNode;
  }
  else //if(DelayTime>TaskDelayP->Time)
  {
    while((TaskDelayP->next!=0)&&(DelayTime >= (TaskDelayP->Time) + (TaskDelayP->next->Time)))
    {
      DelayTime=DelayTime-(TaskDelayP->Time);
      TaskDelayP=TaskDelayP->next;
    }
    if (TaskDelayP->next==0 && DelayTime > (TaskDelayP->Time))
    {//已经包含DelayTime > (TaskDelayP->Time) + (TaskDelayP->next->Time)的情况
      TaskDelayNode=(struct TaskTimeList *)malloc(sizeof(struct TaskTimeList));
      TaskDelayNode->next=0;
      TaskDelayNode->tasknode=(struct TaskPrioNode *)malloc(sizeof(struct TaskPrioNode));
      TaskDelayNode->tasknode->next=0;
      
      TaskDelayNode->Time=DelayTime-(TaskDelayP->Time);
      TaskDelayNode->tasknode->Priority=CurrentPriority;
      TaskDelayP->next=TaskDelayNode;
    }
    else if (DelayTime==TaskDelayP->Time)
    {
      TaskPrioNodeP=TaskDelayP->tasknode;
      while((TaskPrioNodeP->next!=0) && (CurrentPriority > (TaskPrioNodeP)->Priority) 
            && (CurrentPriority > (TaskPrioNodeP->next)->Priority))
        TaskPrioNodeP=TaskPrioNodeP->next;
      if (CurrentPriority == (TaskPrioNodeP->next)->Priority 
          || CurrentPriority == (TaskPrioNodeP)->Priority)
        return 0;
      else if(CurrentPriority < (TaskPrioNodeP)->Priority)
      {//插入到第一个节点之前
        TaskPrioNodeNew=(struct TaskPrioNode *)malloc(sizeof(struct TaskPrioNode));
        TaskPrioNodeNew->next=0;
        TaskPrioNodeNew->Priority=CurrentPriority;
        
        TaskPrioNodeNew->next=TaskPrioNodeP;
        TaskDelayP->tasknode=TaskPrioNodeNew;
      }
      else
      {
        TaskPrioNodeNew=(struct TaskPrioNode *)malloc(sizeof(struct TaskPrioNode));
        TaskPrioNodeNew->next=0;
        
        TaskPrioNodeNew->Priority=CurrentPriority;
        if (TaskPrioNodeP->next!=0)
          TaskPrioNodeNew->next=TaskPrioNodeP->next;
        TaskPrioNodeP->next=TaskPrioNodeNew;
      }
    }
    else if(DelayTime > TaskDelayP->Time && DelayTime < TaskDelayP->next->Time + TaskDelayP->Time)
    {
      TaskDelayNode=(struct TaskTimeList *)malloc(sizeof(struct TaskTimeList));
      TaskDelayNode->next=0;
      TaskDelayNode->tasknode=(struct TaskPrioNode *)malloc(sizeof(struct TaskPrioNode));
      TaskDelayNode->tasknode->next=0;
      
      TaskDelayNode->Time=DelayTime-(TaskDelayP->Time);
      TaskDelayNode->tasknode->Priority=CurrentPriority;
      
      TaskDelayP->next->Time=TaskDelayP->next->Time - TaskDelayNode->Time;
      TaskDelayNode->next=TaskDelayP->next;
      TaskDelayP->next=TaskDelayNode;
    }
  }
  return 0;
}

int TaskSchedule()
{
  unsigned char TaskPriorityTmp;
  struct TaskTimeList *TaskTimeListP;
  struct TaskPrioNode *TaskPrioNodeP,*TaskPrioNodeQ,*TaskPrioNodeU,*TaskPrioNodeF;
  
  TaskPriorityTmp=CurrentPriority;
  TaskPrioNodeP=ListHead->tasknode;
  if(ListHead->Time==0)
  {
    while((TaskPrioNodeP!=0) && (TaskPrioNodeP->Priority <= CurrentPriority))
    {
      if(TaskAlreadySchedule[TaskPrioNodeP->Priority]==0)
      {
        TaskAlreadySchedule[TaskPrioNodeP->Priority]=-1;
        CurrentPriority=TaskPrioNodeP->Priority;
        if(*TaskArray[CurrentPriority] != 0)
          (*TaskArray[CurrentPriority])();
        CurrentPriority=TaskPriorityTmp;
        TaskAlreadySchedule[TaskPrioNodeP->Priority]=0;
        TaskPrioNodeF=TaskPrioNodeP;
        TaskPrioNodeP=TaskPrioNodeP->next;
        free((void*)TaskPrioNodeF);
        
        ListHead->tasknode=TaskPrioNodeP;
        if(ListHead->tasknode!=0)
          CurrentPriority=ListHead->tasknode->Priority;
        else
          CurrentPriority=TaskNum;
      }
      else
        break;
    }
    TaskTimeListP=ListHead->next;
    TaskTimeListP->Time--;
    if(TaskTimeListP->Time==0)
    {
      if(ListHead->tasknode==0)
      {
        free((void*)ListHead);
        ListHead=TaskTimeListP;
      }
      else
      {
        TaskPrioNodeP=ListHead->tasknode;
        TaskPrioNodeQ=TaskTimeListP->tasknode;
        if(TaskPrioNodeP->Priority < TaskPrioNodeQ->Priority)
        {
          TaskPrioNodeU=TaskPrioNodeP;
          TaskPrioNodeP=TaskPrioNodeP->next;
        }
        else
        {
          TaskPrioNodeU=TaskPrioNodeQ;
          TaskPrioNodeQ=TaskPrioNodeQ->next;
        }
        ListHead->tasknode=TaskPrioNodeU;
        while(TaskPrioNodeP!=0 && TaskPrioNodeQ!=0)
        {
          if(TaskPrioNodeP->Priority < TaskPrioNodeQ->Priority)
          {
            TaskPrioNodeU->next=TaskPrioNodeP;
            TaskPrioNodeP=TaskPrioNodeP->next;
          }
          else
          {
            TaskPrioNodeU->next=TaskPrioNodeQ;
            TaskPrioNodeQ=TaskPrioNodeQ->next;
          }
          TaskPrioNodeU=TaskPrioNodeU->next;
        }
        TaskPrioNodeU->next=TaskPrioNodeQ?TaskPrioNodeQ:TaskPrioNodeP;
        ListHead->next=TaskTimeListP->next;
        free((void*)TaskTimeListP);
      }
    }
  }
  else
  {
    ListHead->Time--;
  }
  return 0;
}








