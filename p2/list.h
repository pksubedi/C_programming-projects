#ifndef __LIST_H__
#define __LIST_H__

#include <stdbool.h>
#include "item.h"
#define INIT_CAPACITY 10
#define EX_CAPACITY 20

/** Representation for a shopping list, an arbitrary-length list of Items. */
typedef struct {
  /** Resizable array of pointers to Items. */
  Item **items;
  
  /** Current number of items on the list. */
  int length;
  
  /** Current capacity of the list, how many pointers we have room for. */
  int capacity;
} ShoppingList;

ShoppingList * makeShoppingList();
void freeShoppingList( ShoppingList *list );
void shoppingListAdd( ShoppingList *list, Item *it );
bool shoppingListRemove( ShoppingList *list, int id );
void shoppingListReport( ShoppingList *list, bool test( Item *it, void *arg ), void *arg );

#endif /*__LIST_H__*/
