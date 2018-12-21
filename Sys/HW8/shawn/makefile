
cmdparse.o: cmdparse.h cmdparse.c
	gcc -g -c cmdparse.c 

assign8.o: assign8.c
	gcc -g -c assign8.c 

assign8: assign8.o cmdparse.o
	gcc -g assign8.o cmdparse.o -o myshell

clean:
	rm *.o myshell