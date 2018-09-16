#include <stdbool.h>

/**
   This function will open the unicode.txt file and read all the code point descriptions
   into a data structure, Code point table. If it can't open the unicode.txt file,
   it will print an appropriate error message to standard error and exit unsuccessfully.
 */
void loadTable();


/**
   Given the numeric value of a code point, This function will use binary search
   to quickly find the record for a given code point and prints a line to standard output
   giving the name of the code (and return true).
   If the given code isn't in the code point table, it will return false.
 */
bool reportCode(int code);

/** This function will free the memory for the code point table.*/
void freeTable();

#define NUM_TWO 2
