#include  "BoundedBuffer.h"
#include "thread.h"
#include "system.h"

BoundedBuffer::BoundedBuffer(int maxsize)
{
	bufferSize = maxsize;
	count = 0;
	lock = new Lock("BufferLock");
	notFull = new Condition("Buffer not full cond");
	notEmpty = new Condition("Buffer not empty cond");
	readFrom = 0;
	writeTo = 0;
	buffer = new char[bufferSize];
}

BoundedBuffer::~BoundedBuffer()
{
	delete lock;
	delete notFull;
	delete notEmpty;
	delete buffer;
}

void BoundedBuffer::Read(void* data, int size)
{
    char *readData = (char *)data;
	while (size) {
		lock->Acquire();
		while (count == 0) {
		    notEmpty->Wait(lock);
		}
		*readData++ = buffer[readFrom];
		printf("\nConsumer (Thread %p):%c\n", currentThread, buffer[readFrom]);
		buffer[readFrom] = ' ';
		printf("buffer:%s\n",buffer);
		readFrom = (readFrom + 1) % bufferSize;
		count--;
		notFull->Signal(lock);
		size--;
		lock->Release();
	}
}

void BoundedBuffer::Write(void* data, int size)
{
	char* writeData  = (char *)data;
	while (size) {
		lock->Acquire();
		while (count == bufferSize) {
			notFull->Wait(lock);
		}
		buffer[writeTo] = *writeData++;
		printf("\nProducer (Thread %p):%c\n", currentThread, buffer[writeTo]);
		printf("buffer:%s\n",buffer);
		writeTo = (writeTo + 1) % bufferSize;
		count++;
		notEmpty->Signal(lock);
		size--;
		lock->Release();
	}
}
