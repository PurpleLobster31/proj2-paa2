#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define INF INT_MAX

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

    FILE *arquivo = fopen(argv[1], "r");

    if (arquivo == NULL){
        printf("Erro ao ler o arquivo.\n");
        return -1;
    }
    
    int esquina_incendio;
    int quantEsquinas;

    fscanf(arquivo, "%d", &esquina_incendio);
    fscanf(arquivo, "%d", &quantEsquinas);

    int **grafo = (int **) malloc(quantEsquinas * sizeof(int *));
    for (int i = 0; i < quantEsquinas; i++) {
        grafo[i] = (int *) malloc(quantEsquinas * sizeof(int));
        for (int j = 0; j < quantEsquinas; j++) grafo[i][j] = INF;
    }

    // int contadorTripla = 1;

    // int *inicio = NULL;
    // int *destino = NULL;
    // int *custo = NULL;

    // char caractere;
    // int tamanho = 0;
    // while ((caractere = getc(arquivo)) != '0'){
    //     if (caractere == '\n' || contadorTripla > 3){
    //         contadorTripla = 1;
    //         tamanho++;
    //     }
    //     if (caractere != '\n' && caractere != ' '){
    //         int valor = caractere - '0';
    //         if (contadorTripla == 1){
    //             inicio = (int*) realloc(inicio, (tamanho+1) * sizeof(int));
    //             destino = (int*) realloc(destino, (tamanho+1) * sizeof(int));
    //             custo = (int*) realloc(custo, (tamanho+1) * sizeof(int));
    //             inicio[tamanho] = valor;
                
    //         }
    //         else if (contadorTripla == 2){
    //             destino[tamanho] = valor;
    //         }
    //         else {
    //             custo[tamanho] = valor;
                
    //         }
    //         contadorTripla++;
    //     }
    // }

    int inicio, destino, tempo;
    while (fscanf(arquivo, "%d", &inicio) && inicio != 0) {
        fscanf(arquivo, "%d %d", &destino, &tempo);
        grafo[inicio - 1][destino - 1] = tempo;
    }
    fclose(arquivo);

    int dist[quantEsquinas];
    int visitado[quantEsquinas];
    int antecessor[quantEsquinas];

    for (int i = 0; i < quantEsquinas; i++) {
            dist[i] = INF;
            visitado[i] = 0;
            antecessor[i] = -1;
    }

    dist[0] = 0;

    for (int count = 0; count < quantEsquinas - 1; count++) {
        int u = menorDistancia(dist, visitado, quantEsquinas);
        if (u == -1) break;
        visitado[u] = 1;

        for (int v = 0; v < quantEsquinas; v++) {
            if (!visitado[v] && grafo[u][v] != INF &&
                dist[u] + grafo[u][v] < dist[v]) {
                dist[v] = dist[u] + grafo[u][v];
                antecessor[v] = u;
            }
        }
    }

    // printf("%d\n%d\n", esquina_incendio, quantEsquinas);
    // printf("Inicio, destino e custo:\n");
    // for (int i = 0; i < tamanho; i++){
    //     printf("%d %d %d\n", inicio[i], destino[i], custo[i]);
    // }
    
    // fclose(arquivo);
    // free(inicio);
    // free(destino);
    // free(custo);

    FILE *saida = fopen("saida.txt", "w");
    if (!saida) {
        printf("Erro ao criar saida.txt\n");
        return 1;
    }

    imprimirRota(saida, antecessor, esquina_incendio - 1);
    fprintf(saida, "tempo calculado para rota = %d min.\n", dist[esquina_incendio - 1]);
    fclose(saida);

    printf("rota até a esquina #%d: ", esquina_incendio);
    imprimirRota(stdout, antecessor, esquina_incendio - 1);
    printf("tempo calculado para rota = %d min.\n", dist[esquina_incendio - 1]);

    for (int i = 0; i < quantEsquinas; i++) free(grafo[i]);
    free(grafo);

    return 0;
}