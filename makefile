scopy: scopy.o
	gcc scopy.o -o scopy
scopy.o: scopy.c
	gcc -c scopy.c -o scopy.o
