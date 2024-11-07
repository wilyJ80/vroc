# uso

- Compilar com:

`./debug.sh`

- ou:

`gcc -g ./lexer/*.c ./util/*.c ./debug.c -o croc`

- Executar com:

`./croc <codigo>`

- Executar com arquivo exemplo:

`./croc ./doc/examples/code.proc`

- Em um so comando:

`gcc -g ./lexer/*.c ./util/*.c ./debug.c -o croc && ./croc ./doc/examples/code.proc`

# problemas

- [X] automato nao funciona
- [X] funciona corretamente
    - [X] id
    - [X] intcon
    - [X] realcon
    - [X] charcon
    - [X] stringcon
    - [X] sinais
    - [X] comentario
- [X] \n processado corretamente
- [ ] \n, \0 nao e reconhecido como charcon independente: tem que refazer o automato 😰
- [X] sem tratamento de erro: tokens malformados
- [X] nao retorna palavras reservadas
- [ ] nao lida com lexemas maiores que o tamanho limite
- [X] mudar valor false de outro para enum para maior legibilidade
- [X] contador de linha: retornado pelo erro com sucesso
- [ ] refatorar codigo de print de tag token (sinal)
- [ ] imprimir string corretamente
- [ ] refatorar tratamento de erros?
- [ ] refatorar codigo palavras reservadas
