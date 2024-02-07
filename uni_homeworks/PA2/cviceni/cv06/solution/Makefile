CXXFLAGS=-Wall -pedantic -fsanitize=address -g -std=c++17
LDFLAGS=-fsanitize=address

all: main.out

main.o: main.cpp sharedptr.h yellowpages.h Makefile
	g++ -g $(CXXFLAGS) -c main.cpp -o main.o -D TEST=5

sharedptr.o: sharedptr.cpp sharedptr.h
	g++ -g $(CXXFLAGS) -Wall -pedantic -c sharedptr.cpp -o sharedptr.o

yellowpages.o: yellowpages.cpp yellowpages.h
	g++ -g $(CXXFLAGS) -Wall -pedantic -c yellowpages.cpp -o yellowpages.o

main.out: main.o sharedptr.o yellowpages.o
	g++ $(LDFLAGS) main.o sharedptr.o yellowpages.o -o main.out

clean:
	rm -f *.o main.out

run: main.out
	./main.out

.PHONY: clean run
