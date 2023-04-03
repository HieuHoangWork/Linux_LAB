all:
	gcc -o test src_memleak.c -Wall -ggdb3
	valgrind --leak-check=yes ./test 
clean:
	rm -rf test