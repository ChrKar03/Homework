#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "project2.h"

#define HASH_RESIZE if  \
(operator == head->size * 2) (head->size = head->size * 2); else (head->size = head->size / 2);

/* Types of data */

typedef struct 
{
    char firstname[NAMESIZE];
    char lastname[NAMESIZE];
} nameT;

struct list
{
    unsigned short int course_id;
    struct list *nxt;
};

typedef struct list lisT;

typedef struct 
{
    unsigned long int AEM;
    nameT fullname;
    unsigned short int courses_left;
    lisT *reg_courses;
} entry;

struct hash_list
{
    entry *person;
    struct hash_list *nxt;
    struct hash_list *prv;
};

typedef struct hash_list hash_lisT;

typedef struct
{
    hash_lisT *bucket;
    int bck_size;
} buckeT;

typedef struct
{
    entry **entries;
    int size;
    int validentries;
    int sorted;
    int cleared;
} catalogT;

typedef struct
{
    buckeT *hashead;
    int size;
    int Nofentries;
    int initialsize;
} hash_table;

catalogT *init_entries (catalogT *array, int size);
hash_table *init_hash (int size, int initialsize);
hash_table *clear (catalogT *array, hash_table *head);
unsigned long int sort (catalogT *array);
int add (catalogT *array, hash_table *head, entry persondata, int plusminusval);
int rmv (catalogT *array, hash_table *head, int aem, int plusminusval);
int mod (catalogT *array, int aem, int newNocourses);
int find (catalogT *array, int aem, unsigned long int *comptr);
int basic_find(catalogT *array, int aem, unsigned long int *comp);
int linear_search(catalogT *array, int aem);
int binary_search (catalogT *array, int aem, unsigned long int *comp);
void print(catalogT *array);
int reg (catalogT *array, unsigned long int aem, unsigned short int course_id);
int unreg (catalogT *array, unsigned long int aem, unsigned short int course_id);
lisT *isreg (catalogT *array, unsigned long int aem, unsigned short int course_id, int *position);
void print_course_list (catalogT *array, unsigned long int aem);
unsigned long int hash (char *str);
buckeT *rehash (hash_table *head, entry *person, int operator);
void hash_add (hash_table *head, entry *person);
void hash_rmv (hash_table *head, entry *person);
hash_lisT *find_by_name (hash_table *head, char *lastname, unsigned long int *comps);
void print_by_name (hash_table *head);
int mx_search (hash_table *head, int max);
void quit(catalogT *array, hash_table *head);

