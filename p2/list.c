/**
   @file list.c
   @author Prem Subedi
   This component will define the ShoppingList struct, in the header, and
   functions for working with a ShoppingList
 */
#include "list.h"
#include <stdlib.h>
#include <stdio.h>

/**
   This function will dynamically allocate an instance of ShoppingList, and initialize its fields.
   @return sList pointer to shopping list.
 */
ShoppingList * makeShoppingList()
{
  const int START_CAPACITY = INIT_CAPACITY;
  ShoppingList * sList = malloc( sizeof( ShoppingList ) );
  if ( !sList ) return NULL;
  sList->length = 0;
  sList->items = malloc( START_CAPACITY * sizeof( Item * ) );
  sList->capacity = START_CAPACITY;
  return sList;
}

/**
   This will free all the dynamically allocated memory used by a ShoppingList,
   including the ShoppingList object itself.
   @param list pointer to the shopping list.
 */
void freeShoppingList( ShoppingList *list )
{
  if ( !list ) return;
  for (int i = 0; i < list->length; ++i ) {
    freeItem( list->items[i] );
  }
  list->length = list->capacity = 0;
  free( list->items );
  free( list );
}

/**
   This function adds the given item to the given shopping list,
   resizing its internal array if necessary.
   @param list pointer to shopping list.
   @param it pointer to item.
 */
void shoppingListAdd( ShoppingList *list, Item *it )
{
  const int EXTRA_CAPACITY = EX_CAPACITY;
  if ( !list || !it ) return;
    if ( list->length + 1 >= list->capacity ) {
      list->items = realloc( list->items, ( list->capacity + EXTRA_CAPACITY ) * sizeof( Item * ) );
      if ( !list->items ) {
        list->length = list->capacity = 0;
        return;
      }
      list->capacity += EXTRA_CAPACITY;
    }
    list->items[list->length] = it;
    list->length += 1;
}

/**
   This function removes the item with the given id from the given shopping list,
   returning true if successful or false if there's no item with that id.
   @param list shopping list pointer.
   @param id item id.
 */
bool shoppingListRemove( ShoppingList *list, int id )
{
  bool result = false;
  int i, j;
  for ( i = 0; list && i < list->length; ++i ) {
    if ( list->items[i]->id == id ) {
      freeItem( list->items[i] );
      /** shift elements to removed position */
      for ( j = i; j < list->length - 1; ++j ) {
        list->items[j] = list->items[j + 1];
      }
      /** decrease list size */
      list->length -= 1;
      result = true;
      break;
    }
  }
  return result;
}

/**
   It goes through the shopping list, reporting selected items and computing their total cost.
   It uses the given function pointer to decide which items to report on; for example,
   it can report all items, just those from a particular store,
   or just the items greater than a given price.
   @param list shopping list pointer.
   @param test test function that returns true for successful case, false otherwise.
   @param arg void or general pointer.
 */
void shoppingListReport( ShoppingList *list, bool test( Item *it, void *arg ), void *arg )
{
  int i;
  double total = 0;
  for ( i = 0; list && i < list->length; ++i ) {
    if ( test( list->items[i], arg ) ) {
      printf( "%4d %-12s %7.2lf %s\n", list->items[i]->id, list->items[i]->store,
            list->items[i]->price, list->items[i]->name );
      total += list->items[i]->price;
    }
  }
  printf( "                  %7.2lf\n", total );
}