#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>



buffer_item buffer[BUFFER_SIZE];
buffer_item item;
pthread_mutex_t mutex;
sem_t full, empty;
int pos, num, i;
int front = 0;
int back = 0;
pthread_t tid;
pthread_attr_t attr;



void *producer(void *param);
void *consumer(void *param);
void randomInt(int lower, int upper);
void printDequeue();

int insert_item(buffer_item item)
{
  /* Acquire Empty Semaphore */
	sem_wait(&empty);
		
	/* Acquire mutex lock to protect buffer */
	pthread_mutex_lock(&mutex);
	
	/* Insert item into buffer */
	if(front==0 && back==0){
		if(buffer[0] == 0) {	//buffer is empty	
			buffer[0] = item;
			pos = 1;
		} else { //buffer has only one element at very front
			buffer[1] = item;
			back += 1;
			pos = 2;
		}
	} else if ((back-front)==4) { //Buffer is full
		return;
	} else if (front==back && front<=1){ //one item in buffer
		buffer[front+1] = item;
		back += 1;
		pos = back+1;
	} else if (front==back && back>=2) { //one item in buffer
		buffer[back-1] = item;
		front -= 1;
		pos = front+1;
	} else if(front >= 2) {
		buffer[front-1] = item;
		front -= 1;
		pos = front+1;
	} else if(front < 2) {
		if(back==4) { //only first element in buffer is empty
			buffer[0] = item;
			front = 0;
			pos = 1;
		} else {
			buffer[back+1] = item;
			back += 1;
			pos = back+1;
		}
	}
	/* Release mutex lock and full semaphore */
	pthread_mutex_unlock(&mutex);
	sem_post(&full);
	

	return 0;
}

int remove_item(buffer_item *item)
{
	/* Acquire Full Semaphore */
	sem_wait(&full);

	/* Acquire mutex lock to protect buffer */
	pthread_mutex_lock(&mutex);
	
	/* remove an object from buffer placing it in item */
	if(front==0 && back == 0){ 
		if(buffer[0]==0) { //Buffer is empty
			return;
		} else { //Buffer has one element at the front
			num = buffer[0];
			buffer[0] = 0;
			pos = 1;
		}
	} else if ((back-front)==4) { //Buffer is full
		num = buffer[4];
		buffer[4] = 0;
		pos = 5;
	} else if (front==back) { //Buffer only has one element
		num = buffer[front];
		buffer[front] = 0;
		pos = front+1;
		front = 0;
		back = 0;
	} else if (back <= 2) {
		num = buffer[front];
		buffer[front] = 0;
		pos = front+1;
		front += 1;
	} else {
		if(front <=1 && back == 3) { //front is closer to end
			num = buffer[front];
			buffer[front] = 0;
			pos = front+1;
			front += 1;
		} else { //back is closer to end
			num = buffer[back];
			buffer[back] = 0;
			pos = back+1;
			back -= 1;
		} 
	}

	/* Release mutex lock and empty semaphore */
	pthread_mutex_unlock(&mutex);
	sem_post(&empty);
	
	return 0;
}


int main(int argc, char *argv[])
{
	/* Get command line arguments argv[1],argv[2],argv[3] */
	int sleepTime = atoi(argv[1]);
	int producers = atoi(argv[2]);
	int consumers = atoi(argv[3]);
	
	/* Initialize buffer related synchronization tools */
	pthread_mutex_init(&mutex,NULL);
	sem_init(&full,0,0);
	sem_init(&empty,0,BUFFER_SIZE);
	pthread_attr_init(&attr);
	for(i=0; i<BUFFER_SIZE; i++) {
		buffer[i] = 0;
	}
	
	/* Create producer threads based on the command line input */
	for(i=0; i<producers; i++){
		pthread_create(&tid,&attr,producer,NULL);
	}
	
	/* Create consumer threads based on the command line input */
	for(i=0; i<consumers; i++) {
		pthread_create(&tid,&attr,consumer,NULL);
	}

	/* Sleep for user specified time based on the command line input */
	sleep(sleepTime);
	return 0;
}

void *producer(void *param)
{
	/* producer thread that calls insert_item() */
	randomInt(5,9);
	insert_item(item);
	printf("Producer %u produced %d at position %d...",(unsigned int)pthread_self(),item,pos);
  printDequeue();
}

void *consumer(void *param)
{
	/* consumer thread that calls remove_item() */
	item = 0;
	remove_item(&item);
	printf("Consumer %u consumed %d at position %d...",(unsigned int)pthread_self(),num,pos);
	printDequeue();
}

void randomInt(int lower, int upper)
{
	item = (rand() % (upper-lower+1))+lower;
}

void printDequeue()
{
	printf("        ...[");
	for(i=0; i<BUFFER_SIZE-1; i++) {
		if(buffer[i] == 0) {
			printf("_,");
		} else {
			printf("%d,",buffer[i]);
		}
	}
	if(buffer[4] == 0) {
		printf("_]\n");
	} else {
		printf("%d]\n",buffer[4]);
	}
}
