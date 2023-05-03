CC=gcc
OBJ= cleint_b.o server_b.o
NAME = main
CFLAGS = -Wall -Werror -g

all : chat
chat: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o chat  
server_b.o: server_b.c server_b.h
	$(CC) $(CFLAGS) -c server_b.c
cleint_b.o: cleint_b.c cleint_b.h
	$(CC) $(CFLAGS) -c cleint_b.c

.PHONY = clean all 

clean:
	rm -f *.o *.a chat
