CC=gcc
CFLAGS=

main: myMain
	./myMain
myMain: main.c
	$(CC) main.c -o myMain
clean:
	rm myMain *.o a.out file0.txt