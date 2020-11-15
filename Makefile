exec = tac.out
sources = $(wildcard src/*.c)
objects = $(sources:.c=.o)
flags = -g -Wall -lm -ldl -fPIC -rdynamic
asm = bootstrap.h add.h div.h\
						mul.h sub.h root.h

asm_templates = assign_int.h assign_call.h assign_binop.h assign_default.h\
								function_begin.h int.h access.h



$(exec): $(asm) $(asm_templates) $(objects) 
	gcc $(objects) $(flags) -o $(exec)

$(asm):
	mkdir -p src/include/asm
	xxd -i src/asm/$*.asm > src/include/asm/$@

$(asm_templates):
	mkdir -p src/include/asm/templates
	xxd -i src/asm/templates/$*.asm > src/include/asm/templates/$@

libtac.a: $(objects)
	ar rcs $@ $^

%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@

clean:
	-rm *.out
	-rm *.o
	-rm *.a
	-rm src/*.o
	-rm src/include/asm/*

lint:
	clang-tidy src/*.c src/include/*.h
