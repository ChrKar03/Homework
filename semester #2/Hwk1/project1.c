#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "project1.h"

/* Types of data */

typedef struct 
{
    char firstname[NAMESIZE];
    char lastname[NAMESIZE];
} nameT;

typedef struct 
{
    unsigned long int AEM;
    nameT fullname;
    unsigned short int courses_left;
} entry;

typedef struct 
{
    entry **entries;
    int size;
    int validentries;
    int sorted;
    int cleared;
} catalogT;

catalogT *init (catalogT *array, int size); 
int clear (catalogT *array);
unsigned long int sort (catalogT *array);
int add (catalogT *array, entry persondata, int plusminusval);
int rmv (catalogT *array, int aem, int plusminusval); 
int mod (catalogT *array, int aem, int newNocourses);
int find (catalogT *array, int aem, unsigned long int *comptr);
int basic_find(catalogT *array, int aem, unsigned long int *comp);
int linear_search(catalogT *array, int aem);
int binary_search (catalogT *array, int aem, unsigned long int *comp);
void print(catalogT *array); 
void quit(catalogT *array); 

int main (int argc, char *argv[]) 
{
    if (argc != 3)
        return 42;
   
    clock_t start, end;
    int result, i;
    unsigned long int comps = 0, *comptr = &comps; 
    catalogT *catalog = NULL;
    entry persondata;
    char operator, charbin, *firstptr, *lastptr;

    catalog = init (catalog, atoi(argv[1]));
    catalog->sorted = 0;
    catalog->validentries = 0;
    catalog->cleared = 0;
        
    if (!catalog->entries)
        return 43;

    do
    {
        scanf (" %c", &operator);

        switch (operator)
        {
            case 'a': // Add operator 
            {
                scanf ("%ld %ms %ms %hu", &persondata.AEM, &firstptr, &lastptr, &persondata.courses_left);
                if (persondata.AEM == 0 || persondata.courses_left == 0)
                {
                    charbin = '\0';
                    while (charbin == '\n')
                    {
                        scanf (" %c", &charbin);
                    }
                    printf ("\nInvalid input.\n");
                }
                else
                {
                    if (strlen (firstptr) > NAMESIZE || strlen (lastptr) > NAMESIZE)
                        fprintf (stdout,"\nTrimmed name(s).\n");
                    strncpy (persondata.fullname.firstname, firstptr, NAMESIZE - 1);
                    persondata.fullname.firstname[NAMESIZE - 1] = '\0';
                    strncpy (persondata.fullname.lastname, lastptr, NAMESIZE - 1);
                    persondata.fullname.lastname[NAMESIZE - 1] = '\0';
                    i = 0;
                    while (i < NAMESIZE - 1)                        
                    {
                        persondata.fullname.firstname[i] = toupper (persondata.fullname.firstname[i]);
                        i++;
                    }
                    i = 0;
                    while (i < NAMESIZE - 1)
                    {
                        persondata.fullname.lastname[i] = toupper (persondata.fullname.lastname[i]);
                        i++;
                    }
                    result = add (catalog, persondata, atoi (argv[2]));
                    if (result != -1)
                    {
                        fprintf (stdout, "\nA-OK %ld, %d %d\n", persondata.AEM, catalog->validentries, catalog->size);
                    }
                    else
                    {
                        fprintf (stdout, "\nA-NOK %ld, %d %d\n", persondata.AEM, catalog->validentries, catalog->size);
                    }
                }    
                free (firstptr);
                free (lastptr);
                break;
            }
            case 'r': // Remove operator
            {
                scanf ("%ld", &persondata.AEM);
                result = rmv (catalog, persondata.AEM, atoi (argv[2]));
                if (result != -1)
                {
                    fprintf (stdout, "\nR-OK %ld, %d %d\n", persondata.AEM, catalog->validentries, catalog->size);
                }
                else
                {
                    fprintf (stdout, "\nR-NOK %ld, %d %d\n", persondata.AEM, catalog->validentries, catalog->size);
                }
                break;
            }
            case 'm': // Modify operator
            {
                scanf ("%ld %hu", &persondata.AEM, &persondata.courses_left);
                if (persondata.courses_left == 0)
                {
                    charbin = '\0';
                    while (charbin == '\n')
                    {
                        scanf (" %c", &charbin);
                    }
                    printf ("\nInvalid input.\n");
                }
                else
                {
                    result = mod (catalog, persondata.AEM, persondata.courses_left);
                    if (result != -1)
                    {
                    fprintf (stdout, "\nM-OK %ld\n", persondata.AEM);
                    }
                    else
                    {
                        fprintf (stdout, "\nM-NOK %ld\n", persondata.AEM);
                    }
                }
                break;
            }
            case 's': // Sort operator
            {
                start = clock(); 
                comps = sort (catalog);
                end = clock();
                fprintf (stdout, "\nS-OK\n");
                fprintf (stderr, "$%ld\n", comps);
                fprintf (stderr, "Total time: %lf\n", (double)(end - start)/CLOCKS_PER_SEC); 
                break;
            }
            case 'f': // Find operator
            {
                scanf ("%ld", &persondata.AEM);
                start = clock();
                result = find (catalog, persondata.AEM, comptr);
                end = clock();
                if (result != -1)
                {
                    fprintf (stdout, "\nF-OK %s %s %hu\n", catalog->entries[result]->fullname.firstname, catalog->entries[result]->fullname.lastname, catalog->entries[result]->courses_left);
                    fprintf (stderr, "$%ld\n", comps);
                    fprintf (stderr, "Total time: %lf\n", (double)(end - start)/CLOCKS_PER_SEC);
                }
                else
                {
                    fprintf (stdout, "\nF-NOK %ld\n", persondata.AEM);
                    fprintf (stderr, "$%ld\n", comps);
                    fprintf (stderr, "Total time: %lf\n", (double)(end - start)/CLOCKS_PER_SEC);
                }
                break;
            }
            case 'p': // Print operator
            {
                print (catalog);
                break;
            }
            case 'c': // Clear operator
            {
                catalog->cleared = clear (catalog);
                fprintf (stdout, "\nC-OK\n");
                break;
            }
            case 'q': // Quit operator
            {
                quit (catalog);
                break;
            }
            default: // Wrong command
            {
                charbin = '\0';
                while (charbin != '\n')
                {
                    scanf ("%c", &charbin);
                }
                printf ("\nInvalid command.\n");
                break;
            }
        }
    } while (1);
}

