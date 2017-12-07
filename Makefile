CC = gcc -ggdb -std=c99 -Wall -Werror -O3 -pthread
OUT = semaphore_controller

util/utils.o:
	$(CC) -c util/utils.c -o util/utils.o

util/string_builder.o:
	$(CC) -c util/string_builder.c -o util/string_builder.o

util/stacktrace.o:
	$(CC) -c util/stacktrace.c -o util/stacktrace.o

$(OUT).o:
	$(CC) -c $(OUT).c

all: clean $(OUT).o util/stacktrace.o util/string_builder.o util/utils.o
	$(CC) -o $(OUT) $(OUT).o util/stacktrace.o util/string_builder.o util/utils.o

clean:
	touch dummy.o
	find . -name '*.o' -delete
	rm -f $(OUT)

install: clean all

run: install
	./$(OUT)

rerun: all
	./$(OUT)

valgrind: clean all
	valgrind -v --leak-check=full ./$(OUT)