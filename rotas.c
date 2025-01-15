#include "aeroportoserotas.h"

/** \brief Esta funcao calcula o angulo de latitude de um aeroporto em radianos.
 * \param aero= apontador que aponta para o aeroporto cujo angulo de latitude esta a ser calculado.
 */

double calcula_angulo_latitude(LISTA_AEROPORTO *aero)
{
    double graus_minutos, graus_segundos, graus_total, radianos_total;

    if (aero == NULL)
    {
        printf("Erro: Coordenadas de latitude do aeroporto não encontradas!\n");
        return -1;
    }

    graus_minutos = aero->aeroporto.latitude.minutos / 60.0;
    graus_segundos = aero->aeroporto.latitude.segundos / 3600.0;
    graus_total = aero->aeroporto.latitude.graus + graus_minutos + graus_segundos;
    radianos_total = graus_total * M_PI / 180.0;

    if (aero->aeroporto.latitude.hemisferio == 'S')
    {
        radianos_total *= -1;
    }

    return radianos_total;
}

/** \brief Esta funcao calcula o angulo de longitude de um aeroporto em radianos.
 * \param aero= apontador que aponta para o aeroporto cujo angulo de longitude esta a ser calculado.
 */

double calcula_angulo_longitude(LISTA_AEROPORTO *aero)
{
    double graus_minutos, graus_segundos, graus_total, radianos_total;

    if (aero == NULL)
    {
        printf("Erro: Coordenadas de longitude do aeroporto não encontradas!\n");
        return -1;
    }

    graus_minutos = aero->aeroporto.longitude.minutos / 60.0;
    graus_segundos = aero->aeroporto.longitude.segundos / 3600.0;
    graus_total = aero->aeroporto.longitude.graus + graus_minutos + graus_segundos;
    radianos_total = graus_total * M_PI / 180.0;

    if (aero->aeroporto.longitude.hemisferio == 'W')
    {
        radianos_total *= -1;
    }

    return radianos_total;
}

/** \brief Esta funcao transforma as coordenadas geografica (latitude e longitude) de um aeroporto nas suas coordenadas (x, y, z).
 * \param coord_x= apontador que aponta para o endereco onde será guardada o valor da coordenada x.
 * \param coord_y= apontador que aponta para o endereco onde será guardada o valor da coordenada y.
 * \param coord_z= apontador que aponta para o endereco onde será guardada o valor da coordenada z.
 * \param aero= apontador que aponta para o aeroporto cujas coordenadas (x, y, z) estao a ser determinadas.
 */

void determina_coordenadas(double *coord_x, double *coord_y, double *coord_z, LISTA_AEROPORTO *aero)
{
    double radianos_latitude, radianos_longitude;

    radianos_latitude = calcula_angulo_latitude(aero);
    radianos_longitude = calcula_angulo_longitude(aero);

    *coord_x = R1 * cos(radianos_latitude) * cos(radianos_longitude);
    *coord_y = R1 * cos(radianos_latitude) * sin(radianos_longitude);
    *coord_z = R1 * sin(radianos_latitude);
}

/** \brief Esta funcao calcula o ângulo entre as coordenadas de dois aeroportos (o de partida e o de chegada) no espaço tridimensional.
 * \param partida= apontador que aponta para o aeroporto de partida.
 * \param chegada= apontador que aponta para o aeroporto de chegada.
 */

double calcula_angulo(LISTA_AEROPORTO *partida, LISTA_AEROPORTO *chegada)
{

    double xa, ya, za, xb, yb, zb, prod1, prod2, prod3, cosseno, angulo_total;

    determina_coordenadas(&xa, &ya, &za, partida);
    determina_coordenadas(&xb, &yb, &zb, chegada);

    prod1 = sqrt((xa * xa) + (ya * ya) + (za * za));
    prod2 = sqrt((xb * xb) + (yb * yb) + (zb * zb));
    prod3 = (xa * xb) + (ya * yb) + (za * zb);

    cosseno = prod3 / (prod1 * prod2);

    angulo_total = acos(cosseno);

    return angulo_total;
}

/** \brief Esta funcao calcula a distancia entre dois aeroportos (o de partida e o de chegada).
 * \param partidac= array que armazena o codigo IATA do aeroporto de partida.
 * \param chegadac= array que armazena o codigo IATA do aeroporto de chegada.
 * \param topo= apontador que aponta para o topo da lista de aeroportos.
 */

