    /*
     * ONOMATEPWNYMO: ΧΡΗΣΤΟΣ ΚΑΡΑΓΙΑΝΝΗΣ
     * AEM:03544
     * 
     * This program reads a text, stores every one-time appeared word and the order
     * they are at the text and: Reprint the text (with or without the synonyms of 
     * some  * words, if they are given), calculates the number that a word has 
     * been used and prints a histogram and , finally, calculates the ratio of two 
     * ways we can store  * the data of the text.
     * 
     */

    #include "hw2.h"
    #include <ctype.h>
    #include <stdio.h>
    #include <string.h>

    int main(int argc, char* argv[]) {
        init_stack_size(MAX_WORDS, MAX_WORD_LEN, MAX_SYNONYMS);
        return hw2_main(argc, argv);
    }

    /* 
     * This function registers the inputs words one by one in order. If the word 
     * appears for the first time it stores the word as also the address of the word
     * in the storage matrix, else it just stores the address of the coresponding 
     * address in the storage matrix. 
     */

    void wordRegister (char storage[][MAX_WORD_LEN], char *ptr[][2]) {
        
        int readingoperator, i = 0, j, k, wordcounter = 0, check, samewordcheck,
        exit = 0;
        char input[MAX_WORD_LEN];
        
        ptr[i][0] = storage[0];
        
        do {
            if (exit)
                break;
            /* Array initiallazation */
            for (k = 0; k < MAX_WORD_LEN; k++) {
                input[k] = '\0';
            }
            readingoperator = scanf("%s", input);
            /* Check for the loop to stop when the input ends */
            if (readingoperator != EOF) {
                j = 0;
                /* Convarting Capitals to lower letters */
                while (input[j]) {
                    input[j] = tolower(input[j]);
                    j++;
                }
                check = 0;
                for (j = 0; (j < wordcounter ) || (j == 0); j++) {
                    /* 
                     * If check and samewordcheck have the same value after we 
                     * examine letter by letter the input with the stored words, 
                     * that means the input and the word of the matrix we are at 
                     * are the same 
                     */
                    samewordcheck = check;
                    for (k = 0; k < MAX_WORD_LEN; k++) {
                        if (input[k] != storage[j][k]) {
                            check++;
                            break;
                        }
                    }
                    /* Storage proccess if the input exist in the storage matrix */
                    if (check == samewordcheck) {
                        ptr[i][0] = storage[j];
                        i++;
                        check = 0;
                        if (i == MAX_WORDS)
                            exit++;
                        break;
                    }
                }
                /* New word storage proccess */
                if (check > 0) {
                    ptr[i][0] = storage[wordcounter];
                    strcpy(ptr[i][0], input);
                    i++;
                    if (wordcounter < MAX_WORDS - 1) {
                        wordcounter++;
                    }
                    if (i == MAX_WORDS)
                        exit++;
                }
            }
        } while (readingoperator != EOF);
    }

    /* 
     * This function reprints the text that was inputed, but every line has a 
     * specific amount of characters. If the word which is about to be printed 
     * exeeds the limit, it prints an \n character and then the whole word. 
     */

    void textPrinter (char *ptr[][2], int linelen) {
        
        int i, outputLength = 0, fullslot = 0;
        
        /* Finding the No of slots that aren't empty */
        
        while ((fullslot < MAX_WORDS) && ptr[fullslot][0]) {
            fullslot++;
        }
        
        printf ("\n");
        /* 
         * First character of the text is converted to capital letter and the 
         * synonym (if there is). 
         */
        *ptr[0][0] = toupper(*ptr[0][0]);
        if (ptr[0][1] != '\0') {
            *ptr[0][1] = toupper(*ptr[0][1]);
             /* 
              * Measouring the length of the line with the 
              * space character plus the 3 characters with 
              */
            outputLength += strlen(ptr[0][1]) + 3;
        }
        /* Measouring the length of the line with the space character */
        outputLength += strlen(ptr[0][0]);
        printf ("%s", ptr[0][0]);
        if (ptr[0][1] != '\0')
            printf (" (%s)", ptr[0][1]);
        /* Converting them back to low letters */
        *ptr[0][0] = tolower(*ptr[0][0]);
        if (ptr[0][1] != '\0')
            *ptr[0][1] = tolower(*ptr[0][1]);
        for (i = 1; i < fullslot; i++) {
            if (ptr[i][0] == '\0')
                break;
            /* 
             * If the character that is about to be printed is a one of the three
             * special (. ? !), the capitalEnable becomes true and the next word 
             * that will be printed will be with a capital letter.
             */
            if (((*ptr[i][0] == '!') || (*ptr[i][0] == '?') || \
            (*ptr[i][0] == '.')) && (i + 1 < MAX_WORDS)) {
                if (ptr[i + 1][0] != '\0')
                    *ptr[i + 1][0] = toupper(*ptr[i + 1][0]);
                if (ptr[i + 1][1] != '\0')
                    *ptr[i + 1][1] = toupper(*ptr[i + 1][1]);
            }
            outputLength += strlen(ptr[i][0]) + 1;
            /* 
             * If the outputLength exceeds the line 
             * length, the \n character is printed 
             */
            if (outputLength >= MAX_LINE_LEN + 1) {
                printf ("\n");
                outputLength = strlen(ptr[i][0]);
                printf ("%s", ptr[i][0]);
            }
            else {
                printf (" %s", ptr[i][0]);
            }
            if (ptr[i][1] != '\0') {
                outputLength += strlen(ptr[i][1]) + 3;
                if (outputLength >= MAX_LINE_LEN + 1) {
                    printf ("\n");
                    outputLength = strlen(ptr[i][1]) + 2;
                    printf ("(%s)", ptr[i][1]);
                }
                else {
                    printf (" (%s)", ptr[i][1]);
                }
            }
            /* Lowering the first letter in case it was capital */
            *ptr[i][0] = tolower(*ptr[i][0]);
            if (ptr[i][1] != '\0')
                *ptr[i][1] = tolower(*ptr[i][1]);
        }
        printf ("\n");
    }

    /* 
     * This function calculates how many times each 
     * word has been used from the matrix with words 
     */

    void wordCalc (char *ptr[][2], int array[]) {
        
        int i = 0, j, k = 0, checkedwords[MAX_WORDS];
        char input[MAX_WORD_LEN];
        
        /* 
         * Checkedwords matrix is used to store the lines 
         * of the pointer matrix that have been checked 
         *
         * initiallazation of matrix.
         */
        
        for (j = 0; j < MAX_WORDS; j++) {
            checkedwords[j] = 0;
        }
        
        do {
            
            /* Input initiallazation */
            
            for (j = 0; j < MAX_WORD_LEN; j++) {
                input[j] = '\0';
            }
            
            /* Case where the matrix pointer has the NULL value */
            
            if (ptr[k][0] == '\0') {
                k++;
                continue;
            }
            strcpy (input, ptr[k][0]);
            
            /* 
             * Checking every slot of the matrix and increasing 
             * it's value everytime the same word appears 
             */
            
            for (j = 0; j < MAX_WORDS; j++) {
                if (ptr[j][0] != '\0' && (!strcmp (input, ptr[j][0])) &&  \
                    (checkedwords[j] == 0)) {
                    checkedwords[j]++;
                    array[i]++;
                }
            }
            
            /* 
             * Changing slot only when the one we are at 
             * is non-zero, so the words appear in order 
             */
            
            if (array[i])
                i++;
            k++;
        } while (k < MAX_WORDS);
    }

    /* 
     * This function takes the words that have been stored and 
     * the times that have been used and prints a histogram.
     */

    void histogramPrinter (char storage[][MAX_WORD_LEN], int array[]) {
        
        int i, repeat, inputCopy;
        char formatstr[16];
        
        sprintf (formatstr, "%%%ds: ", MAX_WORD_LEN);
        
        for (i = 0; i < MAX_WORDS; i++) {
            if (!storage[i][0])
                break;
            printf (formatstr, storage[i]);
            inputCopy = array[i];
            /* 
             * We take the actual input from the array that has stored the No 
             * of a word appearences to another variable (inputCopy) an outherBased
             * on the No a word appears, we represent the number using special 
             * characters ($ for thousands # for hundreds @ for decades 
             * and * for units)
             */
            while (inputCopy) {
                if (inputCopy >= 1000) {
                    repeat = inputCopy / 1000;
                    inputCopy = inputCopy - repeat * 1000;
                    for (; repeat > 0; repeat--) {
                        printf ("$");
                    }
                }
                else if (inputCopy >= 100) {
                    repeat = inputCopy / 100;
                    inputCopy = inputCopy - repeat * 100;
                    for (; repeat > 0; repeat--) {
                        printf ("#");
                    }
                }
                else if (inputCopy >= 10) {
                    repeat = inputCopy / 10;
                    inputCopy = inputCopy - repeat * 10;
                    for (; repeat > 0; repeat--) {
                        printf ("@");
                    }
                }
                else {
                    repeat = inputCopy;
                    inputCopy = inputCopy - repeat;
                    for (; repeat > 0; repeat--) {
                        printf ("*");
                    }
                }
            }
            printf ("\n");
        }
    }

    /*
     * This function checks if there is a file with the synonyms of some words
     * and connects the words that are stored from the input with their synonyms.
     */

    void synonyMatcher (char *ptr[][2], char synonymstorage[][MAX_WORD_LEN * 2]) {
        
        int i, j, k;
        
        for (i = 0; i < MAX_WORDS; i++) {
            if (ptr[i][0] == '\0')
                break;
            for (j = 0; j < MAX_SYNONYMS; j++) {
                /*
                 * If there is no deffereance between the two 
                 * words then it matches it with it's synonym.
                 */
                if (!strcmp(ptr[i][0], synonymstorage[j])) {
                    k = strlen (synonymstorage[j]) + 1;
                    ptr[i][1] = &synonymstorage[j][k];
                }
            }
        }
    }

    /* 
     * This function calculates the memory that we use to 
     * store the words and their order with the usage of pointers 
     */

    long int memoryCalcA (char storage[][MAX_WORD_LEN], char *ptr[][2]) {
        
        long int result = 0, i;
        
        /* Calculating the bytes that are used for the characters */
        
        for (i = 0; i < MAX_WORDS; i++) {
            if (storage[i] == '\0')
                break;
            result += strlen (storage[i]);
        }
        
        result *= sizeof (char);
        
        /* Calculating the bytes that are used for the pointers */
        
        for (i = 0; i < MAX_WORDS; i++) {
            if (ptr[i][0] == '\0')
                break;
            result += sizeof (ptr[i][0]);
        }
        
        return result;
    }

    /* 
     * This function calculates the memory that we would
     * hypothetically use to store the whole text as it is.
     */

    long int memoryCalcB (char storage[][MAX_WORD_LEN], char *ptr[][2]) {
        
        long int result = 0, i;
        
        /* Calculating the bytes that are used for the characters */
        
        for (i = 0; i < MAX_WORDS; i++) {
            if (ptr[i][0] == '\0')
                break;
            result += strlen (ptr[i][0]);
        }
        
        result *= sizeof (char);
        
        return result;
    }

    /* 
     * This function calculates the fraction of 
     * the two defferent ways we store the data 
     */

    double ABfraction (char storage[][MAX_WORD_LEN], char *ptr[][2]) {
        
        double result;
        
        result = (double) memoryCalcA (storage, ptr) / memoryCalcB (storage, ptr);
        
        printf ("%.3lf\n", result);
        
        return 0;
    }

    int hw2_main(int argc, char* argv[]) {

        char words[MAX_WORDS][MAX_WORD_LEN] = {{'\0'}}, 
        *wordsptr[MAX_WORDS][2] = {{'\0'}}, *synonymsfile = '\0', 
        synonyms[MAX_SYNONYMS][MAX_WORD_LEN * 2] = {{'\0'}};
        int i, printmode = 0, histogramode = 0, synonymsmode = 0, 
        storagemode = 0, histogram[MAX_WORDS], check = 0;
        
        /* Checking the arguments that come from the terminal */
        
        for (i = 1; i < argc; i++) {
            if (!strcmp (argv[i], "-p")) {
                printmode++;
                check = 0;
            }
            else if (!strcmp (argv[i], "-h")) {
                histogramode++;
                check = 0;
            }
            else if (!strcmp (argv[i], "-r")) {
                synonymsmode++;
                synonymsfile = argv[i + 1];
                check = 0;
            }
            else if (!strcmp (argv[i], "-s")) {
                storagemode++;
                check = 0;
            }
            else {
                /* Case for wrong argument */
                check = 1;
            }
        }
        
        if (check || (argc == 1)) {
            printf ("Incorrect command-line arguments!\n");
        return 1;
        }
        
        wordRegister (words, wordsptr);
        
        /* Triggering the operation the user wants */
        
        if (histogramode) {
            for (i = 0; i < MAX_WORDS; i++) {
                histogram[i] = 0;
            }
            wordCalc (wordsptr, histogram);
            histogramPrinter (words, histogram);
        }
        if (synonymsmode) {
            if (read_synonyms(synonymsfile, synonyms, MAX_SYNONYMS))
                synonyMatcher (wordsptr, synonyms);
        }
        if (printmode) {
            textPrinter (wordsptr, MAX_LINE_LEN);
        }
        
        if (storagemode) {
            ABfraction (words, wordsptr);
        }
        
        return 0;
    }
