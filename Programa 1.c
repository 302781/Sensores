#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 256
#define MAX_SENSORS 100
#define MAX_FILENAME 64

typedef struct {
    time_t timestamp;
    char id_sensor[16];
    float valor;
} Leitura;

int comparar_leituras(const void *a, const void *b) {
    return ((Leitura *)a)->timestamp - ((Leitura *)b)->timestamp;
}

int main() {
    FILE *arquivo = fopen("leituras.txt", "r");
    if (!arquivo) {
        perror("Erro ao abrir arquivo de entrada");
        return 1;
    }

    Leitura *leituras[MAX_SENSORS];
    int contadores[MAX_SENSORS] = {0};
    char sensores[MAX_SENSORS][16];
    int total_sensores = 0;

    char linha[MAX_LINE];
    while (fgets(linha, sizeof(linha), arquivo)) {
        Leitura l;
        if (sscanf(linha, "%ld %s %f", &l.timestamp, l.id_sensor, &l.valor) != 3)
            continue;

        int index = -1;
        for (int i = 0; i < total_sensores; i++) {
            if (strcmp(sensores[i], l.id_sensor) == 0) {
                index = i;
                break;
            }
        }

        if (index == -1) {
            if (total_sensores >= MAX_SENSORS) {
                printf("Limite de sensores atingido.\n");
                break;
            }
            index = total_sensores++;
            strcpy(sensores[index], l.id_sensor);
            leituras[index] = malloc(sizeof(Leitura) * 10000); // espaço inicial
        }

        leituras[index][contadores[index]++] = l;
    }

    fclose(arquivo);

    // Ordena e escreve cada sensor
    for (int i = 0; i < total_sensores; i++) {
        qsort(leituras[i], contadores[i], sizeof(Leitura), comparar_leituras);

        char nome_arquivo[MAX_FILENAME];
        snprintf(nome_arquivo, sizeof(nome_arquivo), "%s.txt", sensores[i]);
        FILE *saida = fopen(nome_arquivo, "w");
        if (!saida) {
            perror("Erro ao criar arquivo de sensor");
            continue;
        }

        for (int j = 0; j < contadores[i]; j++) {
            fprintf(saida, "%ld %s %.2f\n", leituras[i][j].timestamp, sensores[i], leituras[i][j].valor);
        }

        fclose(saida);
        free(leituras[i]);
    }

    printf("Dados organizados com sucesso.\n");
    return 0;
}
