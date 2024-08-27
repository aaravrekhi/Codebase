
# The makefile for MP3.
# Type:
#   make          -- to build programs lab3 and geninput (also "make all")
#   make lab3     -- to build program lab3
#   make geninput -- to build the program to generate test input
#   make clean    -- to delete object files, executable, and core
#   make design   -- check for simple design errors (incomplete)
#   make list.o   -- to compile only list.o (or: use lab3.o, task_support.o)
#
# Format for each entry
#    target : dependency list of targets or files
#    <tab> command 1
#    <tab> command 2
#    ...
#    <tab> last command
#    <blank line>   -- the list of commands must end with a blank line

# CC is the compiler used to compile and link C programs
CC     = gcc
# CFLAGS are the compiler flags used when compiling C programs
CFLAGS = -Wall -g -DVALIDATE

.PHONY : all
all : lab3 geninput

lab3 : list.o task_support.o

list.o : datatypes.h list.h

task_support.o : task_support.h datatypes.h list.h

lab3.o : datatypes.h list.h task_support.h

.PHONY : design
design :
	@grep -e "-> *head" lab3.c task_support.c ||:
	@grep -e "-> *tail" lab3.c task_support.c ||:
	@grep -e "-> *current_list_size" lab3.c task_support.c ||:
	@grep -e "-> *list_sorted_state" lab3.c task_support.c ||:
	@grep -e "-> *next" lab3.c task_support.c ||:
	@grep -e "-> *prev" lab3.c task_support.c ||:
	@grep -e "-> *data_ptr" lab3.c task_support.c ||:
	@grep "comp_proc" lab3.c task_support.c ||:
	@grep "data_clean" lab3.c task_support.c ||:
	@grep "list_node_t" lab3.c task_support.c ||:
	@grep "ListNode" lab3.c task_support.c ||:
	@grep "task_id" list.c ||:
	@grep "priority" list.c ||:
	@grep -e "-> *state" list.c ||:
	@grep "wallclocktime" list.c ||:
	@grep "nargs" list.c ||:
	@grep "args" list.c ||:
	@grep -e "-> *output" list.c ||:
	@grep "task_" list.c ||:
	@grep "Task" list.c ||:

.PHONY : clean
clean :
	rm -f list.o task_support.o lab3.o lab3 geninput.o geninput