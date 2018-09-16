/**
   @file codepoints.c
   @author Prem Subedi
   This component reads and maintains the list of codepoints,
   where each codepoint has a numeric code and a string name.
   I got some help from TA Joy Chakraborty in his office hours.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codepoints.h"
#include <math.h>
#define INIT_CAP 90
#define NEGATIVE_VALUE -1
#define POSITIVE_VALUE 1

/**
   Represents a single CodePoint structure
   It has two fields, numerical value and name of the codepoint.
 */
typedef struct {
   unsigned int numericValue;
   char *name;
} CodePoint;

/** Global variable for array of codepoints structs */
static CodePoint *table;

/** Global variable for length of how many codepoints in a resize-able array */
static unsigned int length;


/**
   This function compares to CodePoints's numerical value and
   returns one if first CodePoint is bigger than the second one,
   returns negative one if it smaller than the second one, returns zero otherwise.
   I got help from TA Joy Chakraborty on Wednesday Nov eight on his office hours.
 */
static int comparison(const void *v1, const void * v2)
{
   CodePoint *cp1 = (CodePoint *) v1;
   CodePoint *cp2 = (CodePoint *) v2;
   if (cp1->numericValue > cp2->numericValue) return POSITIVE_VALUE;
   else if (cp1->numericValue < cp2->numericValue) return NEGATIVE_VALUE;
   else return 0;
}

/**
   This function will open the unicode.txt file and read all the code point descriptions
   into a data structure, Code point table. If it can't open the unicode.txt file,
   it will print an appropriate error message to standard error and exit unsuccessfully.
   I got help from TA Joy Chakraborty on Wednesday Nov 8 in his office hours.
 */
void loadTable()
 {
   unsigned int capacity = INIT_CAP;
   length = 0;
   CodePoint *cp;
   table = (CodePoint *) malloc(capacity * sizeof(CodePoint) );
   FILE *fp = fopen("unicode.txt", "r");
   if ( !fp ) {
      fprintf( stderr, "Can't open input file\n" );
      exit( 1 );
   } else {
      int value = 0;
      char * p = malloc(INIT_CAP * sizeof(char));
      if ( fscanf(fp, "%x %[^\n]", &value, p) != NUM_TWO) {
          fprintf(stderr, "Invalid file contents\n");
      }
      while ( fscanf(fp, "%x %[^\n]", &value, p) == NUM_TWO) {
         if (length >= capacity) {
            capacity *= NUM_TWO;
            table = (CodePoint *) realloc(table, capacity * sizeof( CodePoint ) );
         }
         cp = (CodePoint *) malloc(sizeof(CodePoint));
         cp->name = (char *) malloc(INIT_CAP * sizeof(char) );
         cp->numericValue = value;
         strcpy(cp->name, p);
         table[length++] = *cp;
         free(cp);
      }
      free(p);
    }
    qsort(table, length, sizeof( table[0] ), comparison);
    fclose(fp);
 }
 
/**
   This function will free the memory for the code point table.
*/
void freeTable()
{
   for (int i = 0; i < length; i++) {
      free( (table + i)->name);
   }
   free(table);
}

/**
   This is a binary search of the codepoint from the array of codepoints.
   @param value the numerical value of the code point to be searched for.
   @param size the size of the array.
   @return returns the index of the element found, returns -1 otherwise.
 */
static int search(int value, int size)
{
   int min = 0;
   int max = size - 1;
   int mid;

   do {
      mid = round((min + max) / NUM_TWO);
      if (value < (table + mid)->numericValue)
         max = mid - 1;
      else if (value > (table + mid)->numericValue)
         min = mid + 1;
   } while (value != (table + mid)->numericValue && min <= max);

   if ( value == (table + mid)->numericValue ) {
      return mid;
   } else {
      return NEGATIVE_VALUE;
   }
}

/**
   Given the numeric value of a code point, This function will use binary search
   to quickly find the record for a given code point and prints a line to standard output
   giving the name of the code (and return true).
   If the given code isn't in the code point table, it will return false.
   I got help from TA Joy Chakraborty on Wednesday Nov 8 on his office hours.
 */
bool reportCode( int code )
{
   int index = search(code, length);
   if (index < 0) {
      return false;
   } else {
      if (index != length - 1) printf("%s\n", table[index].name);
      else printf("%s", table[index].name);
      return true;
   }
}