double calcula_distancia(char partidac[], char chegadac[], LISTA_AEROPORTO *topo)
{
    LISTA_AEROPORTO *partida, *chegada;
    double angulo, distancia;
    partida = NULL;
    chegada = NULL;

    while (topo != NULL && (partida == NULL || chegada == NULL))
    {
        if (strcmp(topo->aeroporto.id_iata, partidac) == 0)
        {
            partida = topo;
        }
        else if (strcmp(topo->aeroporto.id_iata, chegadac) == 0)
        {
            chegada = topo;
        }
        topo = topo->proximo_aero;
    }

    angulo = calcula_angulo(partida, chegada);
    distancia = angulo * R1;

    return distancia;
}

/** \brief Esta funcao valida os dados dos voos contidos ma lista de voos.
 * \param rota= apontador que aponta para a o voo que esta a ser validado.
 */

int valida_dados_voo(const LISTA_VOO *rota)
{

    if ((rota->voo.partida_minutos < 0) || (rota->voo.partida_minutos > 59) ||
        (rota->voo.chegada_minutos < 0) || (rota->voo.chegada_minutos > 59))
    {
        return 0;
    }

    if ((rota->voo.partida_horas < 0) || (rota->voo.partida_horas > 23) ||
        (rota->voo.chegada_horas < 0) || (rota->voo.chegada_horas > 23))
    {
        return 0;
    }

    if (!isalpha(rota->voo.codigo_voo[0]) || !isalpha(rota->voo.codigo_voo[1]) ||
        !isdigit(rota->voo.codigo_voo[2]) || !isdigit(rota->voo.codigo_voo[3]) ||
        !isdigit(rota->voo.codigo_voo[4]) || (rota->voo.codigo_voo[5] && !isdigit(rota->voo.codigo_voo[5])))
    {
        return 0;
    }

    if (rota->voo.codigo_voo[5] && rota->voo.codigo_voo[6] != '\0')
    {
        return 0;
    }

    return 1;
}

/** \brief Esta funcao ajusta a hora tendo em consideracao o fuso horario pretendido.
 * \param horas= variavel que armazena o valor das horas.
 * \param timezone= variavel que armazena o valor da timezone.
 */

int ajusta_hora(int horas, int timezone)
{
    horas -= timezone;
    if (horas < 0)
        horas += 24;
    if (horas >= 24)
        horas -= 24;
    return horas;
}

/** \brief Esta funcao verifica se o horario de chegada de um voo e posterior ao horario de partida do mesmo.
 * \param ap= apontador utilizado para acessar as informacoes do voo em analise.
 * \param fusohorario1= variavel que armazena o valor do fuso horario do aeroporto de partida.
 * \param fusohorario2= variavel que armazena o valor do fuso horario do aeroporto de chegada.
 */

int verifica_horario(INFOVOO ap, int fusohorario1, int fusohorario2)
{
    int hora_partida_ajustada, hora_chegada_ajustada, minutos_partida, minutos_chegada;

    hora_partida_ajustada = ajusta_hora(ap.partida_horas, fusohorario1);
    hora_chegada_ajustada = ajusta_hora(ap.chegada_horas, fusohorario2);

    minutos_partida = hora_partida_ajustada * 60 + ap.partida_minutos;
    minutos_chegada = hora_chegada_ajustada * 60 + ap.chegada_minutos;

    if (minutos_chegada < minutos_partida)
    {
        minutos_chegada += 24 * 60;
    }

    if (minutos_chegada <= minutos_partida)
    {
        return 0;
    }

    return 1;
}

/** \brief Funcao que adiciona sequencialmente voos à lista de voos de um determinado aeroporto.
 * \param topo= apontador que aponta para o topo da lista de aeroportos.
 * \param rota= apontador que nos permite aceder às informacoes dos voos a adicionar a lista.
 */

int coloca_voo_na_lista(LISTA_AEROPORTO *topo, LISTA_VOO *rota)
{
    LISTA_AEROPORTO *partida, *chegada, *ap;
    int verifhorario;
    partida = NULL, chegada = NULL, verifhorario = 0;

    if (rota == NULL)
    {
        printf("Erro: Rota não encontrada!\n");
        return -1;
    }

    if (!valida_dados_voo(rota))
    {
        printf("Erro: Dados da rota inválidos!\n");
        return -1;
    }

    for (ap = topo; ap != NULL; ap = ap->proximo_aero)
    {
        if (strcmp(ap->aeroporto.id_iata, rota->voo.aeroporto_partida) == 0)
        {
            partida = ap;
        }
        if (strcmp(ap->aeroporto.id_iata, rota->voo.aeroporto_chegada) == 0)
        {
            chegada = ap;
        }

        if (partida && chegada)
        {
            break;
        }
    }

    if (partida == NULL || chegada == NULL)
    {
        printf("Erro: Aeroporto de partida ou chegada não encontrado!\n");
        return -1;
    }

    verifhorario = verifica_horario(rota->voo, partida->aeroporto.fusohorario, chegada->aeroporto.fusohorario);
    if (!verifhorario)

    {
        printf("O horário da rota é incompatível!\n");
        return -1;
    }

    rota->proximo_voo = partida->aeroporto.voo;
    partida->aeroporto.voo = rota;

    return 1;
}

