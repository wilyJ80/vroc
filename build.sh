#!/bin/bash

if [[ $1 == 'test' ]]; then
		gcc -g ./test/*.c ./lexer/*.c -o croc
		echo 'built test.'
else
		gcc -g ./main.c ./lexer/*.c  -o croc
		echo 'built main.'
fi
