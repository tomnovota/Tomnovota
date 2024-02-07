all: main.out

main.o: main.cpp fraction.h
	g++ -g -Wall -pedantic -c main.cpp -o main.o -D TEST=8

fraction.o: fraction.cpp fraction.h
	g++ -g -Wall -pedantic -c fraction.cpp -o fraction.o

main.out: main.o fraction.o
	g++ main.o fraction.o -o main.out

clean:
	rm -f *.o main.out

run: main.out
	./main.out

.PHONY: clean
