# A makefile with explicit rules for everything we need to build.

# Rebuild the expecutable if one of the objects changes.
shopping: shopping.o list.o item.o
	gcc shopping.o list.o item.o -o shopping

# Rebuild shopping.o if there's a change in its source file or
# in the header it includes.
shopping.o: shopping.c list.h item.h
	gcc -g -Wall -std=c99 -c shopping.c

# Rebuild list.o if there's a change in its implementation
# file or its header.
list.o: list.c list.h item.h
	gcc -g -Wall -std=c99 -c list.c
	
# Rebuild item.o if there's a change in its implementation
# file or its header.
item.o: item.c item.h
	gcc -g -Wall -std=c99 -c item.c
	
# Cleaning all object files
clean:
	rm -f shopping shopping.o list.o item.o

   