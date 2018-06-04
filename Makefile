LIBS = -llua -luv
MODULE = $(patsubst %.c,%.o,$(wildcard module/*.c))

nodelua: main.o $(MODULE)
	@echo LD $^
	@gcc -o nodelua $^ $(LIBS)

%.o: %.c
	@echo CC $^
	@gcc -c $^ -o $@ 

.PHONY: clean
clean:
	rm -rf nodelua *.o $(MODULE)
