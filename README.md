# Another MiniJava Compiler and Interpreter

  Code implemented for MAC5750 - Compiler Theory and Construction

## Dependencies
The following dependencies are required:

    * g++
	* flex
	* bison
	* nasm
	* libcgraph
	* bash

## Quick Install:
Run
```
make
```
This will generate the mjc1 compiler.

## Usage
Run
```
./minijc.sh
```
This is the minijc compiler driver and will call the `mjc1` compiler, the `nasm` assembler and the `ld` linker.

## License
This software is distributed under the GNU Public License v2. Check LICENSE file.

## Bugs
Check Issues tab.
