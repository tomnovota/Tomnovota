OBJS = main.o eval.o parser.o texception.o tfact.o trule.o
TARGET = Predikaty
CC = g++

all:	${OBJS}
	g++ -o ${TARGET} ${OBJS}

run: ${OBJS}
	g++ -o ${TARGET} ${OBJS}
	./${TARGET} rules.txt
clean:
	rm -rf *.o ${TARGET}

