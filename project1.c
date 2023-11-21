/* iaed-23 - Tiago Santos - ist1106329 - project1 */

#include "project1.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>



/* ----- Variaveis Globais ----- */

/* Arrays com todas as carreiras, paragens e ligacoes do sistema */
Carreira g_lista_carreiras[MAX_CARREIRAS];
Paragem g_lista_paragens[MAX_PARAGENS];
Ligacao g_lista_ligacoes[MAX_LIGACOES];
/* Numero de carreiras, paragens e ligacoes do sistema */
int g_num_carreiras;
int g_num_paragens;
int g_num_ligacoes;



/* ----- Funcao Main ----- */

/* Le input do user linha a linha e executa os comandos correspondentes 
 */
int main(void){
    char comando;
    /* Vetor com todos os argumentos de um comando */
    char args[MAX_ARGS][MAX_NOME_ARG] = {0};
    int i;
    
    /* Le o primeiro char de cada linha até ler o comando q */
    while((comando = getchar()) != 'q' && comando != EOF){
        /* Le o resto da linha ate ao \n */
        int num_args = le_linha(args);
        
        /* Chama a funcao correspondente ao comando */
        switch(comando){
            case 'c':
                comando_c(args, num_args);
                break;
            case 'p':
                comando_p(args, num_args);
                break;
            case 'l':
                comando_l(args);
                break;
            case 'i':
                comando_i();
                break;
        }
        /* Reiniciaiza os argumentos */
        for(i=0; i < num_args; i++)
            args[i][0] = '\0';
    }

    return 0;
}



/* ----- Funcoes dos comandos ----- */

/* Executa o comando c - output depende do numero de argumentos
 * @param args --> vetor com os argumentos de um comando
 * @param num_args --> numero de argumentos do vetor args
 */
void comando_c(char args[MAX_ARGS][MAX_NOME_ARG], int num_args){
    int len;

    switch(num_args){
        case 0:
            print_carreiras();
            break;
        case 1:
            /* Se a carreira nao existir */
            if((indice_carreira(args[0]) == -1)) 
                cria_carreira(args[0]);
            else
                /* lista todas as paragens da carreira pela ordem normal */
                print_paragens_carreira(args[0], NORMAL);
            break;
        case 2:
            len = strlen(args[1]);
            /* Se args[1] for alguma das abreviacoes validas de "inverso" */
            if(len >= 3 && strncmp(args[1], "inverso", len) == 0)
                /* lista todas as paragens da carreira pela ordem inversa */
                print_paragens_carreira(args[0], INVERSO);
            else
                printf("incorrect sort option.\n"); /* ERRO */
            break;
    }
}


/* Executa o comando p - output depende do numero de argumentos
 * @param args --> vetor com os argumentos de um comando
 * @param num_args --> numero de argumentos do vetor args
 */
void comando_p(char args[MAX_ARGS][MAX_NOME_ARG], int num_args){ 
    switch(num_args){
        case 0:
            print_paragens();
            break;
        case 1:
            /* Se a paragem nao existir */
            if(indice_paragem(args[0]) == -1)
                printf("%s: no such stop.\n", args[0]); /* ERRO */
            else 
                print_localizacao_paragem(args[0]);
            break;
        case 3:
            /* Se a paragem nao existir */
            if(indice_paragem(args[0]) == -1)
                cria_paragem(args);
            else
                printf("%s: stop already exists.\n", args[0]); /* ERRO */
            break;
    }
}


/* Executa o comando l - verifica possiveis erros e cria ligacao
 * @param args --> vetor com os argumentos de um comando
 */
