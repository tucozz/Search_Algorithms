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
• O nome de um arquivo contendo o labirinto
• As coordenadas (linha e coluna) da célula de entrada
• As coordenadas da célula de saída.
• O algoritmo a ser utilizado (BFS, DFS ou A*).
• Nome do arquivo de saída.

### Labirintos
O labirinto será armazenado em um arquivo binário.
• Os primeiros 8 bytes serão 2 inteiros, L e C, representando o número
de linhas e colunas.
• Cada célula será representada por um unsigned char (1 byte) com
valor 0 se a célula for livre e 1 se for ocupada.
• As linhas serão escritas em sequência no arquivo binário. Isto é, após
os 2 inteiros iniciais, os próximos C bytes serão o conteúdo da primeira
linha, os próximos C bytes serão o conteúdo da segunda linha e assim
por diante.
