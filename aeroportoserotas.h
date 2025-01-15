#ifndef AEROPORTOSEROTAS_H
#define AEROPORTOSEROTAS_H
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#define M_PI 3.14159
#define MAX_ICAO 5
#define MAX_IATA 4
#define MAX_CODIGO 7
#define MAX_CIDADE 30
#define AEROPORTOS 30
#define MAX_COMPANHIA 30
#define R1 6381

/*Estrutura que guarda as coordenadas de um determinado aeroporto.*/
typedef struct coordenadas
{
    int graus, minutos, segundos;
    char hemisferio;
} COORDENADAS;

/*Estrutura que guarda todas as informacoes de um determinado voo.*/
typedef struct infovoo
{
    char codigo_voo[MAX_CODIGO], aeroporto_partida[MAX_IATA], aeroporto_chegada[MAX_IATA];
    char companhia_aerea[MAX_COMPANHIA];
    int partida_minutos, partida_horas, chegada_minutos, chegada_horas;
    double distancia_aeroportos;
} INFOVOO;

/*Estrutura que guarda todas as informacoes dos voos, bem como um ponteiro que percorre a lista de voos.*/
typedef struct lista_voo
{
    INFOVOO voo;
    struct lista_voo *proximo_voo;
} LISTA_VOO;

/*Estrutura que guarda as informacoes de um determinado aeroporto, bem como as informacoes de todos os voos desse aeroporto.*/
typedef struct infoaero
{
    char id_icao[MAX_ICAO], id_iata[MAX_IATA], cidade[MAX_CIDADE];
    COORDENADAS latitude, longitude;
    int fusohorario;
    LISTA_VOO *voo;
} INFOAERO;

/*Estrutura que guarda todas as informacoes dos aeroportos, bem como um ponteiro que percorre a lista de aeroportos.*/
typedef struct lista_aeroportos
{
    INFOAERO aeroporto;
    struct lista_aeroportos *proximo_aero;
} LISTA_AEROPORTO;

/*Estrutura que guarda todas as informacoes de ate tres voos, constituintes de uma rota,
 bem como a quantidade de escalas e a distancia total da rota.*/
typedef struct escala_voos
{
    INFOVOO voo1;
    INFOVOO voo2;
    INFOVOO voo3;
    int qtd_escalas;
    double distancia_total;
} ESCALA;

/*Estrutura que guarda todas as informacoes das escalas, bem como um ponteiro que percorre a lista de escalas.*/
typedef struct lista_escalas
{
    ESCALA escala;
    struct lista_escalas *proxima_escala;
} LISTA_ESCALAS;

/* FUNCOES FICHEIRO AEROPORTOS.C */

LISTA_AEROPORTO *inicializa_aeroportos();
void verificacao_e_leitura_aeroportos(FILE *fp, LISTA_AEROPORTO **topo);
void apresenta_aeroportos_no_ecra(LISTA_AEROPORTO *ap);
int verifica_dados_aeroporto(LISTA_AEROPORTO *ap);
void aeroportos_repetidos(LISTA_AEROPORTO *topo);
void desaloca_lista_voos(LISTA_VOO *voo);
int verifica_linha_em_branco(char linha[100]);
void desaloca_memoria(LISTA_AEROPORTO *topo);

/* FUNCOES FICHEIRO ROTAS.C */

