#include <iostream>
#include <ctime>
#include "dllist.h"
using namespace std;

#define RANGE 100

void
InsertItems(DLList *list, int n, int id) {
    int key;
    static int seed = 0;
    seed++;
    srand(unsigned(time(0)) + seed);

    for (int i = 0; i < n; i++) {
        key = rand() % RANGE + 1;
        list->SortedInsert(NULL, key);
        cout << "Thread " << id << " Insert: key = " << key << endl;
        list->Print();
    }
}


void
RemoveItems(DLList *list, int n, int id) {
    int *key = new int;

    for (int i = 0; i < n; i++) {
        if (list->IsEmpty()) {
            list->Remove(key);
            cout << "Thread " << id << " Remove: key = "<< *key << endl;
            list->Print();
        }
    }
}


