#!/bin/bash

if [[ $1 == 'test' ]]; then
		gcc -g ./test/*.c ./lexer/*.c ./parser/*.c -o croc
		echo 'built test.'
else
		gcc -g ./main.c ./lexer/*.c ./parser/*.c  -o croc
		echo 'built main.'
fi
