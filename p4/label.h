/**
  @file label.h
  @author Prem Subedi
  Header file for label component.
  Mapping from label names to lines of code.
*/

#ifndef _LABEL_H_
#define _LABEL_H_

#define MAX_NAME 20

typedef struct {
   char *name;
   int lineNum;
}Label;


/** Map from label names to locations in the code. */
typedef struct {
   int size;
   int capacity;
   Label **labels; 
} LabelMap;

/** Initialize the fields of the given labelMap structure.
    @param labelMap Addres of the structure to initialize.
*/
void initMap( LabelMap *labelMap );

/** Add a label to the given labelMap.  Print an error message and
    exit if the label is a duplicate.
    @param labelMap LabelMap to add a label to.
    @param name Name of the label to add.
    @param loc Location of the label in the code.
*/
void addLabel( LabelMap *labelMap, char *name, int loc );

/**
    Return the location of the given label name in the program,
    or -1 if its not defined in the label map.
*/
int findLabel( LabelMap *lmap, char *name );

/**
    Free all memory used by the label map.
*/
void freeMap(LabelMap *lmap);

#endif
