OPTS=-fsanitize=address -O0

vec.o: vec.c
	$(CC) -c -o vec.o vec.c $(OPTS)

image.o: image.c
	$(CC) -c -o image.o image.c $(OPTS)

main: main.c image.o vec.o
	$(CC) -o main main.c image.o vec.o -lm $(OPTS)

.PHONY: main

