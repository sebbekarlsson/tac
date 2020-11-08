exec = test.out
sources = $(wildcard src/*.c)
objects = $(sources:.c=.o)
flags = -g -Wall -lm -ldl -fPIC -rdynamic -L../ -ltac

$(exec): libtac.a $(objects)
	gcc $(objects) $(flags) -o $(exec)

libtac.a:
	make libtac.a -C ../

%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@

clean:
	-rm *.out
	-rm *.o
	-rm *.a
	-rm src/*.o

lint:
	clang-tidy src/*.c src/include/*.h
