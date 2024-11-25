![swag](./doc/swag.webp)

![diagram](./doc/flap2.png)

# uso

- Compilar com:

`./build.sh`

- ou:

`gcc -g ./lexer/*.c ./util/*.c ./main.c -o croc`

- Executar com:

`./croc <codigo>`

- Executar com arquivo exemplo:

`./croc ./doc/examples/code.proc`

- Em um so comando:

`gcc -g ./lexer/*.c ./util/*.c ./main.c -o croc && ./croc ./doc/examples/code.proc`

- Teste:

`gcc -g ./lexer/*.c ./util/*.c ./test.c -o croc && ./croc`

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
- [X] \n, \0 nao e reconhecido como charcon independente: tem que refazer o automato 😰
- [X] sem tratamento de erro: tokens malformados
- [X] nao retorna palavras reservadas
- [ ] nao lida com lexemas maiores que o tamanho limite
- [X] mudar valor false de outro para enum para maior legibilidade
- [X] contador de linha: retornado pelo erro com sucesso
- [X] refatorar codigo de print de tag token (sinal)
- [ ] imprimir charcon corretamente?
- [X] refatorar tratamento de erros?
- [X] refatorar codigo palavras reservadas
- [ ] melhorar logica categorias de transicao: precisa de um campo especifico pra outro?

![](./doc/cute.webp)
