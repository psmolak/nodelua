LIBS = -llua5.3 -luv
INCLUDE = -I/usr/include/lua5.3
MODULE = $(patsubst %.c,%.o,$(wildcard src/module/*.c))

nodelua: src/main.o $(MODULE)
	@echo LD $^
	@gcc -o nodelua $^ $(LIBS)

%.o: %.c
	@echo CC $^
	@gcc $(INCLUDE) -c $^ -o $@ 

.PHONY: clean
clean:
	rm -rf src/*.o $(MODULE)

.PHONY: dist-clean
dist-clean: clean
	rm -rf nodelua
