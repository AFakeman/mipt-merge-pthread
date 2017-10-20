COMPILER = gcc-7
CFLAGS = -Wall -Werror -pthread -g

main: main.c misc.o sort.o stack.o thread_pool.o
	$(COMPILER) main.c misc.o sort.o stack.o thread_pool.o -o main $(CFLAGS)

sort.o: sort.c sort.h
	$(COMPILER) -c sort.c $(CFLAGS)

stack.o: stack.c stack.h
	$(COMPILER) -c stack.c $(CFLAGS)

thread_pool.o: thread_pool.c thread_pool.h
	$(COMPILER) -c thread_pool.c $(CFLAGS)

misc.o: misc.c misc.h
	$(COMPILER) -c misc.c $(CFLAGS)

clean:
	rm -rf tests *.o *.gcov *.dSYM *.gcda *.gcno *.swp