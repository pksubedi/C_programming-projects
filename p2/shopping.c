/**
   @file shopping.c
   @author Prem Subedi
   This component will define the main function and any other functions
   not provided by the other two components. It is responsible for reading commands
   from the user and manipulate them by calling functions in other component.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#define MAX_COMMAND_NAME_LENGTH 10
#define LINESIZE 32

/** function to read line from the input stream */
static char * getLine( FILE *stream  );
/** function to display help menu */
static void printHelp();
/** function to process load command */
static void processInputFile( const char * filename, ShoppingList * list, int * nextID  );
/** function to process save command */
static void processOutputFile(const char * filename, ShoppingList * list );
/** predicate for store name check */
static bool checkStoreName(Item *it, void *arg );
/** predicate for greater price check */
static bool checkGreater(Item *it, void *arg );
/** predicate for less price check */
static bool checkLess(Item *it, void *arg );
/** predicate for clear report */
static bool noCheck(Item *it, void *arg );

/**
   The main method or the starting point of the program, which reads the input from the user
   and calls different functions to implement the program's functionality.
 */
int main()
{
  int commandCounter = 0;
  char * input;
  int pos, nextID = 1;
  char commandName[MAX_COMMAND_NAME_LENGTH];
  int needContinue = 1;
  ShoppingList * list = makeShoppingList();
  Item * it;
  int intArg;
  double doubleArg;
  char storeName[STORE_MAX + 1];

  do {
    printf( "%d> ", ++commandCounter );
    input = getLine( stdin );
    /** end of input reached */
    if ( !input ) break;
    pos = 0;
    /** parse command and other intput */
    sscanf( input, " %10s %n", commandName, &pos );
    printf( "\n" );
    /** switch case through input command name */
    if ( strcmp( commandName, "load" ) == 0 ) {
      processInputFile( input + pos, list, &nextID );
    } else if ( strcmp( commandName, "save" ) == 0 ) {
      processOutputFile( input + pos, list );
    } else if ( strcmp( commandName, "add" ) == 0 ) {
      if ( ( it = readItem( input + pos ) ) ) {
        it->id = nextID++;
        shoppingListAdd( list, it );
      } else {
        printf( "Invalid command\n" );
      }
    } else if ( strcmp( commandName, "remove" ) == 0 ) {
      if ( sscanf( input + pos, "%d", &intArg ) == 1 ) {
        shoppingListRemove( list, intArg );
      }
    } else if ( strcmp( commandName, "report" ) == 0 ) {
      if ( sscanf( input + pos, "store %s", storeName ) == 1 ) {
        shoppingListReport( list, checkStoreName, storeName );
      } else if ( sscanf( input + pos, "greater %lf", &doubleArg ) == 1 ) {
        shoppingListReport( list, checkGreater, &doubleArg );
      } else if ( sscanf( input + pos, "less %lf", &doubleArg ) == 1 ) {
        shoppingListReport( list, checkLess, &doubleArg );
      } else if ( strcmp( input, "report" ) == 0 ){
        shoppingListReport( list, noCheck, NULL );
      } else {
        printf( "Invalid command\n" );
      }
    } else if ( strcmp( commandName, "help" ) == 0 ) {
      printHelp(  );
    } else if ( strcmp( commandName, "quit" ) == 0 ) {
      needContinue = 0;
    } else {
      printf( "Invalid command\n" );
    }
    /** Free memory */
    free( input );
  } while ( needContinue );

  freeShoppingList( list );
  return 0;
}

/**
   This function is for reading a line of input, either from standard input or from a file.
   Internally, it uses a resizable array to store an arbitrary-sized string
   as it reads the line. It returns pointer to a dynamically-allocated string
   containing the input line.
   @param stream pointer to either a file or standard input.
   @return str pointer to a dynamically allocated string.
 */
char * getLine( FILE * stream )
{
  const int LINE_SIZE = LINESIZE;
  char * str = NULL;
  int ch, size = 0;
  size_t len = 0;
  
  while (( ch = fgetc( stream ) ) != EOF && ch != '\n' ){
      if ( len + 1 >= size ) {
          str = realloc( str, ( size += LINE_SIZE ) * sizeof( char ) );
          if ( !str ) return str;
      }
      str[len++] = ch;
  }
  
  if ( str ) {
    str[len++] = '\0';
    str = realloc( str, len * sizeof( char ) );
  }
  return str;
}

/**
   This function prints help menu.
 */
void printHelp(  )
{
  printf( "load <file>\n" );
  printf( "save <file>\n" );
  printf( "add <store> <price> <name>\n" );
  printf( "remove <id>\n" );
  printf( "report\n" );
  printf( "report store <store>\n" );
  printf( "report less <price>\n" );
  printf( "report greater <price>\n" );
  printf( "help\n" );
  printf( "quit\n" );
}

/**
   This is a static function, which has only the linkage with shopping component.
   It reads input file given by the user.
   @param filename pointer to the input file.
   @param list pointer to the shopping list.
   @param nextId pointer to the next id of the item in the shopping list.
 */
static void processInputFile( const char * filename, ShoppingList * list, int * nextID )
{
  FILE * fin = fopen( filename, "r" );
  int line = 0;
  char * input;
  Item * item;
  if ( !fin ) {
    printf( "Can't open file" );
    return;
  }
  
  while ( ( input = getLine( fin ) ) ) {
    ++line;
    item = readItem( input );
    if ( item ) {
      item->id = ( *nextID )++;
      shoppingListAdd( list, item );
    } else {
      printf( "Invalid item, line %d\n", line );
    }
    free( input );
  }
  
  fclose( fin );
}

/**
    This is a static function, which has only the linkage with shopping component.
    It writes to the output file.
    @param filename pointer to the output file.
    @param list pointer to the shopping list.
 */
void processOutputFile( const char * filename, ShoppingList * list )
{
  FILE * fout = fopen( filename, "w" );
  int i;
  if ( !fout ) {
    printf( "Can't open file\n" );
    return;
  }

  for ( i = 0; list && i < list->length; ++i ) {
    fprintf( fout, "%s %.2lf %s\n", list->items[i]->store, list->items[i]->price,
     list->items[i]->name );
  }
  fclose( fout );
}

/**
   This function returns true if the item's storename matches with the name point by
   given void pointer, false otherwise.
   @param it item whose store name to be matched.
   @param arg void or general pointer.
 */
bool checkStoreName( Item *it, void *arg )
{
  return strcmp( it->store, ( char * )arg ) == 0;
}

/**
   It checks if the item's price is greater than the given price.
   @param it item whose store name to be matched.
   @param arg void or general pointer.
 */
bool checkGreater( Item *it, void *arg )
{
  return it->price > *( ( double* )arg );
}

/**
   This function checks if the item's price is less than the given price.
   @param it item whose store name to be matched.
   @param arg void or general pointer.
 */
bool checkLess( Item *it, void *arg )
{
  return it->price < *( ( double* )arg );
}

/**
   This function just avoids the warning about unused variables and returns true regardless.
   @return boolean if there is no warning.
 */
bool noCheck( Item *it, void *arg )
{
  /** To omit unused variables warning */
  ( void )it, 
  ( void )arg;
  return true;
}