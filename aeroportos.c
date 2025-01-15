#include "aeroportoserotas.h"

/** \brief Esta funçao abre o ficheiro "aeroportos.txt", le - o, fecha-o e verifica o seu conteudo.*
 */

LISTA_AEROPORTO *inicializa_aeroportos()
{

    LISTA_AEROPORTO *topo = NULL;
    FILE *fp = fopen("aeroportos.txt", "r");
    if (fp == NULL)
    {
        printf("Erro na abertura do ficheiro aeroportos.txt!\n");
        exit(0);
    }

    verificacao_e_leitura_aeroportos(fp, &topo);

    fclose(fp);
    if (topo == NULL)
    {
        printf("Ficheiro aeroportos.txt vazio ou nenhum aeroporto valido encontrado!\n");
        exit(0);
    }
    aeroportos_repetidos(topo);
    return topo;
}

/** \brief Esta função le cada linha do arquivo "aeroportos.txt", extrai os dados dos aeroportos e verifica - os.
 * De seguida, adiciona-os a uma lista de aeroportos.
 * \param topo = apontador para o topo da lista de aeroportos.
 * \param fp = apontador para o ficheiro "aeroportos.txt".
 */

void verificacao_e_leitura_aeroportos(FILE *fp, LISTA_AEROPORTO **topo)
{
    LISTA_AEROPORTO *ap, *anterior;
    char linha[100];
    char resto[100];
    int a, b, i;

    ap = NULL, anterior = NULL;
    a = 0, b = 0, i = 0;

    while (fgets(linha, 100, fp) != NULL)
    {

        i++;

        if ((verifica_linha_em_branco(linha)) == 0)
        {
            ap = (LISTA_AEROPORTO *)calloc(1, sizeof(LISTA_AEROPORTO));
            if (ap == NULL)
            {
                printf("Erro no processo alocacao de memoria!\n");
                exit(0);
            }
            a = sscanf(linha, "%s %s %d %d %d %c %d %d %d %c %s %d %s", ap->aeroporto.id_icao,
                       ap->aeroporto.id_iata, &(ap->aeroporto.latitude.graus), &(ap->aeroporto.latitude.minutos),
                       &(ap->aeroporto.latitude.segundos), &(ap->aeroporto.latitude.hemisferio), &(ap->aeroporto.longitude.graus),
                       &(ap->aeroporto.longitude.minutos), &(ap->aeroporto.longitude.segundos),
                       &(ap->aeroporto.longitude.hemisferio), ap->aeroporto.cidade, &(ap->aeroporto.fusohorario), resto);
            if (a != 12)
            {
                printf("O numero de elementos na linha %d do ficheiro aeroportos.txt está errado!\n", i);
                free(ap);
                continue;
            }

            b = verifica_dados_aeroporto(ap);

            if (b == 0)
            {
                printf("\n Erro na linha %d do ficheiro aeroportos.txt!\n", i);
                free(ap);
                continue;
            }

            ap->aeroporto.voo = NULL;
            ap->proximo_aero = NULL;

            if (*topo == NULL)
            {
                *topo = ap;
                anterior = ap;
                continue;
            }
            anterior->proximo_aero = ap;
            anterior = ap;
        }
    }
}

/** \brief Esta função aprsenta no ecra as informacoes dos aeroportos presentes na lista de aeroportos.
 * \param ap= apontador utilizado para percorrer a lista de aeroportos.
 */

void apresenta_aeroportos_no_ecra(LISTA_AEROPORTO *ap)
{
    if (ap == NULL)
        printf("Não foram encontrados aeroportos!\n");
    while (ap != NULL)
    {
        printf("%s  %s  %d  %d  %d  %c  %d  %d  %d  %c  %s  %d \n", (ap->aeroporto.id_icao),
               (ap->aeroporto.id_iata), (ap->aeroporto.latitude.graus), (ap->aeroporto.latitude.minutos),
               (ap->aeroporto.latitude.segundos), (ap->aeroporto.latitude.hemisferio), (ap->aeroporto.longitude.graus),
               (ap->aeroporto.longitude.minutos), (ap->aeroporto.longitude.segundos),
               (ap->aeroporto.longitude.hemisferio), (ap->aeroporto.cidade), (ap->aeroporto.fusohorario));
        ap = ap->proximo_aero;
    }
}

/** \brief Esta funcao verifica se os dados de um aeroporto são válidos.
 * \param ap= apontador que aponta para o aeroporto em analise.
 */

