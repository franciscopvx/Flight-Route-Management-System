#include "aeroportoserotas.h"

/** \brief Esta funcao imprime as informações das rotas associadas a uma determinada escala no ecra.
 * \param ap= apontador que nos permite aceder a estrtura que contem a informacao da escala em questao.
 */

void mostra_rotas_escala(LISTA_ESCALAS *ap)
{
    if (ap == NULL)
    {
        printf("Erro: apontador para a escala é NULL.\n");
        return;
    }

    printf("\nPRIMEIRO VOO:\n");
    apresenta_rotas_no_ecra(&(ap->escala.voo1));
    printf("\nSEGUNDO VOO:\n");
    apresenta_rotas_no_ecra(&(ap->escala.voo2));

    if (ap->escala.qtd_escalas == 2)
    {
        printf("\nTERCEIRO VOO:\n");
        apresenta_rotas_no_ecra(&(ap->escala.voo3));
    }

    printf("\nDISTANCIA TOTAL: %.02f km\n\n", ap->escala.distancia_total);
    printf("-------------------------------------------\n");
}

/** \brief Esta funcao insere uma nova escala na lista de escalas.
 * \param ap= apontador que nos permite aceder a estrtura que contem a informacao da escala em questao.
 * \param topo= apontador que aponta para o topo da lista de escalas.
 */

LISTA_ESCALAS *insere_na_lista(LISTA_ESCALAS *ap, LISTA_ESCALAS *topo)
{
    ap->proxima_escala = topo;
    return ap;
}

/** \brief Esta funcao cria uma nova escala entre voos e insere-a na lista de escalas.
 * \param a= apontador que nos permite ter acesso à estrtura que contem os dados do primeiro voo.
 * \param b= apontador que nos permite ter acesso à estrtura que contem os dados do segundo voo.
 * \param c= apontador que nos permite ter acesso à estrtura que contem os dados do terceiro voo.
 * \param topo= apontador que aponta para o topo da lista de escalas.
 * \param argumento= string que armazena a ordem dada na linha de comando.
 */

LISTA_ESCALAS *coloca_ligacao(LISTA_VOO *a, LISTA_VOO *b, LISTA_VOO *c, LISTA_ESCALAS *topo, char argumento[])
{
    LISTA_ESCALAS *ap;
    int x;
    x = 1;
    ap = (LISTA_ESCALAS *)malloc(sizeof(LISTA_ESCALAS));
    if (ap == NULL)
    {
        printf("Erro na alocacao de memoria!");
        exit(0);
    }
    ap->escala.voo1 = a->voo;
    ap->escala.voo2 = b->voo;
    ap->escala.distancia_total = ap->escala.voo1.distancia_aeroportos + ap->escala.voo2.distancia_aeroportos;
    ap->escala.qtd_escalas = 1;
    if (c != NULL)
    {
        ap->escala.voo3 = c->voo;
        ap->escala.distancia_total += ap->escala.voo3.distancia_aeroportos;
        ap->escala.qtd_escalas = 2;
    }
    if ((strcmp(argumento, "-TC") == 0) || (strcmp(argumento, "-TD") == 0))
        x = testa_horario_entre_escalas(ap);
    if (x == 1)
    {
        topo = insere_na_lista(ap, topo);
        return topo;
    }
    free(ap);
    return topo;
}

/** \brief Esta funcao identifca e adiciona os voos entre dois aeroportos a lista de escalas.
 * \param partida= string que armazena o identificador do aeroporto de partida,
 * \param chegada= string que armazena o identificador do aeroporto de chegada.
 * \param topo= apontador que aponta para o topo da lista de aeroportos.
 * \param argumento= string que armazena a ordem dada na linha de comando.
 * \param distancia= string que indica se as rotas devem ser ordenadas pela menor distancia.
 * \param numero_escalas= variavel que guarda o numero de escalas da rota em questao.
 */

