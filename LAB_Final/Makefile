.PHONY: all clean

all:
	gcc -o server server.c handle_data.c -I . -Wall -ggdb3 -pthread -lpthread
	gcc -o client_1 client_1.c -Wall -ggdb3
	gcc -o client_2 client_2.c -Wall -ggdb3
valgrind_server:
	valgrind --leak-check=yes ./server 8080 
valgrind_client_1:
	valgrind --leak-check=yes ./client_1 192.168.122.129 8080
valgrind_client_2:
	valgrind --leak-check=yes ./client_2 192.168.122.129 8080  
clean:
	rm -rf server client_1 client_2 Data.txt