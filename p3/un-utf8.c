/**
   @file un-utf8.c
   @author Prem Subedi
   This is the main component, it opens and reads from the input file,
   detect errors and uses the codepoints component to print the names of valid codepoints.
   I got help to implement some part of this component from TA Joymallya Chakraborty
   in his office hours on November fifteen.
 */
#include <stdio.h>
#include <stdlib.h>
#include "codepoints.h"
#define SHIFT_AMOUNT 7
#define NUM_THREE 3
#define NUM_FOUR 4
#define NUM_SIX 6
#define HEX_VALUE_1 0x1
#define HEX_VALUE_2 0x2
#define HEX_VALUE_3 0x3F
#define HEX_VALUE_4 0x80
#define HEX_VALUE_5 0x800
#define HEX_VALUE_6 0x10000
#define HEX_VALUE_7 0xFF
/**
   The main method or the starting point of the program, which opens and reads
   from input file in binary mode and uses codepoints functions to print the name of
   valid codepoints.
   @param argc number of command line arguments.
   @param argv array of command line arguments.
*/
int main (int argc, char *argv[])
{
  unsigned char c = 0;
  int bytesRead = 0;
  bool isValid;
  bool foundZero;
  unsigned int byte1;
  int onebits;
  int shift;

  FILE *fin = NULL;
  if (argc != NUM_TWO) {
    fprintf(stderr, "usage: un-utf8 <input-file>\n");
    return EXIT_FAILURE;
  } else {
     fin = fopen(argv[1], "rb");
     if (!fin) {
       fprintf(stderr, "Can't open file: %s\n", argv[1]);
       fprintf(stderr, "usage: un-utf8 <input-file>\n");
       return EXIT_FAILURE;
     }
  }
  loadTable();
  while (fread(&c, 1, 1, fin) == 1 && c != '\n') {
    bytesRead++;
    isValid = true;
    byte1 = c;
    foundZero = false;
    onebits = 0;
    shift = SHIFT_AMOUNT;

    do {
      if (((byte1 >> shift) & HEX_VALUE_1) == HEX_VALUE_1) {
        onebits++;
        shift--;
      } else {
        foundZero = true;
      }
    } while (!foundZero);

    if (onebits == 1 || onebits > NUM_FOUR) {
      isValid = false;
      fprintf(stderr, "Invalid byte: 0x%X at %d\n", byte1, bytesRead - 1);
    }
    unsigned int code = byte1 & (HEX_VALUE_7 >> onebits);
    for (int i = 1; i < onebits; i++) {
      if (fread(&c, 1, 1, fin) == 1) {
        bytesRead++;
        if ((c >> NUM_SIX) == HEX_VALUE_2) {
          code <<= NUM_SIX;
          c &= HEX_VALUE_3;
          code |= c;
        } else {
          isValid = false;
          fprintf(stderr, "Invalid byte: 0x%X at %d\n", c, bytesRead - 1);
        }
      } else {
        fprintf(stderr, "Incomplete code at %d\n", bytesRead - i);
        freeTable();
        return EXIT_SUCCESS;
      }
    }
    if (isValid) {
      if ((onebits == NUM_TWO && code < HEX_VALUE_4) || (onebits == NUM_THREE && code < HEX_VALUE_5) ||
                    (onebits == NUM_FOUR && code < HEX_VALUE_6)) {
        isValid = false;
        fprintf(stderr, "Invalid encoding: 0x%X at %d\n", code, bytesRead - onebits);
      }
    }
    if (isValid && !reportCode(code)) {
      fprintf(stderr, "Unknown code: 0x%X at %d\n", code, bytesRead - onebits);
    }
  }
  if (c == '\n') {
    reportCode(c);
  }
  freeTable();
  fclose(fin);
  return EXIT_SUCCESS;
}
