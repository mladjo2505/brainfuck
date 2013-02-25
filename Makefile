# Makefile for ANSI C brainf**k interpreter
# by mladjo2505@gmail.com
#

PROGRAM	= brainfuck
CC		= gcc
CFLAGS	= -g -ansi -Wall

$(PROGRAM) : $(PROGRAM).c
	$(CC) -o $(PROGRAM) $(PROGRAM).c

.PHONY : clean beauty dist

clean :
	-rm -f $(PROGRAM) *~

beauty :
	-indent -kr $(PROGRAM).c

dist : clean
	-tar -chvj -C .. -f ../$(PROGRAM).tar.bz2 $(PROGRAM)
