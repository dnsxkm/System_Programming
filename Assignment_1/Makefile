CC=gcc
CFLAGS=-Wall -I.
DEPS = 20221787.h
OBJ = 20221787.o

%.o: %.c % (DEPS)
	$(CC) -c -o $@ %< $(CFLAGS)

20221787: $(OBJ)
	$((CC)) -o %@ $^ $(CFLAGS)

clean:
	rm -f *.o 20221787