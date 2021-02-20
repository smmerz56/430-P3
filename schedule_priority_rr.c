#include "stdio.h"
#include "string.h"
#include <stdbool.h>
#include <stdlib.h>
#include "task.h"
#include "list.h"
#include "cpu.h"

const int timeQuantum = 10;

struct node *g_head = NULL;

struct node *new_head = NULL;

bool rr_list = 0;

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

  while(temp != NULL) {// this loop finds task
    if(temp->task->priority == best_sofar->priority){
      if (comesBefore(temp->task->name, best_sofar->name)){// leave this
          best_sofar = temp->task;
      }
    }
    if(temp->task->priority > best_sofar->priority){
      best_sofar = temp->task;
    }
    temp = temp->next;
  }
  delete (&g_head, best_sofar);
  temp = g_head; 
  
  //very similar loop to above, but looking for multiples of best_sofar
  while(temp != NULL){
    if(temp->task->priority == best_sofar->priority){ // if found a multiple
      rr_list = 1;
      insert(&new_head, temp->task);
      delete (&g_head, temp->task);
    }
    temp = temp->next;
  }
  //if multiples insert into a new list. create a global bool rr_list, and mark true here
  //need to think about the delete below
  
  // delete the node from list, Task will get deleted later
  //delete (&g_head, best_sofar);
  return best_sofar;
}

Task *pickNextTaskRR() {
  // if list is empty, nothing to do
  if (!new_head)
    return NULL;

  struct node *temp;
  temp = new_head;
  Task *best_sofar = temp->task;
  
  // printf("%s\n", temp->task->name); 

  /*while(temp != NULL) {
    if (comesBefore(temp->task->name, best_sofar->name))
      best_sofar = temp->task;
    temp = temp->next;
  }*/
  // delete the node from list, Task will get deleted later
  delete (&new_head, best_sofar);
  return best_sofar;
}

void schedule()
{
  int current_time = 0;
  Task *task = pickNextTask();
  
  // no multiples run whats below  
  while(task){
    if(rr_list == 0){ // no multiples run whats below  
      run(task, task->burst);
      current_time += task->burst;
      printf("%21s%d\n", "Time is now: ", current_time);
      free(task->name); 
     // task = pickNextTask();
    }
    else {
      while(task){
       // printf("RR_List == 1");
        if(task->burst > timeQuantum){
          run(task, timeQuantum);
          task->burst -= timeQuantum;
          current_time += timeQuantum;
          //insert at the end of the list
          insert(&new_head, task);
        }
        else{// if burst is == or < timeQuantum
          run(task, task->burst);
          //update time
          current_time += task->burst;
          free(task->name); 
        }
       
        printf("%21s%d\n", "Time is now: ", current_time);
       
        task = pickNextTaskRR();
       }
       rr_list = 0;
    }
    task = pickNextTask();
  }
  
  
  //else use schedule_rr while loop. Change pickNextTask() to take a head parameter
  //so you can pass it g_head or the head of your new list.
  
}


