CFLAGS = -Wall -Werror -Wextra -O3

.PHONY = all clean

all: reverse_test

%.o: %.c
	gcc $(CFLAGS) -c $^ -o $@

reverse_test: test_reverse.o reverse.o 
	gcc $(CFLAGS) $^ -o reverse_test

clean: 
	rm -f reverse_test *.o