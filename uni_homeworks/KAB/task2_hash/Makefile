CC = g++
CFLAGS = -g
LDFLAGS = -lcrypto

all: hash
run:
	#run your tests here...
hash: hash.o
	$(CC) -o $@ $< $(LDFLAGS)
%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)
