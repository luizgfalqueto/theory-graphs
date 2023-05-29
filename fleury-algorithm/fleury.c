#include <stdio.h>
#include <stdlib.h>

/*
-
-   Disciplina: Teoria dos Grafos 2023/1
-   Atividade: Exercício valendo 1 ponto bonus na prova 1
-   Aluno: Luiz Gustavo Falqueto Baptista
-   Matrícula: 2017203673
-   Data: 25/05/2023
-   
*/

typedef struct vertice{
    int no;
    struct vertice *prox;
}Vertice; // estrutur para armazenar as adjacencias dos vertices dos grafos

typedef struct grafo {
    int V;
    int E;
    Vertice *adj;
    int *graus;
}Grafo; // estrutura para armazenar o grafo

Grafo *criaGrafo(int V){ // funcao para criar um grafo vazio
    Grafo *G = malloc(sizeof(Grafo));
    if(G != NULL){
        G->V = V;
        G->E = 0;
        G->adj = (Vertice *)calloc(V,sizeof(Vertice));
        G->graus = (int*)calloc(V,sizeof(int));
        return G;
    }else { printf("Erro.\n"); exit(1); }
}

void liberaGrafo(Grafo *G){ // funcao para liberar memoria utilizada no grafo
    if(G != NULL){
        Vertice *auxDel,*aux;
        int i;
        for(i=0; i<G->V; i++){
            aux = G->adj[i].prox;
            while (G->adj[i].prox != NULL){
                auxDel = aux;
                aux = aux->prox;
                free(auxDel);
                G->adj[i].prox = aux;
            }
        }
        free(G->adj);
        free(G->graus);
        free(G);
    }
}

Vertice *novoVertice(int x){ // funcao para criar novo vertice
    Vertice *novo = (Vertice*)malloc(sizeof(Vertice));
    if(novo == NULL){ printf("Erro.\n"); exit(1); }
    novo->no = x;
    novo->prox = NULL;
    return novo;
}

void imprimeGrafo(Grafo *G){ // funcao para exibir o grafo
    int v;
    Vertice *w;
    printf("\n\n");

    for(v=0; v < G->V; v++){

        printf("[%d | %d]-> ",v+1, G->graus[v]);

        w = G->adj[v].prox;
        while(w != NULL){
            if(w->prox != NULL) printf(" [%2d ]->",w->no+1);
            else printf(" [%2d ]",w->no+1);
            w = w->prox;
        }
        printf("\n\n");
    }
}

void insereAresta(Grafo *G, int v, int w){ // funcao para inserir uma aresta dados dois vertices
    if(v!=w){ 
        // inserindo v -- w
        Vertice *p = G->adj[v].prox;
        while(p != NULL){
            if( p->no == w) break;
            p = p->prox;
        }

        if( p == NULL){ // a aresta (v,w) nao existe
            Vertice *novo = novoVertice(w);
            novo->prox = G->adj[v].prox;
            G->adj[v].prox = novo;
            G->E++;
            G->graus[v] = G->graus[v] + 1;
        }

        p = G->adj[w].prox;

        // inserindo w -- v
        Vertice *q = G->adj[w].prox;
        while(q != NULL){
            if( q->no == v) break;
            q = q->prox;
        }

        if( q == NULL){ // a aresta (v,w) nao existe
            Vertice *novo = novoVertice(v);
            novo->prox = G->adj[w].prox;
            G->adj[w].prox = novo;
            G->E++;
            G->graus[w] = G->graus[w] + 1;
        }

        p = G->adj[v].prox;
    }
}

void removeAresta(Grafo *G, int v, int w){ // funcao que remove uma aresta do grafo
    if(G->adj[v].prox != NULL){ // removendo v -- w
        Vertice *aux = G->adj[v].prox;
        Vertice *preaux = &(G->adj[v]);
        while(aux != NULL){
            if(aux->no == w) break;
            preaux = aux;
            aux = aux->prox;
        }

        if(aux != NULL) {
            G->E--;
            G->graus[v] = G->graus[v] - 1;
            preaux->prox = aux->prox;
            free(aux);
        }
    }

    if(G->adj[w].prox != NULL){ // removendo w -- v
        Vertice *aux = G->adj[w].prox;
        Vertice *preaux = &(G->adj[w]);
        while(aux != NULL){
            if(aux->no == v) break;
            preaux = aux;
            aux = aux->prox;
        }

        if(aux != NULL) {
            G->E--;
            G->graus[w] = G->graus[w] - 1;
            preaux->prox = aux->prox;
            free(aux);
        }
    }
}

void dfs(Grafo *G, int v, int *visitados) {
    visitados[v] = 1; // atualiza vertice para visitado
    Vertice* temp = G->adj[v].prox; // seleciona o vertice visitado

    while (temp != NULL) { // visitar vertices adjacentes a temp
        int adjacente = temp->no; // visita vertice adjacente
        if (!visitados[adjacente]) {
            dfs(G, adjacente, visitados); // realiza busca nesse novo vertice visitado
        }
        temp = temp->prox;
    }
}

