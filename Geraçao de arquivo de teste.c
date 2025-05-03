#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const char *sensores[] = {"TEMP", "PRES", "VIBR", "UMID", "FLUX"};

time_t gerar_timestamp_aleatorio(int dia, int mes, int ano);

int main() {
    int dia, mes, ano;
    printf("Digite a data (dd mm aaaa): ");
    scanf("%d %d %d", &dia, &mes, &ano);

    srand(time(NULL));
    FILE *arquivo = fopen("leituras.txt", "w");
    if (!arquivo) {
        perror("Erro ao criar arquivo");
        return 1;
    }

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 1000; j++) {
            time_t ts = gerar_timestamp_aleatorio(dia, mes, ano);
            float valor = (float)(rand() % 10000) / 100.0;
            fprintf(arquivo, "%ld %s %.2f\n", ts, sensores[i], valor);
        }
    }

    fclose(arquivo);
    printf("Arquivo de teste gerado com sucesso.\n");
    return 0;
}

time_t gerar_timestamp_aleatorio(int dia, int mes, int ano) {
    struct tm t;
    t.tm_year = ano - 1900;
    t.tm_mon = mes - 1;
    t.tm_mday = dia;
    t.tm_hour = 0;
    t.tm_min = 0;
    t.tm_sec = 0;
    t.tm_isdst = -1;

    time_t inicio = mktime(&t);
    t.tm_hour = 23; t.tm_min = 59; t.tm_sec = 59;
    time_t fim = mktime(&t);

    return inicio + rand() % (fim - inicio + 1);
}
