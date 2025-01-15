#include "aeroportoserotas.h"

/** \brief Funcao que compara dois voos com base no horario de partida e seleciona o que parte mais tarde.
 * \param a= apontador que nos permite ter acesso à estrtura que contem os dados do primeiro voo.
 * \param b= apontador que nos permite ter acesso à estrtura que contem os dados do segundo voo.
 */

LISTA_VOO *compara_partida_decrescente(LISTA_VOO *a, LISTA_VOO *b)
{
    int hora1, hora2;
    hora1 = a->voo.partida_horas;
    hora2 = b->voo.partida_horas;

    if (hora1 > hora2)
    {
        return a;
    }
    else if (hora2 > hora1)
    {
        return b;
    }

    if (a->voo.partida_minutos > b->voo.partida_minutos)
    {
        return a;
    }
    else
    {
        return b;
    }
}

/** \brief Funcao que ordena a lista de voos por ordem decrescente, com base no horario de partida.
 * \param topo= apontador que aponta para o topo da lista de voos.
 */

LISTA_VOO *ordena_rotas_decrescente(LISTA_VOO *topo)
{
    LISTA_VOO *ap1, *maior, *ap2;
    ap1 = NULL, maior = NULL;
    ap2 = topo;

    for (; ap2 != NULL; ap2 = ap2->proximo_voo)
    {
        maior = ap2;
        ap1 = ap2->proximo_voo;
        while (ap1 != NULL)
        {
            maior = compara_partida_decrescente(maior, ap1);
            ap1 = ap1->proximo_voo;
        }
        if (maior != ap2)
            troca_rotas(ap2, maior);
    }
    return topo;
}