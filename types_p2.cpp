#include "types_p2.h"
#include "utils.h"



void Person::set_role(int data) { role = data; }
int Person::get_role(void)      { return role; }

void Person::set_order(unsigned long data) { order = data; }
unsigned long Person::get_order(void)      { return order; }

void Person::set_use_order(unsigned long data) { use_order = data; }
unsigned long Person::get_use_order(void)      { return use_order; }

void Person::set_time(long data) { time_to_stay_ms = data; }
long Person::get_time(void) { return time_to_stay_ms; }
int Person::ready_to_leave(void) {
	struct timeval t_curr;
	gettimeofday(&t_curr, NULL);

	if (get_elasped_time(t_start, t_curr) >= time_to_stay_ms) { return 1; }
	else { return 0; }
}




void Person::start(void) {
	gettimeofday(&t_start, NULL);
	printf("(%lu)th person enters the resource room: \n", order);
	printf(" - (%lu) milliseconds after the creation\n", get_elasped_time(t_create, t_start));
}

void Person::complete(void) {
	gettimeofday(&t_end, NULL);
	printf("(%lu)th person leaves the resource room: \n", order);
	printf(" - (%lu) milliseconds after the creation\n", get_elasped_time(t_create, t_end));
	printf(" - (%lu) milliseconds after using the resource room\n", get_elasped_time(t_start, t_end));
}

Person::Person() {
	gettimeofday(&t_create, NULL);
}



// You need to use this function to print the ResourceRoom's status
void ResourceRoom::print_status(void) {
	printf("Print resource room status\n");
}

int ResourceRoom::get_faculty_count() {
	return faculty_count;
}

int ResourceRoom::get_student_count() {
	return students_count;
}

void ResourceRoom::add_person(Person& p) {
	if (p.get_role()) {	// 1 means student
		students_count++;
		return;
	}
	faculty_count++;
}

void ResourceRoom::faculty_wants_to_enter(Person& p) {
	
}

void ResourceRoom::student_wants_to_enter(Person& p) {
	// TODO: implement student entry synchronization.
}

void ResourceRoom::faculty_leaves(Person& p) {
	// TODO: implement faculty member exit synchronization.
}

void ResourceRoom::student_leaves(Person& p) {
	// TODO: implement student exit synchronization.
}