void comando_l(char args[MAX_ARGS][MAX_NOME_ARG]){
    int carreira, paragem_origem, paragem_destino;

    carreira = indice_carreira(args[0]);
    paragem_origem = indice_paragem(args[1]);
    paragem_destino = indice_paragem(args[2]);

    /* Verificacao de erros */
    if(erros_l(args, carreira, paragem_origem, paragem_destino) == 0){
        /* Cria a primeira ligacao */
        if(g_lista_carreiras[carreira].num_paragens_carreira == 0){
            /* Atualiza paragem de origem e destino da carreira */
            g_lista_carreiras[carreira].i_origem = paragem_origem;
            g_lista_carreiras[carreira].i_destino = paragem_destino;
            /* A funcao cria_ligacao adiciona 1 ao numero de paragens da carreira
            logo é necessario incrementar mais 1 para a primeira ligacao */
            g_lista_carreiras[carreira].num_paragens_carreira++;
            
            cria_ligacao(args, carreira, paragem_origem, paragem_destino);
        }
        /* Cria uma ligacao que forma uma carreira circular */
        else if(g_lista_carreiras[carreira].i_origem == paragem_destino &&
                g_lista_carreiras[carreira].i_destino == paragem_origem){
            /* Atualiza a paragem de destino da carreira */
            g_lista_carreiras[carreira].i_destino = paragem_destino;

            cria_ligacao(args, carreira, paragem_origem, paragem_destino);
        }
        /* Cria uma ligacao no inicio da carreira */
        else if(g_lista_carreiras[carreira].i_origem == paragem_destino){
            /* Atualiza a paragem de origem da carreira */
            g_lista_carreiras[carreira].i_origem = paragem_origem;

            cria_ligacao(args, carreira, paragem_origem, paragem_destino);
        }
        /* Cria uma ligacao no fim da carreira */
        else if(g_lista_carreiras[carreira].i_destino == paragem_origem){
            /* Atualiza a paragem de destino da carreira */
            g_lista_carreiras[carreira].i_destino = paragem_destino;

            cria_ligacao(args, carreira, paragem_origem, paragem_destino);
        }
        else
            printf("link cannot be associated with bus line.\n"); /* ERRO */
    }
}


/* Executa o comando i - print de todas as paragens que pertencem a mais que
 * uma carreira e todas as carreiras que nelas passam por ordem alfabetica
 */
void comando_i(){
    int p, i, num_carreiras;
    /* Vetor com todas as carreiras que passam numa certa paragem */
    char vetor_carreiras[MAX_CARREIRAS][MAX_NOME_ARG];

    for(p=0; p < g_num_paragens; p++){
        /* Obtem as carreiras que passam na paragem p */
        num_carreiras = obtem_carreiras_intersetam(p, vetor_carreiras,
                                                   GUARDAR_INTERSECOES);
        
        if(num_carreiras > 1){
            printf("%s %d:", g_lista_paragens[p].nome, num_carreiras);

            quicksort(vetor_carreiras, 0, num_carreiras -1);

            for(i=0; i < num_carreiras; i++)
                printf(" %s", vetor_carreiras[i]);
            printf("\n");
        }
    }
}



/* ----- Funcoes auxiliares ----- */

/* Le uma linha do input e guarda todas as palavras no vetor args
 * @param args --> vetor onde serao guardados os argumentos de um comando
 * @return num_args --> numero de argumentos do vetor args
 */
int le_linha(char args[MAX_ARGS][MAX_NOME_ARG]){
    char c;
    int estado_palavra = FORA_PALAVRA, estado_aspas = FORA_ASPAS;
    int i=0, num_args=0;

    /* Le caracter a caracter ate ao \n */
    while((c = getchar()) != '\n' && c != EOF){

        if(c == '"' && estado_aspas == FORA_ASPAS) /* Primeiras aspas */
            estado_aspas = DENTRO_ASPAS;

        else if(c == '"' && estado_aspas == DENTRO_ASPAS) /* Segundas aspas */
            estado_aspas = FORA_ASPAS;

        else if(c == ' ' && estado_aspas == FORA_ASPAS){
            if(estado_palavra == DENTRO_PALAVRA){ /* Fim de uma palavra */
                estado_palavra = FORA_PALAVRA;
                args[num_args-1][i] = '\0';
                i = 0;
            } 
        }
        else{
            if(estado_palavra == FORA_PALAVRA){ /* Inicio de uma palavra */
                num_args++;
                estado_palavra = DENTRO_PALAVRA;
            }
            args[num_args-1][i++] = c;
        }
    }
    args[num_args-1][i] = '\0';
    
    return num_args;
}


/* Cria uma carreira e inicializa todas os seus parametros
 * @param nome --> nome da carreira
 */
