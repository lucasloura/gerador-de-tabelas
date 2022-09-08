#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <math.h>
#include <string.h>

struct TAmplitude
{
	double limitInf, pontoMed, freqRelPor, freqRelDec, freqAcuRel;
	int freqAbs, freqAcu;
};

struct TTotal
{
	int freqAbs;
	double freqRelDec, freqRelPer;
};

void exibirTabela (char *arqSaida, TAmplitude *amp, int sobrInt, int numInt, TTotal tot);

void exibirValores (int numDados, double limInf, double limSup, double ampGeral, int pntMed, int numInt, int ampInt);

void escreverTabela (char *arq, char *arqSaida, TAmplitude *amp, int sobrInt, int numInt, TTotal tot);

void escreverValores (char *arqSaida, int numDados, double limInf, double limSup, double ampGeral, int pntMed, int numInt, int ampInt);

TTotal calcFreqAbsRel(TAmplitude *amp, int numInt);

void filtrarLista (double *lista, TAmplitude *amp, int numDados, int sobreInt, int numInt);

void calcLimsAmpsPntsMeds(TAmplitude *amp, int numInt, int ampInt, double limInf);














int main (void)
{
	setlocale(LC_ALL, "portuguese");

	char arquivo[41], arquivoSaida[49];
	char resp, novoLim;
	bool arqExiste;
	double dado, limInf, limSup, amp, pntMed;
	double *listaDados, totfriPor, totfriDec;
	int i, iAux, sobreInt, numDados, numInt, ampInt, totfi;
	TAmplitude *amplit;
	TTotal total;
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
			printf("Insira o nome do arquivo a ser lido: ");
			scanf(" %40[^\n]", arquivo);
	
			if (Arq = fopen(arquivo, "r"))
				arqExiste = true;
			else if (Arq = fopen(strcat(arquivo, ".txt"), "r"))
				arqExiste = true;
		}


		strcat(arquivoSaida, arquivo);

		fflush(stdin);



		// lendo, contando e armazenado os dados do arquivo dinamicamente, assim como obtendo os valores do limite inferior (li)
		// e do Limite superior (Li), além de já fechar o arquivo de texto


		listaDados = NULL;

		fscanf(Arq, " %lf", & dado);
		numDados = 1;

		listaDados = (double *)realloc(listaDados, sizeof(double) * numDados);
		listaDados[numDados - 1] = dado;

		limInf = dado;
		limSup = dado;

		while (fscanf(Arq, " %lf", & dado) != EOF)
		{
			numDados++;
			listaDados = (double *)realloc(listaDados, sizeof(double) * numDados);
			listaDados[numDados - 1] = dado;

			if (dado < limInf)
				limInf = dado;
			if (dado > limSup)
				limSup = dado;
		}

		fclose(Arq);



		// perguntando ao usuário como as informações devem ser organizadas e devolvidas


		novoLim = 'n';

		printf("\nO valor de Limite Inferior encontrado foi %.2lf\n", limInf);
		printf("Deseja alterar esse valor? [s/N]: ");
		scanf(" %c", & novoLim);

		i = 0;

		if (novoLim == 's' || novoLim == 'S')
		{
			printf("\nInsira o novo Limite Inferior: ");
			scanf(" %lf", & limInf);	

			while (limInf >= limSup)
			{
				printf("Insira o novo Limite Inferior: ");
				scanf(" %lf", & limInf);	
			}

			i++;

			if (i > 5)
				break;
		}

		fflush(stdin);

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

		fflush(stdin);

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

		calcLimsAmpsPntsMeds(amplit, numInt, ampInt, limInf);


		// filtrando e libearndo a lista de dados


		filtrarLista (listaDados, amplit, numDados, sobreInt, numInt);

		listaDados = NULL;

		free(listaDados);


		// calculando os valores relacionados às Frequências Acumulada (Fi) e Acumulada Relativa (Fri)


		total = calcFreqAbsRel(amplit, numInt);


		// exibindo a tabela e os valores obtidos ao usuário


		exibirTabela (arquivoSaida, amplit, sobreInt, numInt, total);
		exibirValores (numDados, limInf, limSup, amp, pntMed, numInt, ampInt);


		// gravando em disco a tabela e os valores obtidos


		escreverTabela (arquivo, arquivoSaida, amplit, sobreInt, numInt, total);
		escreverValores (arquivoSaida, numDados, limInf, limSup, amp, pntMed, numInt, ampInt);



		amplit = NULL;
		free(amplit);

		resp = ' ';

		printf("\n    Deseja gerar outra tabela? [s/n]: ");
		scanf(" %c", & resp);
	}

	return (0);

}

