exec = tac.out
sources = $(wildcard src/*.c)
objects = $(sources:.c=.o)
flags = -g -Wall -lm -ldl -fPIC -rdynamic


$(exec): src/bootstrap.c $(objects) 
	gcc $(objects) $(flags) -o $(exec)

src/bootstrap.c:
	xxd -i src/asm/bootstrap.asm > src/include/bootstrap.h

%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@

clean:
	-rm *.out
	-rm *.o
	-rm *.a
	-rm src/*.o
	-rm src/include/bootstrap.h

lint:
	clang-tidy src/*.c src/include/*.h