int main (int argc, char *argv[]) 
{
    if (argc != 4)
        return 42;

    int result, i;
    unsigned short int course_id;
    unsigned long int comps = 0;
    hash_lisT *personToprint = NULL;
    catalogT *catalog = NULL;
    hash_table *head = NULL;
    entry persondata;
    char operator, charbin, *firstptr, *lastptr;

    catalog = init_entries (catalog, atoi(argv[1]));
    head = init_hash (atoi(argv[3]), atoi(argv[3]));

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
                    if (strlen (firstptr) > NAMESIZE - 1 || strlen (lastptr) > NAMESIZE - 1)
                        fprintf (stdout,"\nTrimmed name(s).\n");
                    strncpy (persondata.fullname.firstname, firstptr, NAMESIZE - 1);
                    persondata.fullname.firstname[NAMESIZE - 1] = '\0';
                    strncpy (persondata.fullname.lastname, lastptr, NAMESIZE - 1);
                    persondata.fullname.lastname[NAMESIZE - 1] = '\0';
                    i = 0;
                    while (persondata.fullname.firstname[i])
                    {
                        persondata.fullname.firstname[i] = toupper (persondata.fullname.firstname[i]);
                        i++;
                    }
                    i = 0;
                    while (persondata.fullname.lastname[i])
                    {
                        persondata.fullname.lastname[i] = toupper (persondata.fullname.lastname[i]);
                        i++;
                    }
                    result = add (catalog, head, persondata, atoi (argv[2]));
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
                result = rmv (catalog, head, persondata.AEM, atoi (argv[2]));
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
                comps = sort (catalog);
                fprintf (stdout, "\nS-OK\n");
                fprintf (stderr, "$%ld\n", comps);
                break;
            }
            case 'f': // Find operator
            {
                scanf ("%ld", &persondata.AEM);
                result = find (catalog, persondata.AEM, &comps);
                if (result != -1)
                {
                    fprintf (stdout, "\nF-OK %s %s %hu\n", catalog->entries[result]->fullname.firstname, catalog->entries[result]->fullname.lastname, catalog->entries[result]->courses_left);
                    fprintf (stderr, "$%ld\n", comps);
                }
                else
                {
                    fprintf (stdout, "\nF-NOK %ld\n", persondata.AEM);
                    fprintf (stderr, "$%ld\n", comps);
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
                head = clear (catalog, head);
                fprintf (stdout, "\nC-OK\n");
                break;
            }
            case 'g': // Register operator
            {
                
                scanf ("%ld %hd", &persondata.AEM, &course_id);
                if (persondata.AEM == 0 || course_id == 0)
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
                    result = reg (catalog, persondata.AEM, course_id);
                    switch (result)
                    {
                        case -3: // NO MEMORY
                        {
                            fprintf (stdout, "\nG-NOK MEM\n");
                            break;
                        }
                        case -2: // COURSE ALREADY EXISTS
                        {
                            fprintf (stdout, "\nG-NOK %hd\n", course_id);
                            break;
                        }
                        case -1: // NO AEM
                        {
                            fprintf (stdout, "\nG-NOK %ld\n", persondata.AEM);
                            break;
                        }
                        case 0: // SUCCESSFUL REGISTATION
                        {
                            fprintf (stdout, "\nG-OK %ld %hd\n", persondata.AEM, course_id);
                            break;
                        }
                    }
                }
                break;
            }
            case 'u': // Unregister operator
            {
                scanf ("%ld %hd", &persondata.AEM, &course_id);
                if (persondata.AEM == 0 || course_id == 0)
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
                    result = unreg (catalog, persondata.AEM, course_id);
                    switch (result)
                    {
                        case -2: // COURSE DOESNT EXISTS
                            {
                                fprintf (stdout, "\nU-NOK %hd\n", course_id);
                                break;
                            }
                        case -1: // AEM DOESNT EXIST
                        {
                            fprintf (stdout, "\nU-NOK %ld\n", persondata.AEM);
                            break;
                        }
                        case 0: // SUCCESSFUL
                        {
                            fprintf (stdout, "\nU-OK %ld %hd\n", persondata.AEM, course_id);
                            break;
                        }
                    }
                }
                break;
            }
            case 'i': // Checking if a student is registered operator
            {
                int pos = -1;
                lisT *resulT;
                scanf ("%ld %hd", &persondata.AEM, &course_id);
                if (persondata.AEM == 0 || course_id == 0)
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
                    resulT = isreg (catalog, persondata.AEM, course_id, &pos);
                    // AEM DOESNT EXIST
                    if (!resulT)
                    {
                        fprintf (stdout, "\nI-NOK %ld\n", persondata.AEM);
                    }
                    else if (resulT->course_id == course_id || resulT->nxt->course_id == course_id)
                    {
                        fprintf (stdout, "\nYES\n");
                    }
                    else
                    {
                        fprintf (stdout, "\nNO\n");
                    }
                }
                break;
            }
            case 'l': // Prints the list of courses operator
            {
                scanf ("%ld", &persondata.AEM);
                if (persondata.AEM == 0)
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
                    print_course_list (catalog, persondata.AEM);
                    break;
                }
            }
            case 'n': // Finding person by lastname operator
            {
                comps = 0;
                scanf ("%ms", &lastptr);
                if (strlen (lastptr) > NAMESIZE - 1)
                        fprintf (stdout,"\nTrimmed name(s).\n");
                strncpy (persondata.fullname.lastname, lastptr, NAMESIZE - 1);
                persondata.fullname.lastname[NAMESIZE - 1] = '\0';
                i = 0;
                while (persondata.fullname.lastname[i])
                {
                    persondata.fullname.lastname[i] = toupper (persondata.fullname.lastname[i]);
                    i++;
                }
                personToprint = find_by_name (head, persondata.fullname.lastname, &comps);
                if (!personToprint)
                {
                    printf ("\nN-NOK %s\n", persondata.fullname.lastname);
                }
                else
                {
                    printf ("\nN-OK %s\n", persondata.fullname.lastname);
                    for (; personToprint->person && comps++ && !strcmp(personToprint->person->fullname.lastname, persondata.fullname.lastname); personToprint = personToprint->nxt)
                    {
                        printf ("%s %ld %hd\n", personToprint->person->fullname.firstname, personToprint->person->AEM, personToprint->person->courses_left);
                    }
                }
                fprintf (stderr, "@%ld\n", comps);
                free (lastptr);
                break;
            }
            case 't': // Prints hash table operator
            {
                print_by_name (head);
                break;
            }
            case 'q': // Quit operator
            {
                quit (catalog, head);
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

// Initializes the catalog with the given size. If it fails, returns 43.
catalogT *init_entries (catalogT *array, int size)
{
    int i;

    array = (catalogT *)malloc (sizeof (catalogT));
    array->entries = (entry **)malloc (sizeof(entry *) * size);
    array->size = size;
    array->sorted = 0;
    array->validentries = 0;
    array->cleared = 0;
    for (i = 0; i < array->size; array->entries[i] = NULL, i++);
    return array;
}
// Initializes the hash_size with the given size.
hash_table* init_hash (int size, int initialsize)
{
    int i;

    hash_table* head = (hash_table *)malloc (sizeof(hash_table));
    head->hashead = (buckeT *)malloc (sizeof(buckeT ) * size);
    head->size = size;
    head->initialsize = initialsize;
    head->Nofentries = 0;
    for (i = 0; i < head->size; i++)
    {
        head->hashead[i].bucket = (hash_lisT *)malloc (sizeof (hash_lisT));
        head->hashead[i].bucket->person = NULL;
        head->hashead[i].bucket->nxt = head->hashead[i].bucket;
        head->hashead[i].bucket->prv = head->hashead[i].bucket; 
        head->hashead[i].bck_size = 0;
    }
    return head;
}
// Clears all entries from the catalog.
hash_table *clear (catalogT *array, hash_table *head)
{
    int i, size;
    lisT *storage_node, *delete_node = NULL;
    hash_lisT *tmp = NULL, *rmv = NULL;

    if (!array->cleared)
    {
        /* Free all pointers from hash table and resizing to original size*/
        for (i = 0; i < head->size; i++)
        {
            if (head->hashead[i].bucket)
                for (tmp = head->hashead[i].bucket->nxt, rmv = tmp; tmp->person; tmp = tmp->nxt, free (rmv), rmv = tmp);
            free (head->hashead[i].bucket);
        }
        size = head->initialsize;
        free (head->hashead);
        free (head);
        head = init_hash (size, size);

        /* Free all pointers of courses list and entries */
        for (i = 0;i < array->size; i++)
        {
            storage_node = NULL;
            if (array->entries[i] && array->entries[i]->reg_courses)
                for (storage_node = array->entries[i]->reg_courses->nxt; storage_node != array->entries[i]->reg_courses; delete_node = storage_node, storage_node = storage_node->nxt, free (delete_node));
            free (storage_node);
            free (array->entries[i]);
        }
        /* Free pointer to pointer to entries */
        free (array->entries);
        array->entries = NULL;
        array->size = 0;
        array->validentries = 0;
        array->sorted = 0;
        array->cleared += 1;
    }
    return head;
}
// Sorts the entries in ascending order based on the AEM Number.
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
// Adds a new entry if there is space in the catalog, else it expands by a fixed value given by the user.
int add (catalogT *array, hash_table *head, entry persondata, int plusminusval)
{
    int pos = basic_find (array, persondata.AEM, NULL), i;
    entry **entries = NULL, *tmp = NULL;

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
    tmp = (entry *)malloc (sizeof (entry));
    if (!tmp)
        return -1;
    array->entries[pos] = tmp;
    array->entries[pos]->reg_courses = (lisT *)malloc (sizeof (lisT));
    array->entries[pos]->reg_courses->nxt = array->entries[pos]->reg_courses;
    array->entries[pos]->reg_courses->course_id = 0;
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

    hash_add (head, array->entries[pos]);

    return 0;
}
// Removes an entry that excists in the catalog and if there are more empty slots than a fixed value that is given by the user, it decreases the size by that value.
int rmv (catalogT *array, hash_table *head, int aem, int plusminusval)
{
    int pos = -1, lastpersonpos;
    lisT *tmp = NULL, *delete = NULL;

    if (array->validentries)
    {
        pos = basic_find (array, aem, NULL);
    }

    if (pos == -1 || pos == array->size || pos == array->validentries)
        return -1;

    hash_rmv (head, array->entries[pos]);

    for (lastpersonpos = array->size - 1; (array->entries[lastpersonpos] == NULL) && lastpersonpos != 0; lastpersonpos--);
    /* deleting the memory of the course list that the person has */
    for (tmp = array->entries[pos]->reg_courses->nxt; tmp != array->entries[pos]->reg_courses; delete = tmp, tmp = tmp->nxt, free (delete));
    free (tmp);
    /* When the person we want to delete is not the last person, we copy the last person's address to the entry we want to delete */
    if (pos != lastpersonpos)
    {
        // Moving lasts person data to the pointer we removing
        free (array->entries[pos]);
        array->entries[pos] = array->entries[lastpersonpos];
    }
    else
    {
        free (array->entries[lastpersonpos]);
    }
    /* Freeing the memory from the last person's position */
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
// Modifies the No of courses a student didn't pass.
int mod (catalogT *array, int aem, int newNocourses)
{
    int pos = basic_find (array, aem, NULL);

    if (pos == array->size || pos == -1 || pos == array->validentries)
        return -1;

    array->entries[pos]->courses_left = newNocourses;

    return 0;
}
// Finds a student that excists in the catalog by the AEM No and counting the No of comparisons we made.
int find (catalogT *array, int aem, unsigned long int *comptr)
{
    int pos = basic_find (array, aem, comptr);

    if (pos == array->size || pos == array->validentries)
        return -1;

    return pos;
}
// Helping function to determine wheather the search will be done linearly or binary.
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
int linear_search (catalogT *array, int aem)
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
// Searches in a binary way the AEM that the user asks to be found and counting the No of comparisons we made.
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
// Prints the catalog.
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
// Registers a student to a course.
int reg (catalogT *array, unsigned long int aem, unsigned short int course_id)
{
    int pos;
    lisT *result = isreg (array, aem, course_id, &pos), *tmp = NULL;
    // Case AEM doesnt exists
    if (!result && (pos == -1 || pos == array->validentries))
        return -1;
    // Case were course already exists
    if (result && ((result->course_id == course_id) || (result->nxt && result->nxt->course_id == course_id)))
        return -2;
    // Case for empty course list
    if (result && result->course_id == 0)
    {
        result->course_id = course_id;
    }
    else
    {
        tmp = (lisT *)malloc (sizeof(lisT));
        if (!tmp)
            return -3;
        // Case where the previous pointer in the course list is also the head
        if (result == array->entries[pos]->reg_courses)
        {
            tmp->course_id = course_id;
            // Insert from the right side
            if (course_id > result->course_id)
            {
                tmp->nxt = result->nxt;
                result->nxt = tmp;
            }
            // Insert from the left side
            else
            {
                tmp->nxt = result;
                // Finding the last pointer of the list that points to the first
                if (result->nxt != result)
                    for (; result->nxt != array->entries[pos]->reg_courses; result = result->nxt);
                result->nxt = tmp;
                array->entries[pos]->reg_courses = tmp;
            }
        }
        // Case where we insert between two course list elements. diffrent from the head
        else
        {
            tmp->nxt = result->nxt;
            result->nxt = tmp;
            tmp->course_id = course_id;
        }
    }
    return 0;
}
// Unregisters a student from a course.
int unreg (catalogT *array, unsigned long int aem, unsigned short int course_id)
{
    int pos;
    lisT *result = isreg (array, aem, course_id, &pos), *tmp = NULL;
    // AEM doesnt exist
    if (!result && (pos == -1 || pos == array->validentries))
        return -1;
    // Case where we point to the head of the list
    if (result == array->entries[pos]->reg_courses)
    {
        // Empty list
        if (!result || result->course_id == 0)
            return -2;
        // Removing head as last element
        if (result->nxt == result && result->course_id == course_id)
        {
            result->course_id = 0;
            array->entries[pos]->reg_courses = array->entries[pos]->reg_courses;
        }
        // Removing head from array with more than one elements
        else if (result->course_id == course_id)
        {
            tmp = result;
            // Finding the last element from the list that points to the head
            for (; result->nxt != array->entries[pos]->reg_courses; result = result->nxt);
            result->nxt = tmp->nxt;
            array->entries[pos]->reg_courses = tmp->nxt;
        }
        // Removing the next element from the head
        else if (result->nxt->course_id == course_id)
        {
            tmp = result->nxt;
            result->nxt = tmp->nxt;
        }
        // Course doesnt exist
        else
        {
            return -2;
        }
    }
    // Removing the next element from were we point to
    else if (result->nxt->course_id == course_id)
    {
        tmp = result->nxt;
        result->nxt = tmp->nxt;
    }
    // Course doesnt exist
    else
    {
        return -2;
    }

    free (tmp);
    return 0;
}
// Finds if a student is registered in course.
lisT *isreg (catalogT *array, unsigned long int aem, unsigned short int course_id, int *position)
{
    int pos = basic_find (array, aem, NULL);
    lisT *tmp = NULL;
    // AEM doesnt exist
    if (pos == array->validentries || pos == -1)
    {
        *position = pos;
        return NULL;
    }
    // Returning the previous pointer
    else
    {
        tmp = array->entries[pos]->reg_courses;
        for (; tmp && tmp->nxt != array->entries[pos]->reg_courses && tmp->nxt->course_id < course_id; tmp = tmp->nxt);
        *position = pos;
    }
    return tmp;
}
// Prints the list of the courses that a student had registered to.
void print_course_list (catalogT *array, unsigned long int aem)
{
    int pos = basic_find (array, aem, NULL);
    if (pos == array->validentries || pos == -1)
    {
        printf ("\nL-NOK %ld\n", aem);
        return;
    }
    lisT *tmp = array->entries[pos]->reg_courses;

    printf ("\nL-OK %s %ld\n", array->entries[pos]->fullname.lastname, array->entries[pos]->AEM);
    while (tmp)
    {
        if (tmp->course_id != 0)
            printf ("%d\n", tmp->course_id);
        if (tmp->nxt == array->entries[pos]->reg_courses)
            break;
        tmp = tmp->nxt;
    }
}
// Returns the value of the last name based on a specific encoding
unsigned long int hash (char *str)
{
    unsigned long int hash = 5381;
    int c;

    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}
// Making a new hash table depending on the value of the load factor
buckeT *rehash (hash_table *head, entry *person, int operator)
{
    int i;
    hash_lisT *tmp = NULL, *store = NULL, *delete = NULL;
    hash_table *tmp_hash_table = NULL;
    buckeT *tmp_bucket = NULL;
    // Creating the new hash table
    tmp_hash_table = init_hash (operator, head->initialsize);

    if (!tmp_hash_table)
        return head->hashead;
    // Adding the old elements to the new hash table
    for (i = 0; i < head->size; i++)
    {
        if (head->hashead[i].bucket)
            for (tmp = head->hashead[i].bucket->nxt; tmp->person; hash_add (tmp_hash_table, tmp->person), tmp = tmp->nxt);
    }
    // Adding the new entry to the new hash table directly
    if (operator == head->size * 2)
        hash_add (tmp_hash_table, person);
    // Deleting the old hash table
    for (i = 0; i < head->size; i++)
    {
        for (store = head->hashead[i].bucket->nxt, delete = store; store->person; store = store->nxt, free (delete), delete = store);
        free (delete);
    }
    free (head->hashead);
    // Passing the metadata to the new hash table
    head->Nofentries = tmp_hash_table->Nofentries;
    tmp_bucket = tmp_hash_table->hashead;
    HASH_RESIZE;
    free (tmp_hash_table);

    return tmp_bucket;
}
// Finds a person by the lastname.
hash_lisT *find_by_name (hash_table *head, char *lastname, unsigned long int *comps)
{
    unsigned long int slot = hash (lastname) % head->size;
    hash_lisT *tmp = head->hashead[slot].bucket->nxt;

    while (tmp->person)
    {
        if (!strcmp(tmp->person->fullname.lastname, lastname))
        {
            (*comps)++;
            return tmp;
        }
        tmp = tmp->nxt;
        (*comps)++;
    }
    return NULL;
}
// Adds the person to the hash table.
void hash_add (hash_table *head, entry *person)
{
    double load;
    unsigned long int slot = hash (person->fullname.lastname) % head->size;
    hash_lisT *tmp = NULL, *storage  = NULL;

    // Finding the previous pointer from the place we want to put the hash_lisT struct
    for (tmp = head->hashead[slot].bucket->nxt; tmp->nxt->person && strcmp (tmp->nxt->person->fullname.lastname, person->fullname.lastname) < 0; tmp = tmp->nxt);
    if (tmp->nxt->person && !strcmp (tmp->nxt->person->fullname.lastname, person->fullname.lastname))
    {
        while (tmp->nxt->person && !strcmp (tmp->nxt->person->fullname.lastname, person->fullname.lastname) && tmp->nxt->person->AEM < person->AEM)
        {
            tmp = tmp->nxt;
        }
    }
    
    load = (double)(head->Nofentries + 1) / head->size;
    if (load < HIGH_LOAD)
    {
        storage = (hash_lisT *)malloc (sizeof (hash_lisT));
        if (!storage)
            return;
        // When tmp points at the sentinel
        if (tmp->person)
        {
            // Case we need to put the hast_lisT struct on the right side from where we point to
            if (strcmp (tmp->person->fullname.lastname, person->fullname.lastname) < 0 || (!strcmp (tmp->person->fullname.lastname, person->fullname.lastname) && tmp->person->AEM < person->AEM))
            {
                storage->nxt = tmp->nxt;
                tmp->nxt = storage;
                storage->prv = tmp;
                storage->nxt->prv = storage;
            }
            // Case we need to put the hash_lisT struct on the left side from where we point to (changing where the sentinel points to)
            else if (tmp->nxt->person)
            {
                tmp = tmp->prv;
                storage->nxt = tmp->nxt;
                storage->prv = tmp;
                storage->prv->nxt = storage;
                storage->nxt->prv = storage;
            }
            // Case we change where the sentinel points to but there is one hash_lisT struct only
            else
            {
                storage->nxt = tmp;
                storage->prv = tmp->prv;
                storage->prv->nxt = storage;
                tmp->prv = storage;
            }
        }
        // Case of empty bucket
        else
        {
            storage->nxt = tmp;
            storage->prv = tmp;
            tmp->nxt = storage;
            tmp->prv = storage;
        }
        storage->person = person;
        head->Nofentries++;
        head->hashead[slot].bck_size++;
    }
    else
    {
        head->hashead = rehash (head, person, head->size * 2);
    }
}
// Removes a person from the hash table
void hash_rmv (hash_table *head, entry *person)
{
    double load;
    unsigned long int slot = hash (person->fullname.lastname) % head->size;
    hash_lisT *tmp = NULL, *storage  = NULL;

    // Finding the previous pointer from the place we want to remove the hash_lisT struct
    for (tmp = head->hashead[slot].bucket->nxt; tmp->nxt->person && strcmp(tmp->nxt->person->fullname.lastname, person->fullname.lastname) < 0; tmp = tmp->nxt);
    if (tmp->nxt->person && !strcmp(tmp->nxt->person->fullname.lastname, person->fullname.lastname))
    {
        while (tmp->nxt->person && !strcmp(tmp->nxt->person->fullname.lastname, person->fullname.lastname) && tmp->nxt->person->AEM < person->AEM)
        {
            tmp = tmp->nxt;
        }
    }
    // Removing the head
    if (tmp->person && !strcmp (tmp->person->fullname.lastname, person->fullname.lastname) && tmp->person->AEM == person->AEM)
    {
        storage = tmp;
        tmp = storage->prv;
        tmp->nxt = storage->nxt;
        storage->nxt->prv = storage->prv;
    }
    // Removing the next element
    else
    {
        storage = tmp->nxt;
        tmp->nxt = storage->nxt;
        storage->nxt->prv = tmp;
    }
    // Modifiing the metadata
    head->Nofentries--;
    head->hashead[slot].bck_size--;
    free (storage);

    load = (double) head->Nofentries / head->size;
    if (head->size > head->initialsize && load <= LOW_LOAD)
        head->hashead = rehash (head, person, head->size / 2);

}
// Prints the hash table by last names.
void print_by_name (hash_table *head)
{
    int i;
    hash_lisT *tmp = NULL;

    printf ("\n##\n");
    printf ("%d %d %.2lf %d\n", head->size, head->Nofentries, (double) head->Nofentries / head->size, mx_search (head, 0));

    for (i = 0; i < head->size; i++)
    {
        printf ("%d %d", i, head->hashead[i].bck_size);
        if (head->hashead[i].bucket->nxt->person)
        {
            for (tmp = head->hashead[i].bucket->nxt; tmp->person ; tmp = tmp->nxt)
            {
                printf (" [ %ld %s %s %hd ]", tmp->person->AEM, tmp->person->fullname.firstname, tmp->person->fullname.lastname, tmp->person->courses_left);
            }
        }
        printf ("\n\n");
    }
}
// Returns the biggest bucket in hash table.
int mx_search (hash_table *head, int max)
{
    int i;

    for (i = 0; i < head->size; i++)
    {
        if (head->hashead[i].bck_size > max)
            max = head->hashead[i].bck_size;
    }
    return max;
}
// Erases all the antries and the catalog and then quits.
void quit (catalogT *array, hash_table *head)
{
    int i;
    hash_lisT *tmp = NULL, *rmv = NULL;

    if (!array->cleared)
    {
        head = clear(array, head);
    }
    for (i = 0; i < head->size; i++)
    {
        for (tmp = head->hashead[i].bucket->nxt, rmv = tmp; tmp->person; )
        {
            tmp = tmp->nxt;
            free (rmv);
            rmv = tmp;
        }
        free (head->hashead[i].bucket);
    }
    free (head->hashead);
    free (array);
    free (head);
    exit (0);
}
