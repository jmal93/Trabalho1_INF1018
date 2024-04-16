#include <stdio.h>
#include <stdlib.h>
#ifndef __CODIFICA__
#define __CODIFICA__

struct compactadora {
  char simbolo;
  unsigned int codigo;
  int tamanho;
};
typedef struct compactadora Compactadora;

void compacta(FILE *arqTexto, FILE *arqBin, struct compactadora *v);
void descompacta(FILE *arqBin, FILE *arqTexto, struct compactadora *v);

#endif

int main(void) {
  Compactadora *vCompactadora[32];
  FILE *entrada = fopen("entrada.txt", "r");
  FILE *saida = fopen("saida.bin", "wb");

  if (entrada == NULL){
    printf("Erro na leitura do arquivo\n");
    exit(1);
  }

  fclose(entrada);
  fclose(saida);
  return 0;
}