int componentesConexas(Grafo *G) {
    int qtdVertices = G->V;
    int visitados[qtdVertices]; // vetor que armazena o historico de vertices visitados no percurso
    for (int i = 0; i < qtdVertices; i++) {
        visitados[i] = 0; // inicializando todos vertices como nao visitado
    }

    int count = 0; // contador de componentes conexas
    for (int v = 0; v < qtdVertices; v++) {
        if (!visitados[v]) { // se o vertice nao foi visitado, realiza a busca
            count++;
            dfs(G, v, visitados);
        }
    }

    return count;
}

int verificaGrauPar (Grafo *G) { // funcao que veirifica se todos os graus dos vertices do grafo tem grau par
    int isPar = 0, indexAdj = 0, contList = 0;
    Vertice *aux;

    for (indexAdj = 0 ; indexAdj < G->V ; indexAdj++) { // percorre o vetor adj de G
        aux = G->adj[indexAdj].prox;
        while (aux != NULL) { // percore a lista em adj[x] de G
            contList++;
            aux = aux->prox;
        }
        if (contList % 2 != 0) return 1; // se o contador deu impar
        contList = 0;
    }
    return 0;
}

int ePonte (Grafo *G, int v, int u) { // funcao que verificar se a aresta v -- u eh ponte (retorna 1 se for, 0 se nao for ponte)
    int qtdCompConexasAntes = 0, qtdCompConexasDepois = 0;

    qtdCompConexasAntes = componentesConexas(G); // aramzaena a quantidade de componentes conexas inicial

    removeAresta(G, v, u);

    qtdCompConexasDepois = componentesConexas(G); // armazena a quantidade de componentes conexas apos a remocao da aresta v -- u

    insereAresta(G, v, u); // retorna com a aresta para manter o grafo inicial

    return qtdCompConexasDepois > qtdCompConexasAntes ? 1 : 0; // retorno da funcao
}

void fleury (Grafo *G) { // funcao que realiza o tour de Euler (ja recebe um grafo euleriano)
    int v0 = 0, vi, nextVi, prox;
    printf(" ");
    int *tour = (int*)malloc(G->V * sizeof(int)); // variavel que aramzena o tour de Euler no grafo
    int tourIndex = 0;
    tour[tourIndex] = v0;
    tourIndex++;

    while(G->E > 0) { // enquanto ainda tem aresta para explorar
        vi = tour[tourIndex-1]; // vertice atual visitado

        if(G->adj[vi].prox->prox == NULL) { // verifica se o vertice atual tem apenas uma opcao para seguir
            nextVi = G->adj[vi].prox->no;
        } else { // vertice tem mais de uma opcao para seguir
            Vertice *aux = G->adj[vi].prox; // variavel que percore a lista dos adjentes do vertice atual

            while(aux != NULL) {
                prox = aux->no;
                if(ePonte(G, vi, prox) == 0) { // verifica que a aresta que liga o vertice atual ao possivel proximo nao e uma ponte
                    nextVi = prox; // o proximo vertice a acrescentar no tour foi encontrado
                    break;
                }
                aux = aux->prox;
            }
        }

        removeAresta(G, vi, nextVi); // remove a aresta vi -- ultimo vertice alcancado
        tour[tourIndex] = nextVi; // adiciona no tour
        tourIndex++;
    }

    // printf("Caminho de Euler realizado -> ");
    int i;
    for(i=0 ; i<tourIndex ; i++) { // imprime o tour encontrado
        printf("%d ", tour[i]+1);
    }

    printf("\n");
}

int main() {
    int qtdVertices, qtdArestas, vertice1, vertice2; // variaveis que armazenam os valores de entrada

    scanf("%d %d", &qtdVertices, &qtdArestas); // lendo numero de vertices e arestas do grafo

    Grafo *G = criaGrafo(qtdVertices); // criando o grafo

    int i;
    for (i=0 ; i<qtdArestas; i++) { // cada iteracao representa uma linha da entrada com excessao da primeira linha
        scanf("%d %d", &vertice1, &vertice2);

        insereAresta(G, vertice1-1, vertice2-1);
    }

    int qtdComponentesConexas = componentesConexas(G); // aramzenando numero de componentes conexas
    int temGrauPar = verificaGrauPar(G); // verificando se possui apenas vertices de grau par 

    // imprimeGrafo(G);

    if (qtdComponentesConexas == 1 && temGrauPar == 0) { // verifica se eh grafo euleriano
        // eh euleriano
        fleury(G); // algoritmo que realiza o tour de euler
    } else {
        printf("IMPOSSIVEL\n");
    }
    
    liberaGrafo(G); // liberando memoria utilizada pelo grafo

    return 0;
}