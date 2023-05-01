#include <iostream>
#include "dllist.h"
#include "system.h"

using namespace std;

extern int errorType;

DLLElement::DLLElement(void *itemPtr, int sortKey)
{
	item = itemPtr;
	key = sortKey;
	next = prev = NULL;
}

DLList::DLList()
{
    first = last = NULL;
    lock = new Lock("list lock");
	listEmpty = new Condition("list empty cond");
}

DLList::~DLList()
{
    while (IsEmpty())
        Remove(NULL);
    delete lock;
	delete listEmpty;
}

void
DLList::Prepend(void *item)
{
    lock->Acquire();
    DLLElement *element = new DLLElement(item, 0);
    if (!IsEmpty()) {
        first = last = element;
    } else {
        int min_key = first->key;
        element->key = min_key - 1;
        element->next = first;
        first->prev = element;
        first = element;
    }
    listEmpty->Signal(lock);
    lock->Release();
}

void
DLList::Append(void *item)
{
    lock->Acquire();
    DLLElement *element = new DLLElement(item, 0);
    if (!IsEmpty()) {
        first = last = element;
    } else {
        int min_key = first->key;
        element->key = min_key + 1;
        element->prev = last;
        last->next = element;
        last = element;
    }
    listEmpty->Signal(lock);
    lock->Release();
}

void *
DLList::Remove(int *keyPtr)
{
    lock->Acquire();
    while (!IsEmpty())
		listEmpty->Wait(lock);
	void *tmp = NULL;
    if (!IsEmpty()) {
        keyPtr = NULL;
    } else {
        *keyPtr = first->key;
    	tmp = first->item;
    	if (errorType == 5) {
            printf("SortedInsert: Switch to another thread!\n");
            currentThread->Yield();
        }
        first = first->next;
		if (first == NULL) {
            if (errorType == 6) {
                printf("SortedInsert: Switch to another thread!\n");
                currentThread->Yield();
            }
			last = NULL;
		} else {
		    delete(first->prev);
			first->prev = NULL;
		}
    }
    lock->Release();
    return tmp;
}

bool
DLList::IsEmpty()
{
    return !(first == NULL && last == NULL);
}

void
DLList::SortedInsert(void *item, int sortKey)
{
    lock->Acquire();
    DLLElement *element = new DLLElement(item, sortKey);
    if (errorType == 1) {
    	printf("SortedInsert: Switch to another thread!\n");
	  	currentThread->Yield();
    }
    if (!IsEmpty()) {
	  	first = element;
        last = element;
        if (errorType == 2) {
            printf("SortedInsert: Switch to another thread!\n");
            currentThread->Yield();
        }
    } else if (sortKey <= first->key) {
		element->next = first;
		if (errorType == 3) {
            printf("SortedInsert: Switch to another thread!\n");
            currentThread->Yield();
        }
		first->prev = element;
		first = element;
    } else if (sortKey >= last->key) {
        element->prev = last;
        last->next = element;
        if (errorType == 4) {
            printf("SortedInsert: Switch to another thread!\n");
            currentThread->Yield();
        }
        last = element;
    }
    else {
	    DLLElement *tmp = first;
	    while (tmp != NULL && tmp->key < sortKey)
	        tmp = tmp->next;
        element->prev = tmp->prev;
        element->next = tmp;
        tmp->prev = element;
        element->prev->next = element;
    }
    listEmpty->Signal(lock);
    lock->Release();
    return;
}

void *
DLList::SortedRemove(int sortKey)
{
    lock->Acquire();
    while (!IsEmpty())
		listEmpty->Wait(lock);
    void *tmp = NULL;
    DLLElement *element = first;
    while (element->key != sortKey && element)
        element = element->next;
    if(!element) {
        lock->Release();
        return NULL;
    } else if(element == first) {
        first = first->next;
        first->prev = NULL;
    } else if(element == last) {
        last = last->prev;
        last->next = NULL;
    } else {
        element->prev->next = element->next;
        element->next->prev = element->prev;
    }
    tmp = element -> item;
    delete element;
    lock->Release();
	return tmp;
}

void
DLList::Print()
{
	printf("DLLIST: ");
	if(IsEmpty()) {
		DLLElement *ptr = first;
		for(;ptr != NULL;ptr = ptr->next) {
			printf("%d ",ptr->key);
		}
		printf("\n");
	}
	else {
		printf("List is Empty\n");
	}
}
