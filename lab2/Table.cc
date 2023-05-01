#include "Table.h"
#include "thread.h"
#include "system.h"

Table::Table(int size)
{
    tableSize = size;
    lock = new Lock("TableLock");
    table = new void *[size];
    value = new int[size];
    for (int i = 0; i < size; i++) {
        value[i] = 0;
    }
}

Table::~Table()
{
    delete table;
    delete lock;
    delete value;
}

int Table::Alloc(void *object)
{
    int i;
    lock->Acquire();
	printf("thread\t%p\talloc\t", currentThread);
    for (i = 0; i < tableSize; i++) {
        if (value[i] == 0) {
            table[i] = object;
            value[i] = 1;
            printf("-table[%d]:%d.\n", i, (int)object);
            lock->Release();
            return i;
        }
    }
	printf("alloc failed\n");
    lock->Release();
    return -1;
}

void *Table::Get(int index)
{
    ASSERT(index < tableSize && index >= 0);
    lock->Acquire();
	printf("thread\t%p\tget\t", currentThread);
    if (value[index] == 1) {
        printf("-table[%d]:%d\n", index, (int)table[index]);
        lock->Release();
        return (void *)table[index];
    } else {
        printf("-table[%d] is empty.\n", index);
    }
    lock->Release();
    return NULL;
}

void Table::Release(int index)
{
    ASSERT(index < tableSize && index >= 0);
    lock->Acquire();
	printf("thread\t%p\trelease\t", currentThread);
    if (value[index] == 1) {
        value[index] = 0;
        table[index] = NULL;
        printf("-table[%d] is released.\n", index);
    } else {
        printf("-table[%d] is NULL.\n", index);
    }
    lock->Release();
    return;
}
