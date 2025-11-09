/* 
INTEGRANTES:
Carlos Eduardo Diniz de Almeida RA 10444407
Guilherme Silveira Giacomini    RA 10435311
Matheus Mendonça Lopes          RA 10443495
*/
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define INF INT_MAX

//retorna a esquina com menor distância ainda não visitada
int menorDistancia(int dist[], int visitado[], int n){
    int min = INF;
    int index_minimo =  -1;
    for(int v = 0; v<n; v++){
        if(!visitado[v] && dist[v]<=min) {
            min = dist[v];
            index_minimo = v;
        }
    }
    return index_minimo;
}

//imprime a rota informada
void imprimirRota(FILE *saida, int antecessor[], int destino) {
    int rota[100];
    int tamanho = 0;
    int atual = destino;

    while (atual != -1) {
        rota[tamanho++] = atual + 1; 
        atual = antecessor[atual];
    }

    fprintf(saida, "rota até a esquina #%d: ", destino + 1);
    for (int i = tamanho - 1; i >= 0; i--) {
        fprintf(saida, "%d ", rota[i]);
    }
    fprintf(saida, "\n");
}

int main(int argc, char *argv[]){ 
    
    if (argc != 2) {
        printf("Modo de uso: ./bombeiros {arquivo_entrada.txt}\n");
        return 1;
    }

    //leitura do arquivo informado via CLI
    FILE *arquivo = fopen(argv[1], "r");

    if (arquivo == NULL){
        printf("Erro ao ler o arquivo.\n");
        return -1;
    }
    
    int esquina_incendio; // esquina onde ocorre o incêncio
    int quantEsquinas; // total de esquinas

    fscanf(arquivo, "%d", &esquina_incendio);
    fscanf(arquivo, "%d", &quantEsquinas);

    //cria a matriz de adjacências (grafo) - estrutura auxiliar que armazena as esquinas
    int **grafo = (int **) malloc(quantEsquinas * sizeof(int *));
    for (int i = 0; i < quantEsquinas; i++) {
        grafo[i] = (int *) malloc(quantEsquinas * sizeof(int));
        for (int j = 0; j < quantEsquinas; j++) grafo[i][j] = INF;
    }


    //prenchimento do grafo, onde grafo[i][j] = tempo da esquina i -> j
    int inicio, destino, tempo;
    while (fscanf(arquivo, "%d", &inicio) && inicio != 0) {
        fscanf(arquivo, "%d %d", &destino, &tempo);
        grafo[inicio - 1][destino - 1] = tempo;
    }
    fclose(arquivo);

    int dist[quantEsquinas];  // menor tempo conhecido até cada esquina
    int visitado[quantEsquinas]; // marca quais esquinas já foram visitadas
    int antecessor[quantEsquinas]; // guarda o antecessor para chega em cada esquina na rota

    // inicializa vetores auxiliares
    for (int i = 0; i < quantEsquinas; i++) {
            dist[i] = INF;
            visitado[i] = 0;
            antecessor[i] = -1;
    }

    dist[0] = 0; // a esquina 1 tem distância zero até ela mesma (ponto de início)

    for (int count = 0; count < quantEsquinas - 1; count++) {
        //escolhe a esquina com menor distância ainda não visitado
        int u = menorDistancia(dist, visitado, quantEsquinas);
        if (u == -1) break; 
        visitado[u] = 1;

        // atualização das distâncias
        for (int v = 0; v < quantEsquinas; v++) {
            if (!visitado[v] && grafo[u][v] != INF &&
                dist[u] + grafo[u][v] < dist[v]) {
                dist[v] = dist[u] + grafo[u][v];
                antecessor[v] = u;
            }
        }
    }


    //cria arquivo de saída
    FILE *saida = fopen("saida.txt", "w");
    if (!saida) {
        printf("Erro ao criar saida.txt\n");
        return 1;
    }   

    // escreve a rota mais rápida e o tempo na saida.txt
    imprimirRota(saida, antecessor, esquina_incendio - 1);
    fprintf(saida, "tempo calculado para rota = %d min.\n", dist[esquina_incendio - 1]);
    fclose(saida);


    // escreve a rota mais rápida e o tempo no terminal
    printf("rota até a esquina #%d: ", esquina_incendio);
    imprimirRota(stdout, antecessor, esquina_incendio - 1);
    printf("tempo calculado para rota = %d min.\n", dist[esquina_incendio - 1]);


    // libera memória alocada para o grafo
    for (int i = 0; i < quantEsquinas; i++) free(grafo[i]);
    free(grafo);

    return 0;
}