/* 
 * ONOMATEPWNYMO : ΧΡΗΣΤΟΣ ΚΑΡΑΓΙΑΝΝΗΣ
 * AEM : 03544
 * 
 * This program is full manager for registering-scheduling and removing courses.
 * It take as first arguments the sizes of the rooms that the courses happen in
 * ascending order, and has 6 choices: 1)a: adding a course (it registers the 
 * name of the teacher, the No of the course and the No of students that attend 
 * the lesson), 2)c: prints the list of courses, 3)s: schedules a lesson as 
 * early as possible (in matter of working hours and working days) and checks if 
 * the teacher has another lesson at the same time, 4)p: prints the lessons of 
 * the courses that are scheduled for every room, 5)r: removes a course completely
 * (from course list and the week schedule) and the q: quit from program.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"hw3.h"

struct course {
    int inuse;
    int courseheader;
    char profname[MAX_NAME_LEN];
    int numofstud;
};

struct room {
    int capacity;
    struct course *week[WORKING_HOURS][WORKING_DAYS];
};
/* 
 * This function searches for a course that the user gives. If it already exists
 it returns the address of it, else it returns an address of a free slot to 
 be stored, else , if the is no space in the course array, it returns the NULL value 
 */
struct course *coursefinder (struct course storage[], int n) {
    
    int i;
    struct course *blank = NULL;
    
    for (i = 0; i < MAX_COURSES; i++) {
        /* 
         * In case the slot it's been used, we check 
         * if the header of the courses are the same 
         */
        if (storage[i].inuse) {
            if (n == storage[i].courseheader)
                return &storage[i];
        }
        /* Retuning the first unused slot if the course doesn't exist */
        else if (!blank) {
            blank = &storage[i];
        }
    }
    if (blank) {
        return blank;
    }
    else {
        /* Returns the NULL value if there is no space */
        return NULL;
    }
}
/* 
 * This function registers the course in the course array 
 * by the time the course doesn't exist and there is a free slot 
 */
int courseadder (struct course storage[], int n, int studnum, char input[]) {
    
    struct course *courseaddress;
    
    courseaddress = coursefinder (storage, n);
    /* Storage proccess not successfull */
    if (courseaddress == NULL) {
        return -1;
    }
    else if (n == courseaddress->courseheader) {
        /* Course already exists */
        return 0;
    }
    else {
        /* storage proccess */
        courseaddress->inuse = 1;
        courseaddress->courseheader = n;
        strcpy(courseaddress->profname, input);
        courseaddress->numofstud = studnum;
        return 1;
    }
}
/* 
 * This function has the print format of the 
 * course array, if there are any courses 
 */
void courseprinter (struct course storage[]) {
    
    int i, check = 0;
    
    for (i = 0; i < MAX_COURSES; i++) {
        if (storage[i].inuse) {
            printf ("[%d] %s %d\n", storage[i].courseheader, \
                    storage[i].profname, storage[i].numofstud);
            check++;
        }
    }
    
    if (!check)
        printf ("No courses.\n");
}
/* 
 * This function schedules a lesson in a room. First it checks the first room 
 *that has the right capacity so as there is enough space for the students 
 * that attend the course, then it checks if the room is avaliable so as the
 * lesson can be scheduled and finally checks if the teacher is avaliable at
 * that time. If it doesn't find time at that room, it goes to the next and 
 * searches and so on until there are no other rooms.
 */
