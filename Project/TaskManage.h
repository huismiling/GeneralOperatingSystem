#ifndef __TaskManage
#define __TaskManage

#define TaskNum 64

extern int AddTask(void (*p_Task)(void),unsigned char TaskPriority);
extern int DeleteTask(unsigned char TaskPriority);
extern int TaskDelay(unsigned int DelayTime);
extern int TaskSchedule();

#endif /* #ifndef __TaskManage */