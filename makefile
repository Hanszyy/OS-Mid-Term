CC=gcc
OBJ= main.o cq.o customers.o tellers.o
CFLAGS= -Wall 
EXEC= cq


$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) 


main.o: main.c cq.h customers.h tellers.h
	$(CC) $(CFLAGS) -c main.c
	
cq.o: cq.c cq.h customers.h
	$(CC) $(CFLAGS) -c cq.c 

customers.o: customers.c customers.h 
	$(CC) $(CFLAGS) -c customers.c

tellers.o: tellers.c cq.h tellers.h customers.h
	$(CC) $(CFLAGS) -c tellers.c

clean:
	rm -i $(EXEC) $(OBJ)