LISTA_ESCALAS *identificar_e_adicionar_escalas(char partida[], char chegada[], LISTA_AEROPORTO *topo, char argumento[], int numero_escalas)
{
    LISTA_AEROPORTO *aeroporto_partida, *aeroporto_chegada, *aeroporto_escala, *aeroporto_escala_final, *aeroap;
    LISTA_ESCALAS *topo_ligacao;
    LISTA_VOO *voo_partida, *voo_escala, *voo_escala_final;

    aeroporto_partida = NULL, aeroporto_chegada = NULL, topo_ligacao = NULL;

    for (aeroap = topo; aeroap; aeroap = aeroap->proximo_aero)
    {
        if (strcmp(aeroap->aeroporto.id_iata, partida) == 0)
        {
            aeroporto_partida = aeroap;
        }
        else if (strcmp(aeroap->aeroporto.id_iata, chegada) == 0)
        {
            aeroporto_chegada = aeroap;
        }
    }

    if (!aeroporto_partida || !aeroporto_chegada)
    {
        printf("Erro: Um dos aeroportos (partida ou chegada) não existe na base de dados.\n");
        return NULL;
    }

    for (voo_partida = aeroporto_partida->aeroporto.voo; voo_partida; voo_partida = voo_partida->proximo_voo)
    {
        for (aeroporto_escala = topo; aeroporto_escala; aeroporto_escala = aeroporto_escala->proximo_aero)
        {
            for (voo_escala = aeroporto_escala->aeroporto.voo; voo_escala; voo_escala = voo_escala->proximo_voo)
            {
                if (strcmp(voo_escala->voo.aeroporto_partida, voo_partida->voo.aeroporto_chegada) == 0)
                {
                    if (numero_escalas == 1 && strcmp(voo_escala->voo.aeroporto_chegada, chegada) == 0)
                    {
                        topo_ligacao = coloca_ligacao(voo_partida, voo_escala, NULL, topo_ligacao, argumento);
                    }
                    else if (numero_escalas == 2)
                    {
                        for (aeroporto_escala_final = topo; aeroporto_escala_final; aeroporto_escala_final = aeroporto_escala_final->proximo_aero)
                        {
                            for (voo_escala_final = aeroporto_escala_final->aeroporto.voo; voo_escala_final; voo_escala_final = voo_escala_final->proximo_voo)
                            {
                                if (strcmp(voo_escala_final->voo.aeroporto_partida, voo_escala->voo.aeroporto_chegada) == 0 && strcmp(voo_escala_final->voo.aeroporto_chegada, chegada) == 0)
                                {
                                    topo_ligacao = coloca_ligacao(voo_partida, voo_escala, voo_escala_final, topo_ligacao, argumento);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return topo_ligacao;
}

/** \brief Esta funcao ordena as escalas e mostra - as no ecra
 * \param topo_ligacao= apontador que aponta para alista de escalas e permite acessa - la.
 * \param argumento= string que armazena "-TC" ou "-TD" caso esteja na linha de comando.
 * \param distancia= string que armaziena "-D" caso esteja na linha de comando.
 */

void ordenar_e_mostrar_escalas(LISTA_ESCALAS *topo_ligacao, char argumento[], char distancia[])
{
    LISTA_ESCALAS *escala_atual;
    double menorDistancia;

    if (strcmp(argumento, "-TC") == 0)
    {
        ordena_escalas_tc(topo_ligacao);
    }
    else if (strcmp(argumento, "-TD") == 0)
    {
        ordena_escalas_td(topo_ligacao);
    }

    if (strcmp(distancia, "-D") == 0 && topo_ligacao)
    {
        ordena_dist(topo_ligacao);
        menorDistancia = topo_ligacao->escala.distancia_total;
        if (topo_ligacao != NULL)
        {
            for (escala_atual = topo_ligacao; escala_atual; escala_atual = escala_atual->proxima_escala)
            {
                if (escala_atual->escala.distancia_total == menorDistancia)
                {
                    mostra_rotas_escala(escala_atual);
                }
                else
                {
                    break;
                }
            }
        }
    }
    else
    {
        for (escala_atual = topo_ligacao; escala_atual; escala_atual = escala_atual->proxima_escala)
        {
            mostra_rotas_escala(escala_atual);
        }
    }

    desaloca_memoria_escalas(topo_ligacao);
}

/** \brief Esta funcao permite juntar as funcoes de identifcar, ordenar e mostras as escalas numa so.
 * \param partida= string que armazena o identificador do aeroporto de partida.
 * \param chegada= string que armazena o identificador do aeroporto de chegada.
 * \param topo= apontador que aponta para o topo da lista de aeroportos.
 * \param argumento= string que armazena a ordem dada na linha de comando.
 * \param distancia= string que indica se as rotas devem ser ordenadas pela menor distancia.
 * \param numero_escalas= variavel que guarda o numero de escalas da rota em questao.
 */

void escalas_de_rotas(char partida[], char chegada[], LISTA_AEROPORTO *topo, char argumento[], char distancia[], int numero_escalas)
{
    LISTA_ESCALAS *topo_ligacao;

    topo_ligacao = identificar_e_adicionar_escalas(partida, chegada, topo, argumento, numero_escalas);

    if (!topo_ligacao)
    {
        printf("Erro: Não foram encontradas rotas compatíveis com os critérios fornecidos.\n");
        return;
    }

    ordenar_e_mostrar_escalas(topo_ligacao, argumento, distancia);
}

void desaloca_memoria_escalas(LISTA_ESCALAS *topo)
{
    LISTA_ESCALAS *topoap;
    while (topo != NULL)
    {
        topoap = topo->proxima_escala;
        free(topo);
        topo = topoap;
    }
}
