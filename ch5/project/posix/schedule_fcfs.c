#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "task.h"
#include "list.h"
#include "schedulers.h"
#include "CPU.h"

struct node *head = NULL;
struct node *tail = NULL;
struct node *newNode = NULL;

// add a task to the list 
void add(char *name, int priority, int burst){
    //add to list in order 
    if(head == NULL){
        //create head
        head = malloc(sizeof(struct node));
        tail = malloc(sizeof(struct node));
        head->task = malloc(sizeof(struct task));

        head->task->name = name;
        head->task->priority = priority;
        head->task->burst = burst;
        head->next = NULL;
        tail = head;
    }else{
        //add to tail
        newNode = malloc(sizeof(struct node));
        newNode->task = malloc(sizeof(struct task));
        
        newNode->task->name = name;
        newNode->task->priority = priority;
        newNode->task->burst = burst;
        newNode->next = NULL;
        tail->next = newNode;
        tail = newNode;
    }
}

// invoke the scheduler
void schedule(){
    printf("_______________________RUNNING FCFS_______________________\n");
    int sum = 0;
    double dispatcherCount = 0;
    //char[][2] strArr;
    struct node *curr = head;
    while(curr != NULL){
        run(curr->task, curr->task->burst);
        dispatcherCount++;
        sum+=curr->task->burst;
        printf("\tTime is now: %i\n", sum);
        curr = curr->next;
    }

    //CPU Utilization 
    //printf("HERE: %d, AND %.2f\n", sum, sum + dispatcherCount -1);
    double cpu = (double)(sum / (sum + dispatcherCount-1) * 100);
    printf("CPU Utilization: %.2f%% \n ", cpu);

    //RUN TIME TABLE
    // printf("\t| ")
    printf("________________________FCFS DONE________________________\n\n");
}