/** \brief Esta funcao verifica se um determinado aeroporto existe na lista de aeroportos.
 * \param topo= apontador que aponta para o topo da lista de aeroportos.
 * \param identificador= apontador que aponta para a primeira posição de um array que contem o identificador do aeroporto em analise.
 * \param aero_nao_encontrado= matriz que armazena os identificadores dos aeroportos nao encontrados.
 * \param aero_nao_encontrado_contador= apontador que armazena o endereco do numero inteiro que contem o numero de aeroportos nao encontrados.
 */

int verifica_existencia_aeroporto(LISTA_AEROPORTO *topo, char *identificador, char aero_nao_encontrado[100][6], int *aero_nao_encontrado_contador)
{
    LISTA_AEROPORTO *atual;
    int i;
    for (i = 0; i < *aero_nao_encontrado_contador; i++)
    {
        if (strcmp(aero_nao_encontrado[i], identificador) == 0)
        {
            return 2;
        }
    }

    atual = topo;
    while (atual != NULL)
    {
        if (strcmp(atual->aeroporto.id_iata, identificador) == 0)
        {
            return 1;
        }
        atual = atual->proximo_aero;
    }

    if (*aero_nao_encontrado_contador < 100)
    {
        strcpy(aero_nao_encontrado[*aero_nao_encontrado_contador], identificador);
        (*aero_nao_encontrado_contador)++;
    }
    return 0;
}

/** \brief Funcao que abre o ficheiro rotas.txt, fecha-o e verifica o seu conteudo.
 * \param topo= apontador que aponta para o topo da lista de aeroportos.
 */

void inicializa_voos(LISTA_AEROPORTO *topo)
{
    int rotas_inseridas;
    FILE *fp;
    fp = fopen("rotas.txt", "r");
    if (fp == NULL)
    {
        printf("Erro na abertura de rotas.txt.\n");
        return;
    }

    verificacao_e_leitura_voos(fp, topo, &rotas_inseridas);

    fclose(fp);

    if (rotas_inseridas == 0)
    {
        printf("Não foram encontradas rotas válidas no ficheiro rotas.txt.\n");
    }

    verifica_rotas(topo);
}

/** \brief Esta funcao lê e verifica os parametros das rotas que se encontram no documento "rotas.txt", linha por linha.
 * \param linha= variavel que guarda os parametros do voo em analise.
 * \param topo= apontador que aponta para o topo da lista de aeroportos.
 * \param companhia_aerea= variavel que guarda a companhia aerea do voo em analise.
 * \param aero_nao_encontrado= matriz que guarda os códigos IATA dos aeroportos nao encontrados.
 * \param aero_nao_encontrado_contador= variavel que guarda o numero de aeroportos nao encontrados.
 */

LISTA_VOO *processa_linha_voo(char *linha, LISTA_AEROPORTO *topo, char *companhia_aerea, char aero_nao_encontrado[100][6], int *aero_nao_encontrado_contador)
{
    LISTA_VOO *rota;
    rota = (LISTA_VOO *)malloc(sizeof(LISTA_VOO));

    if (rota == NULL)
    {
        printf("Erro na alocação de memória\n");
        return NULL;
    }

    if (sscanf(linha, "%s %s %d:%d %s %d:%d",
               rota->voo.codigo_voo, rota->voo.aeroporto_partida, &rota->voo.partida_horas,
               &rota->voo.partida_minutos, rota->voo.aeroporto_chegada,
               &rota->voo.chegada_horas, &rota->voo.chegada_minutos) == 7)
    {

        if (!verifica_existencia_aeroporto(topo, rota->voo.aeroporto_partida, aero_nao_encontrado, aero_nao_encontrado_contador))
        {
            printf("Erro: Aeroporto %s não encontrado!\n", rota->voo.aeroporto_partida);
            free(rota);
            return NULL;
        }

        if (!verifica_existencia_aeroporto(topo, rota->voo.aeroporto_chegada, aero_nao_encontrado, aero_nao_encontrado_contador))
        {
            printf("Erro: Aeroporto %s não encontrado!\n", rota->voo.aeroporto_chegada);
            free(rota);
            return NULL;
        }

        rota->voo.distancia_aeroportos = calcula_distancia(rota->voo.aeroporto_partida, rota->voo.aeroporto_chegada, topo);
        strcpy(rota->voo.companhia_aerea, companhia_aerea);

        return rota;
    }
    else
    {
        printf("Formato de rota inválido numa das linhas de rotas.txt!");
        free(rota);
        return NULL;
    }
}

