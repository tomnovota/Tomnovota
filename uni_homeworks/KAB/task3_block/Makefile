CC = g++
CFLAGS = -g -fsanitize="address"
LDFLAGS = -lcrypto

all: a.out

a.out: main.o
	$(CC) -o $@ $< $(LDFLAGS)
%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)
