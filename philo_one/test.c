#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#define NUMBER_OF_PHILOSOPHERS 5

void *philosopher(void * philosopherNumber);
void think(int philosopherNumber);
void pickUp(int philosopherNumber);
void eat(int philosopherNumber);
void putDown(int philosopherNumber);

pthread_mutex_t forks[NUMBER_OF_PHILOSOPHERS];
pthread_t philosophers[NUMBER_OF_PHILOSOPHERS];
pthread_attr_t attributes[NUMBER_OF_PHILOSOPHERS];

int main() {
	int i;
	srand(time(NULL));
	for (i = 0; i < NUMBER_OF_PHILOSOPHERS; ++i) {
		pthread_mutex_init(&forks[i], NULL);
	}

	for (i = 0; i < NUMBER_OF_PHILOSOPHERS; ++i) {
		pthread_attr_init(&attributes[i]);
	}
	
	for (i = 0; i < NUMBER_OF_PHILOSOPHERS; ++i) {
		pthread_create(&philosophers[i], &attributes[i], philosopher, (void *)(i));
	}

	for (i = 0; i < NUMBER_OF_PHILOSOPHERS; ++i) {
		pthread_join(philosophers[i], NULL);
	}
	return 0;
}

void *philosopher(void *philosopherNumber) {
	while (1) {
		think(philosopherNumber);
		pickUp(philosopherNumber);
		eat(philosopherNumber);
		putDown(philosopherNumber);
	}
}

void think(int philosopherNumber) {
	int sleepTime = rand() % 3 + 1;
	printf("Philosopher %d will think for %d seconds\n", philosopherNumber, sleepTime);
	sleep(sleepTime);
}

void pickUp(int philosopherNumber) {
	int right = (philosopherNumber + 1) % NUMBER_OF_PHILOSOPHERS;
	int left = (philosopherNumber + NUMBER_OF_PHILOSOPHERS) % NUMBER_OF_PHILOSOPHERS;
	if (philosopherNumber & 1) {
		printf("Philosopher %d is waiting to pick up fork %d\n", philosopherNumber, right);
		pthread_mutex_lock(&forks[right]);
		printf("Philosopher %d picked up fork %d\n", philosopherNumber, right);
		printf("Philosopher %d is waiting to pick up fork %d\n", philosopherNumber, left);
		pthread_mutex_lock(&forks[left]);
		printf("Philosopher %d picked up fork %d\n", philosopherNumber, left);
	}
	else {
		printf("Philosopher %d is waiting to pick up fork %d\n", philosopherNumber, left);
		pthread_mutex_lock(&forks[left]);
		printf("Philosopher %d picked up fork %d\n", philosopherNumber, left);
		printf("Philosopher %d is waiting to pick up fork %d\n", philosopherNumber, right);
		pthread_mutex_lock(&forks[right]);
		printf("Philosopher %d picked up fork %d\n", philosopherNumber, right);
	}
}

void eat(int philosopherNumber) {
	int eatTime = rand() % 3 + 1;
	printf("Philosopher %d will eat for %d seconds\n", philosopherNumber, eatTime);
	sleep(eatTime);
}

void putDown(int philosopherNumber) {
	printf("Philosopher %d will will put down her forks\n", philosopherNumber);
	pthread_mutex_unlock(&forks[(philosopherNumber + 1) % NUMBER_OF_PHILOSOPHERS]);
	pthread_mutex_unlock(&forks[(philosopherNumber + NUMBER_OF_PHILOSOPHERS) % NUMBER_OF_PHILOSOPHERS]);
}