/**
   @file command.c
   @author Prem Subedi
   This provides the representation and supporting functions for implementing commands.
   It also contains the parser function for parsing individual commands
   and creating objects to represent them.
 */

#include <stdlib.h>
#include "command.h"
#include <stdio.h>
#include <string.h>
#include "label.h"
#include "parse.h"

/**
   This method copies the string from the given char pointer
   to the one returned.
   @param str constant char pointer (string).
   @return copied string.
*/
static char *copyString( char const *str )
{
  char *cpy = (char *) malloc( strlen( str ) + 1 );
  return strcpy( cpy, str );
}

/**
   Representation of struct PrintCommand, a sub class of command.
*/
typedef struct {

  int (*execute)( Command *cmd, LabelMap *labelMap, int pc );
  int (*destroy)( Command *cmd);
  int lineNum;
  char *arg;
} PrintCommand;

/**
   Function to free PrintCommand
   @param cmd a command to be freed.
*/
static int destroyPrintCommand( Command *cmd ) {
  PrintCommand *this = (PrintCommand *)cmd;
  free( this->arg );
  free( this );
  return 0;
}

/** Pointer to a function to execute this command.
   @param cmd The command to be executed.
   @param labelMap Map for where all the labels are.
   @param pc Index of the command being run (program counter), so
   this command can return the index of the next command.
   @return Index of the next instruction to run in the program (new
   program counter).  
*/
static int executePrint( Command *cmd, LabelMap *labelMap, int pc )
{

  PrintCommand *this = (PrintCommand *)cmd;
  if ( isVarName( this->arg ) ) {
      if ( getenv(this->arg) == NULL ) {
         fprintf(stderr, "Undefined variable: %s (line %d)\n", this->arg, this->lineNum );
         exit( 1 );
      }
      printf( "%s", getenv(this->arg));
  } else {
      printf( "%s", this->arg + 1 );
  }

  return pc + 1;
}

/**
   Constructs this command.
   @param arg a constant char pointer, the only argument.
*/
static Command *makePrint( char const *arg )
{
  PrintCommand *this = (PrintCommand *) malloc( sizeof( PrintCommand ) );
  this->execute = executePrint;
  this->destroy = destroyPrintCommand;
  this->lineNum = getLineNumber();
  this->arg = copyString( arg );
  return (Command *) this;
}

/**
    Representation of struct SetCommand, a sub class of command.
*/
typedef struct {
  int (*execute)( Command *cmd, LabelMap *labelMap, int pc );
  int (*destroy)( Command *cmd);
  int line;
  char *arg1;
  char *arg2;
} SetCommand;

/**
   Function to free SetCommand
   @param cmd a command to be freed.
*/
static int destroySetCommand( Command *cmd ) {
  SetCommand *this = (SetCommand *)cmd;
  free( this->arg1 );
  free( this->arg2 );
  free( this );
  return 0;
}

/** Pointer to a function to execute this command.
   @param cmd The command to be executed.
   @param labelMap Map for where all the labels are.
   @param pc Index of the command being run (program counter), so
   this command can return the index of the next command.
   @return Index of the next instruction to run in the program (new
   program counter).
*/
static int executeSet( Command *cmd, LabelMap *labelMap, int pc )
{
  SetCommand *this = (SetCommand *)cmd;
  if ( isVarName( this->arg2 ) ) {
    setenv( this->arg1, getenv( this->arg2 ), 1 );
  } else {
     setenv( this->arg1, this->arg2 + 1, 1 );
  }

  return pc + 1;
}

/**
   Constructs this command.
   @param arg1 a variable
   @param arg2 either a variable or literal
*/
static Command *makeSet( char const *arg1, char const *arg2 )
{
  SetCommand *this = (SetCommand *) malloc( sizeof( SetCommand ) );
  this->execute = executeSet;
  this->destroy = destroySetCommand;
  this->line = getLineNumber();
  this->arg1 = copyString( arg1 );
  this->arg2 = copyString( arg2 );
  return (Command *) this;
}

/**
   Enumerated type for Arithmetic operations involved in the command.
*/
typedef enum ArithmeticType { add, subtract, multiply, divide, mod, equal, less } Type;

