#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <getopt.h>
#include "aco.h"
#include "util.h"

char *nomeArquivo;

instancia_t lerArquivo(char *nome){
	int i, j;
	instancia_t inst;

	FILE* arquivo = fopen(nome, "r");

	if (!arquivo){
		erroSair("ERRO: Nao foi possivel abrir o arquivo.");
	}

	if(fscanf(arquivo, "%d %d", &inst.l, &inst.c) != 2){
		erroSair("ERRO: Nao foi possivel ler numero de linhas ou colunas do arquivo.");
	}

	inst.custo = (int*)(malloc(inst.c * sizeof(int)));
	for (i = 0; i < inst.c; ++i){
		if (fscanf(arquivo, "%d", &inst.custo[i]) != 1){
			erroSair("ERRO: Nao foi possivel ler custos.");
		}
	}

	inst.linha = (int**)(malloc(inst.l * sizeof(int*)));
	inst.ncolunas = (int*)(malloc(inst.l * sizeof(int)));
	inst.nlinhas = (int*)(malloc(inst.c * sizeof(int)));
	memset(inst.nlinhas, 0, inst.c * sizeof(int));

	for (i = 0; i < inst.l; ++i){
		if (fscanf(arquivo, "%d", &inst.ncolunas[i]) != 1){
			erroSair("ERRO: Nao foi possivel ler o numero de colunas.");
		}

		inst.linha[i] = (int*)(malloc(inst.ncolunas[i] * sizeof(int)));
		for (j = 0; j < inst.ncolunas[i]; ++j){
			if (fscanf(arquivo, "%d", &inst.linha[i][j]) != 1){
				erroSair("ERRO: Nao foi possivel ler colunas.");
			}
			inst.linha[i][j]--;
			inst.nlinhas[inst.linha[i][j]]++;   // contando numero de linhas que sao cobertas pela coluna
		}
	}

	inst.coluna = (int**)(malloc(inst.c * sizeof(int*)));
	int cont[inst.c];
	for (i = 0; i < inst.c; i++){
		inst.coluna[i] = (int*)(malloc(inst.nlinhas[i] * sizeof(int)));
		cont[i] = 0;
	}
	
	int k;
	for (i = 0; i < inst.l; i++){
		for (j = 0; j < inst.ncolunas[i]; j++){
			k = inst.linha[i][j];
			inst.coluna[k][cont[k]] = i;
			cont[k]++;
		}
	}

	fclose(arquivo);
	return inst;
}

void mostrarAjuda(){
	//colocar --alfa=ALFA  [default] 
	printf("\n\
	Uso: aco <nome_arquivo.txt> <opcoes>\n\
	-h, --help                  mostra tela de ajuda.\n\
	-a, --alfa=ALFA             seta a variavel alfa do algoritmo.\n\
	-b, --beta=BETA             seta a variavel beta do algoritmo.\n\
	-r, --rho=RHO               seta a variavel rho do algoritmo.\n\
	-f, --formigas=FORMIGAS     seta o numero de formigas do algoritmo.\n\
	-c, --ciclos=CICLOS         seta o numero de ciclos da condicao de parada do algoritmo.\n") ;
	exit(-1);
}

void lerArgumentos(int argc, char *argv[]){
	int opt;

	const struct option opcoes[] = {
		{"help", no_argument, 0, 'h'},
		{"alfa", required_argument, 0, 'a'},
		{"beta", required_argument, 0, 'b'},
		{"rho", required_argument, 0, 'r'},
		{"formigas", required_argument, 0, 'f'},
		{"ciclos", required_argument , 0, 'c'},
		{0, 0, 0, 0},
	};

	while((opt = getopt_long(argc, argv, "ha:b:r:f:c:", opcoes, NULL)) != -1) {
		switch (opt) {
			case 'h':
				mostrarAjuda();
				break;
			case 'a':
				alfa = atof(optarg);
				break;
			case 'b':
				beta = atof(optarg);
				break;
			case 'r':
				rho = atof(optarg);
				break;
			case 'f':
				n_formigas = atoi(optarg);
				break;
			case 'c':
				n_ciclos = atoi(optarg);
				break;
			default:
				mostrarAjuda();
				return;
		}
	}

	if (optind < argc){
		nomeArquivo = argv[optind++];
	}

	/*if (!alfa || !beta || !rho || !n_ciclos || !n_formigas || !nomeArquivo){
		printf("\nERRO: Algum argumento nao foi especificado.\n");
		mostrarAjuda();
	}*/
}

void inicializarParametros(){
	alfa = 1.0;
    beta = 1.0;
    rho = 0.2;
    q0 = 1.0;
    n_formigas = 10;
    n_ciclos = 10;
}

int main(int argc, char *argv[]){
	srand((unsigned)time(NULL));
	/*timeval t1;
	gettimeofday(&t1, NULL);
	srand(t1.tv_usec * t1.tv_sec);*/
	//printf("%lf\n", random_double());
	//printf("%d\n", random_int(10));


	inicializarParametros();
	lerArgumentos(argc, argv);

	char caminho[32] = "../Instancias/";
	strcat(caminho, nomeArquivo);

	instancia_t instancia = lerArquivo(caminho);

	printf("\nDados da instancia:\n");
	printf("Linhas: %d\nColunas: %d\n", instancia.l, instancia.c);

	ant_colony(instancia);
	
	return 0;
}