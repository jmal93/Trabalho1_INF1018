#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
  Compactadora vCompactadora[32] = {
      {'K', 1, 12}, {'W', 2, 12}, {'Y', 3, 12}, {'Z', 2, 11}, {'J', 3, 11},
      {'X', 1, 9},  {'\n', 1, 8}, {'H', 1, 7},  {'F', 36, 7}, {'G', 37, 7},
      {'B', 1, 6},  {'Q', 16, 6}, {',', 17, 6}, {'V', 19, 6}, {'.', 49, 6},
      {'P', 48, 6}, {'D', 1, 5},  {'C', 17, 5}, {'O', 16, 5}, {'M', 25, 5},
      {'R', 1, 4},  {'L', 7, 4},  {'N', 6, 4},  {'A', 5, 4},  {'T', 9, 4},
      {'S', 14, 4}, {'U', 15, 4}, {'I', 13, 4}, {'E', 1, 3},  {' ', 5, 3},
      {0, 0, 0},    {4, 0, 12}};
  FILE *entrada = fopen("entrada.txt", "r");
  FILE *saida = fopen("saida.bin", "wb");

  if (entrada == NULL) {
    printf("Erro na leitura do arquivo\n");
    exit(1);
  } else
    printf("Leitura de arquivo bem-sucedida\n");

  compacta(entrada, saida, vCompactadora);

  printf("Fim do codigo\n");
  fclose(entrada);
  fclose(saida);
  return 0;
}

char *intPraBin(unsigned int num, int size) {
    // Tamanho máximo do número binário para um unsigned int é 32 (com 32 bits)
    char *bin = (char *)malloc((size+1) * sizeof(char));
    if (bin == NULL) {
        fprintf(stderr, "Erro de alocação de memória\n");
        exit(1);
    }
    bin[size] = '\0'; // Terminate a string

    // Loop para converter o número para binário
    for (int i = size; i >= 0; --i) {
        bin[i] = (num & 1) ? '1' : '0'; // Extrai o bit menos significativo
        num >>= 1; // Desloca para a direita para verificar o próximo bit
    }

    return bin;
}

void descompacta(FILE *arqBin, FILE *arqTexto, struct compactadora *v)
{
  
}

void writeBitsToFile(const char *bits, int numBits, FILE *file) { //Traduzir a func dps, peguei o GPT
    int numBytes = numBits / 8;
    int remainder = numBits % 8;
    int i, j;
    unsigned char byte = 0;

    for (i = 0; i < numBytes; i++) {
        byte = 0;
        for (j = 0; j < 8; j++) {
            byte |= (bits[i * 8 + j] - '0') << (7 - j);
        }
        fwrite(&byte, sizeof(unsigned char), 1, file);
    }

    if (remainder > 0) {
        byte = 0;
        for (i = 0; i < remainder; i++) {
            byte |= (bits[numBytes * 8 + i] - '0') << (7 - i);
        }
        fwrite(&byte, sizeof(unsigned char), 1, file);
    }
}

void compacta(FILE *arqTexto, FILE *arqBin, struct compactadora *v) {
  char simbolo;
  unsigned int codigo;
  char bin[100] = "";
  
  
  while (!feof(arqTexto)) {
    simbolo = fgetc(arqTexto);
    for (int i = 0; i < 32; i++) {
      if (simbolo == v[i].simbolo) {
        char* codigoBin = intPraBin(v[i].codigo, v[i].tamanho);;
        printf("%s\n", codigoBin);
        strcat(bin, codigoBin);
        break;
      }
    }
  }

  char* codigoBin = intPraBin(v[31].codigo, v[31].tamanho);;
  printf("%s\n", codigoBin);
  strcat(bin, codigoBin);
  printf("%s\n", bin);

  writeBitsToFile(bin, strlen(bin), arqBin);
}
