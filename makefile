# The makefile for MP1.
TARGET = lab1
CC     = gcc
CFLAGS = -Wall -g -c

lab1: task_list.o lab1.o
	$(CC) lab1.o task_list.o -o $(TARGET)

lab1.o: task_list.h
	$(CC) $(CFLAGS) lab1.c

task_listo: task_list.h
	$(CC) $(CFLAGS) task_list.c

.PHONY: test
test: lab1
	./lab1 10 < input1.txt

.PHONY: clean
clean:
	rm -f task_list.o lab1.o lab1