double calcula_angulo_latitude(LISTA_AEROPORTO *aero);
double calcula_angulo_longitude(LISTA_AEROPORTO *aero);
void determina_coordenadas(double *coord_x, double *coord_y, double *coord_z, LISTA_AEROPORTO *aero);
double calcula_angulo(LISTA_AEROPORTO *partida, LISTA_AEROPORTO *chegada);
double calcula_distancia(char partidac[], char chegadac[], LISTA_AEROPORTO *topo);
int valida_dados_voo(const LISTA_VOO *rota);
int ajusta_hora(int horas, int timezone);
int verifica_horario(INFOVOO ap, int fusohorario1, int fusohorario2);
int coloca_voo_na_lista(LISTA_AEROPORTO *topo, LISTA_VOO *rota);
int verifica_existencia_aeroporto(LISTA_AEROPORTO *topo, char *identificador, char aero_nao_encontrado[100][6], int *aero_nao_encontrado_contador);
void inicializa_voos(LISTA_AEROPORTO *topo);
LISTA_VOO *processa_linha_voo(char *linha, LISTA_AEROPORTO *topo, char *companhia_aerea, char aero_nao_encontrado[100][6], int *aero_nao_encontrado_contador);
void verificacao_e_leitura_voos(FILE *fp, LISTA_AEROPORTO *topo, int *rotas_inseridas);
void verifica_rotas(LISTA_AEROPORTO *topo);
void imprime_rotas(LISTA_AEROPORTO *topo);
void apresenta_rotas_no_ecra(INFOVOO *rota);

/* FUNCOES FICHEIRO ROTASDIRETAS.C */

void troca_rotas(LISTA_VOO *a, LISTA_VOO *b);
void adiciona_a_rota_direta(LISTA_VOO **lista_rotas_diretas, LISTA_VOO *rota_a_adicionar);
void insere_rotas_diretas(char id_iata_partida[], char id_iata_chegada[], LISTA_AEROPORTO *topo, LISTA_VOO **rotas_diretas);
void mostra_e_ordena_rotas_diretas(char argumento[], LISTA_VOO *rotas_diretas);
void processa_rota_direta(char id_iata_partida[], char id_iata_chegada[], LISTA_AEROPORTO *topo, char argumento[]);
void liberta_memoria(LISTA_VOO *topo);

/* FUNCOES FICHEIRO ROTASDIRETASCRESCENTE.C */

LISTA_VOO *compara_partida_crescente(LISTA_VOO *a, LISTA_VOO *b);
LISTA_VOO *ordena_rotas_crescente(LISTA_VOO *topo);

/* FUNCOES FICHEIRO ROTASDIRETASDESCRESCENTE.C */

LISTA_VOO *compara_partida_decrescente(LISTA_VOO *a, LISTA_VOO *b);
LISTA_VOO *ordena_rotas_decrescente(LISTA_VOO *topo);

/* FUNCOES FICHEIRO ORDENACAOESCALAS.C */

int testa_horario_entre_escalas(LISTA_ESCALAS *ap);
LISTA_ESCALAS *escolhe_menor_dist(LISTA_ESCALAS *a, LISTA_ESCALAS *b);
void troca_escalas(LISTA_ESCALAS *a, LISTA_ESCALAS *b);
LISTA_ESCALAS *ordena_dist(LISTA_ESCALAS *topo);
LISTA_ESCALAS *analise_escalas_tc(LISTA_ESCALAS *a, LISTA_ESCALAS *b);
LISTA_ESCALAS *ordena_escalas_tc(LISTA_ESCALAS *topo);
LISTA_ESCALAS *analise_escalas_td(LISTA_ESCALAS *a, LISTA_ESCALAS *b);
LISTA_ESCALAS *ordena_escalas_td(LISTA_ESCALAS *topo);

/* FUNCOES FICHEIRO ROTASCOMESCALA.C */

void mostra_rotas_escala(LISTA_ESCALAS *ap);
LISTA_ESCALAS *insere_na_lista(LISTA_ESCALAS *ap, LISTA_ESCALAS *topo);
LISTA_ESCALAS *coloca_ligacao(LISTA_VOO *a, LISTA_VOO *b, LISTA_VOO *c, LISTA_ESCALAS *topo, char argumento[]);
LISTA_ESCALAS *identificar_e_adicionar_escalas(char partida[], char chegada[], LISTA_AEROPORTO *topo, char argumento[], int numero_escalas);
void ordenar_e_mostrar_escalas(LISTA_ESCALAS *topo_ligacao, char argumento[], char distancia[]);
void escalas_de_rotas(char partida[], char chegada[], LISTA_AEROPORTO *topo, char argumento[], char distancia[], int numero_escalas);
void desaloca_memoria_escalas(LISTA_ESCALAS *topo);

#endif