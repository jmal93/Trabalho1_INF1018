/*NOME: Vagner Messias da Costa Junior MATRÍCULA: 2112851 TURMA: 3WA*/
/*NOME: João Marcello Amaral Lima MATRÍCULA: 2010580 TURMA: 3WA*/

#include "codifica.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *intPraBin(unsigned int num, int size);		// Passa de unsigned int pra uma string Binaria
void escreveByte(char *byte, FILE *arq);			// Escreve o codigo binário em byte no arquivo binário
char *completaByte(char *byte, char *resto, int n); // Completa uma variável byte com os bits de outra e guarda o resto

void compacta(FILE *arqTexto, FILE *arqBin, struct compactadora *v)
{
	char simbolo;				   // O caractere atual do arquivo texto
	char byte[9] = "", *codigoBin; // O código binário do byte a ser escrito e o codigo binário do caractere lido

	while (!feof(arqTexto))
	{
		simbolo = fgetc(arqTexto);
		// Procura o caractere na tabela
		for (int i = 31; i >= 0; i--)
		{
			if (simbolo == v[i].simbolo)
			{
				codigoBin = intPraBin(v[i].codigo, v[i].tamanho);
				// Caso codigoBin não caiba totalmente em byte
				while ((strlen(byte) + strlen(codigoBin)) > 8 && (strlen(byte) != 8))
				{
					codigoBin = completaByte(byte, codigoBin, 8);
					escreveByte(byte, arqBin);
				}
				strcat(byte, codigoBin);
				if (strlen(byte) == 8)
				{
					escreveByte(byte, arqBin);
				}
				break;
			}
		}
	}
	// Insere o EOT no final do arquivo binário
	codigoBin = intPraBin(v[31].codigo, v[31].tamanho);
	while ((strlen(byte) + strlen(codigoBin)) > 8 && (strlen(byte) != 8))
	{
		codigoBin = completaByte(byte, codigoBin, 8);
		escreveByte(byte, arqBin);
	}
	if (strlen(codigoBin) <= 8 && strlen(codigoBin) > 0)
	{
		escreveByte(byte, arqBin);
	}
}
void descompacta(FILE *arqBin, FILE *arqTexto, struct compactadora *v)
{
	int maior = 0, menor = v[0].tamanho; // Maiores e menores tamanhos da tabela
	unsigned int byte;					 // Byte lido atual

	rewind(arqBin);

	// Pega os maiores e menores tamanhos da tabela
	for (int i = 0; i < 32; i++)
	{
		if (v[i].tamanho > maior)
			maior = v[i].tamanho;
		if (v[i].tamanho < menor && v[i].tamanho != 0)
			menor = v[i].tamanho;
	}

	// Aloca dois array de char com tamanho do maior
	char *bits = (char *)malloc(maior + 1);
	char *buffer = (char *)malloc(maior + 1);
	char *codigoBin;
	while (!feof(arqBin))
	{
		fread(&byte, sizeof(unsigned char), 1, arqBin);
		// codigoBin recebe o código binário do byte lido como uma string
		codigoBin = intPraBin(byte, 8);
		// Caso codigoBin não caiba totalmente em bits
		if (strlen(bits) + strlen(codigoBin) >= maior)
		{
			codigoBin = completaByte(bits, codigoBin, maior);
		}
		// Caso contrário, concatena codigoBin em bits
		else
		{
			strcat(bits, codigoBin);
		}
		// Caso bits seja EOT, finaliza leitura do arquivo
		if (strlen(bits) == 12 && atoi(bits) == 0)
		{
			break;
		}

		// Analise bits
		for (int i = 0; i < strlen(bits) && strlen(bits) == maior; i++)
		{
			char bit[2];
			bit[1] = '\0';
			bit[0] = bits[i];
			strcat(buffer, bit);
			// Não analisa os bits caso o tamanho seja menor que o menor tamanho da tabela
			if (i >= menor - 1)
			{
				// Busca o caractere correspondente aos bits
				for (int j = 0; j < 32; j++)
				{
					// Caso ache, escreve no arquivo texto de saída e zera a buffer
					if (!strcmp(buffer, intPraBin(v[j].codigo, v[j].tamanho)))
					{
						fputc(v[j].simbolo, arqTexto);
						buffer[0] = '\0';
						break;
					}
				}
			}
		}
		// Zera a bits caso ela esteja totalmente preenchida (depois de achar os caracteres),
		// e concatena o resto (se tiver) em bits
		if (strlen(bits) == maior)
		{
			bits[0] = '\0';
			if (strlen(buffer) > 0)
			{
				strcpy(bits, buffer);
				buffer[0] = '\0';
			}
			if (strlen(codigoBin) > 0)
			{
				strcat(bits, codigoBin);
			}
		}
	}
}

char *intPraBin(unsigned int num, int size)
{
	// Tamanho máximo do número binário para um unsigned int é 32 (com 32 bits)
	char *bin = (char *)malloc((size + 1) * sizeof(char));
	if (bin == NULL)
	{
		exit(1);
	}
	bin[size] = '\0'; // Zera a string

	// Loop para converter o número para binário
	for (int i = size - 1; i >= 0; --i)
	{
		bin[i] = (num & 1) ? '1' : '0'; // Extrai o bit menos significativo
		num >>= 1;						// Desloca para a direita para verificar o próximo bit
	}

	return bin;
}

void escreveByte(char *byte, FILE *arq)
{
	char c = 0x00;
	for (int i = 0; i < 8; i++)
	{
		c = ((c << 1) | (byte[i] - '0'));
	}
	fwrite(&c, 1, 1, arq);
	byte[0] = '\0';
}

char *completaByte(char *byte, char *resto, int n)
{
	for (int i = strlen(byte); i < n && *resto; i++)
	{
		byte[i] = *resto;
		resto++;
	}

	return resto;
}