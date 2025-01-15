#include "aeroportoserotas.h"

/** \brief Funcao que compara dois voos com base no horario de partida e seleciona o que parte mais cedo.
 * \param a= apontador que nos permite ter acesso à estrutura que contem os dados do primeiro voo.
 * \param b= apontador que nos permite ter acesso à estrutura que contem os dados do segundo voo.
 */

LISTA_VOO *compara_partida_crescente(LISTA_VOO *a, LISTA_VOO *b)

{

    int hora1, hora2;
    hora1 = a->voo.partida_horas;
    hora2 = b->voo.partida_horas;

    if (hora1 < hora2)
    {
        return a;
    }

    else if (hora2 < hora1)
    {
        return b;
    }

    if (a->voo.partida_minutos < b->voo.partida_minutos)
    {
        return a;
    }

    else
    {
        return b;
    }
}

/** \brief Funcao que ordena a lista de voos por ordem crescente, com base no horario de partida.
 * \param topo= apontador que aponta para o topo da lista de voos.
 */

LISTA_VOO *ordena_rotas_crescente(LISTA_VOO *topo)

{
    LISTA_VOO *ap1, *menor, *ap2;
    ap1 = NULL, menor = NULL;
    ap2 = topo;

    for (; ap2 != NULL; ap2 = ap2->proximo_voo)
    {
        menor = ap2;
        ap1 = ap2->proximo_voo;
        while (ap1 != NULL)
        {
            menor = compara_partida_crescente(menor, ap1);
            ap1 = ap1->proximo_voo;
        }

        if (menor != ap2)
            troca_rotas(ap2, menor);
    }

    return topo;
}