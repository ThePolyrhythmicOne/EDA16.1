#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <assert.h>
#include "grafoMarcelo.h"


typedef struct _viz Viz;
struct _viz {
	int noj;
	float peso;
	Viz*  prox;
};

struct _grafo {
	int nv;           /* numero de nos ou vertices */
	int na;           /* numero de arestas */
	char orientado;   /* 0 = nao orientado */
	Viz** viz;         /* viz[i] aponta para a lista de arestas vizinhas do no i */
};


/* aux para fila e pilha */
typedef struct _sq SQ;
struct _sq {
	int info;
	SQ* prox;
};


static SQ* push(SQ* stack, int info)
{
	SQ* novo = (SQ*)malloc(sizeof(SQ));
	assert(novo);
	novo->info = info;
	novo->prox = stack;
	return novo;
}

static SQ* queue(SQ* queue, int info)
{
	SQ* novo = (SQ*)malloc(sizeof(SQ));
	SQ* aux = queue;
	assert(novo);
	novo->info = info;
	novo->prox = NULL;
	if (!queue){
		return novo;
	}
	while (aux->prox){
		aux = aux->prox;
	}
	aux->prox = novo;
	return queue;
}

static SQ* pop(SQ* stack, int* popped_info)
{
	SQ* libera;
	if (!stack){
		*popped_info = -1;
		return NULL;
	}
	*popped_info = stack->info;
	libera = stack;
	stack = stack->prox;
	free(libera);
	return stack;
}



static Viz* criaViz(Viz* head, int noj, float peso)
{
	Viz* no = (Viz*)malloc(sizeof(Viz));
	assert(no);
	no->noj = noj;
	no->peso = peso;
	no->prox = head;
	return no;
}


/* implemente */

Grafo* grafoCria(int nv, int na, int orientado) 
{

	int i;
	Grafo* g = (Grafo *)malloc(sizeof(Grafo));
	g->orientado = orientado;
	g->nv = nv;
	g->na = na;
	g->viz = (Viz **)malloc(sizeof(Viz *) * nv);
	for (i = 0; i < nv; i++)
		g->viz[i] = NULL;
	return g;
}

Grafo* grafoLe(char* filename)
{
	FILE *arq = fopen(filename, "rt");
	int nv, na, orientacao, no1, no2, i = 0;
	float peso;
	Grafo* novo;

	fscanf(arq, "%d %d %d", &nv, &na, &orientacao);
	novo = grafoCria(nv, na, orientacao);
	assert(novo);
	while (fscanf(arq, "%d %d %f", &no1, &no2, &peso) == 3) {
		novo->viz[no1] = criaViz(novo->viz[no1], no2, peso);
		if (!orientacao)
			novo->viz[no2] = criaViz(novo->viz[no2], no1, peso);
	}
	return novo;
}

Grafo*   grafoLibera(Grafo* grafo) 
{
	if (grafo) {
		int i = 0;
		Viz* no, *aux;
		for (i = 0; i < grafo->nv; i++){
			no = grafo->viz[i];
			while (no){
				aux = no->prox;
				free(no);
				no = aux;
			}
		}
		free(grafo->viz);
		free(grafo);
	}
	return NULL;
}

void grafoMostra(char* title, Grafo * grafo)
{
	int i;
	if (title)
		printf("%s", title);
	if (grafo)
	{
		printf("NV: %d, NA: %d, %s\n", grafo->nv, grafo->na, (grafo->orientado != 0 ? "orientado" : "nao orientado"));
		for (i = 0; i < grafo->nv; i++){
			Viz* viz = grafo->viz[i];
			printf("[%d]->", i);
			while (viz)
			{
				printf("{%d, %g}->", viz->noj, viz->peso);
				viz = viz->prox;
			}
			printf("NULL\n");
		}
	}
}

void grafoPercorreProfundidade(Grafo * grafo, int no_inicial)
{
	int *cor = (int *)malloc(grafo->nv*sizeof(int)), i, no;
	for (i = 0; i<grafo->nv; i++)
		cor[i] = 0;
	SQ *stack = NULL;
	stack = push(stack, no_inicial);
	cor[no_inicial] = 1;
	while (stack)
	{  
		stack = pop(stack, &no);
		Viz *viz = grafo->viz[no];
		printf("%d ", no);
		while (viz)
		{
			if (!cor[viz->noj])
			{
				stack = push(stack, viz->noj);
				cor[viz->noj] = 1;
			}
			viz = viz->prox;
		}
	}
}

