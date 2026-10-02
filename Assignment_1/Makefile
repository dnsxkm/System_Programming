CC=gcc
CFLAGS=-Wall -I.
DEPS = 20221787.h
OBJ = 20221787.o

# "X.o는 X.c와 헤더로 만든다"
# $@ = 20221787.o, $< = 20221787.c
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# "실행파일은 .o로 만든다"
# $@ = 20221787, $^ = 20221787.o
20221787: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o 20221787