int scheduler (struct room rooms[], struct course *courseaddress, int n) {
    
    int i, j, k, w, t, check, registed = 0, timeleft = n, reset = 0, roomnum, 
    finalstep = 0, rowreminder = 0, collumreminder = 0, duration, slot = 0, 
    roomreminder = 0;
    /* 
     * If the hours that are requested to be 
     *booked exceeds the working hours 
     */
    if (n > WORKING_HOURS)
        return -2;
    /* If the header of the course we are about to schedule is invalid */
    for (k = 0; k < MAX_COURSES; k++) {
        if (courseaddress == NULL || courseaddress->courseheader == 0)
            return -1;
    }
    check = courseaddress->numofstud;
    /*
     * k = No of room
     * i = No of hour
     * j = No of day
     */
    for (k = 0; k < MAX_ROOMS; k++) {
        /* Initialization by the time we change room */
        if (roomreminder) {
            roomreminder = 0;
            rowreminder = 0;
            collumreminder = 0;
        }
        /* 
         * Finding the room that can fit the 
         * No of students that the course has 
         */
        if ((!slot) && rooms[k].capacity >= check) {
            roomnum = k;
        }
        else {
            continue;
        }
        /* 
         * Finalstep is activated only when we are ready to check 
         * if the teacher has anothe leasson at the same time 
         */
        if (!finalstep) {
            for (i = rowreminder; i < WORKING_HOURS; i++) {
                /* 
                 * When we check all days in an hour and 
                 * we have not found time for the leasson 
                 * we change the row and the supportive 
                 * variable row reminder 
                 */
                if (!slot && j == WORKING_DAYS)
                    rowreminder = i;
                /* 
                 * When there is no time left for the lesson that we want to 
                 * schedule, we proceed to check the other parameters before 
                 * we register the leasson in the week schedule 
                 */
                if (!timeleft) {
                    finalstep++;
                    registed++;
                    break;
                }
                for (j = collumreminder; j < WORKING_DAYS; j++) {
                    /* 
                     * When there is no more time for the lesson, we change
                     * the value of the supportive variable collumreminder 
                     * by one, in case the registration fails, so we are 
                     * ready to search for the next day 
                     */
                    if (!timeleft) {
                        collumreminder = j + 1;
                        break;
                    }
                    /* 
                     * If the reset is triggered, we initialize all the 
                     * variables that asociate in the storage proccess 
                     */
                    if (reset) {
                        reset = 0;
                        registed = 0;
                        timeleft = n;
                        slot = 0;
                    }
                    /* 
                     * Check that the position in the schedule is free and 
                     * we can proceed in the storage proccess of the leasson
                     */
                    if (rooms[k].week[i][j] == NULL) {
                        /* 
                         * If we haven't found yet a position that is free, 
                         * when we find we store all the data about is position 
                         */
                        if (slot == 0) {
                            slot++;
                            timeleft--;
                            rowreminder = i;
                            collumreminder = j;
                            registed++;
                            break;
                        }
                        else {
                            /* 
                             * The slot of the time in the week is free, but 
                             * we have already found another position from 
                             * were the lesson begins 
                             */
                            timeleft--;
                            break;
                        }
                    }
                    else {
                        /* When we find a position that is in use */
                        reset++;
                        i = rowreminder;
                    }
                }
            }
        }
        /* 
         * When we have searched all the week and didn't 
         * register the leasson, we move to the next room 
         */
        if ((slot == 0 || timeleft) && i == WORKING_HOURS && j == WORKING_DAYS) {
            roomreminder++;
            continue;
        }
        /* 
         * When we have checked all the hours 
         * from a day and there is some time left 
         */
        if (timeleft && i == WORKING_HOURS) {
            rowreminder = 0;
            /* If we are at the final working day */
			if (j == WORKING_DAYS - 1) {
				collumreminder = 0;
			}
            else {
				collumreminder++;
			}
            slot = 0;
            timeleft = n;
            continue;
        }
        /* 
         * Start checking every other room, except the one that we have
         * registed the lesson at, by comparing the names of the course
         * that is requested to be scheduled with the one that already 
         * exists(if it exists) 
         */
        for (w = 0; w < MAX_ROOMS; w++) {
            if (w == k)
                continue;
            duration = i - n;
            for (t = 0; t < WORKING_HOURS; t++) {
                if (rooms[w].week[t][j] == NULL)
                    continue;
                /* 
                 * Proccess when we find the name of the professor from 
                 * the requested lesson in the same hour in an other room 
                 */
                if(!strcmp(rooms[w].week[t][j]->profname, \
                    courseaddress->profname) && (t == duration)) {
                    k = roomnum - 1;
                    registed = 0;
                    finalstep = 0;
                    slot = 0;
                    collumreminder++;
                    /* 
                     * While we are not at the final working hour of
                     * a day but we have checked all the possible days
                     */
                    if ((rowreminder != WORKING_HOURS - 1) &&  \
                        collumreminder == WORKING_DAYS) {
                        collumreminder = 0;
                        rowreminder++;
                    }
                    timeleft = n;
                    reset++;
                    break;
                }
                /* 
                 * Here we check the time that we have stored of the first 
                 * lesson and the time of the checking t-variable are at the 
                 * same hour in the day so that we can synchronize them and 
                 * check for the same hours of the day if the professor has 
                 * a lesson at another room 
                 */
                if (duration < i && t == duration)
                    duration++;
                if (duration == i)
                    break;
            }
            /* Reseating the storage proccess */
            if (reset)
                break;
        }
        /* Emptying the slot if there is time left in the storage proccess */
        if (timeleft && k < MAX_ROOMS - 1) {
            slot = 0;
            continue;
        }
        /* 
         * If we finally found the time that the lesson can be scheduled 
         * we move on from the search of the rooms, so to register it 
         */
        if (registed)
            break;
    }
    /* If there is any time left from the lesson that is 
     * requested to be stored we return that it is not scheduled 
     */
    if (timeleft) {
        return -2;
    }
    else {
        /* Storage proccess of the course in the week schedule */
        for (t = i - n; t < i; t++) {
            rooms[roomnum].week[t][j] = courseaddress;
        }
        return roomnum;
    }
}
/* This function has the print format of the schedule */
void scheduleformat (struct room *roomsaddress) {
    
    int d, i;
    char weekdays[5][4] = {{"MON"}, {"TUE"}, {"WED"}, {"THU"}, {"FRI"}};
    
    printf ("\n");
    printf ("[%d]\n", roomsaddress->capacity);
    printf ("       ");
    
    for (d = 0; d < WORKING_DAYS; d++) {
        printf ("%s", weekdays[d]);
        if (d < WORKING_DAYS - 1)
            printf (" ");
    }
    printf ("\n");
    for (d = 0; d < WORKING_HOURS; d++) {
        printf ("%d:00  ", 10 + d);
        for (i = 0; i < WORKING_DAYS; i++) {
            if (roomsaddress->week[d][i] != NULL && roomsaddress->week[d][i]->courseheader) {
                printf ("%-4d", roomsaddress->week[d][i]->courseheader);
            }
            else {
                printf (" -  ");
            }
        }
        printf ("\n");
    }
}
/* 
 * This function prints the week schedule of the rooms with 
 * the courses that are scheduled in every one of them 
 */
