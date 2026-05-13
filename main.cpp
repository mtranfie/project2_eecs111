#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <pthread.h>
#include "types_p2.h"
#include "p2_threads.h"
#include "utils.h"

pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
struct timeval t_global_start;

const char* print_role(int& role) {
	return (role == 1) ? "Student" : "Faculty Member";
}

int main(int argc, char** argv)
{
	if (argc != 2) {
		fprintf(stderr, "[ERROR] Expected 1 argument, but got (%d)\n", argc-1);
		fprintf(stderr, "[USAGE] p2_exec <number>\n");
		exit(1);
	}

	int N = 0;

	try {
		N = atoi(argv[1]);
	} catch (const std::runtime_error e) {
		fprintf(stderr, "Please enter a valid integer.\n");
		exit(1);
	}

	// This is to set the global start time
	gettimeofday(&t_global_start, NULL);
	std::srand(t_global_start.tv_usec);

	pthread_t       tid;
	int             status = 0;

	ResourceRoom room;
	Person people[2*N];
	pthread_t tids[2*N];

	// faculty loop
	for (int i = 0 ; i < 2*N ; i++) {
		Person p;
		people[i] = p;
		int role;
		// random role create unless role is maxed
		if (room.get_faculty_count() == N) {
			role = 1;
		} else if (room.get_student_count() == N) {
			role = 0;
		} else {
			role = std::rand() % 2;
		}
		p.set_role(role);
		p.set_time(get_resource_room_duration());
		p.set_order(i);
		room.add_person(p);

		// create thread for person
		pthread_t 	tid;

		struct timeval t_curr;
		gettimeofday(&t_curr, NULL);

		printf("[%lu][Input] A person (%s) goes into the queue.\n", get_elasped_time(t_global_start, t_curr), print_role(role));

		if (pthread_create(&tid, NULL, threadfunc, &p)) {
			fprintf(stderr, "[ERROR] Failed to create thread.\n");
		}

		tids[0] = tid;
		
		usleep(MSEC(get_next_person_interval()));	// sleep for 1-5 ms;
	}

	for (int i = 0 ; i < 2*N ; i++) {
		pthread_join(tids[i], NULL);
	}


	/*
	// Example code for sleep and class usage.
	Person p1;
	p1.set_order(1);

	usleep(MSEC(200));
	p1.start();

	usleep(MSEC(150));
	p1.complete();

	if(pthread_create(&tid, NULL, threadfunc, NULL)) {
		fprintf(stderr, "Error creating thread\n");
	}
	usleep(MSEC(10));

	for (int i=0; i<5; i++) {
		printf("Wake up thread after (%d) seconds\n", (5-i));
		usleep(MSEC(1000));
	}

	printf("Wake up thread\n");
	status = pthread_cond_signal(&cond);

	if(pthread_join(tid, NULL)) {
		fprintf(stderr, "Error joining thread\n");
	}
	*/

	return 0;
}
