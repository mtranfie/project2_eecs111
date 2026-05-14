#ifndef __TYPES_P2_H
#define __TYPES_P2_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <pthread.h>

#define EMPTY        0
#define FACULTYINSIDE 1
#define STUDENTINSIDE 2


class Person
{
	int role; // 0: faculty 1: student
	std::string str_role;
	struct timeval t_create;
	struct timeval t_start;
	struct timeval t_end;
	long time_to_stay_ms;


	unsigned long order;
	unsigned long use_order;

public:
	Person();

	void set_role(int data);
	int get_role(void);

	void set_order(unsigned long data);
	unsigned long get_order(void);

	void set_use_order(unsigned long data);
	unsigned long get_use_order(void);

	void set_time(long data);
	long get_time(void);
	int ready_to_leave(void);

	void start(void);
	void complete(void);
};


// Class for the specialized resource room
// You may need to add more class member variables and functions
class ResourceRoom {
	int status;	// 0: empty, 1: faculty, 2: students
	
	int students_count;
	int faculty_count;
	int waiting_student;
	int waiting_faculty;

	pthread_mutex_t mutex;
	pthread_cond_t	faculty_cond;
	pthread_cond_t	student_cond;

public:
	ResourceRoom(){
		status 			= EMPTY;
		students_count 	= 0;
		faculty_count 	= 0;
		waiting_student = 0;
		waiting_faculty	= 0;
		mutex			= PTHREAD_MUTEX_INITIALIZER;
		faculty_cond 	= PTHREAD_COND_INITIALIZER;
		student_cond 	= PTHREAD_COND_INITIALIZER;
	}

	// You need to use this function to print the ResourceRoom's status
	void print_status(void);

	void add_person(Person& p);

	int get_faculty_count();
	int get_student_count();

	void enqueue(Person& p);

	void faculty_wants_to_enter(Person& p);
	void student_wants_to_enter(Person& p);

	void faculty_leaves(Person& p);
	void student_leaves(Person& p);
};


// Enum for sign
enum Sign { 
	empty,
	student,
	faculty
};


#endif