void scheduleprinter (struct room rooms[]) {
    
    int i;
    struct room *roomsaddress;
    
    for (i = 0; i < MAX_ROOMS; i++) {
        if (rooms[i].capacity == 0) {
            break;
        }
        else {
            roomsaddress = &rooms[i];
            scheduleformat (roomsaddress);
        }
    }
}
/* This function deletes a course from the course array and the week schedule */
int coursedeleter (struct course storage[], int n, struct room rooms[]) {
    
    int i, j, k, result;
    struct course *courseaddress;
    
    result = 0;
    /* Finding the address of the leasson we want to delete */
    courseaddress = coursefinder (storage, n);
    /* Delete proccess from the week schedule */
    if (courseaddress != NULL) {
        for (k = 0; k < MAX_ROOMS; k++) {
            for (i = 0; i < WORKING_HOURS; i++) {
                for (j = 0; j < WORKING_DAYS; j++) {
                    if (rooms[k].week[i][j] == courseaddress)
                        rooms[k].week[i][j] = NULL;
                }
            }
        }
    }
    /* Delete proccess from the course array */
    if (courseaddress != NULL) {
        if (courseaddress->courseheader == n) {
            result++;
            courseaddress->inuse = 0;
            courseaddress->courseheader = 0;
            for (i = 0;i < MAX_NAME_LEN; i++) {
                courseaddress->profname[i] = '\0';
            }
            courseaddress->numofstud = 0;
        }
    }
    
    return result;
}

