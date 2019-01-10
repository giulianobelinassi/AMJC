#!/bin/bash

MODE=""
INPUT_FILE=""
OUTPUT_FILE=""

print_usage_message()
{
    echo "Minijava Interpreter and Compiler driver"
    echo "Options:"
    echo "  --compile        Compile the program into i486 assembler"
    echo "  --interp         Interpretate the program"
    echo "  --dump-tree      Dump AST into graph.pdf"
    echo "  -o               Output file. Else it will be dumped into stdout"
    echo "  filename         Input file."
    echo ""
    echo "This program is Free Software and is distributed in the GNU Public License"
    echo "version 2. There is no warranty; not even from MERCHANTABILITY or FITNESS FOR"
    echo "A PARTICULAR PURPOSE."
}

check_mode()
{

	local possibles=("--compile" "--interp" "--dump-tree")

	for i in "${possibles[@]}"; do
		if [ "$i" == "$MODE" ]; then
			return 0
		fi
	done
	return 1
}

check_dependencies()
{
    nasm --version > /dev/null
    if [ $? -ne 0 ]; then
        echo "ERROR: nasm (Netwide Assembler) not found."
        exit
    fi

    ld --version > /dev/null
    if [ $? -ne 0 ]; then
        echo "ERROR: GNU ld (LinkEditor) not found."
        exit
    fi

    flex --version > /dev/null
    if [ $? -ne 0 ]; then
        echo "ERROR: GNU flex not found."
        exit
    fi

    bison --version > /dev/null
    if [ $? -ne 0 ]; then
        echo "ERROR: GNU bison not found."
        exit
    fi
}

check_dependencies

while [[ $# -gt 0 ]]; do
	key="$1"
	case $key in
		--compile)
		MODE="--compile"
		shift # past argument
		;;
		--interp)
		MODE="--interp"
		shift # past argument
		;;
		--dump-tree)
		MODE="--dump-tree"
		shift # past argument
		;;
		-o)
		OUTPUT_FILE="$2"
		shift # past argument
		shift
		;;
		*)    # unknown option
		INPUT_FILE="$1"
		shift # past argument
		;;
	esac
done

check_mode
if [ $? -ne 0 ]; then
	print_usage_message
	exit
fi

if [ -z "$INPUT_FILE" ]; then
	print_usage_message
	exit
fi


if [ -z "$OUTPUT_FILE" ]; then
	./mjc1 $MODE < $INPUT_FILE
	exit
else
	./mjc1 $MODE < $INPUT_FILE > $OUTPUT_FILE.s
    nasm -f elf32 $OUTPUT_FILE.s
    ld -melf_i386 -dynamic-linker /lib32/ld-linux.so.2 -lc $OUTPUT_FILE.o -o $OUTPUT_FILE
	exit
fi


#set -- "${POSITIONAL[@]}" # restore positional parameters

print_usage_message
