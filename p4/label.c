/**
   @file label.c
   @author Prem Subedi
   This component contains data structures and functions for keeping up
   with where the labels are in the script.
 */

#include "label.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define CAPACITY 5

/**
    Return the location of the given label name in the program,
    or -1 if its not defined in the label map.
*/
int findLabel( LabelMap *lmap, char *name )
{
   for (int i = 0; i < lmap->size; i++) {
     if( strcmp(lmap->labels[i]->name, name) == 0) {
       return lmap->labels[i]->lineNum;
     }
   }
  return -1;
}

/** 
   Initialize the fields of the given labelMap structure.
   @param labelMap Addres of the structure to initialize.
*/
void initMap( LabelMap *labelMap )
{
  labelMap->size = 0;
  labelMap->capacity = CAPACITY;
  labelMap->labels = ( Label **) malloc( CAPACITY * sizeof( Label* ) );
}


/** 
    Add a label to the given labelMap.  Print an error message and
    exit if the label is a duplicate.
    @param labelMap LabelMap to add a label to.
    @param name Name of the label to add.
    @param loc Location of the label in the code.
*/
void addLabel( LabelMap *labelMap, char *name, int loc )
{
  if ( findLabel(labelMap, name) != -1) {
    printf("Duplicate label: %s", name);
    exit(1);
  }
  
  Label *label = (Label *) malloc( sizeof( Label ) );
  label->lineNum = loc;
  label->name = (char *) malloc( sizeof( char ) * (MAX_NAME + 1) );
  strcpy( label->name, name );
  
  if(labelMap->size >= labelMap->capacity) {
    labelMap->capacity *= 2;
    labelMap->labels = (Label ** ) realloc(labelMap->labels, labelMap->capacity * sizeof( Label* ) );
  }
  labelMap->labels[labelMap->size] = label;
  ++(labelMap->size);
}

/**
    Free all memory used by the label map.
*/
void freeMap(LabelMap *lmap)
{
  for (int i = 0; i < lmap->size; i++ ) {
    free(lmap->labels[i]->name);
    free(lmap->labels[i]);
  }
  free(lmap->labels );
}


