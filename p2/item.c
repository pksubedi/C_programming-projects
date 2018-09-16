/**
   @file item.c
   @author Prem Subedi
   This component will define the item struct, in the header, and
   functions for working with items on the shopping list.
 */

#include "item.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
   This function will create a new, dynamically allocated item
   based on the text in the given string.
   @param str input string that needs to be parsed.
 */
Item * readItem( char * str )
{
  const int INVALDID_POS = INVALID_VALUE;
  Item * result = NULL;
  Item tmp;
  int pos = INVALDID_POS;
  int len;
  
    /* try parse input string and extract values */
    if (sscanf( str, " %12s %lf %n", tmp.store, &tmp.price, &pos) != TWO || pos == INVALDID_POS )
       return NULL;
    len = strlen( str + pos );
    if ( !len ) return NULL;
    result = malloc( sizeof( Item ) );
    if ( !result ) return NULL;
    *result = tmp;
    result->name = malloc( (len + 1) * sizeof( char ) );
    /*copy name*/
    if ( result->name ) {
      strcpy( result->name, str + pos );
    }
  return result;
}

/**
   This function will free the memory for the given item,
   including the item struct and the memory for the item's name.
   @param it a pointer to the item that needs to be freed.
 */
void freeItem( Item *it )
{
  if (it) {
    free( it->name );
    free( it );
  }
}
