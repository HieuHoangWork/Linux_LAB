.PHONY: all clean

CC := gcc
CFLAGS := -I./inc
VALG_LAGS := -Wall -ggdb3
IP_CLIE := 192.168.181.129

all:
	$(CC) -o bin/client_1 client_1.c $(VALG_LAGS)
	$(CC) -o bin/client_2 client_2.c $(VALG_LAGS)

	$(CC) -c server.c -o obj_static/server.o $(CFLAGS) $(VALG_LAGS) -pthread -lpthread
	$(CC) -c src/processing_linked_list.c -o obj_static/processing_linked_list.o $(CFLAGS)
	$(CC) -c src/processing_FIFO.c -o obj_static/processing_FIFO.o $(CFLAGS)
	$(CC) -c src/socket_server.c -o obj_static/socket_server.o $(CFLAGS)

	ar rcs lib/static/libstatic.a obj_static/processing_linked_list.o obj_static/processing_FIFO.o  obj_static/socket_server.o 
	$(CC) obj_static/server.o -Llib/static -lstatic -o bin/server

valgrind_server:
	cd bin && valgrind --leak-check=yes ./server 8080 
valgrind_client_1:
	cd bin && valgrind --leak-check=yes ./client_1 $(IP_CLIE) 8080
valgrind_client_2:
	cd bin && valgrind --leak-check=yes ./client_2 $(IP_CLIE) 8080  
clean:
	find bin -type f ! -name ".gitkeep" -exec rm -f {} +	
	find obj_static -type f ! -name ".gitkeep" -exec rm -f {} +
	find lib/static -type f ! -name ".gitkeep" -exec rm -f {} +
	find tmp -type f ! -name ".gitkeep" -exec rm -f {} +
	rm -f tmp/myfifo