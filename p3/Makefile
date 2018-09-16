# A makefile with explicit rules for everything we need to build.

# Rebuild the expecutable if one of the objects changes.
un-utf8: un-utf8.o codepoints.o
	gcc un-utf8.o codepoints.o -o un-utf8 -lm

# Rebuild un-utf8.o if there's a change in its source file or
# in the header it includes.
un-utf8.o: un-utf8.c codepoints.h
	gcc -g -Wall -std=c99 -c un-utf8.c -lm

# Rebuild codepoints.o if there's a change in its implementation
# file or its header.
codepoints.o: codepoints.c codepoints.h
	gcc -g -Wall -std=c99 -c codepoints.c -lm
	
	
# Cleaning all object files
clean:
	rm -f un-utf8 un-utf8.o codepoints.o

   