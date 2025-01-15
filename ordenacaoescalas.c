#include "aeroportoserotas.h"

/** \brief Esta funcao verifica se os horários de partida e chegada entre as escalas de voos são compativeis.
 * \param ap= apontador que permite ter acesso à lista que contem todas as escalas.
 */

int testa_horario_entre_escalas(LISTA_ESCALAS *ap)
{
    if (ap == NULL)
    {
        return -1;
    }

    if (ap->escala.voo2.partida_horas < ap->escala.voo1.chegada_horas ||
        (ap->escala.voo2.partida_horas == ap->escala.voo1.chegada_horas &&
         ap->escala.voo2.partida_minutos <= ap->escala.voo1.chegada_minutos))
    {
        return 0;
    }

    if (ap->escala.qtd_escalas == 2 &&
        (ap->escala.voo3.partida_horas < ap->escala.voo2.chegada_horas ||
         (ap->escala.voo3.partida_horas == ap->escala.voo2.chegada_horas &&
          ap->escala.voo3.partida_minutos <= ap->escala.voo2.chegada_minutos)))
    {
        return 0;
    }

    return 1;
}

/** \brief Esta funcao compara duas rotas com escala e escolhe a que tiver menor distancia total.
 * \param a= apontador para a primeira estrutura LISTA_ESCALAS que será comparada.
 * \param b= apontador para a segunda estrutura LISTA_ESCALAS que será comparada.
 */

LISTA_ESCALAS *escolhe_menor_dist(LISTA_ESCALAS *a, LISTA_ESCALAS *b)
{
    if (a == NULL)
        return b;

    if (b == NULL)
        return a;

    if (a->escala.distancia_total < b->escala.distancia_total)
    {
        return a;
    }

    else
    {
        return b;
    }
}

/** \brief Esta funcao troca os dados de duas escalas.
 * \param a= apontador que aponta para uma das escalas.
 * \param b= apontador que aponta para a outra escala.
 */

void troca_escalas(LISTA_ESCALAS *a, LISTA_ESCALAS *b)
{
    ESCALA ap;
    if (a == NULL || b == NULL)
    {
        return;
    }

    ap = a->escala;
    a->escala = b->escala;
    b->escala = ap;
}

/** \brief Esta funcao ordena a lista de escalas com base na sua distancia total (de forma crescente).
 * \param topo= apontador que aponta para o topo da lista de escalas.
 */

LISTA_ESCALAS *ordena_dist(LISTA_ESCALAS *topo)
{
    LISTA_ESCALAS *atual, *menor_dist, *proximo;
    atual = topo;
    while (atual != NULL)

    {
        menor_dist = atual;
        proximo = atual->proxima_escala;
        while (proximo != NULL)
        {
            if (escolhe_menor_dist(menor_dist, proximo) == proximo)
            {
                menor_dist = proximo;
            }

            proximo = proximo->proxima_escala;
        }

        if (menor_dist != atual)
        {
            troca_escalas(atual, menor_dist);
        }

        atual = atual->proxima_escala;
    }

    return topo;
}

/** \brief Esta funcao compara duas escalas com base no horário de partida do primeiro voo associado a cada escala e seleciona a que parte mais cedo.
 * \param a= apontador que aponta para uma das escalas.
 * \param b= apontador que aponta para a outra escala.
 */

LISTA_ESCALAS *analise_escalas_tc(LISTA_ESCALAS *a, LISTA_ESCALAS *b)
{
    if (a->escala.voo1.partida_horas < b->escala.voo1.partida_horas)
    {
        return a;
    }

    if ((a->escala.voo1.partida_horas == b->escala.voo1.partida_horas) && (a->escala.voo1.partida_minutos <= b->escala.voo1.partida_minutos))
    {
        return a;
    }

    return b;
}

/** \brief Esta funcao ordena, de forma crescente, a lista de escalas com base no horário de partida do primeiro voo associado a cada escala.
 * \param topo= apontador que aponta para o topo da lista de escalas.
 */

LISTA_ESCALAS *ordena_escalas_tc(LISTA_ESCALAS *topo)
{
    LISTA_ESCALAS *atual, *tc, *prox;
    atual = topo;

    while (atual != NULL)
    {
        tc = atual;
        prox = atual->proxima_escala;
        while (prox != NULL)

        {
            if (analise_escalas_tc(tc, prox) == prox)
            {
                tc = prox;
            }
            prox = prox->proxima_escala;
        }

        if (tc != atual)
        {
            troca_escalas(atual, tc);
        }

        atual = atual->proxima_escala;
    }

    return topo;
}

/** \brief Esta funcao compara duas escalas com base no horário de partida do primeiro voo associado a cada escala e seleciona a que parte mais tarde.
 * \param a= apontador que aponta para uma das escalas.
 * \param b= apontador que aponta para a outra escala.
 */

LISTA_ESCALAS *analise_escalas_td(LISTA_ESCALAS *a, LISTA_ESCALAS *b)
{
    if (a->escala.voo1.partida_horas > b->escala.voo1.partida_horas)
    {
        return a;
    }

    if ((a->escala.voo1.partida_horas == b->escala.voo1.partida_horas) && (a->escala.voo1.partida_minutos >= b->escala.voo1.partida_minutos))
    {
        return a;
    }

    return b;
}

/** \brief Esta funcao ordena, de forma decrescente, a lista de escalas com base no horário de partida do primeiro voo associado a cada escala.
 * \param topo= apontador que aponta para o topo da lista de escalas.
 */

LISTA_ESCALAS *ordena_escalas_td(LISTA_ESCALAS *topo)
{

    LISTA_ESCALAS *atual, *td, *prox;
    atual = topo;

    while (atual != NULL)
    {
        td = atual;
        prox = atual->proxima_escala;
        while (prox != NULL)
        {
            if (analise_escalas_td(td, prox) == prox)
            {
                td = prox;
            }

            prox = prox->proxima_escala;
        }

        if (td != atual)
        {
            troca_escalas(atual, td);
        }

        atual = atual->proxima_escala;
    }

    return topo;
}
