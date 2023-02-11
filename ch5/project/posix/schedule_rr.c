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
        int id = name[1] - '0';
        newNode->task->tid = id;
        // printf("ID: %d",newNode->task->tid);
        newNode->next = NULL;
        tail->next = newNode;
        tail = newNode;
    }
}

struct node* makeCopy(struct node* curr){
    struct node *copy = malloc(sizeof(struct node));
    copy->task = malloc(sizeof(struct task));
    copy->task->name = curr->task->name;
    copy->task->priority = curr->task->priority;
    copy->task->burst = curr->task->burst;
    copy->task->tid = curr->task->tid;
    return copy;
}

// invoke the scheduler
void schedule(){
    printf("_______________________RUNNING ROUND ROBIN_______________________\n");
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
        int count = 0;
        //make a shadow copy
        // struct node *copy = malloc(sizeof(struct node));
        // copy->task = malloc(sizeof(struct task));
        // copy->task->name = curr->task->name;
        // copy->task->priority = curr->task->priority;
        // copy->task->burst = curr->task->burst;
        struct node* copy = makeCopy(curr);
        while(count < 10 && copy->task->burst > 0){
            copy->task->burst--;
            count++;
        }
        if(copy->task->burst > 0){
            // newNode = malloc(sizeof(struct node));
            // newNode->task = malloc(sizeof(struct task));
            // newNode->task->name = copy->task->name;
            // newNode->task->priority = copy->task->priority;
            // newNode->task->burst = copy->task->burst;
            struct node* newNode = makeCopy(copy);
            tail->next = newNode;
            tail = newNode;
        }
        run(curr->task, count);
        dispatcherCount++;
        sum+=count;
        taskArr[curr->task->tid].start = sum;
        taskArr[curr->task->tid].TAT = sum;
        printf("\tTime is now: %d\n", sum);
        curr = curr->next;
        count = 0;
    }

    //CPU Utilization 
    //printf("HERE: %d, AND %.2f\n", sum, sum + dispatcherCount -1);
    double cpu = (double)(sum / (sum + dispatcherCount-1) * 100);
    printf("\n\nCPU Utilization: %.2f%% \n ", cpu);

    runTables(taskArr);
    printf("_______________________ROUND ROBIN DONE_______________________\n\n");
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