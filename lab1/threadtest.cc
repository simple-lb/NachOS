// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "dllist.h"

extern void InsertItems(DLList *list,int n, int id);
extern void RemoveItems(DLList *list,int n, int id);

int testnum = 1;
int threadNum = 3;
int itemNum = 5;
int errorType = 0;

DLList* list;

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

void
DLListThread(int id)
{
    InsertItems(list, itemNum, id);
    RemoveItems(list, itemNum, id);
}

//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between two threads, by forking a thread
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void
ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");
    list = new DLList();
    for (int i = 1; i < threadNum; i++) {
	  Thread *t = new Thread("forked thread");
        t->Fork(DLListThread, i);
     }
    DLListThread(0);
}

//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void
ThreadTest()
{
    switch (testnum) {
        case 1:
            ThreadTest1();
            break;
        default:
            printf("No test specified.\n");
            break;
    }
}
