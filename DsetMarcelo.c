#include <stdio.h>
#include <stdlib.h>
#include "DsetMarcelo.h"

struct _dset{
	int  n;
	int* vet;
};

DSet* dsCreate(int n){
	int i;
	DSet* dset = (DSet*)malloc(sizeof(DSet));
	dset->vet = (int*)malloc(n * sizeof(int));
	dset->n = n;
	for (i = 0; i < n; i++){
		dset->vet[i] = -1;
	}
	return dset;
}

int dsFind(DSet* dset, int elem)
{
	if (dset->vet[elem] == -1)
		return elem;
	return dsFind(dset, dset->vet[elem]);
}

int dsUnion(DSet* dset, int elem1, int elem2)
{
	int r1 = dsFind(dset, elem1), r2 = dsFind(dset, elem2);
	if (r1 != r2)
		dset->vet[r1] = r2;
	return r2;
}

int dsNSets(DSet* dset) 
{
	int i, cont = 0;
	for (i = 0; i < dset->n; i++)
		if (dset->vet[i] == -1)
			cont++;
	return cont;
}

void dsShow(char* title, DSet* dset){
	int i;
	printf("%s", title);
	for (i = 0; i < dset->n; i++)
		printf("%d ", dset->vet[i]);
	printf("\n");

}