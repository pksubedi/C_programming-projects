/**
   @file nonde.c
   @author Prem Subedi
   This component contains the main() function.
   It uses the other components to parse and execute commands from the input script.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "label.h"
#include "parse.h"

/** Initial capacity for resizable arrays. */
#define INITIAL_CAPACITY 5

/** Growth rate (multiplier) for resizable arrays. */
#define GROWTH_RATE 2

/** Type used to represent a whole program, including a list of commands and
    a record of where all the labels are. */
typedef struct {
  /** Sequence of all the commands in the program. */
  Command **cmd;

  /** Number of commands in the program. */
  int count;

  /** Capacity of the command list, free resize behavior. */
  int cap;

  /** Label map, for the targets of if and goto. */
  LabelMap *labelMap;
} Program;

/** Print a short usage message, then exit. */
static void usage()
{
  fprintf( stderr, "usage: nonde <script>\n" );
  exit( EXIT_FAILURE );
}

/** Initialize the given Program structure and read in the program
    definition from the given file.
    @param prog Program structure to populate.
    @param fp File to read from.
*/
static void loadProgram( Program *prog, FILE *fp )
{
  // Initialize the array of command pointers.
  prog->count = 0;
  prog->cap = INITIAL_CAPACITY;
  prog->cmd = (Command **) malloc( prog->cap * sizeof( Command * ) );

  // Initialize the labelMap structure in the program.
  initMap( &prog->labelMap );

  // One token of read-ahead, so we can tell what's next in the program.
  char tok[ MAX_TOKEN + 1 ];
  while ( parseToken( tok, fp ) ) {

    // Is this token a label?
    int tlen = strlen( tok );
    if ( tok[ tlen - 1 ] == ':' ) {
      // Throw away the : at the end, and put it in the map.
      tok[ tlen - 1 ] = '\0';
      if ( !isVarName( tok ) )
        syntaxError();
      addLabel( &prog->labelMap, tok, prog->count );
    } else {
      // If it's not a label, it must be a command.
      Command *cmd = parseCommand( tok, fp );

      // Enlarge the command list if needed, and store the new command.
      if ( prog->count >= prog->cap ) {
        prog->cap *= GROWTH_RATE;
        prog->cmd = (Command **) realloc( prog->cmd, prog->cap * sizeof( Command * ) );
      }
      prog->cmd[ prog->count ++ ] = cmd;
    }
  }
}

/** Free memory for a program.
    @param prog A pointer to the program we're supposed to free.
*/
static void freeProgram( Program *prog )
{
    if ( !prog ) return;
   
    for (int i = 0; i < prog->count; i++ ) {
       prog->cmd[i]->destroy(prog->cmd[i]);
    }
    freeMap( &prog->labelMap);
    free(prog->cmd);
    prog->count = prog->cap = 0;
}

/** Starting point for the program
    @param argc number of command-line arguments
    @param argv array of command-line arguments
    @return exit status
*/
int main( int argc, char *argv[] )
{
  // Make sure we get one filename on the command line, and that we can open the file.
  if ( argc != 2 )
    usage();

  FILE *fp = fopen( argv[ 1 ], "r" );
  if ( fp == NULL ) {
    fprintf( stderr, "Can't open file: %s\n", argv[ 1 ] );
    usage();
  }

  // Make a program structure, and load it from the given file.
  Program prog;
  loadProgram( &prog, fp );
  fclose( fp );

  // Index of the current command.
  int pc = 0;

  // Run commands int he program until we reach the end (possibly
  // looping as we run).
  while ( pc < prog.count )
    pc = prog.cmd[ pc ]->execute( prog.cmd[ pc ], & prog.labelMap, pc );

  freeProgram( &prog );
}
