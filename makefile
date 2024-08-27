# makefile for MP4
#
# -lm is used to link in the math library

CC = gcc
CFLAGS = -Wall -g
LDLIBS = -lm

lab4 : lab4.o mem.o

lab4.o : lab4.c mem.h

mem.o : mem.c mem.h

.PHONY: clean
clean :
	rm -f mem.o lab4.o lab4

