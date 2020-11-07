# tac
> A programming language inspired by **Typescript**, **Assembly** and **C**.

## Introduction
> **Tac** is currently only being developed by me, Sebastian.  
> The reason I am creating **Tac** is because I wanted to teach my self how to
> write a compiler.  
> **Tac** is not to be taken seriously in any kind of way at this moment,
> since it is just another toy project of mine.

## "Hello World" Example
> Here is "Hello World" in **Tac**:
```
main = (argc: int, argv: Array<string>):int => {
  print("Hello World\n");
  return 0;
};
```

## Installing & Running Tac Compiler

### Dependencies
#### Compiler Build Dependencies
> To build the compiler you will need:
* gcc                  - GNU Compiler Collection ( C compiler )

#### Compiler Runtime Dependencies
> The compiler currently has these dependencies:
* as (1)               - the portable GNU assembler.
* ld (1)               - The GNU linker

### Building
> To build the **Tac** compiler, simply run:
```bash
make
```
> This will produce the `./tac.out` binary which is used to compile Tac-code.

### Compiling Tac-code
> To compile Tac-code, simply use the generated binary from the build-step:
```bash
./tac.out <name-of-file>.tac
```
> This will generate a `./a.out` file which is the compiled executable of
> your source code.  
> **Note** Tac only supports building 32-bit executables at the moment.

## Todo ( in no particular order )
```
X = done
/ = in progress
```
- [X] Implement variable assignments
- [X] Implement function definitions
- [X] Implement function calls
- [ ] Implement conditions
- [/] Implement arithmetic
- [ ] Implement while-loops
- [ ] Implement for-loops
- [ ] Implement floats
- [X] Implement integers
- [X] Implement main method
- [ ] Implement objects
- [/] Implement arrays
- [ ] Implement enums
- [X] Implement comments
- [ ] Implement C-header parser
- [ ] Implement calling functions from shared libraries (.so, .obj, .a, ...)
- [ ] Add 64-bit support
- [ ] Implement type guards in the compiler
- [ ] Get rid of "GNU Assembler" dependency
- [ ] Get rid of "GNU Linker" dependency
- [ ] Get rid of "xxdi" dependency