void   grafoPercorreLargura(Grafo * grafo, int no_inicial)
{
	int *cor = (int *)malloc(grafo->nv*sizeof(int)), i, no;
	for (i = 0; i<grafo->nv; i++)
		cor[i] = 0;
	SQ *stack = NULL;
	stack = queue(stack, no_inicial);
	cor[no_inicial] = 1;
	while (stack)
	{
		stack = pop(stack, &no);
		Viz *viz = grafo->viz[no];
		printf("%d ", no);
		while (viz)
		{
			if (!cor[viz->noj])
			{
				stack = queue(stack, viz->noj);
				cor[viz->noj] = 1;
			}
			viz = viz->prox;
		}
	}
}

void grafoMostraDijkstra(Grafo* grafo, int source)
{
	int no = source, i, tovisit = grafo->nv - 1;
	float dmin;
	float *peso = (float *)malloc(grafo->nv*sizeof(float));
	int *cor = (int *)malloc(grafo->nv*sizeof(int));
	for (i = 0; i<grafo->nv; i++)
	{
		peso[i] = FLT_MAX;
		cor[i] = 0;
	}
	peso[no] = 0;
	cor[no] = 1;
	while (tovisit)
	{
		dmin = FLT_MAX;
		Viz *viz = grafo->viz[no];
		while (viz)
		{
			float dist = peso[no] + viz->peso;
			if (!cor[viz->noj])
				if (dist<peso[viz->noj])
					peso[viz->noj] = dist;
			viz = viz->prox;
		}
		for (i = 0; i<grafo->nv; i++)
		{
			if (!cor[i] && peso[i]<dmin)
			{
				dmin = peso[i];
				no = i;
			}
		}
		cor[no] = 1;
		tovisit--;
	}
	for (i = 0; i < grafo->nv; i++)
		printf("No %d - Peso:%g\n", i,peso[i]);
}

typedef struct Aresta{
	int noi, nof;
	float peso;
}aresta;

typedef struct Dset{
	int n;
	int *v;
}dset;

int dsFind(dset *d, int elem1)
{
	if (d->v[elem1] == -1)
		return elem1;
	else
		return dsFind(d, d->v[elem1]);
}

int compara(const void *p1, const void *p2)
{
	aresta *pa = (aresta*)p1;
	aresta *pb = (aresta*)p2;
	if (pa->peso < pb->peso)
		return -1;
	else if (pa->peso == pb->peso)
		return 0;
	else return 1;
}

void dsUnion(dset *d, int elem1, int elem2)
{
	int r1 = dsFind(d, elem1), r2 = dsFind(d, elem2);
	if (r1 != r2)
		d->v[r1] = r2;
}
void   grafoMostraKruskal(Grafo* grafo)
{
	aresta *ares = (aresta*)malloc(grafo->na*sizeof(aresta));
	dset *d = (dset*)malloc(sizeof(dset));
	int i, j=0;
	d->v = (int*)malloc(grafo->nv*sizeof(int));
	for (i = 0; i < grafo->nv; i++)
		d->v[i] = -1;

	for (i = 0; i < grafo->nv; i++)
	{
		Viz *viz = grafo->viz[i];
		while (viz)
		{
			if (i > viz->noj)
			{
				ares[j].noi = i;
				ares[j].nof = viz->noj;
				ares[j].peso = viz->peso;
				j++;
			}
			viz = viz->prox;
		}
	}

	qsort(ares, grafo->na, sizeof(aresta), compara);

	for (i = 0; i < grafo->nv; i++)
	{
		int r1 = dsFind(d, ares[i].noi), r2 = dsFind(d, ares[i].nof);
		if (r1 != r2)
		{
			printf("noi:%d nof:%d peso:%g\n", ares[i].noi, ares[i].nof, ares[i].peso);
			dsUnion(d, ares[i].noi, ares[i].nof);
		}
	}
}