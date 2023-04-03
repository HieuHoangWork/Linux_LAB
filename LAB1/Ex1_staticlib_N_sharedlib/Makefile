.PHONY: all_static static_lib clean_static all_shared shared_lib move_default_location use_with_default_location clean_shared

CC := gcc
CFLAGS := -I./inc

all_static:
	$(CC) -c main.c -o obj_static/main.o $(CFLAGS)
	$(CC) -c src/Addition.c -o obj_static/Addition.o $(CFLAGS)
	$(CC) -c src/Subtraction.c -o obj_static/Subtraction.o $(CFLAGS)
static_lib: 
	ar rcs lib/static/libstatic.a obj_static/Addition.o obj_static/Subtraction.o
link_static:
	$(CC) obj_static/main.o -Llib/static -lstatic -o bin/exam_static
clean_static:
	rm -rf obj_static/* bin/*


all_shared:
	$(CC) -c main.c -o obj_shared/main.o $(CFLAGS)
	$(CC) -c -fPIC src/Addition.c -o obj_shared/Addition.o $(CFLAGS)
	$(CC) -c -fPIC src/Subtraction.c -o obj_shared/Subtraction.o $(CFLAGS)
shared_lib:
	$(CC) -shared obj_shared/Addition.o obj_shared/Subtraction.o -o lib/shared/libshared.so
link_shared:
	$(CC) obj_shared/main.o -Llib/shared -lshared -o bin/exam_shared
move_default_location:
	sudo mv lib/shared/libshared.so /usr/lib
	sudo chmod 755 /usr/lib/libshared.so
use_with_default_location:
	bin/exam_shared
clean_shared:
	rm -rf obj_shared/* bin/*

