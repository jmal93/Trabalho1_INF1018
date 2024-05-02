#include "codifica.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  struct compactadora vCompactadora[32] = {
      {'\n', 1, 8}, {'B', 1, 6},  {'D', 1, 5}, {'O', 16, 5}, {'M', 25, 5},
      {'A', 5, 4},  {'I', 13, 4}, {0, 0, 0},   {0, 0, 0},    {0, 0, 0},
      {0, 0, 0},    {0, 0, 0},    {0, 0, 0},   {0, 0, 0},    {0, 0, 0},
      {0, 0, 0},    {0, 0, 0},    {0, 0, 0},   {0, 0, 0},    {0, 0, 0},
      {0, 0, 0},    {0, 0, 0},    {0, 0, 0},   {0, 0, 0},    {0, 0, 0},
      {0, 0, 0},    {0, 0, 0},    {0, 0, 0},   {0, 0, 0},    {0, 0, 0},
      {0, 0, 0},    {4, 0, 12}};
  FILE *entradaTexto = fopen("entradaTexto.txt", "r");
  FILE *saidaTexto = fopen("saidaTexto.txt", "w");
  FILE *Binario = fopen("Binario.bin", "rb+");

  if (!entradaTexto || !saidaTexto || !Binario) {
    fprintf(stderr, "Erro ao abrir o arquivo\n");
    exit(1);
  }

  compacta(entradaTexto, Binario, vCompactadora);
  descompacta(Binario, saidaTexto, vCompactadora);

  fclose(entradaTexto);
  fclose(saidaTexto);
  fclose(Binario);
  printf("Fim do programa\n");
  return 0;
}