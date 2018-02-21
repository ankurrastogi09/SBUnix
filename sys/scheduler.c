#include <stdio.h>
#include <unistd.h>
#include <sys/kprintf.h>
#include <sys/defs.h>
#include <sys/struct_utility.h>
#include <sys/syscall.h>
#include <sys/tarfs.h>

//struct task_struct tasksQueue[1000];

uint64_t* kmalloc(size_t size);

struct task_struct* tasksQueue;

int schedulerHead = -1;

int schedulerTail = -1;

//Will return 1 if it is empty otherwise 0
uint64_t isQueueEmpty()
{
    if(schedulerHead == -1) return 1;
    else return 0;
}

void enQueue(struct task_struct newTask)
{

//	kprintf("hello %d \n", 1234);
        if((schedulerHead == -1) && (schedulerTail == -1))
        {
                *tasksQueue = newTask;
                schedulerHead = 0;
                schedulerTail = 0;
        }
        else
        {
                schedulerTail = (schedulerTail + 1) % 1000;
                *(tasksQueue + schedulerTail)  = newTask;
        }
}

struct task_struct deQueue()
{
        struct task_struct newTask = tasksQueue[schedulerHead];
        if(schedulerHead == schedulerTail)
        {
                //newTask = tasksQueue[schedulerHead];
                schedulerHead = -1;
                schedulerTail  = -1;
        }
        else
        {
                //newTask = tasksQueue[schedulerHead];
                schedulerHead = (schedulerHead + 1) % 1000;
        }
        return newTask;
}

void schedulerInit(){
        tasksQueue = (struct task_struct*) kmalloc(20);
}

void testScheduler()
{
        schedulerInit();
        struct task_struct newTask;
        newTask.pid = 1;
        enQueue(newTask);
	//while(1);
        newTask.pid = 2;
        enQueue(newTask);
       	//while(1);
	newTask.pid = 3;
        enQueue(newTask);
       	//while(1);
	newTask = deQueue();
	//while(1);
	kprintf("%d \n", newTask.pid);
	newTask = deQueue();
	kprintf("YoYo2 %d \n" , newTask.pid);
        newTask = deQueue();
	kprintf("YoYo3 %d \n", newTask.pid);
	while(1);
}
