/*
 * ONOMATEPWNYMO: ΧΡΗΣΤΟΣ ΚΑΡΑΓΙΑΝΝΗΣ
 * AEM:03544
 *
 * This program grades a multiple choise test. Specifically, it set's the values 
 * for some parameters of th test, such as the number of choises in every  
 * question, the penalty for the wrong answers, the right answers etc. and then 
 * evaluates every students grade as also the gpa of the of the test, the 
 * standart daviation and prints some information about the students such as:
 * 1) the number of students that got a grade between an interval, 2) some 
 * stats for a number of students between an interval and finally a histogram
 * that repesents the number of students that got a grade between an interval
 * graphicaly.    
 *
 */
#include <stdio.h>
#include<math.h>
#include"hw1.h"

int main (int argc, char *argv[]) {

    /* 
    studstats[NUM_STUDENTS][0] = right answ. 
    studstats[NUM_STUDENTS][1] = wrong answ.  
    studstats[NUM_STUDENTS][2] = grade
    */
    
    int const CAP_ASCII = 64, GRADE_INTERVAL = 10, MAX_SCORE = 10;
    double studstats[NUM_STUDENTS][3] = {{0}}, gpa = 0, standardeviation = 0, 
    penaltyvalue;
    int row, col, numberofchoises, studposition, low_value, high_value, 
    histogram[GRADE_INTERVAL], numofpeople, repeat;
    char answers[NUM_QUESTIONS], studanswers[NUM_STUDENTS][NUM_QUESTIONS], 
    input = 0;
    
    /* Stores data about the parameters of the test */
    
    do {
        printf ("Enter number of choices:\n");
        scanf ("%d", &numberofchoises);
    } while ((numberofchoises < 1) || (numberofchoises > 26));    
        
    printf ("Max choice: '%c'\n", numberofchoises + CAP_ASCII);
    
    /* Stores the correct answers */
    
    printf ("Enter answer key:\n");
    
    for (col = 0; col < NUM_QUESTIONS; col++) {
        scanf (" %c", &answers[col]);
        /* Check for valid input */
        while ((answers[col] < CAP_ASCII) || (answers[col] > (numberofchoises + \
         CAP_ASCII))) {
            printf ("Error. Enter value A-%c:\n", numberofchoises + CAP_ASCII);
            scanf (" %c", &answers[col]);
        }
    }
    
    /* Set value for penalty */
    
    printf ("Enter wrong answer penalty:\n");
    
    scanf ("%lf", &penaltyvalue);
    /* Check for valid input */
    while (penaltyvalue > 0) {
        printf ("Error. Enter non-positive value:\n");
        scanf ("%lf", &penaltyvalue);
    }
    
    /* Initialization of the matrix */
    
    for (col = 0; col < MAX_SCORE; col++) {
        histogram[col] = 0;
    }
            
    /* Stores student answers and calculates    */
    /* the number of the correct and false ones */ 
    
    for (row = 0; row < NUM_STUDENTS; row++) {
        printf ("Enter student answers:\n");
        for (col = 0; col < NUM_QUESTIONS; col++) {
            scanf (" %c", &studanswers[row][col]);
            /* Check for valid input */
            while (((studanswers[row][col] < CAP_ASCII) || (studanswers[row][col] > \
            (numberofchoises + CAP_ASCII))) && (studanswers[row][col] != '-')) { 
                printf ("Error. Enter valid answer:\n");
                scanf (" %c", &studanswers[row][col]);
            }
            /* Student's answer discrimination proccess */ 
            if (answers[col] == studanswers[row][col]) {
                studstats[row][0]++;
            } 
            else if (studanswers[row][col] == '-') {
            }
            else {
                studstats[row][1]++;   
            }
        }

        /* Calculation of student grade and storage proccess for the histogram */

        studstats[row][2] = ((studstats[row][0] + (studstats[row][1] * \
        penaltyvalue)) * 10) / (double)NUM_QUESTIONS;
        if (studstats[row][2] < 0)
        	studstats[row][2] = 0; 
        studposition = (int)floor(studstats[row][2]);
        if (studposition == MAX_SCORE) {
            histogram[GRADE_INTERVAL - 1]++;
        }
        else {
			histogram[studposition]++;
        }
    }
    
    printf ("\n");
    
    /* GPA and STANDARDDEVIATION */
    
    for (row = 0; row < NUM_STUDENTS; row++) {
        gpa += studstats[row][2];
    }
    gpa = gpa / NUM_STUDENTS;
    for (row = 0; row < NUM_STUDENTS; row++) {
        standardeviation += pow(studstats[row][2] - gpa, 2);
    }
    standardeviation = sqrt(standardeviation / NUM_STUDENTS);
    
    /************** MENU ***************/
    
    do {
    printf("[M/m] Print mean\n");
    printf("[S/s] Print standard deviation\n"); 
    printf("[G/g] Print grade range\n"); 
    printf("[I/i] Print student info in range\n"); 
    printf("[H/h] Draw histogram\n"); 
    printf("[Q/q] Quit\n");
    scanf (" %c", &input);
    
    if (input == 'Q' || input == 'q')
        return 0;
        
    switch (input) {
        case 'M':
        case 'm': {
            printf ("%.2lf", gpa);
            printf ("\n##\n");
            input = 0;
        	break;
        }
        case 'S':
        case 's': {
            printf ("%.2lf", standardeviation);
            printf ("\n##\n");
            input = 0;
        	break;
        }
        case 'G':
        case 'g': {
            do {
                numofpeople = 0;
                printf ("Enter grade range:\n");
                scanf ("%d-%d", &low_value, &high_value);
                /* Check for valid input */
            } while (low_value < 0 || high_value < 0 || low_value > GRADE_INTERVAL || \
            high_value > GRADE_INTERVAL || low_value >= high_value);
            /*  Finding the number of students that are between the interval */
            for (col = low_value; col < high_value; col++) {
                numofpeople = numofpeople + histogram[col];
            }
            printf ("%d", numofpeople);
            printf ("\n##\n");
            input = 0;
        	break;
        }
        case 'I':
        case 'i': {
            printf ("Enter index range:\n");
            scanf ("%d-%d", &low_value, &high_value);
            /* Check for valid input */
            while (low_value < 0 || high_value < 0 || low_value >= NUM_STUDENTS ||\
            high_value >= NUM_STUDENTS || low_value > high_value) {
                printf ("Enter index range:\n");
                scanf ("%d-%d", &low_value, &high_value);         
            }
            /* Printing format */
            for (row = low_value; row <= high_value; row++) {
                printf ("%03d:%4.0lf,%4.0lf,%4.0lf,%6.2lf\n", row, \
                studstats[row][0], studstats[row][1], 
                NUM_QUESTIONS - studstats[row][0] - \
                studstats[row][1], studstats[row][2]);
            }
            printf ("\n##\n");
            input = 0;
        	break;
        }
        case 'H':
        case 'h': {
            for (col = 0; col < GRADE_INTERVAL; col++) {
                /* Printing format */
                if (col == 9) {
                    printf ("[%2d,%2d]: ", col, col +1);
                }
                else {
                    printf ("[%2d,%2d): ", col, col +1);
                }
                /* Repeat Initialization */ 
                repeat = histogram[col];
                if (NUM_STUDENTS > 100) 
                    repeat = histogram[col] / 10;
                for (row = 0; row < repeat; row++) {
                    printf ("*");
                }
                printf("\n");
            }
            printf ("\n##\n");
            input = 0;
        	break;
        }
        default: {printf ("Error. Invalid option.\n");}
    }
    printf ("\n");
    } while (1);
    return 0;
}
