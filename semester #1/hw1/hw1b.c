/*
 * ONOMATEPWNYMO: ΧΡΗΣΤΟΣ ΚΑΡΑΓΙΑΝΝΗΣ
 * AEM:03544
 *
 * This program runs an election about 7 candidates and an amount of voters. The
 * informations of the voters (Age, Gender and their vote) are encrypted to a 
 * unsigned short integer. After every input is evaluated to be valid gets stored
 * . Finally the program prints a Menu for the user to choose an action:
 * 1) print voters inf, 2) print candidates stats, 3) print the winner of the 
 * election, 4) print every winner based on the gender and finally the number of
 * some age teams.   
 *
 */
#include"hw1.h"
#include <stdio.h>

int main(int argc, char *argv[]) {

    /*
    voters[MAX_VOTERS][0] = vote
    voters[MAX_VOTERS][1] = gender
    voters[MAX_VOTERS][2] = age
    candidates[NUM_CANDIDATES][0] = male votes
    candidates[NUM_CANDIDATES][1] = female votes
    candidates[NUM_CANDIDATES][2] = other votes
    */
    
    char input = 0;
    unsigned short int voters[MAX_VOTERS][3], candidates[NUM_CANDIDATES][3], 
    agestat[4] = {0}, maska, row, col, voter_age, voter_gender, finalvote, 
    voter_info = 1, invalid = 0, mostvotes = 0, sumofvotes = 0, mostmale = 0, 
    mostfemale = 0, mostother = 0, counter, check, vote, invalidage,
    invalidgen, invalidvote;
    
    /* Initialization of the matrixes*/ 

    for (row = 0; row < MAX_VOTERS; row++) {
        for (col = 0; col < 3; col++) {
            voters[row][col] = 0;
        }
    }
    
    for (row = 0; row < NUM_CANDIDATES; row++) {
        for (col = 0; col < 3; col++) {
            candidates[row][col] = 0;
        }
    }
    
    /* Voters info storage proccess */

    for (row = 0; row < MAX_VOTERS; row++) {
        printf ("Enter voter info:\n");
        scanf ("%hu", &voter_info);
        if (voter_info == 0)
            break;
        /* A loop that decrypts voter's info and */
        /* trigers in case of an invalid input   */
        do {
            if (invalid) {
            	if (invalidage) 
            		printf("Invalid age %hu.\n", voter_age);
            	if (invalidgen)
            		printf("Invalid gender.\n");
            	if (invalidvote)
            		printf("Invalid votes %hu.\n", check);
                printf ("Enter voter info:\n");
                scanf ("%hu", &voter_info);
                if (voter_info == 0)
                    break;
            }
            invalid = 0;
            for (col = 0; col < 3; col++) {
                if (col == 0) {
                    /* Parameters intialization */
                    invalidvote = 0;
                    check =0;
                    finalvote = 0;
                    vote = voter_info;
                    maska = 1;
                    /* Finding final vote */
                    for (counter = 0; counter < 7; counter++) {
                        if (vote & maska) {
                            check++;
                            vote = vote>>1;
                            finalvote = counter;
                        }
                        else {
                            vote = vote>>1;
                        }
                    }
                    /* Blank vote case */
                    if (check == 0)
                        finalvote = 7;
                    /* Check for invalid input */
                    if (check > 1) {
                        invalid++;
                        invalidvote++;
                    }
                }
                else if (col == 1) {
                    /* Gender storage proccess */
                    invalidgen = 0;
                    voter_gender = 0;
                    maska = 384;
                    voter_gender = voter_info & maska;
                    voter_gender = voter_gender>>7;
                    /* Check for invalid input */
                    if (voter_gender == 0) {
                        invalid++;
						invalidgen++;
                    }
                }
                else {
                    /* Age storage proccess at the right age interval */
                    maska = 65024;
                    invalidage = 0;
                    voter_age = 0;
                    voter_age = voter_info & maska;
                    voter_age = voter_age>>9;
                    /* Case for invalid input */
                    if (voter_age <= 17) {    
                        invalid++;
                        invalidage++;
                    }
                }
            }
        } while (invalid >= 1);
        
        if (voter_info == 0)
            break;
        
        /* Official storage of the valid inputs at the matrixes */
        
        if (voter_age >= 18 && voter_age <= 29) {
            agestat[0]++;
        }
        else if (voter_age >= 30 && voter_age <= 44) {
            agestat[1]++;
        }
        else if (voter_age >= 45 && voter_age <= 59) {
            agestat[2]++;
        }
        else if (voter_age >= 60) {
            agestat[3]++;
        }
        voters[row][2] = voter_age;
        
        /* Vote insert at candidate's stats by gender */
        
        switch (voter_gender) {
            case 1: {
                if (finalvote != 7)
                    candidates[finalvote][0]++;
                voters[row][1] = 77;
                break;
                }
            case 2: {
                if (finalvote != 7)
                    candidates[finalvote][1]++;
                voters[row][1] = 70;
                break;
            }
            case 3: {
                if (finalvote != 7)
                    candidates[finalvote][2]++;
                voters[row][1] = 79;
                break;
            }
        }
        
        voters[row][0] = finalvote;
        
    }
    
    /* Winner candidate finding proccess */

    for (row = 1; row < NUM_CANDIDATES; row++){
        if ((candidates[mostvotes][0] + candidates[mostvotes][1] + \
        candidates[mostvotes][2] < candidates[row][0] + candidates[row][1] + \
        candidates[row][2]))
            mostvotes = row;
    }
    /* Sum of votes for winner candidate */
    sumofvotes = candidates[mostvotes][0] + candidates[mostvotes][1] + \
    candidates[mostvotes][2];
    
    /* Finding who candidate had most votes by gender */

    for (row = 1; row < NUM_CANDIDATES; row++){
        if (candidates[mostmale][0] < candidates[row][0])
            mostmale = row;
    }
    for (row = 1; row < NUM_CANDIDATES; row++){
        if (candidates[mostfemale][1] < candidates[row][1])
            mostfemale = row;
    }
    for (row = 1; row < NUM_CANDIDATES; row++){
        if (candidates[mostother][2] < candidates[row][2])
            mostother = row;
    }
    
    /************** MENU **************/

    do {
        printf ("\n##\n");
        printf("[P/p] Print voter info\n");
        printf("[C/c] Print candidate info\n");
        printf("[W/w] Print winner\n");
        printf("[G/g] Print choice by gender\n");
        printf("[A/a] Print ages\n");
        printf("[Q/q] Quit\n");
        printf ("\n"); 
        scanf (" %c", &input);
        
        if (input == 'Q' || input == 'q')
                return 0;
        
        switch (input) {
            case 'P':
            case 'p': {
                for (row = 0; row < MAX_VOTERS; row++) {
                    /* Printing only filled voters slots*/
                    if (voters[row][2] == 0)
                        continue;
                        /* Case for non-blank vote printing format */
                    if (voters[row][0] != 7) {
                        printf("%3d: %3hu %c %hu\n", row, voters[row][2], \
                        (char)voters[row][1], voters[row][0]);
                    }
                    else {
                        /* Case for blank vote printing format */
                        printf("%3d: %3hu %c %c\n", row , voters[row][2], \
                        (char)voters[row][1], (char)(voters[row][0] + 38));
                    }
                }
                input = 0;
                break;
            }
            case 'C':
            case 'c': {
                for (row = 0; row < NUM_CANDIDATES; row++) {
                    printf("%3d: %3hu %3hu %3hu\n", row, candidates[row][0], \
                    candidates[row][1], candidates[row][2]);
                }
                input = 0;
                break;
            }
            case 'W': 
            case 'w': {
                printf("%d: %d\n", mostvotes, sumofvotes);
                input = 0;
                break;
            }
            case 'G': 
            case 'g': {
                printf("M: %hu (%hu)\n", mostmale , candidates[mostmale][0]);
                printf("F: %hu (%hu)\n", mostfemale , candidates[mostfemale][1]);
                printf("O: %hu (%hu)\n", mostother , candidates[mostother][2]);
                input = 0;
                break;
            }
            case 'A': 
            case 'a': {
                printf("[18-29]: %d\n", agestat[0]);
                printf("[30-44]: %d\n", agestat[1]);
                printf("[45-59]: %d\n", agestat[2]);
                printf("[60-127]: %d\n", agestat[3]);
                input = 0;
                break;
            }
            default: {printf ("Error. Invalid option.\n");}
        }  
    } while (1);
    return 0;
}
