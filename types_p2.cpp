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



// may delete these because what is the purpose?
void Person::start(void) {
	gettimeofday(&t_start, NULL);
	printf("[%lu][Input] A person (%s) goes into the queue.\n", get_elasped_time(t_create, t_start), print_role(role));

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



void ResourceRoom::enqueue(Person& p) {
	pthread_mutex_lock(&mutex);

	if (p.get_role() == 0) waiting_faculty++;
	else                   waiting_student++;

	int total = waiting_faculty + waiting_student;
	const char* q_state = (total > 0) ? "not empty" : "empty";

	printf("[%02ld ms][Input] A person (%s) goes into the queue.\n",
		get_ms_since_start(), print_role(p.get_role()));
	printf("[%02ld ms][Queue] Waiting queue is %s. Status: Total: %d (Faculty: %d, Students: %d)\n",
		get_ms_since_start(), q_state, total, waiting_faculty, waiting_student);

	pthread_mutex_unlock(&mutex);
}

// You need to use this function to print the ResourceRoom's status
void ResourceRoom::print_status(void) {
	printf("Status: Total: %d (Faculty: %d, Students: %d)\n", students_count + faculty_count, faculty_count, students_count);
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
	pthread_mutex_lock(&mutex);

	while (status == STUDENTINSIDE) {
		pthread_cond_wait(&faculty_cond, &mutex);
	}

	waiting_faculty--;	// dequeue after wait, so count is correct while waiting

	printf("[%02ld ms][Queue] Send (Faculty Member) into the resource room (Stay %ld ms), Status: Total: %d (Faculty: %d, Students: %d)\n",
		get_ms_since_start(), p.get_time(), waiting_faculty + waiting_student, waiting_faculty, waiting_student);

	faculty_count++;
	status = FACULTYINSIDE;

	printf("[%02ld ms][Resource Room] (Faculty Member) goes into the resource room, State is (FacultyInside): Total: %d (Faculty: %d, Students: %d)\n",
		get_ms_since_start(), faculty_count + students_count, faculty_count, students_count);

	pthread_mutex_unlock(&mutex);
}

void ResourceRoom::student_wants_to_enter(Person& p) {
	pthread_mutex_lock(&mutex);

	while (status == FACULTYINSIDE) {
		pthread_cond_wait(&student_cond, &mutex);
	}

	waiting_student--;	// dequeue after wait, so count is correct while waiting

	printf("[%02ld ms][Queue] Send (Student) into the resource room (Stay %ld ms), Status: Total: %d (Faculty: %d, Students: %d)\n",
		get_ms_since_start(), p.get_time(), waiting_faculty + waiting_student, waiting_faculty, waiting_student);

	students_count++;
	status = STUDENTINSIDE;

	printf("[%02ld ms][Resource Room] (Student) goes into the resource room, State is (StudentInside): Total: %d (Faculty: %d, Students: %d)\n",
		get_ms_since_start(), faculty_count + students_count, faculty_count, students_count);

	pthread_mutex_unlock(&mutex);
}

void ResourceRoom::faculty_leaves(Person& p) {
	pthread_mutex_lock(&mutex);

	int old_status = status;

	faculty_count--;

	if (faculty_count == 0) {
		status = EMPTY;
		pthread_cond_broadcast(&student_cond);	// wake students first to avoid starvation
		pthread_cond_broadcast(&faculty_cond);
	}

	const char* status_change_text = (old_status != status) ? "Status is changed, Status is" : "State is";
	const char* status_text = (status == EMPTY) ? "Empty" : (status == STUDENTINSIDE) ? "StudentInside" : "FacultyInside";

	printf("[%02ld ms][Resource Room] (Faculty Member) left the resource room. %s (%s) : Total: %d (Faculty: %d, Students: %d)\n",
		get_ms_since_start(), status_change_text, status_text, faculty_count + students_count, faculty_count, students_count);

	pthread_mutex_unlock(&mutex);
}

void ResourceRoom::student_leaves(Person& p) {
	pthread_mutex_lock(&mutex);

	int old_status = status;

	students_count--;

	if (students_count == 0) {
		status = EMPTY;
		pthread_cond_broadcast(&faculty_cond);	// wake faculty first to avoid starvation
		pthread_cond_broadcast(&student_cond);
	}

	const char* status_change_text = (old_status != status) ? "Status is changed, Status is" : "State is";
	const char* status_text = (status == EMPTY) ? "Empty" : (status == STUDENTINSIDE) ? "StudentInside" : "FacultyInside";

	printf("[%02ld ms][Resource Room] (Student) left the resource room. %s (%s) : Total: %d (Faculty: %d, Students: %d)\n",
		get_ms_since_start(), status_change_text, status_text, faculty_count + students_count, faculty_count, students_count);

	pthread_mutex_unlock(&mutex);
}
