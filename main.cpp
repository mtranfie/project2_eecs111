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

struct timeval t_global_start;
ResourceRoom room;

int main(int argc, char** argv)
{
	if (argc != 2) {
		fprintf(stderr, "[ERROR] Expected 1 argument, but got (%d)\n", argc-1);
		fprintf(stderr, "[USAGE] p2_exec <number>\n");
		exit(1);
	}

	int N = atoi(argv[1]);
	if (N <= 0) {
		fprintf(stderr, "[ERROR] Expected 1 argument, but got (%d)\n", argc-1);
		fprintf(stderr, "[USAGE] p2_exec <number>\n");
		exit(1);
	}

	gettimeofday(&t_global_start, NULL);
	std::srand(t_global_start.tv_usec);

	Person people[2*N];
	pthread_t tids[2*N];
	int studentsCount = 0, facultyCount = 0;

	for (int i = 0; i < 2*N; i++) {
		Person p;	// t_create set at construction, marking when person arrives
		int role;

		if (studentsCount == N)
			role = 0;		// all students placed, must be faculty
		else if (facultyCount == N)
			role = 1;		// all faculty placed, must be student
		else
			role = std::rand() % 2;

		if (role == 0) facultyCount++;
		else           studentsCount++;

		p.set_role(role);
		p.set_time(get_resource_room_duration());
		p.set_order(i);
		people[i] = p;		// copy after fields are set; thread uses &people[i]

		room.enqueue(people[i]);	// prints [Input] and [Queue] under lock

		if (pthread_create(&tids[i], NULL, threadfunc, &people[i])) {
			fprintf(stderr, "[ERROR] Failed to create thread.\n");
		}

		usleep(MSEC(get_next_person_interval()));
	}

	for (int i = 0; i < 2*N; i++) {
		pthread_join(tids[i], NULL);
	}

	long total = get_ms_since_start();
	printf("[%02ld ms][Summary] Finished all %d people. Total runtime: %ld ms\n",
	       total, 2*N, total);

	return 0;
}
