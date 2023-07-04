<div align="center">
  <h3 align="center">Search Algorithms</h3>
</div>

## Sobre
Trabalho 2 da disciplina de Estrutura de Dados, UFES. Objetivo: Usar estruturas de dados para implementar algorítimos de busca BFS, DFS e A*.

### Pré-requisitos

- Linux
- [GNU Compiler Collection](https://gcc.gnu.org/)

### Instalando

1. Clone o repositório

   ```sh
   git clone https://github.com/tucozz/Search_Algorithms.git
   ```

2. Dirija-se ao seu diretório

   ```sh
   cd Search_Algorithms
   ```

3. Compile o código-fonte

   ```sh
   make
   ```

## Usando

Intruções de uso o programa.

### Entradas
O usuário do programa deverá digitar como entrada:

```sh
./main <maze> <x1 y1> <x2 y1> <alg>
```

Onde,

- `maze` - é o arquivo (maze.bin) que contém os labirinto a ser resolvido.
- `x1 y1` - são as coordenadas de início.
- `x2 y2` - são as coordenadas de fim
- `alg`- é o algorítmo a ser usado (BFS, DFS ou A*)


### Labirintos
O labirinto será armazenado em um arquivo binário.

- Os primeiros 8 bytes serão 2 inteiros, L e C, representando o número
de linhas e colunas.
- Cada célula será representada por um unsigned char (1 byte) com
valor 0 se a célula for livre e 1 se for ocupada.
- As linhas serão escritas em sequência no arquivo binário. Isto é, após
os 2 inteiros iniciais, os próximos C bytes serão o conteúdo da primeira
linha, os próximos C bytes serão o conteúdo da segunda linha e assim
por diante.