void cria_carreira(char nome[MAX_NOME_ARG]){
    strcpy(g_lista_carreiras[g_num_carreiras].nome, nome);

    g_lista_carreiras[g_num_carreiras].num_paragens_carreira = 0;
    g_lista_carreiras[g_num_carreiras].custo_total = 0;
    g_lista_carreiras[g_num_carreiras].duracao_total = 0;
    
    g_num_carreiras++;
}



/* Lista todas as carreiras do sistema e alguns dos seus dados 
 */
void print_carreiras(){
    int i;

    for(i=0; i < g_num_carreiras; i++){
        if(g_lista_carreiras[i].num_paragens_carreira == 0)
            printf("%s 0 %.2f %.2f\n", g_lista_carreiras[i].nome,
                            g_lista_carreiras[i].custo_total,
                            g_lista_carreiras[i].duracao_total);
        else
            printf("%s %s %s %d %.2f %.2f\n", g_lista_carreiras[i].nome,
                            g_lista_paragens[g_lista_carreiras[i].i_origem].nome,
                            g_lista_paragens[g_lista_carreiras[i].i_destino].nome,
                            g_lista_carreiras[i].num_paragens_carreira,
                            g_lista_carreiras[i].custo_total,
                            g_lista_carreiras[i].duracao_total);
    }
}


/* Lista todas as paragens de uma carreira pela ordem pedida 
 * @param nome --> nome da carreira
 * @param ordem --> ordem pela qual queremos listar as paragens. Pode ter como
 *      valores NORMAL ou INVERSO 
 */
void print_paragens_carreira(char nome[MAX_NOME_ARG], int ordem){
    int carreira, n=0, j, paragem_atual;
    
    carreira = indice_carreira(nome);

    if(ordem == INVERSO) /* Comecar na paragem de destino da carreira */
        paragem_atual = g_lista_carreiras[carreira].i_destino;
    else /* Comecar na paragem de origem da carreira */
        paragem_atual = g_lista_carreiras[carreira].i_origem;

    /* Ciclar pelas ligacoes varias vezes até ter listado todas as 
    paragens da carreira -1 */
    while(n < g_lista_carreiras[carreira].num_paragens_carreira - 1){
        for(j=0; j < g_num_ligacoes; j++){

            if(ordem == INVERSO &&
                    g_lista_ligacoes[j].i_carreira == carreira &&
                    g_lista_ligacoes[j].i_destino == paragem_atual){
                /* Listar paragem de destino da ligacao */
                printf("%s, ", g_lista_paragens[paragem_atual].nome);
                /* Atualizar a paragem_atual para a paragem de origem */
                paragem_atual = g_lista_ligacoes[j].i_origem;
                n++;
            }
            else if(ordem == NORMAL &&
                    g_lista_ligacoes[j].i_carreira == carreira &&
                    g_lista_ligacoes[j].i_origem == paragem_atual){
                /* Listar paragem de origem da ligacao */
                printf("%s, ", g_lista_paragens[paragem_atual].nome);
                /* Atualizar a paragem_atual para a paragem de destino */
                paragem_atual = g_lista_ligacoes[j].i_destino;
                n++;
            }
        }
            
    }   
    /* Listar a ultima paragem */
    if(g_lista_carreiras[carreira].num_paragens_carreira != 0)
        printf("%s\n", g_lista_paragens[paragem_atual].nome);
}



/* Obtem o indice da carreira com o nome dado
 * @param nome --> nome da carreira
 * @return --> indice da carreira ou -1 se nao existir
 */
int indice_carreira(char nome[MAX_NOME_ARG]){
    int i;

    for(i=0; i < g_num_carreiras; i++)
        if(strcmp(g_lista_carreiras[i].nome, nome) == 0)
            return i;
    return -1;
}


/* Cria uma paragem e inicializa todas os seus parametros
 * @param args --> vetor com os dados da paragem
 */
void cria_paragem(char args[MAX_ARGS][MAX_NOME_ARG]){
    strcpy(g_lista_paragens[g_num_paragens].nome, args[0]);
    
    g_lista_paragens[g_num_paragens].localizacao.latitude = strtod(args[1], NULL);
    g_lista_paragens[g_num_paragens].localizacao.longitude = strtod(args[2], NULL);

    g_num_paragens++;
}


