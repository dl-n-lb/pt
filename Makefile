OPTS=-fsanitize=address -O3 -Wall -Wextra -fopenmp -march=native

build/vec.o: vec.c
	$(CC) -c -o build/vec.o vec.c $(OPTS)

build/image.o: image.c
	$(CC) -c -o build/image.o image.c $(OPTS)

build/ray.o: ray.c
	$(CC) -c -o build/ray.o ray.c $(OPTS)

build/cam.o: cam.c
	$(CC) -c -o build/cam.o cam.c $(OPTS)

main: main.c build/image.o build/vec.o build/ray.o build/cam.o
	$(CC) -o main main.c build/image.o build/vec.o build/ray.o build/cam.o -lm $(OPTS)

clean:
	rm build/*.o
	rm main

.PHONY: main

