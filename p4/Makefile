# @file Makefile
# @author Prem Subedi
# complies all the files and links
# them together

CC=gcc -Wall -std=c99 -g -D_POSIX_C_SOURCE=200112L
# A makefile with explicit rules for everything we need to build.

# Rebuild the expecutable if one of the objects changes.
nonde: nonde.o command.o parse.o label.o 
	gcc nonde.o command.o parse.o label.o -o nonde

# Rebuild nonde.o if there's a change in its source file or
# in the header it includes.
nonde.o: nonde.c parse.h label.h command.h
	$(CC) -c nonde.c

# Rebuild parse.o if there's a change in its implementation
# file or its header.
parse.o: parse.c parse.h
	$(CC) -c parse.c
	
# Rebuild label.o if there's a change in its implementation
# file or its header.
label.o: label.c label.h
	$(CC) -c label.c
	
# Rebuild command.o if there's a change in its implementation
# file or its header.
command.o: command.c command.h label.h parse.h
	$(CC) -c command.c
	
	
# Cleaning all object files
clean:
	rm -f nonde nonde.o parse.o label.o command.o

   