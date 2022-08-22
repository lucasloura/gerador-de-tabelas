#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <math.h>
#include <string.h>

struct TAmplitude
{
	double limitInf, pontoMed, freqRel, freqAcuRel;
	int freqAbs, freqAcu;
};

int main (void)
{
	setlocale(LC_ALL, "portuguese");

	char arquivo[41], arquivoSaida[49];
	char resp, novoLim;
	bool arqExiste;
	double dado, limInf, limSup, amp, pntMed;
	double *listaDados, totfri;
	int i, iAux, sobreInt, numDados, numInt, ampInt, totfi;
	TAmplitude *amplit;
	FILE *Arq;



	resp = 's';

	while (resp == 's' || resp == 'S')
	{
		system("cls");

		strcpy(arquivo, "");
		strcpy(arquivoSaida, "tabela ");



		// obtendo o nome e abrindo o arquivo de texto a ser lido


		arqExiste = false;

		while (!arqExiste)
		{
			printf("Insira o nome + a extensão do arquivo a ser lido: ");
			scanf(" %40[^\n]", arquivo);
	
			if (Arq = fopen(arquivo, "r"))
				arqExiste = true;
		}


		strcat(arquivoSaida, arquivo);

		fflush(stdin);



		// lendo, contando e armazenado os dados do arquivo dinamicamente, assim como obtendo os valores do limite inferior (li)
		// e do Limite superior (Li), além de já fechar o arquivo de texto


		numDados = 0;
		listaDados = NULL;

		while (fscanf(Arq, " %lf", & dado) != EOF)
		{
			numDados++;
			listaDados = (double *)realloc(listaDados, sizeof(double) * numDados);
			listaDados[numDados - 1] = dado;

			if (numDados == 1)
			{
				limInf = dado;
				limSup = dado;
			}
			else
			{
				if (dado < limInf)
					limInf = dado;
				if (dado > limSup)
					limSup = dado;
			}
		}

		fclose(Arq);



		// perguntando ao usuário como as informações devem ser organizadas e devolvidas


		novoLim = 'n';

		printf("\nO valor de Limite Inferior encontrado foi %5.2lf\n", limInf);
		printf("Deseja alterar esse valor? [s/N]: ");
		scanf(" %c", & novoLim);

		if (novoLim == 's' || novoLim == 'S')
		{
			printf("\nInsira o novo Limite Inferior: ");
			scanf(" %lf", & limInf);
		}

		printf("\nSobre o intervalo dos valores reais:\n");
		printf("[1] A |- B (Fechado em A e aberto em B)\n");
		printf("[2] A -| B (Aberto em A e fechado em B)\n\n");

		i = 0;

		do
		{
			printf("Insira: ");
			scanf(" %d", & sobreInt);

			i++;

			if (i > 5)
				break;	

		} while (sobreInt != 1 && sobreInt != 2);

		system("cls");



		// calculando os valores de Amplitude (h), Ponto Médio (xi), Número de Intervalos (k) e Amplitude de Intervalos (hk)


		amp = limSup - limInf;
		pntMed = (limInf + limSup) / 2;

		if (numDados <= 25)
			numInt = 5;
		else
			numInt = round(sqrt(numDados));

		ampInt = round((limSup - limInf) / numInt);



		// calculando os valores dos Limites Inferior (li) e Superior (Li) das amplitudes e o Ponto Médio (xi) entre os mesmos


		amplit = NULL;

		amplit = (TAmplitude *)malloc(sizeof(TAmplitude) * numInt + 1);

		amplit[0].limitInf = floor(limInf);
		amplit[0].freqAbs = 0;

		for (i = 0; i < numInt; i++)
		{
			amplit[i + 1].limitInf = amplit[i].limitInf + ampInt;
			amplit[i + 1].freqAbs = 0;
			amplit[i].pontoMed = (amplit[i + 1].limitInf + amplit[i].limitInf) / 2;
		}



		// filtrando a lista de dados de acordo com a preferência de intervalo do usuário para conseguir os números de ocorrências
		// pertinentes à cada amplitude, assim como também obtendo os valores das Frequências Absoluta (fi) e Relativa (fri)


		if (sobreInt == 1)
		{
			for (i = 0; i < numDados; i++)
			{
				for (iAux = 0; iAux < numInt; iAux++)
				{
					if (listaDados[i] >= amplit[iAux].limitInf && listaDados[i] < amplit[iAux + 1].limitInf)
					{
						amplit[iAux].freqAbs++;
						amplit[iAux].freqRel = (amplit[iAux].freqAbs / double(numDados)) * 100.0;
					}
				}
			}
		}
		else
		{
			for (i = 0; i < numDados; i++)
			{
				for (iAux = 0; iAux < numInt; iAux++)
				{
					if (listaDados[i] > amplit[iAux].limitInf && listaDados[i] <= amplit[iAux + 1].limitInf)
					{
						amplit[iAux].freqAbs++;
						amplit[iAux].freqRel = (amplit[iAux].freqAbs / double(numDados)) * 100.0;
					}
				}
			}
		}

		listaDados = NULL;
		free(listaDados);



		// calculando os valores relacionados às Frequências Acumulada (Fi) e Acumulada Relativa (Fri)

		totfi = 0;
		totfri = 0.0;

		amplit[0].freqAcu = amplit[0].freqAbs;
		amplit[0].freqAcuRel = amplit[0].freqRel;

		for (i = 0; i < numInt; i++)
		{
			totfi += amplit[i].freqAbs;
			totfri += amplit[i].freqRel;
			amplit[i + 1].freqAcu = amplit[i].freqAcu + amplit[i + 1].freqAbs;
			amplit[i + 1].freqAcuRel = amplit[i].freqAcuRel + amplit[i + 1].freqRel;
		}



		// exibindo ao usuário os valores obtidos (em relação aos intervalos) das variáveis Amplitude (hk), Ponto Médio (xi),
		// Frequência Absoluta (fi), Frequência Relativa (fri), Frequência Acumulada (Fi) e Frequência Acumulada Relativa (%) (Fri)


		printf("\n    %40s\n", arquivoSaida);
		printf("__________________________________________________________________\n\n");
		printf("         hk            xi       fi      fri       Fi       Fri\n");
		printf("__________________________________________________________________\n\n");
		if (sobreInt == 1)
		{
			for (i = 0; i < numInt; i++)
				printf("   %5.1lf |- %5.1lf    %5.1lf     %4d    %5.2lf    %4d     %6.2lf\n", amplit[i].limitInf, amplit[i + 1].limitInf, amplit[i].pontoMed, amplit[i].freqAbs, amplit[i].freqRel, amplit[i].freqAcu, amplit[i].freqAcuRel);	
		}
		else
		{
			for (i = 0; i < numInt; i++)
				printf("   %5.1lf -| %5.1lf    %5.1lf     %4d    %5.2lf    %4d     %6.2lf\n", amplit[i].limitInf, amplit[i + 1].limitInf, amplit[i].pontoMed, amplit[i].freqAbs, amplit[i].freqRel, amplit[i].freqAcu, amplit[i].freqAcuRel);	
		}
		printf("__________________________________________________________________\n\n");
		printf("       Total           --     %4d    %5.2lf      --       --- \n", totfi, totfri);
		printf("__________________________________________________________________\n");



		// exibindo ao usuário os valores obtidos (em relação à lista geral de dados) das variáveis Amplitude (hk), Ponto Médio (xi),
		// Frequência Absoluta (fi), Frequência Relativa (fri), Frequência Acumulada (Fi) e Frequência Acumulada Relativa (%) (Fri)


		printf("\n   n   =   %5d   |    Número de Dados\n", numDados);
		printf("   li  =   %5.1lf   |    Limite Inferior\n", limInf);
		printf("   Li  =   %5.1lf   |    Limite Superior\n", limSup);
		printf("   h   =   %5.1lf   |    Amplitude\n", amp);
		printf("   xi  =  %6.2lf   |    Ponto Médio\n", pntMed);
		printf("   k   =   %5d   |    Número de intervalos\n", numInt);
		printf("   hk  =   %5d   |    Amplitude dos intervalos\n\n", ampInt);



		// escrevendo em disco os valores obtidos (em relação aos intervalos) das variáveis Amplitude (hk), Ponto Médio (xi),
		// Frequência Absoluta (fi), Frequência Relativa (fri), Frequência Acumulada (Fi) e Frequência Acumulada Relativa (%) (Fri)


		Arq = fopen(arquivoSaida, "w");

		fprintf(Arq, "\n    %49s\n", arquivoSaida);
		fprintf(Arq,"__________________________________________________________________\n\n");
		fprintf(Arq,"         hk            xi       fi      fri       Fi       Fri\n");
		fprintf(Arq,"__________________________________________________________________\n\n");
		if (sobreInt == 1)
		{
			for (i = 0; i < numInt; i++)
				fprintf(Arq,"   %5.1lf |- %5.1lf    %5.1lf     %4d    %5.2lf    %4d     %6.2lf\n", amplit[i].limitInf, amplit[i + 1].limitInf, amplit[i].pontoMed, amplit[i].freqAbs, amplit[i].freqRel, amplit[i].freqAcu, amplit[i].freqAcuRel);	
		}
		else
		{
			for (i = 0; i < numInt; i++)
				fprintf(Arq,"   %5.1lf -| %5.1lf    %5.1lf     %4d    %5.2lf    %4d     %6.2lf\n", amplit[i].limitInf, amplit[i + 1].limitInf, amplit[i].pontoMed, amplit[i].freqAbs, amplit[i].freqRel, amplit[i].freqAcu, amplit[i].freqAcuRel);	
		}
		fprintf(Arq,"__________________________________________________________________\n\n");
		fprintf(Arq,"       Total           --     %4d    %5.2lf      --       --- \n", totfi, totfri);
		fprintf(Arq,"__________________________________________________________________\n");



		// escrevendo em disco os valores obtidos (em relação à lista geral de dados) das variáveis Amplitude (hk), Ponto Médio (xi),
		// Frequência Absoluta (fi), Frequência Relativa (fri), Frequência Acumulada (Fi) e Frequência Acumulada Relativa (%) (Fri)


		fprintf(Arq,"\n   n   =   %5d   |    Número de Dados\n", numDados);
		fprintf(Arq,"   li  =   %5.1lf   |    Limite Inferior\n", limInf);
		fprintf(Arq,"   Li  =   %5.1lf   |    Limite Superior\n", limSup);
		fprintf(Arq,"   h   =   %5.1lf   |    Amplitude\n", amp);
		fprintf(Arq,"   xi  =  %6.2lf   |    Ponto Médio\n", pntMed);
		fprintf(Arq,"   k   =   %5d   |    Número de intervalos\n", numInt);
		fprintf(Arq,"   hk  =   %5d   |    Amplitude dos intervalos", ampInt);

		fclose(Arq);

		amplit = NULL;
		free(amplit);

		resp = ' ';

		printf("   Deseja gerar outra tabela? [s/n]: ");
		scanf(" %c", & resp);
	}

	return (0);

}
