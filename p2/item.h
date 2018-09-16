#ifndef __ITEM_H__
#define __ITEM_H__
/** Maximum length of a store name */
#define STORE_MAX 12
#define TWO 2 /** To avoid magic number */
#define TWELVE 12  /** To avoid magic number */
#define INVALID_VALUE -1

/** Representation for an item to be purchased. */
typedef struct {
  /** Unique ID for this particular item.  */
  int id;
  
  /** Store where we're supposed to buy the item.  */
  char store[ STORE_MAX + 1 ];
  
  /** Price of this item in dollars (but you might normally store this
      as an integer number of cents). */
  double price;
  
  /** Name of this item.  Pointer to a string of arbitrary length. */
  char *name;
} Item;

Item *readItem( char *str );
void freeItem( Item *it );

#endif /** __ITEM_H__ */
