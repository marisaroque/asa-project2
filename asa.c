#include <stdio.h>
#include <stdlib.h>

/************************** Constantes **************************/

#define FALSE 0
#define TRUE  1
#define NIL  -1
#define INF  -1
#define ERROR_QUEUE   2
#define ERROR_MEMORY  3
#define MAX_QUEUE_SIZE  100

/************************** Estruturas **************************/

enum colors {black, gray, white};

typedef int Vertex;
typedef int QueueElement;
typedef enum colors Color;
typedef struct node *Node;
typedef struct graph *Graph;
typedef struct queue *Queue;

struct node {
    Vertex w;
    int capacity;
    int flow;
    Node next;
};

struct graph {
    int V;
    Node *adj;
};

typedef struct queue {
    QueueElement contents[MAX_QUEUE_SIZE];
    int front;
    int count;
} queueCDT;

/************************** Cabeçalhos **************************/

Queue QUEUEcreate(void);
void QUEUEdestroy(Queue queue);
void QUEUEenter(Queue queue, QueueElement element);
QueueElement QUEUEdelete(Queue queue);
int QUEUEempty(Queue queue);
int QUEUEfull(Queue queue);
void QUEUEshow(Queue queue);
void GRAPHshow (Graph G);
void PATHshow(int *path, int path_size);
void GRAPHclear_flow(Graph G);
Node NEWnode(Vertex w, int cap, Node next);
Graph GRAPHinit(int V);
void GRAPHdestroy(Graph G);
void GRAPHinsertE(Graph G, Vertex v, Vertex w);
Graph GRAPHresidual(Graph G);
Vertex *bfs(Graph G, Vertex s, Vertex t, int *size, int *gray_size);
int ford_fulkerson(Graph G, Vertex s, Vertex t);

/************************** Fila de Vértices **************************/

Queue QUEUEcreate(void) {
    Queue queue;
    
    queue = (Queue)malloc(sizeof(queueCDT));
    
    if (queue == NULL) {
        fprintf(stderr, "Insufficient Memory for new Queue.\n");
        exit(ERROR_MEMORY);
    }
    
    queue->front = 0;
    queue->count = 0;
    
    return queue;
}

void QUEUEdestroy(Queue queue) {
    free(queue);
}

void QUEUEenter(Queue queue, QueueElement element) {
    int newElementIndex;
    
    if (queue->count >= MAX_QUEUE_SIZE) {
        fprintf(stderr, "QueueEnter on Full Queue.\n");
        exit(ERROR_QUEUE);
    }
    
    newElementIndex = (queue->front + queue->count)
    % MAX_QUEUE_SIZE;
    queue->contents[newElementIndex] = element;
    
    queue->count++;
}

QueueElement QUEUEdelete(Queue queue) {
    QueueElement oldElement;
    
    if (queue->count <= 0) {
        fprintf(stderr, "QueueDelete on Empty Queue.\n");
        exit(ERROR_QUEUE);
    }
    
    oldElement = queue->contents[queue->front];
    
    queue->front++;
    queue->front %= MAX_QUEUE_SIZE;
    
    queue->count--;
    
    return oldElement;
}

int QUEUEempty(Queue queue) {
    return queue->count <= 0;
}

int QUEUEfull(Queue queue) {
    return queue->count >= MAX_QUEUE_SIZE;
}

void QUEUEshow(Queue queue) {
    int k;
    for (k= queue->front; k< queue->front + queue->count; k++) {
        printf("%d ", queue->contents[k]);
    }
    printf("\n");
}

/************************** Representação de grafos **************************/

/* Para cada vértice v do grafo G, esta função imprime, numa linha,
 todos os vértices adjacentes a v. */
void GRAPHshow (Graph G) {
    Vertex v;
    Node a;
    for(v = 0; v < G->V; v++){
        printf("%2d:", v);
        for(a = G->adj[v]; a != NULL; a = a->next){
            printf(" %2d", a->w);
            printf(" %2d/%1d", a->flow, a->capacity);
        }
        printf("\n");
    }
    printf("\n");
}