/**
   Representation of struct ArithmeticCommand, a sub class of command.
*/
typedef struct {
  int (*execute)( Command *cmd, LabelMap *labelMap, int pc );
  int (*destroy)( Command *cmd);
  int line;
  Type type;
  char *dest;
  char *arg1;
  char *arg2;
} ArithmeticCommand;

/**
   Function to free ArithmeticCommand
   @param cmd a command to be freed.
*/
static int destroyArithmeticCommand( Command *cmd ) {
  ArithmeticCommand *this = (ArithmeticCommand *)cmd;
  free( this->dest );
  free( this->arg1 );
  free( this->arg2 );
  free( this );
  return 0;
}

/**
   This method determines if the given char pointer is a variable or a 
   literal and parses it accordingly. If is a variable, it extracts it's value 
   using getenv().
   @param str char pointer or an argument.
   @i integer value
   @return rlong the returned literal or the variable 

*/
static long valOfVar( char *str, int *i )
{
  long rlong = 0;
  int numOfMatches;
  if ( isVarName( str ) ) {
    numOfMatches = sscanf(  getenv( str ), "%ld", &rlong ); 
  } else {
    numOfMatches = sscanf( str + 1, "%ld", &rlong );
  }
  if ( i && numOfMatches != 1 ) (*i)++;
  return rlong;
}

/** Pointer to a function to execute this command.
   @param cmd The command to be executed.
   @param labelMap Map for where all the labels are.
   @param pc Index of the command being run (program counter), so
   this command can return the index of the next command.
   @return Index of the next instruction to run in the program (new
   program counter).
*/
static int executeArithmetic( Command *cmd, LabelMap *labelMap, int pc )
{
  ArithmeticCommand *this = (ArithmeticCommand *)cmd;
  long ans;
  int i = 0;
  char *buffer[ 30 ];
  int l1 = valOfVar( this->arg1, &i );
  int l2 = valOfVar( this->arg2, &i );
  if ( i != 0 ) {
    fprintf( stderr, "Invalid number (line %d)\n", this->line );
    exit( 1 );
  }

  if (this->type == add ) ans = l1 + l2;
  else if (this->type == subtract ) ans = l1 - l2;
  else if (this->type == multiply ) ans = l1 * l2;
  else if (this->type == divide ) {
     if ( l2 == 0) {
         fprintf( stderr, "Divide by zero (line %d)\n", this->line );
         exit( 1 );
       }
     ans = l1 / l2;
  } else if (this->type == mod ) ans = l1 % l2;
  else if (this->type == equal ) ans = l1 == l2;
  else if (this->type == less ) ans = l1 < l2;
  else printf("Invalid command\n");
  
 
  if ( this->type != less && this->type != equal ) sprintf( buffer, "%ld", ans );
  else {
     if ( ans ) sprintf( buffer, "%s", "1");
     else sprintf( buffer, "%s", "");
  }
  setenv( this->dest, (const char *) buffer, 1 );
  return pc + 1;
}

/**
    I got help for this method from TA Joy in his office hours on Monday (November 27).
*/
static Command *makeArithmetic( Type type, char const *dest, char const *arg1, char const *arg2 )
{

  ArithmeticCommand *this = (ArithmeticCommand *) malloc( sizeof( ArithmeticCommand ) );

  this->execute = executeArithmetic;
  this->destroy = destroyArithmeticCommand;
  this->line = getLineNumber();
  this->type = type;
  this->dest = copyString( dest );
  this->arg1 = copyString( arg1 );
  this->arg2 = copyString( arg2 );
  return (Command *) this;
}

/**
    Representation of struct IfCommand, a sub class of command.
*/
typedef struct {

  int (*execute)( Command *cmd, LabelMap *labelMap, int pc );
  int (*destroy)( Command *cmd);
  int line;
  char *condition;
  char *label;
} IfCommand;

/** Pointer to a function to execute this command.
   @param cmd The command to be executed.
   @param labelMap Map for where all the labels are.
   @param pc Index of the command being run (program counter), so
   this command can return the index of the next command.
   @return Index of the next instruction to run in the program (new
   program counter).
*/
static int executeIf( Command *cmd, LabelMap *labelMap, int pc )
{
  IfCommand *this = (IfCommand *)cmd;
  if ( valOfVar( this->condition, NULL ) ) return findLabel( labelMap, this->label );
  else return pc + 1;
}

