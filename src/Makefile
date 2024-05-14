OBJS = array.o createGraph.o traverse.o main.o getDelay.o cfg_parse.o

CC = gcc
# CFLAGS = -O2 -Wno-unused-result -Wno-format -pg -fprofile-arcs -ftest-coverage -D_GNU_SOURCE -g -Wall -Wextra -std=c99 #-Wall
# CFLAGS = -Wno-unused-result -Wno-format -pg -fprofile-arcs -ftest-coverage -D_GNU_SOURCE -g -Wall -Wextra -std=c99 #-Wall
CFLAGS = -std=c11 -g -w -m64 #-Wall
MFLAGS = -lm -lreadline

all: astra

tabularasa: all
	rm data/*

astra: $(OBJS)
	$(CC) $(CFLAGS) $(FFLAGS) -o astra $(OBJS) $(MFLAGS)
clean:
	rm -f astra *.o

array.o: array.c array.h
	$(CC) $(INCLUDES) $(CFLAGS) $(FFLAGS) -c array.c 

createGraph.o: createGraph.c graph.h array.h
	$(CC) $(INCLUDES) $(CFLAGS) $(FFLAGS) -c createGraph.c 

getDelay.o: getDelay.c graph.h array.h
	$(CC) $(INCLUDES) $(CFLAGS) $(FFLAGS) -c getDelay.c 

traverse.o: traverse.c graph.h array.h
	$(CC) $(INCLUDES) $(CFLAGS) $(FFLAGS) -c traverse.c

cfg_parse.o: cfg_parse.c cfg_parse.h
	$(CC) $(INCLUDES) $(CFLAGS) $(FFLAGS) -c cfg_parse.c

main.o: main.c graph.h array.h 
	$(CC) $(INCLUDES) $(CFLAGS) $(FFLAGS) -c main.c