/* Lista todas as paragens do sistema e alguns dos seus dados 
 */
void print_paragens(){
    int p, num_carreiras;
    /* Vetor temporario. Usado apenas para poder chamar a funcao 
    obtem_intersecoes_paragem */
    char temp[MAX_CARREIRAS][MAX_NOME_ARG];

    for(p=0; p < g_num_paragens; p++){
        /* Obtem o numero de carreiras que passam na paragem p */
        num_carreiras = obtem_carreiras_intersetam(p, temp, NAO_GUARDAR_INTERSECOES);

        printf("%s: %16.12f %16.12f %d\n", g_lista_paragens[p].nome,
                                        g_lista_paragens[p].localizacao.latitude,
                                        g_lista_paragens[p].localizacao.longitude,
                                        num_carreiras); 
    }
}


/* Obtem o indice da paragem com o nome dado
 * @param nome --> nome da paragem
 * @return --> indice da paragem ou -1 se nao existir
 */
int indice_paragem(char nome[MAX_NOME_ARG]){
    int i;

    for(i=0; i < g_num_paragens; i++){
        if(strcmp(g_lista_paragens[i].nome, nome) == 0)
            return i;
    }
    return -1;
}


/* Escreve a latitude e longitude* da paragem 
 * @param nome --> nome da paragem
 */
void print_localizacao_paragem(char nome[MAX_NOME_ARG]){
    int paragem = indice_paragem(nome);
    
    printf("%16.12f %16.12f\n", g_lista_paragens[paragem].localizacao.latitude,
                                g_lista_paragens[paragem].localizacao.longitude);
}


/* Obtem as carreiras que passam na paragem e guarda-as num vetor se o estado
 * passado como argumento for igual a GUARDAR_INTERSECOES, senao apenas conta o
 * numero de carreiras que passam pela paragem
 * @param paragem --> indice da paragem
 * @param carreiras -->  vetor onde serao guardadas as carreiras que passam
 *      pela paragem
 * @param estado --> define se a funcao deve guardar as carreiras no vetor ou
 *      nao. Pode ter como valores GURADAR_INTERSECOES ou NAO_GUARDAR_INTERSECOES
 * @return --> numero de carreiras que passam na paragem
 */
int obtem_carreiras_intersetam(int paragem, 
                               char carreiras[MAX_CARREIRAS][MAX_NOME_ARG],
                               int estado){
    int i, num_carreiras=0;
    /* Vetor que guarda os indices das carreiras que passam na paragem para
       que seja mais facil verificar se uma carreira ja foi contada ou nao */
    int indices_carreiras[MAX_CARREIRAS];
    
    for(i=0; i < g_num_ligacoes; i++){
        /* Se a paragem for a origem ou destino da ligacao atual e a carreira
           dessa ligacao ainda nao tiver sido adicionada ao indices_carreira */
        if((g_lista_ligacoes[i].i_destino == paragem || 
            g_lista_ligacoes[i].i_origem == paragem) &&
            !(eh_membro(g_lista_ligacoes[i].i_carreira, 
                        indices_carreiras, num_carreiras))){
            /* Adiciona o nome da carreira ao vetor carreiras */
            if(estado == GUARDAR_INTERSECOES)
                strcpy(carreiras[num_carreiras], 
                        g_lista_carreiras[g_lista_ligacoes[i].i_carreira].nome);
            /* Adiciona o indice da carreira ao vetor indices_carreira */
            indices_carreiras[num_carreiras] = g_lista_ligacoes[i].i_carreira;
            
            num_carreiras++;
        }
    }

    return num_carreiras;
}


/* Cria a ligacao atualizando todos os parametros necessarios quer da ligacao
 * como da carreira
 * @param args --> vetor com os argumentos de um comando
 * @param carreira --> indice da carreira
 * @param origem --> indice da paragem de origem da ligacao
 * @param destino --> indice da paragem de destino da ligacao
 */
