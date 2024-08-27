# makefile for MP5
# Version: 2
#
# -lm is used to link in the math library
#
# -Wall turns on all warning messages 
#
#
CC = gcc
CPPFLAGS = -DVALIDATE
CFLAGS = -g -Wall
LDLIBS = -lm  

# the default target is to build the lab5 executable
lab5 : bst.o lab5.o

bst.o : bst.c bst.h

lab5.o : lab5.c bst.h

clean :
	rm -f bst.o lab5.o lab5