void exibirTabela (char *arqSaida, TAmplitude *amp, int sobrInt, int numInt, TTotal tot)
{
	int i;

	// exibindo ao usuário os valores obtidos (em relação aos intervalos) das variáveis Amplitude (hk), Ponto Médio (xi),
	// Frequência Absoluta (fi), Frequência Relativa (fri, decimal e percentual), Frequência Acumulada (Fi) e Frequência Acumulada Relativa (%) (Fri)

	printf("\n                Arquivo salvo como: %-40s\n", arqSaida);
	printf("_______________________________________________________________________________\n\n");
	printf("         hk            xi       fi        fri        %%        Fi        Fri\n");
	printf("_______________________________________________________________________________\n\n");
	if (sobrInt == 1)
	{
		for (i = 0; i < numInt; i++)
			printf("   %5.1lf |- %5.1lf    %5.1lf     %4d     %5.4lf     %5.2lf     %4d     %6.2lf\n", amp[i].limitInf, amp[i + 1].limitInf, amp[i].pontoMed, amp[i].freqAbs, amp[i].freqRelDec, amp[i].freqRelPor, amp[i].freqAcu, amp[i].freqAcuRel);	
	}
	else
	{
		for (i = 0; i < numInt; i++)
			printf("   %5.1lf -| %5.1lf    %5.1lf     %4d     %5.4lf     %5.2lf     %4d     %6.2lf\n", amp[i].limitInf, amp[i + 1].limitInf, amp[i].pontoMed, amp[i].freqAbs, amp[i].freqRelDec, amp[i].freqRelPor, amp[i].freqAcu, amp[i].freqAcuRel);	
	}
	printf("_______________________________________________________________________________\n\n");
	printf("        Total          --     %4d      %5.4lf    %5.2lf      --        --- \n", tot.freqAbs, tot.freqRelDec, tot.freqRelPer);
	printf("_______________________________________________________________________________\n");
}

void exibirValores (int numDados, double limInf, double limSup, double ampGeral, int pntMed, int numInt, int ampInt)
{
	// exibindo ao usuário os valores obtidos (em relação à lista geral de dados) das variáveis Amplitude (hk), Ponto Médio (xi),
	// Frequência Absoluta (fi), Frequência Relativa (fri, decimal e percentual), Frequência Acumulada (Fi) e Frequência Acumulada Relativa (%) (Fri)

	printf("\n    n   =   %5d   |    Número de Dados\n", numDados);
	printf("    li  =   %5.1lf   |    Limite Inferior\n", limInf);
	printf("    Li  =   %5.1lf   |    Limite Superior\n", limSup);
	printf("    h   =   %5.1lf   |    Amplitude\n", ampGeral);
	printf("    xi  =  %6.2lf   |    Ponto Médio\n", pntMed);
	printf("    k   =   %5d   |    Número de intervalos\n", numInt);
	printf("    hk  =   %5d   |    Amplitude dos intervalos\n\n", ampInt);
}

void escreverTabela (char *arqOrg, char *arqSaida, TAmplitude *amp, int sobrInt, int numInt, TTotal tot)
{
	int i;
	FILE *arq;

	// escrevendo em disco os valores obtidos (em relação aos intervalos) das variáveis Amplitude (hk), Ponto Médio (xi),
	// Frequência Absoluta (fi), Frequência Relativa (fri, decimal e percentual), Frequência Acumulada (Fi) e Frequência Acumulada Relativa (%) (Fri)

	arq = fopen(arqSaida, "w");

	fprintf(arq, "\n                                    %-40s\n", arqOrg);
	fprintf(arq,"_______________________________________________________________________________\n\n");
	fprintf(arq,"         hk            xi       fi        fri        %%        Fi        Fri\n");
	fprintf(arq,"_______________________________________________________________________________\n\n");
	if (sobrInt == 1)
	{
		for (i = 0; i < numInt; i++)
			fprintf(arq,"   %5.1lf |- %5.1lf    %5.1lf     %4d     %5.4lf     %5.2lf     %4d     %6.2lf\n", amp[i].limitInf, amp[i + 1].limitInf, amp[i].pontoMed, amp[i].freqAbs, amp[i].freqRelDec, amp[i].freqRelPor, amp[i].freqAcu, amp[i].freqAcuRel);	
	}
	else
	{
		for (i = 0; i < numInt; i++)
			fprintf(arq,"   %5.1lf -| %5.1lf    %5.1lf     %4d     %5.4lf     %5.2lf     %4d     %6.2lf\n", amp[i].limitInf, amp[i + 1].limitInf, amp[i].pontoMed, amp[i].freqAbs, amp[i].freqRelDec, amp[i].freqRelPor, amp[i].freqAcu, amp[i].freqAcuRel);	
	}
	fprintf(arq,"_______________________________________________________________________________\n\n");
	fprintf(arq,"        Total          --     %4d      %5.4lf    %5.2lf      --        --- \n", tot.freqAbs, tot.freqRelDec, tot.freqRelPer);
	fprintf(arq,"_______________________________________________________________________________\n\n");

	fclose(arq);
}

