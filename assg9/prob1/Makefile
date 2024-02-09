mymake2 : mymake2.o utils.o
	gcc -g -Wall mymake2.o utils.o -o mymake2

mymake2.o : mymake2.c utils.h
	gcc -g -Wall -c mymake2.c

utils.o : utils.c utils.h
	gcc -g -Wall -c utils.c
.PHONY: clean
clean:
	rm -f mymake2 *.o