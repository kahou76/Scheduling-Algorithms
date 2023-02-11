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
        // int id = name[1] - '0';
        // head->task->tid = id;
        // printf("ID: %d",head->task->tid);
        head->next = NULL;
        tail = head;
    }else{
        //add to tail
        newNode = malloc(sizeof(struct node));
        newNode->task = malloc(sizeof(struct task));
        
        newNode->task->name = name;
        newNode->task->priority = priority;
        newNode->task->burst = burst;
        // int id = name[1] - '0';
        // newNode->task->tid = id;
        // printf("ID: %d",newNode->task->tid);
        newNode->next = NULL;
        tail->next = newNode;
        tail = newNode;
    }
}

// invoke the scheduler
void schedule(){
    printf("_______________________RUNNING ROUND ROBIN_______________________\n");
    int sum = 0;
    double dispatcherCount = 0;
    struct node *curr = head;
    while(curr != NULL){
        int count = 0;
        //make a shadow copy
        struct node *copy = malloc(sizeof(struct node));
        copy->task = malloc(sizeof(struct task));
        copy->task->name = curr->task->name;
        copy->task->priority = curr->task->priority;
        copy->task->burst = curr->task->burst;
        while(count < 10 && copy->task->burst > 0){
            copy->task->burst--;
            count++;
        }
        if(copy->task->burst > 0){
            newNode = malloc(sizeof(struct node));
            newNode->task = malloc(sizeof(struct task));
            newNode->task->name = copy->task->name;
            newNode->task->priority = copy->task->priority;
            newNode->task->burst = copy->task->burst;
            tail->next = newNode;
            tail = newNode;
        }
        run(curr->task, count);
        dispatcherCount++;
        sum+=count;
        printf("\tTime is now: %d\n", sum);
        curr = curr->next;
        count = 0;
    }

    //CPU Utilization 
    //printf("HERE: %d, AND %.2f\n", sum, sum + dispatcherCount -1);
    double cpu = (double)(sum / (sum + dispatcherCount-1) * 100);
    printf("CPU Utilization: %.2f%% \n ", cpu);
    printf("_______________________ROUND ROBIN DONE_______________________\n\n");
}