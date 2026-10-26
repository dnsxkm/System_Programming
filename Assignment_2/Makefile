CC=gcc
CFLAGS=-Wall
TARGET=20221787
OBJ=20221787.o

# Build the executable named after the student ID
$(TARGET): $(OBJ)
	$(CC) -o $@ $^

%.o: %.c 20221787.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o $(TARGET)
