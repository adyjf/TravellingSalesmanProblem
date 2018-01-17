CC=g++
CFLAGS=-lemon -std=c++11
LDFLAGS=-lemon
EXEC=TSP

all: $(EXEC)

TSP: main.o
	$(CC) -o $@ $^ $(LDFLAGS)

main.o: main.cpp
	$(CC) -o main.o -c main.cpp $(CFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
