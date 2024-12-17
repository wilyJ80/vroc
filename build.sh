#!/bin/bash

if [[ $1 == 'test' ]]; then
	gcc -g ./test/test.c ./test/new_parser_tests.c ./test/lexer_test.c ./lexer/*.c ./parser/*.c -o croc
	echo 'built test.'
elif [[ $1 == 'debug' ]]; then
	gcc -g ./debug.c ./lexer/*.c ./parser/*.c -o croc
	echo 'built debug.'
else
	gcc -g ./main.c ./lexer/*.c ./parser/*.c  -o croc
	echo 'built main.'
fi
