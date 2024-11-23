#!/bin/bash

gcc -g ./lexer/*.c ./util/*.c ./main.c -o croc

#!/bin/bash

if [[ $1 == 'test' ]]; then
		gcc -g ./test/*.c ./lexer/*.c ./util/*.c -o croc
		echo 'built test.'
else
		gcc -g ./main.c ./lexer/*.c ./util/*.c -o croc
		echo 'built main.'
fi
