CXXFLAGS=-Wall -pedantic -fsanitize=address -g
LDFLAGS=-fsanitize=address

all: main.out

main.o: main.cpp yellowpages.h Makefile
	g++ -g $(CXXFLAGS) -c main.cpp -o main.o -D TEST=0

yellowpages.o: yellowpages.cpp yellowpages.h
	g++ -g $(CXXFLAGS) -Wall -pedantic -c yellowpages.cpp -o yellowpages.o

main.out: main.o yellowpages.o
	g++ $(LDFLAGS) main.o yellowpages.o -o main.out

clean:
	rm -f *.o main.out

run: main.out
	./main.out

.PHONY: clean run
