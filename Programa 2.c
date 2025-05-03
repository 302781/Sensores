#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    time_t timestamp;
    float valor;
} Leitura;

time_t capturar_timestamp_valido();

int busca_binaria(Leitura *dados, int tamanho, time_t alvo) {
    int esq = 0, dir = tamanho - 1, meio, melhor = -1;
    while (esq <= dir) {
        meio = (esq + dir) / 2;
        if (dados[meio].timestamp == alvo)
            return meio;
        if (dados[meio].timestamp < alvo) {
            melhor = meio;
            esq = meio + 1;
        } else {
            dir = meio - 1;
        }
    }

    if (melhor == -1) return 0;
    if (melhor + 1 < tamanho && 
        abs(dados[melhor + 1].timestamp - alvo) < abs(dados[melhor].timestamp - alvo))
        return melhor + 1;
    return melhor;
}

int main() {
    char sensor[32];
    printf("Digite o nome do sensor: ");
    scanf("%s", sensor);

    char nome_arquivo[64];
    snprintf(nome_arquivo, sizeof(nome_arquivo), "%s.txt", sensor);

    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        printf("Arquivo do sensor não encontrado.\n");
        return 1;
    }

    Leitura *dados = malloc(sizeof(Leitura) * 10000);
    int count = 0;
    time_t ts;
    char id[16];
    while (fscanf(arquivo, "%ld %s %f", &ts, id, &dados[count].valor) == 3) {
        dados[count].timestamp = ts;
        count++;
    }
    fclose(arquivo);

    time_t alvo = capturar_timestamp_valido();
    int indice = busca_binaria(dados, count, alvo);
    printf("Leitura mais próxima: %ld %.2f\n", dados[indice].timestamp, dados[indice].valor);

    free(dados);
    return 0;
}

// Reutiliza função dada na descrição para capturar timestamp válido
time_t capturar_timestamp_valido() {
    int dia, mes, ano, hora, min, seg;
    struct tm t;
    while (1) {
        printf("Digite a data e hora (dd mm aaaa hh mm ss): ");
        if (scanf("%d %d %d %d %d %d", &dia, &mes, &ano, &hora, &min, &seg) != 6) {
            while (getchar() != '\n');
            printf("Entrada inválida.\n");
            continue;
        }
        t.tm_year = ano - 1900;
        t.tm_mon = mes - 1;
        t.tm_mday = dia;
        t.tm_hour = hora;
        t.tm_min = min;
        t.tm_sec = seg;
        t.tm_isdst = -1;
        time_t ts = mktime(&t);
        if (ts == -1)
            printf("Data inválida.\n");
        else
            return ts;
    }
}
