#include "aeroportoserotas.h"

/** \brief Esta funcao troca os dados entre dois voos na lista de voos.
 * \param a= apontador que nos permite ter acesso à estrutura que contem os dados do primeiro voo.
 * \param b= apontador que nos permite ter acesso à estrutura que contem os dados do segundo voo.
 */

void troca_rotas(LISTA_VOO *a, LISTA_VOO *b)
{
    INFOVOO aux;
    aux = a->voo;
    a->voo = b->voo;
    b->voo = aux;
}

/** \brief adiciona uma nova rota direta a uma lista de rotas diretas.
 * \param lista_rotas_diretas= ponteiro que aponta para um ponteiro que aponta para o inicio da lista de rotas diretas.
 * \param rota_a_adicionar= ponteiro que aponta para a rota que sera adicionada à lista.
 */

void adiciona_a_rota_direta(LISTA_VOO **lista_rotas_diretas, LISTA_VOO *rota_a_adicionar)
{

    LISTA_VOO *nova_rota, *aux;
    nova_rota = (LISTA_VOO *)malloc(sizeof(LISTA_VOO));
    if (!nova_rota)
    {
        printf("Erro ao alocar memória para a nova rota direta!\n");
        return;
    }

    nova_rota->voo = rota_a_adicionar->voo;
    nova_rota->proximo_voo = NULL;

    if (*lista_rotas_diretas == NULL)
    {
        *lista_rotas_diretas = nova_rota;
    }
    else
    {
        aux = *lista_rotas_diretas;
        while (aux->proximo_voo != NULL)
        {
            aux = aux->proximo_voo;
        }
        aux->proximo_voo = nova_rota;
    }
}

/** \brief Esta funcao procura os voos que serao posteriormente adicionados a lista de rotas diretas.
 * \param id_iata_partida= array que guarda o iata do aeroporto de partida.
 * \param id_iata_chegada= array que guarda o iata do aeroporto de chegada.
 * \param topo= apontador que aponta para o topo da lista de aeroportos.
 * \param argumento= string que armazena a ordem dada na linha de comando.
 */

void insere_rotas_diretas(char id_iata_partida[], char id_iata_chegada[], LISTA_AEROPORTO *topo, LISTA_VOO **rotas_diretas)
{
    LISTA_AEROPORTO *topoaux;
    LISTA_VOO *aux;

    topoaux = topo;
    while (topoaux != NULL)
    {
        if (strcmp(id_iata_partida, topoaux->aeroporto.id_iata) == 0)
        {
            aux = topoaux->aeroporto.voo;
            while (aux != NULL)
            {
                if (strcmp(id_iata_chegada, aux->voo.aeroporto_chegada) == 0)
                {
                    adiciona_a_rota_direta(rotas_diretas, aux);
                }
                aux = aux->proximo_voo;
            }
        }
        topoaux = topoaux->proximo_aero;
    }
}

/** \brief Esta funcao ordena as rotas depenendo da linha de comando do utilizador e mostra -as no ecra.
 * \param argumento= array que guarda "-TC" ou "-TD" caso esteja na linha de comando do utilizador.
 * \param rotas_diretas= apontador que aponta para a lista de voos e motra - os ordenados no ecra.
 */

void mostra_e_ordena_rotas_diretas(char argumento[], LISTA_VOO *rotas_diretas)
{
    LISTA_VOO *atual;

    if (strcmp(argumento, "-TC") == 0)
    {
        rotas_diretas = ordena_rotas_crescente(rotas_diretas);
    }
    else if (strcmp(argumento, "-TD") == 0)
    {
        rotas_diretas = ordena_rotas_decrescente(rotas_diretas);
    }

    atual = rotas_diretas;
    while (atual != NULL)
    {
        apresenta_rotas_no_ecra(&atual->voo);
        atual = atual->proximo_voo;
    }

    liberta_memoria(rotas_diretas);
}

/** \brief Funcao que verifica os parametros de uma rota direta.
 * \param id_iata_partida= array que guarda o iata do aeroporto de partida.
 * \param id_iata_chegada= array que guarda o iata do aeroporto de chegada.
 * \param topo= apontador que aponta para o topo da lista de aeroportos.
 * \param argumento= string que armazena a ordem dada na linha de comando.
 */

void processa_rota_direta(char id_iata_partida[], char id_iata_chegada[], LISTA_AEROPORTO *topo, char argumento[])
{
    LISTA_AEROPORTO *topoaux;
    LISTA_VOO *rotas_diretas;
    int partida_encontrada, chegada_encontrada;

    if (strcmp(id_iata_partida, id_iata_chegada) == 0)
    {
        printf("Erro: Aeroportos de partida e chegada iguais!\n");
        return;
    }

    partida_encontrada = 0, chegada_encontrada = 0;
    rotas_diretas = NULL;

    topoaux = topo;
    while (topoaux != NULL)
    {
        if (strcmp(id_iata_partida, topoaux->aeroporto.id_iata) == 0)
        {
            partida_encontrada = 1;
        }
        if (strcmp(id_iata_chegada, topoaux->aeroporto.id_iata) == 0)
        {
            chegada_encontrada = 1;
        }
        topoaux = topoaux->proximo_aero;
    }

    if (!partida_encontrada || !chegada_encontrada)
    {
        if (!partida_encontrada)
        {
            printf("O aeroporto de partida nao existe na base de dados.\n");
        }
        if (!chegada_encontrada)
        {
            printf("O aeroporto de chegada nao existe na base de dados.\n");
        }
        return;
    }

    insere_rotas_diretas(id_iata_partida, id_iata_chegada, topo, &rotas_diretas);

    if (rotas_diretas == NULL)
    {
        printf("Não foi encontrada correspondencia para a rota pretendida\n");
        return;
    }

    mostra_e_ordena_rotas_diretas(argumento, rotas_diretas);
}

/** \brief Funcao que liberta a memoria alocada para a lista de voos.
 * \param topo= apontador que aponta para o topo da lista de voos.
 */

void liberta_memoria(LISTA_VOO *topo)
{
    LISTA_VOO *aux;
    while (topo != NULL)
    {
        aux = topo->proximo_voo;
        free(topo);
        topo = aux;
    }
}