void cria_ligacao(char args[MAX_ARGS][MAX_NOME_ARG], 
                  int carreira, int origem, int destino){
    
    double custo = strtod(args[3], NULL);
    double duracao = strtod(args[4], NULL);
    /* Altera custo e duracao total da carreira */
    g_lista_carreiras[carreira].custo_total += custo;
    g_lista_carreiras[carreira].duracao_total += duracao;

    /* Incrementa o numero de paragens da carreira */
    g_lista_carreiras[carreira].num_paragens_carreira++;

    /* Cria ligacao atualizando todos os parametros */
    g_lista_ligacoes[g_num_ligacoes].i_carreira = carreira;
    g_lista_ligacoes[g_num_ligacoes].i_origem = origem;
    g_lista_ligacoes[g_num_ligacoes].i_destino = destino;
    g_lista_ligacoes[g_num_ligacoes].custo = custo;
    g_lista_ligacoes[g_num_ligacoes].duracao = duracao;

    g_num_ligacoes++;
}


/* Verifica se os dados da criacao da ligacao sao validos
 * @param args --> vetor com os argumentos de um comando
 * @param carreira --> indice da carreira
 * @param origem --> indice da paragem de origem da ligacao
 * @param destino --> indice da paragem de destino da ligacao
 * @return --> 0 se nao houver erros e 1 caso contrario
 */
int erros_l(char args[MAX_ARGS][MAX_NOME_ARG], 
            int carreira, int origem, int destino){
    /* Se carreira nao existir */
    if(carreira == -1){          
        printf("%s: no such line.\n", args[0]); /* ERRO */
        return 1;
    }
    /* Se paragem origem nao existir */
    else if(origem == -1){
        printf("%s: no such stop.\n", args[1]); /* ERRO */
        return 1;
    }
    /* Se paragem destino nao existir */
    else if(destino == -1){
        printf("%s: no such stop.\n", args[2]); /* ERRO */
        return 1;
    }
    /* Se custo ou duracao forem negativos */
    else if((args[3][0] == '-' || args[4][0] == '-')){
        printf("negative cost or duration.\n"); /* ERRO */
        return 1;
    }
    return 0;
}


/* Ve se x e membro da lista dada 
 * @param x --> inteiro que queremos verificar
 * @param lista --> lista de inteiros
 * @param dim --> dimensao da lista
 * @return --> 1 se x pertencer a lista e 0 se nao pertencer 
 */
int eh_membro(int x, int lista[MAX_CARREIRAS], int dim){
    int i;

    for(i=0; i < dim; i++){
        if(x == lista[i]){
            return 1;
        }
    }
    return 0;
}


/* Ordena um vetor de strings por ordem alfabetica 
 * @param a --> vetor de strings
 * @param left --> indice do elemento mais a esquerda
 * @param right --> indice do elemento mais a direita 
 */
void quicksort(char a[MAX_CARREIRAS][MAX_NOME_ARG], int left, int right){
    int i;

    if(right <= left)
        return;

    i = partition(a, left, right);

    quicksort(a, left, i-1);
    quicksort(a, i+1, right);
}


/* Ordena um vetor de strings em torno de um elemento (o pivot) colocando os
   elementos menores que ele a sua esquerda e os maiores a sua direita 
 * @param a --> vetor de strings
 * @param left --> indice do elemento mais a esquerda
 * @param right --> indice do elemento mais a direita 
 */
int partition(char a[MAX_CARREIRAS][MAX_NOME_ARG], int left, int right){
    int i = left-1;
    int j = right;

    /* Declarar o pivot como a string mais a direita */
    char pivot[MAX_NOME_ARG];
    strcpy(pivot, a[right]);

    /* Com dois contadores comecando em lados opostos do vetor ir trocando
    elementos que estejam na posicao errada ate se encontrarem*/
    while(i < j){
        while(strcmp(a[++i], pivot) < 0);
        while(strcmp(pivot, a[--j]) < 0)
            if(j == left)
                break;
        if(i < j)
            troca(a[i], a[j]);
    }
    /* Trocar o pivot com o elemento onde os contadores se encontraram*/
    troca(a[i], a[right]);
    
    return i;
}


/* Troca o conteudo de duas strings uma com a outra
 * @param s1 --> ponteiro para uma das strings que queremos trocar
 * @param s2 --> ponteiro para a outra string
 */
void troca(char *s1, char *s2){
    char temp[MAX_NOME_ARG];
    strcpy(temp, s1);
    strcpy(s1, s2);
    strcpy(s2, temp);
}

