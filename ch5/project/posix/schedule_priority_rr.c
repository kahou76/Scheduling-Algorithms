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
        //printf("START INSERTING\n");
        if(newNode->task->priority > head->task->priority){
            newNode->next = head;
            head = newNode;
        }
        //if middle or tail
        else{
            struct node* curr = head;
            while(curr != NULL&& curr->next != NULL && newNode->task->priority <= curr->next->task->priority){
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
    printf("_______________________RUNNING PRIORITY ROUND ROBIN_______________________\n");
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
    //taskArr[curr->task->tid].responseTime  = sum;
    while(curr != NULL){
        //create new list with same priority
        struct node* copyHead = makeCopy(curr);
        copyHead->next = NULL;
        struct node *copyTail = malloc(sizeof(struct node));
        copyTail = copyHead;
        while(curr->next != NULL && curr->task->priority == curr->next->task->priority){
            struct node* newCopy = makeCopy(curr->next);
            copyTail->next = newCopy;
            copyTail = newCopy;
            curr = curr->next;
        }

        //Round-Robin traverse
        struct node* currCopy = copyHead;
        //taskArr[currCopy->task->tid].responseTime = sum;
        //taskArr[currCopy->task->tid].waitTime = sum -  taskArr[currCopy->task->tid].start;
        while(currCopy != NULL){
            if(taskArr[currCopy->task->tid].arrIndex == -1){
                taskArr[currCopy->task->tid].arrIndex = index;
                strcpy(taskArr[currCopy->task->tid].name, currCopy->task->name);
                index++;
            }
            if(taskArr[currCopy->task->tid].responseTime == -1){
                taskArr[currCopy->task->tid].responseTime = sum;
            }
            taskArr[currCopy->task->tid].waitTime += sum - taskArr[currCopy->task->tid].start;
            int count = 0;
            struct node* copyAgain = makeCopy(currCopy);
            while(count < 10 && copyAgain->task->burst > 0){
                copyAgain->task->burst--;
                count++;
            }
            if(copyAgain->task->burst > 0){
                // newNode = malloc(sizeof(struct node));
                // newNode->task = malloc(sizeof(struct task));
                // newNode->task->name = copyAgain->task->name;
                // newNode->task->priority = copyAgain->task->priority;
                // newNode->task->burst = copyAgain->task->burst;
                newNode = makeCopy(copyAgain);
                copyTail->next = newNode;
                copyTail = newNode;
            }
            run(currCopy->task, count);
            dispatcherCount++;
            sum+=count;
            taskArr[currCopy->task->tid].start = sum;
            taskArr[currCopy->task->tid].TAT = sum;
            printf("\tTime is now: %d\n", sum);
            currCopy = currCopy->next;
            count = 0;
        }
        curr = curr->next;
    }

    //CPU Utilization 
    //printf("HERE: %d, AND %.2f\n", sum, sum + dispatcherCount -1);
    double cpu = (double)(sum / (sum + dispatcherCount-1) * 100);
    printf("\n\nCPU Utilization: %.2f%% \n ", cpu);

    //RUN ALL TABLES
    runTables(taskArr);

    printf("_______________________PRIORITY ROUND ROBIN DONE_______________________\n");
    free(taskArr);
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