void escreverValores (char *arqSaida, int numDados, double limInf, double limSup, double ampGeral, int pntMed, int numInt, int ampInt)
{
	FILE *arq;

	// escrevendo em disco os valores obtidos (em relação à lista geral de dados) das variáveis Amplitude (hk), Ponto Médio (xi),
	// Frequência Absoluta (fi), Frequência Relativa (fri, decimal e percentual), Frequência Acumulada (Fi) e Frequência Acumulada Relativa (%) (Fri)

	arq = fopen(arqSaida, "a");

	fprintf(arq,"\n    n   =   %5d   |    Número de Dados\n", numDados);
	fprintf(arq,"    li  =   %5.1lf   |    Limite Inferior\n", limInf);
	fprintf(arq,"    Li  =   %5.1lf   |    Limite Superior\n", limSup);
	fprintf(arq,"    h   =   %5.1lf   |    Amplitude\n", ampGeral);
	fprintf(arq,"    xi  =  %6.2lf   |    Ponto Médio\n", pntMed);
	fprintf(arq,"    k   =   %5d   |    Número de intervalos\n", numInt);
	fprintf(arq,"    hk  =   %5d   |    Amplitude dos intervalos", ampInt);

	fclose(arq);	
}

TTotal calcFreqAbsRel(TAmplitude *amp, int numInt)
{
	int i;
	TTotal tot;

	// calculando os valores relacionados às Frequências Acumulada (Fi) e Acumulada Relativa (Fri), tanto de forma decimal quanto percentual

	tot.freqAbs = 0;
	tot.freqRelDec = 0.0;
	tot.freqRelPer = 0.0;

	amp[0].freqAcu = amp[0].freqAbs;
	amp[0].freqAcuRel = amp[0].freqRelPor;

	for (i = 0; i < numInt; i++)
	{
		tot.freqAbs += amp[i].freqAbs;
		tot.freqRelDec += amp[i].freqRelDec;
		tot.freqRelPer += amp[i].freqRelPor;
		amp[i + 1].freqAcu = amp[i].freqAcu + amp[i + 1].freqAbs;
		amp[i + 1].freqAcuRel = amp[i].freqAcuRel + amp[i + 1].freqRelPor;
	}

	return (tot);
}

void filtrarLista (double *lista, TAmplitude *amp, int numDados, int sobreInt, int numInt)
{
	int i, iAux;

	// filtrando a lista de dados de acordo com a preferência de intervalo do usuário para conseguir os números de ocorrências
	// pertinentes à cada amplitude, assim como também obtendo os valores das Frequências Absoluta (fi) e Relativa (fri, decimal e percentual)

	if (sobreInt == 1)
	{
		for (i = 0; i < numDados; i++)
		{
			for (iAux = 0; iAux < numInt; iAux++)
			{
				if (lista[i] >= amp[iAux].limitInf && lista[i] < amp[iAux + 1].limitInf)
				{
					amp[iAux].freqAbs++;
					amp[iAux].freqRelDec = amp[iAux].freqAbs / double(numDados);
					amp[iAux].freqRelPor = (amp[iAux].freqAbs / double(numDados)) * 100.0;
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
				if (lista[i] > amp[iAux].limitInf && lista[i] <= amp[iAux + 1].limitInf)
				{
					amp[iAux].freqAbs++;
					amp[iAux].freqRelDec = amp[iAux].freqAbs / double(numDados);
					amp[iAux].freqRelPor = (amp[iAux].freqAbs / double(numDados)) * 100.0;
				}
			}
		}
	}
}

void calcLimsAmpsPntsMeds(TAmplitude *amp, int numInt, int ampInt, double limInf)
{
	int i;

	// calculando os valores dos Limites Inferior (li) e Superior (Li), das amplitudes e o Ponto Médio (xi) entre os mesmos

	amp[0].limitInf = floor(limInf);
	amp[0].freqAbs = 0;

	for (i = 0; i < numInt; i++)
	{
		amp[i + 1].limitInf = amp[i].limitInf + ampInt;
		amp[i + 1].freqAbs = 0;
		amp[i].pontoMed = (amp[i + 1].limitInf + amp[i].limitInf) / 2;
	}
}

