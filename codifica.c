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
int verificaFrequencias(unsigned int *frequencias, char *caracteres,FILE *arqTexto);

#endif

struct minHeap {
  char simbolo;
  int num;
  struct minHeap *pEsquerdo, *pDireito;
};
typedef struct minHeap MinHeap;

int main(void) {
  Compactadora vCompactadora[32];
  FILE *entrada = fopen("entrada.txt", "r");
  FILE *saida = fopen("saida.bin", "wb");

  if (entrada == NULL){
    printf("Erro na leitura do arquivo\n");
    exit(1);
  }
  else
    printf("Leitura de arquivo bem-sucedida\n");

  compacta(entrada, saida, vCompactadora);
  
  fclose(entrada);
  fclose(saida);
  return 0;
}

void compacta(FILE *arqTexto, FILE *arqBin, struct compactadora *v){
  char caracteres[32];
  unsigned int frequencias[32];
  
  

  for(int i = 0; i<32; i++)
    {
      caracteres[i] = 0;
      frequencias[i] = 0;
    }
  
  int qtdChar = verificaFrequencias(frequencias, caracteres, arqTexto);

  for(int i = 0; i<qtdChar; i++)
  {
    printf("Caracter: %c Freq: %d\n", caracteres[i], frequencias[i]);
  }

    //Aqui os vetores vão estar inicializados com os valores de frequencias e caracteres
    //Chamar as funcoes de min heap pra criar os Nodos e fazer a arvore
}

int verificaFrequencias(unsigned int *frequencias, char *caracteres,FILE *arqTexto)
{
  int contaChar;
  char simbolo;
  int flag = 0;
  while (!feof(arqTexto))
    {
      simbolo = fgetc(arqTexto);
      for(int j = 0; j<32; j++)
        {
          if(simbolo == caracteres[j])
          {
            frequencias[j]++;
            flag = !flag;
            break;
          }
        }
      if(flag == 0)
      {
        caracteres[contaChar] = simbolo;
        frequencias[contaChar]++;
        contaChar++;
      }

      flag = 0;
    }

  return contaChar;
}
