#include "stdio.h"
#include "string.h"
#include <stdbool.h>
#include <stdlib.h>
#include "task.h"
#include "list.h"
#include "cpu.h"

const int timeQuantum = 10;

struct node *g_head = NULL;

bool comesBefore(char *a, char *b) { return strcmp(a, b) < 0; }

void add(char *name, int priority, int burst)
{
  Task *newTask = malloc(sizeof(Task));
  newTask->priority = priority;
  newTask->burst = burst;
  newTask->name = strdup(name);
  insert(&g_head, newTask);
}

// based on traverse from list.c
// finds the task whose name comes first in dictionary
Task *pickNextTask() {
  // if list is empty, nothing to do
  if (!g_head)
    return NULL;

  struct node *temp;
  temp = g_head;
  Task *best_sofar = temp->task;
  //Task *end_list = temp->task;
  
  
  
  
  /*while(temp != NULL) {
    if (comesBefore(temp->task->name, best_sofar->name))
      best_sofar = temp->task;
    temp = temp->next;
  }*/
  
  //here need to place best_sofar in a new list
  
  //only delete node if burst == 0  
  // delete the node from list, Task will get deleted later
  delete (&g_head, best_sofar);
  return best_sofar;
}

void schedule()
{
  int current_time = 0;
  Task *task = pickNextTask();
  
  while(task)
  {
  
    if(task->burst > timeQuantum){
      run(task, timeQuantum);
      task->burst -= timeQuantum;
      current_time += timeQuantum;
      //insert at the end of the list
      insert(&g_head, task);
    }
    else{// if burst is == or < timeQuantum
      run(task, task->burst);
      //update time
      current_time += task->burst;
      free(task->name); 
    }
   // run(task, task->burst);
    //current_time += task->burst;
    printf("%21s%d\n", "Time is now: ", current_time);
   // free(task->name); //not here
    task = pickNextTask();
  }
}