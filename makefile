CC = gcc
CFLAGS = -Wall -g

OBJS = main.o lexer.o parser.o

compiler: $(OBJS)
	$(CC) $(CFLAGS) -o compiler $(OBJS)

clean:
	rm -f *.o compiler
