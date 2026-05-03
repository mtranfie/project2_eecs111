#ifndef __TYPES_P2_H
#define __TYPES_P2_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>

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
	int status;


	// You need to define the data structure to
    // save the information of people using the resource room
	// You can probably use Standard Template Library (STL) vector


public:
	ResourceRoom(){
		status = EMPTY;
	}

	// You need to use this function to print the ResourceRoom's status
	void print_status(void);


	// Call by reference
	// This is just an example. You can implement any function you need
	void add_person(Person& p);

	void faculty_wants_to_enter(Person& p);
	void student_wants_to_enter(Person& p);

	void faculty_leaves(Person& p);
	void student_leaves(Person& p);
};










#endif