/** \brief Esta funcao verifica as rotas que se encontram no documento "rotas.txt", linha por linha.
 * \param fp= apontador para o ficheiro "rotas.txt".
 * \param topo= apontador que aponta para o topo da lista de aeroportos.
 * \param rotas_inseridas= apontador que aponta para um endereco que armazena o número de rotas válidas
 * inseridas na lista de voos durante a leitura do ficheiro.
 */

void verificacao_e_leitura_voos(FILE *fp, LISTA_AEROPORTO *topo, int *rotas_inseridas)
{
    char linha[100], companhia_aerea[30] = {0}, aero_nao_encontrado[100][6] = {0};
    int conta_linhas = 0, verifica_companhia = 0, aero_nao_encontrado_contador = 0;
    LISTA_VOO *rota;

    while (fgets(linha, sizeof(linha), fp) != NULL)
    {
        conta_linhas++;
        if (verifica_linha_em_branco(linha))
            continue;

        if (strncmp(linha, "AIRLINE:", 8) == 0)
        {
            if (sscanf(linha, "AIRLINE: %[^\n]", companhia_aerea) == 1 && strlen(companhia_aerea) > 0)
            {
                verifica_companhia = 0;
            }
            else
            {
                printf("Airline em branco na linha %d, ignorando voos subsequentes.\n", conta_linhas);
                verifica_companhia = 1;
            }
            continue;
        }

        if (verifica_companhia)
            continue;

        rota = processa_linha_voo(linha, topo, companhia_aerea, aero_nao_encontrado, &aero_nao_encontrado_contador);
        if (rota)
        {
            if (coloca_voo_na_lista(topo, rota))
            {
                (*rotas_inseridas)++;
            }
            else
            {
                free(rota);
                printf("Erro ao inserir rota na linha %d.\n", conta_linhas);
            }
        }
    }

    if (*rotas_inseridas == 0)
    {
        printf("Não foram encontradas rotas válidas no ficheiro rotas.txt.\n");
    }
}

/** \brief Esta funcao verifica se existem rotas repetidas na lista de voos de cada aeroporto. Se existirem, elimina as rotas que estão a mais.
 * \param topo= apontador que aponta para o topo da lista de aeroportos.
 */

void verifica_rotas(LISTA_AEROPORTO *topo)
{
    LISTA_AEROPORTO *aeroporto_atual;
    LISTA_VOO *rota_atual, *rota_comparadora, *rota_comparada;
    aeroporto_atual = topo;

    while (aeroporto_atual != NULL)
    {
        rota_atual = aeroporto_atual->aeroporto.voo;
        while (rota_atual != NULL && rota_atual->proximo_voo != NULL)
        {
            rota_comparadora = rota_atual;
            rota_comparada = rota_atual->proximo_voo;
            while (rota_comparada != NULL)
            {
                if (strcmp(rota_atual->voo.codigo_voo, rota_comparada->voo.codigo_voo) == 0)
                {
                    rota_comparadora->proximo_voo = rota_comparada->proximo_voo;
                    free(rota_comparada);
                    rota_comparada = NULL;
                    printf("Existe uma rota repetida no ficheiro rotas.txt, que foi suprimida\n");
                    rota_comparada = rota_comparadora->proximo_voo;
                }
                else
                {
                    rota_comparadora = rota_comparada;
                    rota_comparada = rota_comparada->proximo_voo;
                }
            }
            rota_atual = rota_atual->proximo_voo;
        }
        aeroporto_atual = aeroporto_atual->proximo_aero;
    }
}

/** \brief Funcao que imprime todas as rotas de voos armazenadas na lista de voos.
 * \param topo= apontador que aponta para o topo da lista de aeroportos.
 */

void imprime_rotas(LISTA_AEROPORTO *topo)
{
    LISTA_VOO *aux;
    aux = NULL;
    for (; topo != NULL; topo = topo->proximo_aero)
    {
        for (aux = topo->aeroporto.voo; aux != NULL; aux = aux->proximo_voo)
        {
            apresenta_rotas_no_ecra(&(aux->voo));
        }
    }
}

/** \brief Funcao que apresenta no ecra as informacoes de uma determinada rota.
 * \param rota= apontador que aponta para o voo em questao e permite aceder a todas as informacoes do voo cuja rota aparecera no ecra.
 */

void apresenta_rotas_no_ecra(INFOVOO *rota)
{
    printf("\nAIRLINE: %s \n%s %s %02d:%02d %s %02d:%02d \nDISTANCIA: %.2f Km\n",
           rota->companhia_aerea,
           rota->codigo_voo, rota->aeroporto_partida,
           rota->partida_horas,
           rota->partida_minutos, rota->aeroporto_chegada,
           rota->chegada_horas,
           rota->chegada_minutos, rota->distancia_aeroportos);
}
