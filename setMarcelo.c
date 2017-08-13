#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "setMarcelo.h"

struct set{
	int set_size;  /* numero de elementos do conjunto */
	int vet_size;  /* numero de elementos no vetor de inteiros semi-positivos */
	unsigned int* vet; /* vetor de inteiros que armazena os bits que representam o conjunto */
};

/* cria um conjunto com n elementos */
Set* setCreate(int set_size)
{
	Set* set = (Set*)malloc(sizeof(Set));
	if (set != NULL){
		int i;
		set->set_size = set_size;
		set->vet_size = (int)ceil(set_size / (8.0*sizeof(unsigned int)));
		set->vet = (unsigned int*)malloc(set->vet_size*sizeof(unsigned int));
		for (i = 0; i < set->vet_size; i++)
			set->vet[i] = 0;
	}
	return set;
}

/* destroi (desaloca) o conjunto */
Set* setDestroy(Set* set) 
{
	if (set) {
		if (set->vet) free(set->vet);
		free(set);
	}
	return NULL;
}

/* cria uma copia do conjunto */
Set* setCopy(Set* set)
{
	Set *novo = setCreate(set->set_size);
	novo = set;
	return novo;
}

/* cria o complemento do conjunto */
Set* setComplement(Set* set)
{
	Set *novo = setCreate(set->set_size);
	int i = 0;
	while (i<=set->vet_size)
	{
		novo->vet[i] = ~set->vet[i];
		i++;
	}
	return novo;
}

/* insere o elemento i no conjunto */
void setInsert(Set *set, int i) 
{
	int byte = i / (8 * sizeof(unsigned int));
	int bit = i % (8 * sizeof(unsigned int));
    set->vet[byte] |= 1 << bit;
}

/* remove o elemento i do conjunto */
void setRemove(Set *set, int i)
{
	if (setIsMember(set, i))
	{
		int byte = i / (8 * sizeof(unsigned int));
		int bit = i % (8 * sizeof(unsigned int));
		set->vet[byte] &= ~(1 << bit);
	}
}


/* testa se o elemento i pertence ao conjunto */
int setIsMember(Set *set, int i)
{
	int byte = i / (8 * sizeof(unsigned int));
	int bit = i % (8 * sizeof(unsigned int));
	//return set->vet[byte] & 1 << bit;
	if (set->vet[byte] & 1 << bit)
		return 1;
	else
		return 0;
}


/* cria a uniao de dois conjunto que podem ter tamanhos diferentes */
/* naturalmente o numero de elementos do universo e' o do maior dos dois */
Set* setUnion(Set *set1, Set *set2)
{
	Set *novo;
	int i = 0;
	if (set1->set_size > set2->set_size)
	{
		novo = setCreate(set1->set_size);
		while (i<novo->vet_size)
		{
			if (set2->vet[i])
				novo->vet[i] = set1->vet[i] | set2->vet[i];
			else
				novo->vet[i] = set1->vet[i];
			i++;
		}
	}
	else
	{
		novo = setCreate(set2->set_size);
		while (i<novo->vet_size)
		{
			if (set1->vet[i])
				novo->vet[i] = set1->vet[i] | set2->vet[i];
			else
				novo->vet[i] = set2->vet[i];
			i++;
		}
	}
	return novo;
}

/* calcula a intersecao de dois conjuntos, a cardinalidade e' a do menor */
Set* setIntersection(Set *set1, Set *set2)
{
	Set *novo;
	int i = 0;
	if (set1->set_size < set2->set_size)
	{
		novo = setCreate(set1->set_size);
		while (i<set1->vet_size)
		{
			novo->vet[i] = set1->vet[i] & set2->vet[i];
			i++;
		}
	}
	else
	{
		novo = setCreate(set2->set_size);
		while (i<set2->vet_size)
		{
			novo->vet[i] = set1->vet[i] & set2->vet[i];
			i++;
		}
	}
	return novo;
}

/* calcula a diferenca de set1-set2. a cardinalidade e'a de set1. */
Set* setDifference(Set *set1, Set *set2)
{
	Set *novo;
	int i = 0;
	if (set1->set_size > set2->set_size)
	{
		novo = setCreate(set1->set_size);
		while (i<set1->vet_size)
		{
			if (set2->vet[i])
				novo->vet[i] = set1->vet[i] & ~set2->vet[i];
			i++;
		}
	}
	else
	{
		novo = setCreate(set2->set_size);
		while (i<set2->vet_size)
		{
			if (set1->vet[i])
				novo->vet[i] = set1->vet[i] & ~set2->vet[i];
			i++;
		}
	}
	return novo;
}



/* verifica se set2 e' um sub conjunto de set1 */
int setIsSubset(Set *set1, Set *set2) 
{
	/*int i = 0;
	while (setIsMember(set1, set2->vet[i])) <--problemas
		i++;
	if (i == set2->vet_size)
		return 1;
	else
		return 0;*/
	int i, j = 0;
	for (i = 0; i < set2->set_size; i++) 
	{
		if (setIsMember(set2, i)) 
			if (setIsMember(set1, i))
				j++;
	}
	if (j == setNumberOfElements(set2))
		return 1;
	else return 0;
}


/* verifica se dois conjuntos sao iguais */
int setIsEqual(Set *set1, Set *set2) 
{
	int i = 0;
	while (set1->vet[i] == set2->vet[i])
		i++;
	if (i == set1->vet_size)
		return 1;
	else
		return 0;
}

/* informa a cardinaidade do conjunto */
int setNumberOfElements(Set* set)
{
	int i, j = 0;
	for (i = 0; i < set->set_size; i++)
		if (setIsMember(set, i))
			j++;
	return j;
}

/* mostra os elementos do conjunto */
void setShow(char* title, Set* set)
{
	int i, first = 1;
	printf("%s={", title);
	for (i = 0; i <= set->set_size; i++) {
		if (setIsMember(set, i)) {
			if (first) {
				printf("%d", i);
				first = 0;
			}
			else
				printf(",%d", i);
		}
	}
	printf("}\n");
}