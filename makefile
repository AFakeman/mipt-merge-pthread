CFLAGS = -Wall -Werror -pthread -g
CC = gcc-7

main: main.c misc.o sort.o stack.o thread_pool.o
	$(CC) main.c misc.o sort.o stack.o thread_pool.o -o main $(CFLAGS)

sort.o: sort.c sort.h
	$(CC) -c sort.c $(CFLAGS)

stack.o: stack.c stack.h
	$(CC) -c stack.c $(CFLAGS)

thread_pool.o: thread_pool.c thread_pool.h
	$(CC) -c thread_pool.c $(CFLAGS)

misc.o: misc.c misc.h
	$(CC) -c misc.c $(CFLAGS)

clean:
	rm -rf tests *.o *.gcov *.dSYM *.gcda *.gcno *.swp