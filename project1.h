/* iaed-23 - Tiago Santos - ist1106329 - project1 */



/* ---- Constantes ----- */

#define MAX_CARREIRAS 200   /* Numero maximo de carreiras */
#define MAX_PARAGENS 10000  /* Numero maximo de paragens */
#define MAX_LIGACOES 30000  /* Numero maximo de ligacoes */
#define MAX_NOME_CARREIA 21 /* Comprimento maximo do nome das carreiras */
#define MAX_NOME_PARAGEM 51 /* Comprimento maximo do nome das paragens */

#define MAX_ARGS 5       /* Numero maximo de argumentos */
#define MAX_NOME_ARG 51  /* Comprimento maximo do nome dos argumentos */



/* ----- Estados ----- */

/* Define se o char lido pertence ou nao a uma palavra */
#define DENTRO_PALAVRA 0
#define FORA_PALAVRA 1

/* Define se o char lido esta ou nao entre aspas */
#define DENTRO_ASPAS 2
#define FORA_ASPAS 3

/* Define se queremos as paragens da carreira por ordem inversa ou normal */
#define INVERSO 4 
#define NORMAL 5

/* Define se queremos ou nao guardar o nome das carreiras que passam na paragem */
#define GUARDAR_INTERSECOES 6
#define NAO_GUARDAR_INTERSECOES 7



/* ----- Estruturas ----- */

typedef struct{
    double latitude;
    double longitude;
} Localizacao;

typedef struct{
    char nome[MAX_NOME_PARAGEM];
    Localizacao localizacao;
} Paragem;

typedef struct{
    char nome[MAX_NOME_CARREIA];
    int i_origem;  /* Indice da paragem de origem da carreira */
    int i_destino; /* Indice da paragem de destino da carreira */
    int num_paragens_carreira;
    double custo_total;   /* Soma do custo de todas as ligacoes da carreira */
    double duracao_total; /* Soma da duracao de todas as ligacoes da carreira */
} Carreira;

typedef struct{
    int i_carreira; /* Indice da carreira */
    int i_origem;   /* Indice da paragem de origem da ligacao */
    int i_destino;  /* Indice da paragem de destino da ligacao */
    double custo;
    double duracao;
} Ligacao;



/* ----- Prototipos ----- */

void comando_c(char args[MAX_ARGS][MAX_NOME_ARG], int num_args);
void comando_p(char args[MAX_ARGS][MAX_NOME_ARG], int num_args);
void comando_l(char args[MAX_ARGS][MAX_NOME_ARG]);
void comando_i();

int le_linha(char args[MAX_ARGS][MAX_NOME_ARG]);

void cria_carreira(char nome[MAX_NOME_ARG]);
void print_carreiras();
void print_paragens_carreira(char nome[MAX_NOME_ARG], int ordem);
int indice_carreira(char nome[MAX_NOME_ARG]);

void cria_paragem(char args[MAX_ARGS][MAX_NOME_ARG]);
void print_paragens();
void print_localizacao_paragem(char nome[MAX_NOME_ARG]);
int indice_paragem(char nome[MAX_NOME_ARG]);
int obtem_carreiras_intersetam(int paragem, 
                               char carreiras[MAX_CARREIRAS][MAX_NOME_ARG], 
                               int estado);

void cria_ligacao(char args[MAX_ARGS][MAX_NOME_ARG], 
                  int carreira, int origem, int destino);
int erros_l(char args[MAX_ARGS][MAX_NOME_ARG], 
            int carreira, int origem, int destino);

int eh_membro(int x, int lista[MAX_CARREIRAS], int dim);
void quicksort(char a[MAX_CARREIRAS][MAX_NOME_ARG], int left, int right);
int partition(char a[MAX_CARREIRAS][MAX_NOME_ARG], int left, int right);
void troca(char *s1, char *s2);
