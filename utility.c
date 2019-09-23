/******************************************************************************
** Student name: Kevin Wong   ...
** Student number: s3603055 ...
** Course:          Advanced Programming Techniques - S1 2018
******************************************************************************/


#include "utility.h"

/**
 * Function required to be used when clearing the buffer. It simply reads
 * each char from the buffer until the buffer is empty again. Please refer
 * to the materials on string and buffer handling in the course for more
 * information.
 **/
void readRestOfLine()
{
    int ch;
    while(ch = getc(stdin), ch != EOF && ch != '\n')
    { } /* Gobble each character. */

    /* Reset the error status of the stream. */
    clearerr(stdin);
}

/* Text input is collected from user, returns false if no characters were entered, returns true if input entered was valid. */
Boolean getInputValidation(char * string, int max) 
{
    Boolean valid = FALSE;
    while (!valid) {
        fgets(string, max, stdin);
        if (string[0] == '\n' || string[0] == '\0') return FALSE;
        /* Check if character limited is exceeded */
        if (string[strlen(string)-1] != '\n') 
        {
            readRestOfLine();
            printf("Character input limited.");
            printf("Please try again:");
            continue;
        }
        valid = TRUE;
    }
    if (string[strlen(string)-1] == '\n') string[strlen(string)-1] = '\0'; 

    return TRUE;
}

/* Counts & returns how many characters in a number given */
unsigned countChar(unsigned n) 
{
    size_t count = 0;
    while (n != 0) 
    {
        n /= 10;
        count++;
    }
    return count;
}