// Initializes the catalog with the given size. If it fails, returns 43
catalogT *init (catalogT *array, int size)
{
    int i;

    array = (catalogT *)malloc (sizeof (catalogT ));
    array->entries = (entry **)malloc (sizeof(entry *) * size);
    array->size = size;
    for (i = 0; i < array->size; array->entries[i] = NULL, i++);
    return array;
}
// Clears all entries from the catalog
int clear (catalogT *array)
{
    int i;

    if (!array->cleared)   
    {
        /* Free all pointer to entries */
        for (i = 0;i < array->size; i++)
        {
            free (array->entries[i]);        
        }
        /* Free pointer to pointer to entries */
        free (array->entries);
        array->entries = NULL;
        array->size = 0;
        array->validentries = 0;
        array->sorted = 0;
        return array->cleared + 1;
    }
    else
    {
        return array->cleared;
    }
}
// Sorts the entries in ascending order based on the AEM Number
unsigned long int sort (catalogT *array)
{
    int i, j; 
    unsigned long int comps = 0;
    entry *temp = NULL;

    /* Insertion sort algorithm */
    for (i = 1; (array->size > 1) && (i < array->validentries); i++)
    {
        j = i - 1;
        temp = array->entries[i];
        while (j >= 0 && array->entries[j]->AEM > temp->AEM)
        {
            array->entries[j + 1] = array->entries[j];
            j--;
            comps++;
        }
        // If loop exited because AEM < tempt->AEM, count an extra comparison
        if (j >= 0)
            comps++;
        array->entries[j + 1] = temp;
    }
    array->sorted++;

    return comps;
}
// Adds a new entry if there is space in the catalog, else it expands by a fixed value given by the user
int add (catalogT *array, entry persondata, int plusminusval)
{
    int pos = basic_find (array, persondata.AEM, NULL), i;
    entry **entries = NULL;

    /* Failed search */
    if (pos == -1)
    {
        if (array->validentries == 0)
            pos = 0;
        pos = array->validentries;
    }

    if (pos < array->size)
        if (array->entries[pos] != NULL)
            if (array->entries[pos]->AEM == persondata.AEM)
                return -1;

    /* Expanding the size of catalog by plusminusval */ 
    if (pos == array->size && array->validentries == array->size)
    {
        entries = (entry **)realloc (array->entries, (array->size + plusminusval) * sizeof(entry *));
        if (!entries)
            return -1;
        array->size += plusminusval;
        for (i = pos; i < array->size; entries[i] = NULL, i++);
        array->entries = entries;
    }
    /* Allocating memory and copying the data to the array of entries */
    array->entries[pos] = (entry *)malloc (sizeof (entry));
    for (i = 0; i < NAMESIZE; array->entries[pos]->fullname.firstname[i] = '\0', i++);
    for (i = 0; i < NAMESIZE; array->entries[pos]->fullname.lastname[i] = '\0', i++);
    array->entries[pos]->AEM = persondata.AEM;
    strcpy (array->entries[pos]->fullname.firstname, persondata.fullname.firstname);
    strcpy (array->entries[pos]->fullname.lastname, persondata.fullname.lastname);
    array->entries[pos]->courses_left = persondata.courses_left;
    /* Keeping track of the No of entries, if the catalog is sorted or cleared */
    array->cleared = 0;
    array->validentries++;
    array->sorted = 0;

    return 0;
}
// Removes an entry that excists in the catalog and if there are more empty slots than a fixed value that is given by the user, it decreases the size by that value
int rmv (catalogT *array, int aem, int plusminusval)
{
    int pos = -1, lastpersonpos;

    if (array->validentries)
    {
        pos = basic_find (array, aem, NULL);
    }

    if (pos == -1 || pos == array->size || pos == array->validentries)
        return -1;

    for (lastpersonpos = array->size - 1; (array->entries[lastpersonpos] == NULL) && lastpersonpos != 0; lastpersonpos--);
    /* When the person we want to delete is not the last person by copying the last person's data to the entry we want to delete */
    if (pos != lastpersonpos)
    {
        array->entries[pos]->AEM = array->entries[lastpersonpos]->AEM;
        strncpy (array->entries[pos]->fullname.firstname, array->entries[lastpersonpos]->fullname.firstname, NAMESIZE - 1);
        strncpy (array->entries[pos]->fullname.lastname, array->entries[lastpersonpos]->fullname.lastname, NAMESIZE - 1);
        array->entries[pos]->courses_left = array->entries[lastpersonpos]->courses_left;
    }
    /* Freeing the memory from the last person's position */
    free (array->entries[lastpersonpos]);
    array->entries[lastpersonpos] = NULL;
    array->sorted = 0;
    array->validentries--;

    /* When the size of the array minus the No entries is greater than the plusminusval we decrease the size by plusminusval */
    if (array->size - array->validentries >= plusminusval)
    {
        array->size -= plusminusval;
        array->entries = (entry **)realloc (array->entries, array->size * sizeof(entry *));
    }

    return 0;
}
// Modifies the No of courses a student didn't pass
int mod (catalogT *array, int aem, int newNocourses)
{
    int pos = basic_find (array, aem, NULL);

    if (pos == array->size || pos == -1 || pos == array->validentries)
        return -1;

    array->entries[pos]->courses_left = newNocourses;

    return 0;
}
// Finds a student that excists in the catalog by the AEM No and counting the No of comparisons we made
int find (catalogT *array, int aem, unsigned long int *comptr)
{
    int pos = basic_find (array, aem, comptr);

    if (pos == array->size || pos == array->validentries)
        return -1;

    return pos;
}
// Helping function to determine wheather the search will be done linearly or binary
int basic_find (catalogT *array, int aem, unsigned long int *comptr)
{
    int pos;

    if (array->cleared)
        return -1;

    if (!array->sorted)
    {
        pos = linear_search (array, aem);
        if (comptr != NULL)
        {
            if (array->validentries != pos)
            {
                *comptr = pos + 1;
            }
            else
            {
                *comptr = pos;
            }
        }
    }
    else
    {
        pos = binary_search (array, aem, comptr);
    }

    return pos;
}
// Searches in a linear way the AEM that the user asks to be found
int linear_search (catalogT *array,int aem)
{
    int i;
    if (!array->validentries)
        return -1;
    for (i = 0; i < array->validentries; i++)
    {
        if (array->entries[i] != NULL)
            if (array->entries[i]->AEM == aem)
                return i;
    }

    return i;
}
// Searches in a binary way the AEM that the user asks to be found and counting the No of comparisons we made
int binary_search (catalogT *array, int aem, unsigned long int *comptr)
{
    int i, from = 0, to = array->validentries - 1;

    if (comptr != NULL)
        *comptr = 0;

    while (from <= to)
    {
        i = (from + to) / 2;
        if (array->entries[i]->AEM == aem)
        {
            if (comptr != NULL)
                (*comptr)++;
            return i;
        }
        else if (array->entries[i]->AEM > aem)
        {
            to = i - 1;
            if (comptr != NULL)
                (*comptr) += 2;
        }
        else
        {
            from = i + 1;
            if (comptr != NULL)
                (*comptr) += 2;
        }
    }
    return -1;
}
// Prints the catalog
void print (catalogT *array)
{
    int i;
    printf ("\n##\n");
    for (i = 0; i < array->validentries; i++)
    {
        if (array->entries[i] != NULL)
            printf ("%lu %s %s %hu\n", array->entries[i]->AEM, array->entries[i]->fullname.firstname, array->entries[i]->fullname.lastname, array->entries[i]->courses_left);
    }
}
// Erases all the antries and the catalog and then quits
void quit (catalogT *array)
{
    if (!array->cleared)
        clear(array);     
    free (array);
    exit (0);
}
unsigned long int quick_sort (catalogT *array, int from, int to)
{
    // Devided in half method
    entry *indexleft = NULL, *indexright = NULL, *temp;
    int pivot;
    unsigned long int comps = 0;

    if (array->validentries == 1 || to - from == 1)
    {
        array->sorted++;
        return comps;
    }

    if (array->validentries % 2)
    {
    pivot = (array->validentries / 2);
    }
    else
    {
        pivot = (array->validentries / 2) - 1;
    }
    temp = array->entries[array->validentries - 1];
    array->entries[array->validentries - 1] = array->entries[pivot];
    array->entries[pivot] = temp;
    indexleft = array->entries[from];
    indexright = array->entries[to];

    do
    {
        while (indexleft->AEM < array->entries[pivot]->AEM && from < array->validentries)
        {
            from++;
            indexleft = array->entries[from];
        }
        while (indexright->AEM < array->entries[pivot]->AEM && to > 0)
        {
            to--;
            indexright = array->entries[to];
        }

        if (from == array->validentries)
        {
            if (indexright->AEM > array->entries[to - 1])
            {
                array->sorted++;
                return comps;
            }
            else 
        }
        else if (to == 0)
        {
            array
        }

        if (from > to)
        {
            temp = array->entries[pivot];
            array->entries[pivot] = array->entries[array->validentries - 1];
            array->entries[array->validentries - 1] = temp;
            comps++;
            from = pivot + 1;
            to = pivot - 1;
            comps += quick_sort (array, 0, to);
            comps += quick_sort (array, from, array->validentries);
            return comps;
        }
        else
        {
            temp = array->entries[from];
            array->entries[from] = array->entries[to];
            array->entries[to] = temp;
            comps++;
        }
    } while (!array->sorted);
    
}