/**
   Function to free IfCommand
   @param cmd a command to be freed.
*/
static int destroyIf( Command *cmd )
{
  IfCommand *this = (IfCommand *)cmd;
  free( this );
  return 0;
}

static Command *makeIf( char const *condition, char const *label )
{

  IfCommand *this = (IfCommand *) malloc( sizeof( IfCommand ) );
  this->execute = executeIf;
  this->destroy = destroyIf;
  this->line = getLineNumber();
  this->condition = copyString( condition );
  this->label = copyString( label );
  return (Command *) this;
}

/**
   Representation of struct GotoCommand, a sub class of command.
*/
typedef struct {

  int (*execute)( Command *cmd, LabelMap *labelMap, int pc );
  int (*destroy)( Command *cmd);
  int line;
  char *label;
} GotoCommand;

/**
   Function to free Goto Command
   @param cmd a command to be freed.
*/
static int destroyGoto( Command *cmd )
{
  GotoCommand *this = (GotoCommand *)cmd;
  free( this );
  return 0;
}

/** Pointer to a function to execute this command.
   @param cmd The command to be executed.
   @param labelMap Map for where all the labels are.
   @param pc Index of the command being run (program counter), so
   this command can return the index of the next command.
   @return Index of the next instruction to run in the program (new
   program counter).
*/
static int executeGoto( Command *cmd, LabelMap *labelMap, int pc )
{
  GotoCommand *this = (GotoCommand *)cmd;
  return findLabel( labelMap, this->label );
}

static Command *makeGoto( char const *label )
{
  GotoCommand *this = (GotoCommand *) malloc( sizeof( GotoCommand ) );
  this->execute = executeGoto;
  this->destroy = destroyGoto;
  this->line = getLineNumber();
  this->label = copyString( label );
  return (Command *) this;
}

/**
   This method parses the given command and calls the appropriate functions 
   for the necessary operations.
   I received help for this from TA Joy in his office hours,
   we discussed about enumerated types.
   @param cmdName a char pointer for command name
   @param fp a pointer to the given file.
*/
Command *parseCommand( char *cmdName, FILE *fp )
{

  char dest[ MAX_VARNAME + 1 ];
  char arg1[ MAX_TOKEN + 1 ];
  char arg2[ MAX_TOKEN + 1 ];

  if ( strcmp( cmdName, "print" ) == 0 ) {
    expectToken( dest, fp );
    requireToken( ";", fp );
    return makePrint( dest );

  } else if ( strcmp( cmdName, "goto" ) == 0 ) {
    expectToken( dest, fp );
    requireToken( ";", fp );
    return makeGoto( dest );

  } else if ( strcmp( cmdName, "if" ) == 0 ) {
    expectToken( dest, fp );
    expectToken( arg1, fp );
    requireToken( ";", fp );
    return makeIf( dest, arg1 );

  } else if ( strcmp( cmdName, "set" ) == 0 ) {
    expectToken( dest, fp );
    expectToken( arg1, fp );
    requireToken( ";", fp );
    return makeSet( dest, arg1 );

  } else {
    Type tp;
    
    if ( strcmp( cmdName, "add" ) == 0 ) tp = add;
    else if ( strcmp( cmdName, "sub" ) == 0 ) tp = subtract;
    else if ( strcmp( cmdName, "mult" ) == 0 ) tp = multiply;
    else if ( strcmp( cmdName, "div" ) == 0 ) tp = divide;
    else if ( strcmp( cmdName, "mod" ) == 0 ) tp = mod;
    else if ( strcmp( cmdName, "eq" ) == 0 ) tp = equal;
    else if ( strcmp( cmdName, "less" ) == 0 ) tp = less;
    else syntaxError();
    
    expectToken( dest, fp );
    expectToken( arg1, fp );
    expectToken( arg2, fp );
    requireToken( ";", fp );
    return makeArithmetic( tp, dest, arg1, arg2 );    
  }
  return NULL;
}
