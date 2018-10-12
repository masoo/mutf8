CC := gcc
CFLAGS := -g -Wall -Wextra
test_main: test_main.o mutf8.o

clean:
	rm -f *.o test_main

test:
	./test_main
