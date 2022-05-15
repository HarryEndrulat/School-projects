CC = gcc
CFLAGS = -Og -std=c99 -Wall -g
DEPS = challenge-response
LIBS = -lm
OBJ = problem2.o problem3.o challenge-response.o pointerlab.o
EXEC = pointerlab

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

pointerlab: challenge-response.o pointerlab.o problem2.o problem3.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

all: $(EXEC)

clean:
	rm -f $(OBJ) *.s *~ core

clear: clean
	rm $(EXEC)
