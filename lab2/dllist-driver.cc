#include <iostream>
#include <ctime>
#include "dllist.h"
using namespace std;

#define RANGE 100

int getRandom() {
    static int seed = 0;
    seed++;
    srand(unsigned(time(0)) + seed);
    return rand() % RANGE + 1;
}

void
InsertItems(DLList *list, int n, int id) {
    int key;
    for (int i = 0; i < n; i++) {
        key = getRandom();
        list->SortedInsert(NULL, key);
        cout << "Thread " << id << " Insert: key = " << key << endl;
        list->Print();
    }
}


void
RemoveItems(DLList *list, int n, int id) {
    int *key = new int;

    for (int i = 0; i < n; i++) {
        list->Remove(key);
        cout << "Thread " << id << " Remove: key = "<< *key << endl;
        list->Print();
    }
}


