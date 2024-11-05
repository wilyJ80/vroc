#!/bin/bash

gcc -g ./lexer/*.c ./util/*.c ./debug.c -o debug && ./debug