int verifica_dados_aeroporto(LISTA_AEROPORTO *ap)
{
    int i;
    if (((ap->aeroporto.latitude.hemisferio != 'S') && (ap->aeroporto.latitude.hemisferio != 'N')) || (ap->aeroporto.latitude.graus > 90) || (ap->aeroporto.latitude.graus < 0) ||
        (ap->aeroporto.latitude.minutos > 59) || (ap->aeroporto.latitude.minutos < 0) || (ap->aeroporto.latitude.segundos > 59) || (ap->aeroporto.latitude.segundos < 0))
        return 0;

    if (((ap->aeroporto.longitude.hemisferio != 'E') && (ap->aeroporto.longitude.hemisferio != 'W')) || (ap->aeroporto.longitude.graus > 180) || (ap->aeroporto.longitude.graus < 0) ||
        (ap->aeroporto.longitude.minutos > 59) || (ap->aeroporto.longitude.minutos < 0) || (ap->aeroporto.longitude.segundos > 59) || (ap->aeroporto.longitude.segundos < 0))
        return 0;
    if ((ap->aeroporto.fusohorario < -10) || (ap->aeroporto.fusohorario > 11))
        return 0;

    for (i = 0; i <= 3; i++)
    {
        if (ap->aeroporto.id_icao[i] < 'A' || ap->aeroporto.id_icao[i] > 'Z')
        {
            return 0;
        }
    }
    for (i = 0; i <= 2; i++)
    {
        if (ap->aeroporto.id_iata[i] < 'A' || ap->aeroporto.id_iata[i] > 'Z')
        {
            return 0;
        }
    }

    if (ap->aeroporto.id_icao[4] != '\0')
        return 0;

    if (ap->aeroporto.id_iata[3] != '\0')
        return 0;

    return 1;
}

/** \brief Esta funcao verifica se ha aeroportos repetidos na lista de aeroportos e, se houver, elimina os duplicados.
 * \param topo= apontador para o topo da lista de aeroportos.
 */

void aeroportos_repetidos(LISTA_AEROPORTO *topo)
{
    LISTA_AEROPORTO *atual, *proximo, *anterior, *temp;
    atual = topo;
    while (atual != NULL && atual->proximo_aero != NULL)
    {
        anterior = atual;
        proximo = atual->proximo_aero;
        while (proximo != NULL)
        {
            if ((strcmp(atual->aeroporto.id_icao, proximo->aeroporto.id_icao) == 0) ||
                (strcmp(atual->aeroporto.id_iata, proximo->aeroporto.id_iata) == 0))
            {

                desaloca_lista_voos(proximo->aeroporto.voo);

                anterior->proximo_aero = proximo->proximo_aero;
                temp = proximo;
                proximo = proximo->proximo_aero;
                free(temp);
                printf("Detetado aeroporto repetido no ficheiro aeroportos.txt. Este aeroporto foi suprimido.\n");
            }
            else
            {
                anterior = proximo;
                proximo = proximo->proximo_aero;
            }
        }
        atual = atual->proximo_aero;
    }
}

/** \brief Esta funcao desaloca a memoria da lista de voos.
 * \param voo= apontador utilizado para percorrer a lista de voos.
 */

void desaloca_lista_voos(LISTA_VOO *voo)
{
    LISTA_VOO *temp;
    while (voo != NULL)
    {
        temp = voo;
        voo = voo->proximo_voo;
        free(temp);
    }
}

/** \brief Esta funcao verifica se ha linhas de texto em branco no ficheiro "aeroportos.txt".
 * \param linha= array utilizado para armazenar uma linha de texto do
 * ficheiro "aeroportos.txt" de cada vez durante a leitura do ficheiro.
 */

int verifica_linha_em_branco(char linha[100])
{
    int i;

    for (i = 0; linha[i] != '\0'; i++)
    {
        if (linha[i] != ' ' && linha[i] != '\n' && linha[i] != '\r' && linha[i] != '\t')
        {

            return 0;
        }
    }

    return 1;
}

/** \brief Esta funcao desaloca a memoria da lista de aeroportos e da lista de voos.
 * \param topo= apontador que aponta para o topo da lista de aeroportos.
 */

void desaloca_memoria(LISTA_AEROPORTO *topo)
{
    LISTA_AEROPORTO *aero_atual, *aero_temporario;
    LISTA_VOO *voo_atual, *voo_temporario;
    aero_atual = topo;
    while (aero_atual != NULL)
    {

        voo_atual = aero_atual->aeroporto.voo;
        while (voo_atual != NULL)
        {
            voo_temporario = voo_atual;
            voo_atual = voo_atual->proximo_voo;
            free(voo_temporario);
            voo_temporario = NULL;
        }

        aero_temporario = aero_atual;
        aero_atual = aero_atual->proximo_aero;
        free(aero_temporario);
        aero_temporario = NULL;
    }
}
