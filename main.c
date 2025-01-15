/*O trabalho desenvolvido tem como finalidade o calculo de rotas aéreas entre um aeroporto de partida e um aeroporto de chegada.
Trabalho realizado por Ema Alexandra Zeferino (106313) e Francisco Valerio (106533).*/

#include "aeroportoserotas.h"

int main(int argc, char *argv[])
{
    LISTA_AEROPORTO *topo;
    int tc, td, d, l, dois, um, zero, i;
    tc = 7, td = 7, d = 7, l = 7, dois = 7, um = 7, zero = 7, i = 2;

    topo = inicializa_aeroportos();
    inicializa_voos(topo);
    if ((argc == 2) && (strcmp("-aeroportos", argv[1]) == 0)) /*Leitura da linha de comando para serem mostrados todos os aeroportos.*/
        apresenta_aeroportos_no_ecra(topo);
    else if ((argc == 2) && (strcmp("-voos", argv[1]) == 0)) /*Leitura da linha de comando para serem mostradas todas as rotas.*/
        imprime_rotas(topo);
    else if (argc > 2)
    {
        for (i = 2; i < argc; i++) /*Leitura das indicacoes dadas na linha de comando.*/
        {
            if (strcmp(argv[i], "-TC") == 0)
                tc = i;
            if (strcmp(argv[i], "-TD") == 0)
                td = i;
            if (strcmp(argv[i], "-D") == 0)
                d = i;
            if (strcmp(argv[i], "-L") == 0)
                l = i;
            if (strcmp(argv[i], "0") == 0)
                zero = i;
            if (strcmp(argv[i], "1") == 0)
                um = i;
            if (strcmp(argv[i], "2") == 0)
                dois = i;
        }

        /*Le linhas de comando com indicacao de rotas diretas.*/

        if ((argc == 5) && (l < 5) && (zero < 5))
            processa_rota_direta(argv[1], argv[2], topo, argv[zero]);
        else if ((argc == 6) && (l < 6) && (zero < 6) && (tc < 6))
            processa_rota_direta(argv[1], argv[2], topo, argv[tc]);
        else if ((argc == 6) && (l < 6) && (zero < 6) && (td < 6))
            processa_rota_direta(argv[1], argv[2], topo, argv[td]);

        /*Le linhas de comando com indicacao de rotas com uma escala e sem preocupacao horaria.*/
        else if ((argc == 5) && (l < 5) && (um < 5))
            escalas_de_rotas(argv[1], argv[2], topo, "", "", atoi(argv[4]));

        /*Le linhas de comando com indicacao de rotas com uma escala e preocupacao horaria.*/
        else if ((argc == 6) && (l < 6) && (um < 6) && (tc < 6))
            escalas_de_rotas(argv[1], argv[2], topo, argv[tc], "", atoi(argv[4]));
        else if ((argc == 6) && (l < 6) && (um < 6) && (td < 6))
            escalas_de_rotas(argv[1], argv[2], topo, argv[td], "", atoi(argv[4]));

        /*Le linhas de comando com indicacao de rotas com duas escalas.*/
        else if ((argc == 5) && (l < 5) && (dois < 5))
            escalas_de_rotas(argv[1], argv[2], topo, "", "", atoi(argv[4]));
        else if ((argc == 6) && (l < 6) && (dois < 6) && (tc < 6))
            escalas_de_rotas(argv[1], argv[2], topo, argv[tc], "", atoi(argv[4]));
        else if ((argc == 6) && (l < 6) && (dois < 6) && (td < 6))
            escalas_de_rotas(argv[1], argv[2], topo, argv[td], "", atoi(argv[4]));
        else if ((argc == 7) && (l < 7) && ((dois < 7) || (um < 7)) && (tc < 7) && (d < 7))
            escalas_de_rotas(argv[1], argv[2], topo, argv[tc], argv[d], atoi(argv[4]));
        else
            printf("\nOs argumentos passados são inválidos.\n");
    }
    else
    {
        printf("\nArgumentos insuficientes.\n");
    }
    desaloca_memoria(topo); /*limpeza da memoria alocada.*/
    return 0;
}