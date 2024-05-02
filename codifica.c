#include "codifica.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *intPraBin(unsigned int num,
                int size); // Passa de unsigned int pra uma string Binaria
void escreveBitsNoFile(
    const char *bits, int numBits,
    FILE *file); // Pega a sequência de Bits e escreve no arquivo Binario
void lebyte(FILE *file); // Faz o dump da sequência de bits do arquivo binario

void escreveByte(char *byte, FILE *arq) {
  char c = 0x00;
  for (int i = 0; i < 8; i++) {
    c = ((c << 1) | (byte[i] - '0'));
  }
  fwrite(&c, 1, 1, arq);
  byte[0] = '\0';
}

char *completaByte(char *byte, char *resto, int n) {
  for (int i = strlen(byte); i < n && *resto; i++) {
    byte[i] = *resto;
    resto++;
  }

  return resto;
}

// Precisamos resolver o problema de tamanho (bin[100])...
void compacta(FILE *arqTexto, FILE *arqBin, struct compactadora *v) {
  char simbolo;
  char byte[9] = "", *codigoBin;

  while (!feof(arqTexto)) {
    simbolo = fgetc(arqTexto);
    for (int i = 31; i >= 0; i--) {
      if (simbolo == v[i].simbolo) {
        codigoBin = intPraBin(v[i].codigo, v[i].tamanho);
        printf("%s ", codigoBin);
        while ((strlen(byte) + strlen(codigoBin)) > 8 && (strlen(byte) != 8)) {
          codigoBin = completaByte(byte, codigoBin, 8);
          escreveByte(byte, arqBin);
        }
        strcat(byte, codigoBin);
        if (strlen(byte) == 8) {
          escreveByte(byte, arqBin);
        }
        break;
      }
    }
  }
  codigoBin = intPraBin(v[31].codigo, v[31].tamanho);
  while ((strlen(byte) + strlen(codigoBin)) > 8 && (strlen(byte) != 8)) {
    codigoBin = completaByte(byte, codigoBin, 8);
    escreveByte(byte, arqBin);
  }
  if (strlen(codigoBin) <= 8 && strlen(codigoBin) > 0) {
    strcpy(byte, "00000000");
    escreveByte(byte, arqBin);
  }
}
void descompacta(FILE *arqBin, FILE *arqTexto, struct compactadora *v) {
  int maior = 0, menor = v[0].tamanho;
  unsigned int byte;

  rewind(arqBin);

  for (int i = 0; i < 32; i++) {
    if (v[i].tamanho > maior)
      maior = v[i].tamanho;
    if (v[i].tamanho < menor && v[i].tamanho != 0)
      menor = v[i].tamanho;
  }
  printf("maior: %d\n", maior);

  char *bits = (char *)malloc(maior + 1);
  char *buffer = (char *)malloc(maior + 1);
  char *codigoBin;
  while (!feof(arqBin)) {
    printf("a\n");
    fread(&byte, sizeof(unsigned char), 1, arqBin);
    codigoBin = intPraBin(byte, 8);
    printf("Soma de len: %d\n", strlen(bits) + strlen(codigoBin));
    if (strlen(bits) + strlen(codigoBin) >= maior) {
      codigoBin = completaByte(bits, codigoBin, maior);
    } else {
      strcat(bits, codigoBin);
    }
    if (strlen(bits) == 12 && atoi(bits) == 0)
    {
      printf("%s\n", bits);
      printf("Break\n");
      break;
    }
      
    printf("Strlen bits: %ld\n", strlen(bits));
    printf("codigoBin: %s\tbits: %s\n", codigoBin, bits);
    // Analise bits
    for (int i = 0; i < strlen(bits) && strlen(bits) == maior; i++) {
      char bit[2];
      bit[1] = '\0';
      bit[0] = bits[i];
      strcat(buffer, bit);
      if (i >= menor - 1) {
        for (int j = 0; j < 32; j++) {
          if (!strcmp(buffer, intPraBin(v[j].codigo, v[j].tamanho))) {
            printf("Caractere: %c\nCodigo: %s\n", v[j].simbolo, intPraBin(v[j].codigo, v[j].tamanho));
            fputc(v[j].simbolo, arqTexto);
            buffer[0] = '\0';
            break;
          }
        }
      }
    }
    if(strlen(bits) == maior)
    {
        bits[0] = '\0';
        if (strlen(buffer) > 0) {
          printf("Concatena a buffer\n");
          strcpy(bits, buffer);
          buffer[0] = '\0';
        }
        if (strlen(codigoBin) > 0) {
          printf("Concatena a codigoBin\n");
          strcat(bits, codigoBin);
        }
        printf("Bits: %s\n", bits);
        printf("\n");
    }
    
  }
}

void lebyte(FILE *file) {
  printf("Lebyte: \n");
  unsigned char byte;
  while (fread(&byte, sizeof(byte), 1, file)) {
    printf("%02x ", byte);
  }
  putchar('\n');
}
char *intPraBin(unsigned int num, int size) {
  // Tamanho máximo do número binário para um unsigned int é 32 (com 32 bits)
  char *bin = (char *)malloc((size + 1) * sizeof(char));
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
void escreveBitsNoFile(const char *bits, int numBits, FILE *arq) {
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