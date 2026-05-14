#ifndef __UTILS_H
#define __UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>

#define MSEC(x) x*1000

extern struct timeval t_global_start;

long get_elasped_time(struct timeval& start, struct timeval& end);

/**
 * Returns random long 1-5. Interval between next person generating.
 */
long get_next_person_interval();

/**
 * Returns a random long 3-10. Duration for how long person stays in resource room.
 */
long get_resource_room_duration();

/**
 * Returns const char* of role.
 */
const char* print_role(int role);

long get_ms_since_start();


#endif
