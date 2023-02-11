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

typedef struct taskTime{
    int start;
    int waitTime;
    int TAT;
    int responseTime;
    int arrIndex;
    char name[2];
} TaskTime;
void runTables(struct taskTime *taskArr);

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
        int id = name[1] - '0';
        head->task->tid = id;
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
        int id = name[1] - '0';
        newNode->task->tid = id;
        tail->next = newNode;
        tail = newNode;
    }
}

// invoke the scheduler
void schedule(){
    printf("_______________________RUNNING FCFS_______________________\n");
    int sum = 0;
    double dispatcherCount = 0;
    struct node *curr = head;
    struct taskTime* taskArr = malloc(9 * sizeof(struct taskTime));
    for(int i=1; i<9; i++){
        taskArr[i].start = 0;
        taskArr[i].waitTime = 0;
        taskArr[i].responseTime = -1;
        taskArr[i].TAT = -1;
        taskArr[i].arrIndex = -1;
    }
    int index = 1;
    while(curr != NULL){
        if(taskArr[curr->task->tid].arrIndex == -1){
            taskArr[curr->task->tid].arrIndex = index;
            strcpy(taskArr[curr->task->tid].name, curr->task->name);
            index++;
        }
        if(taskArr[curr->task->tid].responseTime == -1){
            taskArr[curr->task->tid].responseTime = sum;
        }
        taskArr[curr->task->tid].waitTime += sum - taskArr[curr->task->tid].start;
        run(curr->task, curr->task->burst);
        dispatcherCount++;
        sum+=curr->task->burst;
        taskArr[curr->task->tid].start = sum;
        taskArr[curr->task->tid].TAT = sum;
        printf("\tTime is now: %i\n", sum);
        curr = curr->next;
    }

    //CPU Utilization 
    //printf("HERE: %d, AND %.2f\n", sum, sum + dispatcherCount -1);
    double cpu = (double)(sum / (sum + dispatcherCount-1) * 100);
    printf("\n\nCPU Utilization: %.2f%% \n ", cpu);

    //RUN TIME TABLE
    runTables(taskArr);

    printf("________________________FCFS DONE________________________\n\n");
}

void runTables(struct taskTime *taskArr){
    //ALL TIME TABLES
    printf("\n\n");
    printf("   |");
    for(int i=1; i<9; i++){
        for(int j=1; j<9; j++){
            if(taskArr[j].arrIndex == i){
                printf(" %s |", taskArr[j].name);
            }
        }
    }
    printf("\n");
    printf("---|");
    for(int i=1; i<9; i++){
        printf("----|");
    }
    printf("\n");
    printf("TAT|");
    for(int i=1; i<9; i++){
        for(int j=1; j<9; j++){
            if(taskArr[j].arrIndex == i){
                if(taskArr[j].TAT >= 100){
                    printf(" %d|", taskArr[j].TAT);
                }else if(taskArr[j].TAT == 0){
                    printf(" %d  |", taskArr[j].TAT);
                }else{
                    printf(" %d |", taskArr[j].TAT);
                }
            }
        }
    }
    
    printf("\n");
    printf("WT |");
    for(int i=1; i<9; i++){
        for(int j=1; j<9; j++){
            if(taskArr[j].arrIndex == i){
                if(taskArr[j].waitTime >= 100){
                    printf(" %d|", taskArr[j].waitTime);
                }else if(taskArr[j].waitTime == 0){
                    printf(" %d  |", taskArr[j].waitTime);
                }else{
                    printf(" %d |", taskArr[j].waitTime);
                }
            }
        }
    }
    printf("\n");
    printf("RT |");
    for(int i=1; i<9; i++){
        for(int j=1; j<9; j++){
            if(taskArr[j].arrIndex == i){
                if(taskArr[j].responseTime >= 100){
                    printf(" %d|", taskArr[j].responseTime);
                }else if(taskArr[j].responseTime == 0){
                    printf(" %d  |", taskArr[j].responseTime);
                }else{
                    printf(" %d |", taskArr[j].responseTime);
                }
            }
        }
    }

    printf("\n\n");
}