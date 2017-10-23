CFLAGS = -Wall -Werror -pthread -g
CC = gcc-7

main: main.c misc.o sort.o thread_pool.o queue.o
	$(CC) main.c misc.o sort.o thread_pool.o queue.o -o main $(CFLAGS)

sort.o: sort.c sort.h
	$(CC) -c sort.c $(CFLAGS)

thread_pool.o: thread_pool.c thread_pool.h
	$(CC) -c thread_pool.c $(CFLAGS)

queue.o: queue.c queue.h
	$(CC) -c queue.c $(CFLAGS)

misc.o: misc.c misc.h
	$(CC) -c misc.c $(CFLAGS)

clean:
	rm -rf tests *.o *.gcov *.dSYM *.gcda *.gcno *.swp