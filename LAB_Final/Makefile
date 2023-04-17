.PHONY: all clean

CC := gcc
CFLAGS := -I./inc
VALG_LAGS := -Wall -ggdb3
IP_CLIE := 192.168.181.129

all:
	$(CC) -o bin/client_1 client_1.c $(VALG_LAGS)
	$(CC) -o bin/client_2 client_2.c $(VALG_LAGS)

	$(CC) -c server.c -o obj_static/server.o $(CFLAGS) $(VALG_LAGS) -pthread -lpthread
	$(CC) -c src/handle_data.c -o obj_static/handle_data.o $(CFLAGS)

	ar rcs lib/static/libstatic.a obj_static/handle_data.o 
	$(CC) obj_static/server.o -Llib/static -lstatic -o bin/server

valgrind_server:
	valgrind --leak-check=yes ./bin/server 8080 
valgrind_client_1:
	valgrind --leak-check=yes ./bin/client_1 $(IP_CLIE) 8080
valgrind_client_2:
	valgrind --leak-check=yes ./bin/client_2 $(IP_CLIE) 8080  
clean:
	rm -rf obj_static/* bin/* lib/static/* tmp/*