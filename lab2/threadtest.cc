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
#include "Table.h"
#include "BoundedBuffer.h"

extern void InsertItems(DLList *list,int n, int id);
extern void RemoveItems(DLList *list,int n, int id);
extern int getRandom();

int testnum = 1;
int threadNum = 3;
int itemNum = 5;
int errorType = 0;

DLList* list;
Table *table;
BoundedBuffer* buffer;

char buf[10] = "0";
char out[10] = {0};

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

void TableThread(int id)
{
	int elem = getRandom();
	void *obj = (void *)elem;
	int position;

	position = table->Alloc(obj);

	if (position != -1) {
		obj = table->Get(position);
		table->Release(position);
	}
}

void BoundedBufferThread(int id)
{
	if ((id % 2) == 0) {
		buf[0]++;
		if (buf[0] > '9') buf[0] = '0';
		buffer->Write(buf, 1);
	} else {
		buffer->Read(out, 1);
	}
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

void ThreadTest2()
{
	table = new Table(itemNum);
	for (int i = 1; i < threadNum; i++)
	{
		Thread *t = new Thread("forked thread");
		t->Fork(TableThread, i);
	}
	TableThread(0);
}

void ThreadTest3()
{
	buffer = new BoundedBuffer(itemNum);
	for (int i = 1; i < threadNum; i++)
	{
		Thread *t = new Thread("forked thread");
		t->Fork(BoundedBufferThread, i);
	}
	BoundedBufferThread(0);
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
        case 2:
            ThreadTest2();
            break;
        case 3:
            ThreadTest3();
            break;
        default:
            printf("No test specified.\n");
            break;
    }
}
