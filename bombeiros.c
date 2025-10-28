#include <stdio.h>
#include <stdlib.h>

int main(){

    
    FILE *arquivo = fopen("arquivo.txt", "r");

    if (arquivo == NULL){
        printf("Erro ao ler o arquivo.\n");
        return -1;
    }
    
    int esquina = getc(arquivo) - '0';
    
    getc(arquivo);

    int quantEsquinas = getc(arquivo) - '0';
    
    getc(arquivo);

    int contadorTripla = 1;

    int *inicio = NULL;
    int *destino = NULL;
    int *custo = NULL;

    char caractere;
    int tamanho = 0;
    while ((caractere = getc(arquivo)) != '0'){
        if (caractere == '\n' || contadorTripla > 3){
            contadorTripla = 1;
            tamanho++;
        }
        if (caractere != '\n' && caractere != ' '){
            int valor = caractere - '0';
            if (contadorTripla == 1){
                inicio = (int*) realloc(inicio, (tamanho+1) * sizeof(int));
                destino = (int*) realloc(destino, (tamanho+1) * sizeof(int));
                custo = (int*) realloc(custo, (tamanho+1) * sizeof(int));
                inicio[tamanho] = valor;
                
            }
            else if (contadorTripla == 2){
                destino[tamanho] = valor;
            }
            else {
                custo[tamanho] = valor;
                
            }
            contadorTripla++;
        }
    }

    printf("%d\n%d\n", esquina, quantEsquinas);
    printf("Inicio, destino e custo:\n");
    for (int i = 0; i < tamanho; i++){
        printf("%d %d %d\n", inicio[i], destino[i], custo[i]);
    }
    
    fclose(arquivo);
    free(inicio);
    free(destino);
    free(custo);

    return 0;
}