void PATHshow(int *path, int path_size) {
    int k;
    printf("path (%d): ", path_size);
    for (k=0; k< path_size; k++) {
        printf("%d ", path[k]);
    }
    printf("\n");
}

void GRAPHclear_flow(Graph G) {
    int k;
    Node n;
    
    for(k=0; k< G->V; k++){
        for( n = G->adj[k]; n!= NULL; n = n->next){
            n->flow = 0;
        }
    }
}

Node NEWnode(Vertex w, int cap, Node next) {
    Node a = malloc(sizeof (struct node));
    a->w = w;
    a->next = next;
    a->capacity = cap;
    a->flow = 0;
    return a;
}


/* A função GRAPHinit devolve (o endereço de) um novo grafo com vértices 0 1 ... V-1 e nenhum arco. */
Graph GRAPHinit(int V) {
    int k;
    
    Graph G = malloc(sizeof *G);
    G->V = V;
    G->adj = malloc(V * sizeof (Node));
    
    for(k = 0; k < G->V; k++){
        G->adj[k] = NULL;
    }
    
    return G;
}

/* A função GRAPHdestroy destroi o grafo. */
void GRAPHdestroy(Graph G) {
    int v;
    
    /* Liberta memória dos nós da lista de adjacências */
    for(v = 0; v < G->V; v++){
        free(G->adj[v]);
    }
    
    /* Liberta memória do vector da lista de adjacências */
    free(G->adj);
    
    /* Liberta memória do grafo */
    free(G);
}

/* A função GRAPHinsertE insere um arco v-w no grafo G. */
void GRAPHinsertE(Graph G, Vertex v, Vertex w) {
    G->adj[v] = NEWnode(w, 1, G->adj[v]);
    G->adj[w] = NEWnode(v, 1, G->adj[w]);
}

Graph GRAPHresidual(Graph G){
    Graph Gf = GRAPHinit(G->V);
    int k;
    Node n;
    
    for(k=0; k< G->V; k++){
        for( n = G->adj[k]; n!= NULL; n = n->next){
            int cf = n->capacity - n->flow;
            if( cf > 0){
                Gf->adj[k] = NEWnode(n->w, cf, Gf->adj[k]);
            }
        }
    }
    
    return  Gf;
}

/************************** Procura em Largura Primeiro **************************/

/*
 branco: não visitado
 cinzento: já visitado, mas algum dos adjacentes não visitado ou procura em algum dos adjacentes não terminada
 preto: já visitado e procura nos adjacentes já terminada
 */

Vertex *bfs(Graph G, Vertex s, Vertex t, int *size, int *gray_size) {
    int *discovery;
    int *pi;
    Color *colors;
    int k;
    Vertex *path = NULL;
    Queue q = QUEUEcreate();
    Node n;
    Vertex v, w;
    
    discovery = malloc(G->V * sizeof(int));
    pi = malloc(G->V * sizeof(int));
    colors = malloc(G->V * sizeof(Color));
    
    
    for(k=0; k<G->V; k++){
        colors[k] = white;
        discovery[k] = INF;
        pi[k] = NIL;
    }
    colors[s] = gray;
    discovery[s] = 0;
    pi[s] = NIL;
    
    
    *size = 0;
    QUEUEenter(q,s);
    while(!QUEUEempty(q)){
        Vertex u = QUEUEdelete(q);
        
        if(u == t){
            *size = discovery[u]+1;
            path = malloc( (*size) * sizeof(Vertex));
            w = u;
            for (k= *size-1 ; k >= 0; k--) {
                path[k] = w;
                w = pi[w];
            }
            break;
        }
        
        for( n = G->adj[u]; n!= NULL; n = n->next){
            v = n->w;
            if(colors[v] == white){
                colors[v] = gray;
                discovery[v] = discovery[u] + 1;
                pi[v] = u;
                QUEUEenter(q,v);
            }
        }
        colors[u] = black;
    }
    
    *gray_size = 0;
    for (k=0; k<G->V; k++) {
        for( n = G->adj[k]; n!= NULL; n = n->next){
            Vertex v = n->w;
            
            if (colors[k] == white && colors[v] != white){
                (*gray_size) ++;
            }
        }
    }
    
    free(discovery);
    free(pi);
    free(colors);
    
    return path;
}


