CC=gcc
CFLAGS=-Wall -I.
DEPS = 20221787.h
OBJ = 20221787.o

%.o: %.c $(DEPS)		# "X.o는 X.c와 헤더로 만든다"
	$(CC) -c -o $@ $< $(CFLAGS)		# $@ = 20221787, $< = 20221787.c

20221787: $(OBJ)		# "실행파일은 .o로 만든다:
	$(CC) -o $@ $^ $(CFLAGS)		# $@ = 20221787, $^ = 20221787.o

clean:
	rm -f *.o 20221787