int main (int argc, char *argv[]) {

    struct room numofrooms[MAX_ROOMS];
    struct course courses[MAX_COURSES];
    int i, j, k, max = 1, check = 0, header, studnumber, result, courseduration;
    char input, name[MAX_NAME_LEN], extracharbin = 'a';
    
    /* checking if the arguments are correct */
    for (i = 2; i < argc; i++) {
        if ((atoi(argv[max]) > 0 && atoi(argv[i]) > 0) && (atoi(argv[max]) <= atoi(argv[i]))) {
            max = i;
        }
        else {
            check++;
        }
    }
    if (argc - 1 > MAX_ROOMS || argc < 2 || check) {
        printf ("Incorrect command-line arguments!\n");
        return 1;
    }
    /* initializing the room arra*/
    for (i = 0; i < MAX_ROOMS; i++) {
        numofrooms[i].capacity = 0;
        for (j = 0; j < WORKING_HOURS; j++) {
            for (k = 0; k < WORKING_DAYS; k++) {
                numofrooms[i].week[j][k] = NULL;
            }
        }
    }
    /*initializing the course array*/
    for (i = 0; i < MAX_COURSES; i++) {
        courses[i].inuse = 0;
        courses[i].courseheader = 0;
        courses[i].numofstud = 0;
    }
    /* Registering the room capacity */
    for (i = 0; i < MAX_ROOMS; i++) {
        if (i + 1 >= argc)
            break;
        numofrooms[i].capacity = atoi(argv[i + 1]);
    }
    /* Running ultil the user quits from the program */
    do {
        print_menu();
        scanf (" %c", &input);
        if (input == 'q')
            return 0;
        
        switch (input) {
            /* Adding a course */
            case 'a': {
                printf ("Professor course students:\n");
                scanf ("%s %d %d", name, &header, &studnumber);
                result = courseadder (courses, header, studnumber, name);
                switch (result) {
                    case -1: {
                        printf ("No space.\n");
                        break;
                    }
                    case 0: {
                        printf ("%d exists.\n", header);
                        break;
                    }
                    case 1: {
                        printf ("%d added.\n", header);
                        break;
                    }
                }
                break;
            }
            case 'c': {
                /* Printing the courses */
                courseprinter (courses);
                break;
            }
            case 's': {
                /* Scheduling a course */
                printf ("Course duration:\n");
                scanf ("%d %d", &header, &courseduration);
                if (courseduration < 1) {
                    printf ("Invalid duration.\n");
                    break;
                }
                result = scheduler (numofrooms, coursefinder (courses, header), courseduration);
                switch (result) {
                    case -2: {
                        printf ("%d not scheduled.\n", header);
                        break;
                    }
                    case -1: {
                        printf ("%d not found.\n", header);
                        break;
                    }
                }
                if (result >= 0)
                    printf ("%d scheduled in %d.\n", header, result);
                break;
            }
            case 'p': {
                /* Printing the week program */
                scheduleprinter (numofrooms);
                break;
            }
            case 'r': {
                /* Removing a course */
                printf ("Course:\n");
                scanf ("%d", &header);
                result = coursedeleter (courses, header, numofrooms);
                if (result) {
                    printf ("%d deleted.\n",header);
                }
                else {
                    printf ("%d not deleted.\n", header);
                }
                break;
            }
            default: { /* Cleaning the buffer */
                extracharbin = '\0';
				while (extracharbin != '\n') {
					scanf ("%c", &extracharbin);
				}
                printf ("Invalid choice.\n");
                break;
            }
        }
    } while (1);
}
