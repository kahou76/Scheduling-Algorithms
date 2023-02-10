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
    return copy;
}

// invoke the scheduler
void schedule(){
    printf("_______________________RUNNING PRIORITY ROUND ROBIN_______________________\n");
    int sum = 0;
    struct node *curr = head;
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
        while(currCopy != NULL){
            int count = 0;
            struct node* copyAgain = makeCopy(currCopy);
            while(count < 10 && copyAgain->task->burst > 0){
                copyAgain->task->burst--;
                count++;
            }
            if(copyAgain->task->burst > 0){
                newNode = malloc(sizeof(struct node));
                newNode->task = malloc(sizeof(struct task));
                newNode->task->name = copyAgain->task->name;
                newNode->task->priority = copyAgain->task->priority;
                newNode->task->burst = copyAgain->task->burst;
                copyTail->next = newNode;
                copyTail = newNode;
            }
            run(currCopy->task, count);
            sum+=count;
            printf("\tTime is now: %d\n", sum);
            currCopy = currCopy->next;
            count = 0;
        }
        curr = curr->next;
    }
    printf("_______________________PRIORITY ROUND ROBIN DONE_______________________\n");
}