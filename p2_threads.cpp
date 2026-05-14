#include "p2_threads.h"
#include "utils.h"

extern pthread_cond_t  cond;
extern pthread_mutex_t mutex;
extern ResourceRoom room;

void *threadfunc(void *param)
{
	Person* p = (Person*)param;
	int status;

	if (p->get_role() == 0) {
		room.faculty_wants_to_enter(*p);
	} else {
		room.student_wants_to_enter(*p);
	}

	usleep(MSEC(p->get_time()));

	if (p->get_role() == 0) {
		room.faculty_leaves(*p);
	} else {
		room.student_leaves(*p);
	}

	return NULL;
}