/************************** Algoritmo Ford-Fulkerson **************************/

int ford_fulkerson(Graph G, Vertex s, Vertex t) {
    Vertex *path;
    int path_size;
    int k;
    int cf = 0;
    int gray_size;
    Node n;
    Vertex u,v;
    Graph Gf;
    
    GRAPHclear_flow(G);
    
    while(1){
        Gf = GRAPHresidual(G);
        
        path = bfs(Gf, s, t, &path_size, &gray_size);
        
        if(path_size >= 2){
            Vertex u = path[0];
            Vertex v = path[1];
            for( n = G->adj[u]; n!= NULL; n = n->next){
                if(n->w == v){
                    cf = n->capacity;
                }
            }
        } else {
            return gray_size;
        }
        for (k=0; k<path_size-1; k++) {
            u = path[k];
            v = path[k+1];
            for( n = G->adj[u]; n!= NULL; n = n->next){
                if(n->w == v){
                    if(cf > n->capacity){
                        cf = n->capacity;
                    }
                }
            }
        }
        
        for (k=0; k<path_size-1; k++) {
            Vertex u = path[k];
            Vertex v = path[k+1];
            Node n1, n2;
            for(n1 = G->adj[u]; n1 != NULL; n1 = n1->next){
                if(n1->w == v){
                    n1->flow = n1->flow + cf;
                    break;
                }
            }
            for(n2 = G->adj[v]; n2 != NULL; n2 = n2->next){
                if(n2->w == u){
                    n2->flow = - n1->flow ;
                    break;
                }
            }
        }
        
    }
    return -1;
}

/************************** Função principal **************************/

int main(int argc, char *argv[]) {
    int N, P, k, i, j, h, n, c, cut, cut_index;
    int *criticos;
    int *cuts;
    Graph G;
    FILE *fp;
    
    if(argc <=1){
    	fp = stdin;
    } else {
    	fp = fopen(argv[1], "r");
    }
    
    fscanf(fp,"%d %d", &N, &P);
    
    
    G = GRAPHinit(N);
    
    for(k = 0; k < P; k++){
        Vertex v, w;
        fscanf(fp,"%d %d",&v,&w);
        GRAPHinsertE(G, v, w);
    }
    
    fscanf(fp,"%d", &h);
    
    
    for(k = 0; k< h; k++){
        fscanf(fp,"%d ", &n);
        criticos = malloc(n * sizeof(int));
        if (criticos == NULL){ printf("criticos"); return EXIT_FAILURE; }
        
        for(i = 0; i<n; i++){
            fscanf(fp,"%d ", &c);
            criticos[i] = c;
        }
        
        cuts = malloc( n*(n-1)/2 * sizeof(int));
        if (cuts == NULL){ printf("cuts"); return EXIT_FAILURE; }
        
        cut_index = 0;
        for (i = 0;  i< n; i++) {
            for(j = i+1; j< n; j++){
                cut = ford_fulkerson(G, criticos[i], criticos[j]);
                cuts[cut_index] = cut;
                cut_index++;
            }
        }
        
        cut = cuts[0];
        for (i=0; i< n*(n-1)/2; i++) {
            if (cut > cuts[i]) {
                cut = cuts[i];
            }
        }
        printf("%d\n",cut);
        
        free(criticos);
        free(cuts);
    }
    
    GRAPHdestroy(G);
    
    return 0;
}