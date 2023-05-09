CC=gcc
OBJ= cleint_b.o server_b.o chat.o
NAME = main
CFLAGS = -Wall -g

all : stnc
stnc: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o chat  
server_b.o: server_b.c server_b.h
	$(CC) $(CFLAGS) -c server_b.c
cleint_b.o: cleint_b.c cleint_b.h
	$(CC) $(CFLAGS) -c cleint_b.c
chat.o: chat.c
	$(CC) $(CFLAGS) -c chat.c

.PHONY = clean all 




clean:
	rm -f *.o *.a chat