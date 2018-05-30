nodelua: main.c module.c
	gcc -o nodelua main.c module.c -llua -luv

.PHONY: clean
clean:
	rm -rf nodelua *.o
