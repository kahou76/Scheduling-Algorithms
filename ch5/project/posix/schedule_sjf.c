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
        //printf("START MAKING HEAD\n");
        head = malloc(sizeof(struct node));
        tail = malloc(sizeof(struct node));
        head->task = malloc(sizeof(struct task));

        head->task->name = name;
        head->task->priority = priority;
        head->task->burst = burst;
        int id = name[1] - '0';
        head->task->tid = id;
        // printf("ID: %d",head->task->tid);
        head->next = NULL;
        tail = head;
    }else{
        //printf("START DOING JOB\n");
        //traverse the list and insert
        newNode = malloc(sizeof(struct node));
        newNode->task = malloc(sizeof(struct task));
        
        newNode->task->name = name;
        newNode->task->priority = priority;
        newNode->task->burst = burst;
        int id = name[1] - '0';
        newNode->task->tid = id;
        // printf("ID: %d",newNode->task->tid);
        //int inserted = -1;
        //if head
        //printf("START INSERTING\n");
        if(newNode->task->burst < head->task->burst){
            newNode->next = head;
            head = newNode;
        }
        //if middle or tail
        else{
            struct node* curr = head;
            while(curr != NULL&& curr->next != NULL && newNode->task->burst >= curr->next->task->burst){
                curr = curr->next;
            }
            //found
            //no tail
                if(curr->next != NULL){
                    //
                    //printf("FOUND IN MIDDLE, curr value: %d\n", curr->task->tid);
                    newNode->next = curr->next;
                    curr->next = newNode;
                //tail
                }else{
                    //printf("TAIL\n");
                    curr->next = newNode;
                    newNode->next = NULL;
                }
        }
        
        
        //if tail
        // newNode->next = NULL;
        // tail->next = newNode;
        // tail = newNode;
    }
}

// invoke the scheduler
void schedule(){
    printf("_______________________RUNNING SJF_______________________\n");
    int sum = 0;
    double dispatcherCount = 0;
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
    printf("_______________________SJF DONE_______________________\n\n");
}