COMP = gcc
#OPTIONS = -lm
OPTIONS_CC = -g -Wall

main: main.o store.o
	$(COMP) -o main main.o store.o $(OPTIONS)
	rm *.o

main.o: main.c store.h
	$(COMP) -o main.o -c main.c $(OPTIONS_CC)

store.o: store.c store.h
	$(COMP) -o store.o -c store.c $(OPTIONS_CC)

archive: 
	tar cvzf CS353-TP3.tar.gz store.c store.h main.c Makefile CS353-TP3-4.pdf

clean :
	rm -f *.o 
	rm -f main-test main 
	

