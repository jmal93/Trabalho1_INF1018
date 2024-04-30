#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codifica.h"

char* intPraBin(unsigned int num, int size); //Passa de unsigned int pra uma string Binaria
void escreveBitsNoFile(const char* bits, int numBits, FILE* file); //Pega a sequência de Bits e escreve no arquivo Binario
void lebyte(FILE* file); // Faz o dump da sequência de bits do arquivo binario

// Precisamos resolver o problema de tamanho (bin[100])...
void compacta(FILE* arqTexto, FILE* arqBin, struct compactadora* v) {
    char simbolo;
    unsigned int codigo;
    char byte[16], resto[8];
  
    while (!feof(arqTexto)) {
        simbolo = fgetc(arqTexto);
        for (int i = 31; i >= 0; i--) {
            if (simbolo == v[i].simbolo) {
                char* codigoBin = intPraBin(v[i].codigo, v[i].tamanho);;
				strcat(byte, codigoBin);
				if (strlen(byte) >= 8)
				{
					// escreve no arquivo bin
					
				}
                break;
            }
        }
    }

}
void descompacta(FILE* arqBin, FILE* arqTexto, struct compactadora* v){
  unsigned int byte;
  char bits[10000] = "";
  rewind(arqBin);
  while (!feof(arqBin)) {
      fread(&byte, sizeof(unsigned char), 1, arqBin);
      char *codigoBin = intPraBin(byte, 8);
      strcat(bits, codigoBin);
      if (byte == 0)
          break;
  }

  char buffer[10000] = "";
  for (int i = 0; i < strlen(bits); i++){
          char bit = bits[i];
          strcat(buffer, &bit);
          for (int j = 0; j < 32; j++){
              if (!strcmp(buffer, intPraBin(v[j].codigo, v[j].tamanho))){
                  fputc(v[j].simbolo, arqTexto);
                  strcpy(buffer, "\0");
                  break;
              }
          }
      }
}


void lebyte(FILE* file) { 
    printf("Lebyte: \n");
    unsigned char byte;
    while (fread(&byte, sizeof(byte), 1, file)) {
        printf("%02x ", byte);
    }
    putchar('\n');
}
char* intPraBin(unsigned int num, int size) {
  // Tamanho máximo do número binário para um unsigned int é 32 (com 32 bits)
  char* bin = (char*)malloc((size + 1) * sizeof(char));
  if (bin == NULL) {
      fprintf(stderr, "Erro de alocação de memória\n");
      exit(1);
  }
  bin[size] = '\0'; // Terminate a string

  // Loop para converter o número para binário
  for (int i = size - 1; i >= 0; --i) {
      bin[i] = (num & 1) ? '1' : '0'; // Extrai o bit menos significativo
      num >>= 1; // Desloca para a direita para verificar o próximo bit
  }

  return bin;
}
void escreveBitsNoFile(const char* bits, int numBits, FILE* arq) {
  int numBytes = numBits / 8;
  int resto = numBits % 8;
  int i, j;
  unsigned char byte = 0;

  for (i = 0; i < numBytes; i++) {
      byte = 0;
      for (j = 0; j < 8; j++) {
          byte |= (bits[i * 8 + j] - '0') << (7 - j);
      }
      fwrite(&byte, sizeof(unsigned char), 1, arq);
  }

  if (resto > 0) {
      byte = 0;
      for (i = 0; i < resto; i++) {
          byte |= (bits[numBytes * 8 + i] - '0') << (7 - i);
      }
      fwrite(&byte, sizeof(unsigned char), 1, arq);
  }
}