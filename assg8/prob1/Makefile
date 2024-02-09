mymake : mymake.o utils.o
	gcc -g -Wall mymake.o utils.o -o mymake

mymake.o : mymake.c utils.h
	gcc -g -Wall -c mymake.c

utils.o : utils.c utils.h
	gcc -g -Wall -c utils.c