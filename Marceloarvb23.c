#include "arvb23Marcelo.h"
#include <stdio.h>
#include <stdlib.h>

struct _arvb23
{
	int      kp, kg;   /* chaves: kp<kg, se kg existir. Se kg=-1, significa que ele não existe. */
	ArvB23  *pai;
	ArvB23  *esq;
	ArvB23  *meio;
	ArvB23  *dir;
};

ArvB23* a23Cria(int chave)
{
	ArvB23 *raiz = (ArvB23 *)malloc(sizeof(ArvB23));
	raiz->kp = chave;
	raiz->kg = -1;
	raiz->pai = raiz->esq = raiz->meio = raiz->dir = NULL;
	return raiz;
}

ArvB23* a23Libera(ArvB23* raiz)
{
	if (raiz == NULL)
		return NULL;
	if (raiz->esq)
		return a23Libera(raiz->esq);
	else if (raiz->meio)
		return a23Libera(raiz->meio);
	else
		return a23Libera(raiz->dir);
	free(raiz);
	return raiz;
}

ArvB23* a23Busca(ArvB23* raiz, int chave)
{
	if (raiz == NULL)
		return NULL;
	if (raiz->kg == chave || raiz->kp == chave)
		return raiz;
	if (chave < raiz->kp)
		return a23Busca(raiz->esq, chave);
	else if (chave>raiz->kg && raiz->kg!=-1)
		return a23Busca(raiz->dir, chave);
	else 
		return a23Busca(raiz->meio, chave);
}

static void reusa_no(ArvB23* no, ArvB23* esq, ArvB23* meio, ArvB23* dir, int kp, int kg) {
	no->kp = kp;
	no->kg = kg;
	no->esq = esq; 	no->meio = meio; no->dir = dir;
	if (no->esq)  no->esq->pai = no;
	if (no->meio) no->meio->pai = no;
	if (no->dir)  no->dir->pai = no;
}

ArvB23 *insereChave(ArvB23 *no, int chave, ArvB23 *men, ArvB23 *mai, ArvB23 *raiz)
{
	if (no->kg == -1)//so tem uma chave
	{
		if (chave > no->kp)
			no->kg = chave;
		else
		{
			no->kg = no->kp;
			no->kp = chave;
		}
		no->dir = no->meio;
		no->meio = mai;
		if (mai)
			mai->pai = no;
		no->esq = men;
		if (men)
			men->pai = no;
	}
	else if (chave < no->kp)
	{
		int chavekp = no->kp;
		ArvB23 *pai = no->pai;
		ArvB23 *novo = a23Cria(no->kg); //criando novo no para explosao
		novo->esq = no->meio;
		if (no->meio)
			no->meio->pai = novo;
		novo->meio = no->dir;
		if (no->dir)
			no->dir->pai = novo;
		//reusando o no explodido
		reusa_no(no, men, mai, NULL, chave, -1);
		if (pai) //caso o pai exista
			raiz = insereChave(pai, chavekp, no, novo, raiz);
		else
		{
			raiz = a23Cria(chavekp);
			no->pai = raiz;
			raiz->esq = no;
			novo->pai = raiz;
			raiz->meio = novo;
		}
	}
	else if (no->kg != -1 && chave<no->kg)
	{
		ArvB23 *novo = a23Cria(no->kg); //criando novo no para explosao
		ArvB23 *pai = no->pai;
		novo->esq = mai;
		if (mai)
			mai->pai = novo;
		novo->meio = no->dir;
		if (no->dir)
			no->dir->pai = novo;
		//reusando o no explodido
		reusa_no(no, no->esq, men, NULL, no->kp, -1);
		if (pai) //caso o pai exista
			raiz = insereChave(pai, chave, no, novo, raiz);
		else
		{
			raiz = a23Cria(chave);
			no->pai = raiz;
			raiz->esq = no;
			novo->pai = raiz;
			raiz->meio = novo;
		}
	}
	else
	{
		int chavekg = no->kg;
		ArvB23 *pai = no->pai;
		ArvB23 *novo = a23Cria(chave); //criando novo no para explosao
		novo->esq = men;
		if (men) 
			men->pai = no;
		novo->meio = mai;
		if (mai)
			mai->pai = no;
		//reusando o no explodido
		reusa_no(no, no->esq, no->meio, NULL, no->kp, -1);
		if (pai) //caso o pai exista
			raiz = insereChave(pai, chavekg, no, novo, raiz);
		else
		{
			raiz = a23Cria(chavekg);
			no->pai = raiz;
			raiz->esq = no;
			novo->pai = raiz;
			raiz->meio = novo;
		}
	}
	return raiz;
}

ArvB23* a23Insere(ArvB23* raiz, int chave)
{
	ArvB23 *p = raiz;
	while (p->esq != NULL)
	{
		if (chave < p->kp)
			p = p->esq;
		else if (chave < p->kg || p->kg == -1)
			p = p->meio;
		else
			p = p->dir;
	}
	raiz = insereChave(p, chave, NULL, NULL, raiz);
	return raiz;
}

ArvB23* retiraChave(ArvB23 *no, ArvB23 *filho, int chave, ArvB23 *raiz)
{
	if (no->kg != -1) //caso tenha duas chaves
	{
		if (chave != no->kg)
			no->kp = no->kg;
		no->kg = -1;
		if (no->esq)
		{
			no->kg = no->dir->kp;
			no->dir->kp = no->dir->kg;
			no->dir->kg = -1;
		}
	}
	else if (no->pai)
	{
		if (no->pai->esq == no) //caso seja filho a esq
		{
			if (no->pai->meio->kg != -1) //caso o irmao tenha 2 chaves
			{
				no->kp = no->pai->kp;
				no->pai->kp = no->pai->meio->kp;
				no->pai->meio->kp = no->pai->meio->kg;
				no->pai->meio->kg = -1;
				//checar se os filhos nao sao null para atualizar a ligacao com os pais
				no->esq = filho;
				if (filho)
					filho->pai = no;

				no->meio = no->pai->meio->esq;
				if (no->pai->meio->esq)
					no->pai->meio->esq->pai = no;

				no->pai->meio->esq = no->pai->meio->meio;
				if (no->pai->meio->meio)
					no->pai->meio->meio->pai = no->pai->meio->esq;

				no->pai->meio->meio = no->pai->meio->dir;
				if (no->pai->meio->dir)
					no->pai->meio->dir->pai = no->pai->meio->meio;

				no->pai->meio->dir = NULL;

			}
			else if (no->pai->kg != -1) //caso o pai tenha 2 chaves
			{
				no->kp = no->pai->kp;
				no->kg = no->pai->meio->kp;
				//checar se os filhos nao sao null para atualizar a ligacao com os pais
				no->esq = filho;
				if (filho)
					filho->pai = no->esq;

				no->meio = no->pai->meio->esq;
				if (no->pai->meio->esq)
					no->pai->meio->esq->pai = no->meio;

				no->dir = no->pai->meio->meio;
				if (no->pai->meio->meio)
					no->pai->meio->meio->pai = no->dir;

				no->pai->meio = no->pai->dir;
				if (no->pai->dir)
					no->pai->dir->pai = no->pai->meio;

				no->pai->kg = -1;

			}
			else //caso ninguem tenha 2 chaves 
			{
				no->kp = no->pai->kp;
				no->kg = no->pai->meio->kp;
				no->esq = filho;
				//checar se os filhos nao sao null para atualizar a ligacao com os pais
				if (filho)
					filho->pai = no;

				no->meio = no->pai->meio->esq;
				if (no->pai->meio->esq)
					no->pai->meio->esq->pai = no;

				no->dir = no->pai->meio->meio;
				if (no->pai->meio->meio)
					no->pai->meio->meio->pai=no;

				no->pai->kp = -1;
				no->pai->kg = -1;
				if (no->pai == raiz)
				{
					raiz = no->pai->esq;
					free(no->pai); 
					raiz->pai = NULL;
				}
				else
					raiz = retiraChave(no->pai, no->pai->esq, chave, raiz);
			}
		}
		else if (no->pai->meio == no) //se for filho do meio
		{
			if (no->pai->esq->kg != -1) //caso o irmao a esq tenha 2 chaves
			{
				no->kp = no->pai->kp;
				no->pai->kp = no->pai->esq->kg;
				no->pai->esq->kg = -1;
				//checar se os filhos nao sao null para atualizar a ligacao com os pais
				no->meio = filho;
				if (filho)
					filho->pai = no;

				no->esq = no->pai->esq->dir;
				if (no->pai->esq->dir)
					no->pai->esq->dir->pai = no;
			}
			else if (no->pai->kg != -1) //caso o pai tenha 2 chaves
			{
				no->pai->esq->kg = no->pai->kp;
				no->pai->kp = no->pai->kg;
				//checar se os filhos nao sao null para atualizar a ligacao com os pais
				no->pai->dir = filho;
				if (filho)
					filho->pai = no->pai->dir;

				no->pai->meio = no->pai->dir;
				if (no->pai->dir)
					no->pai->dir->pai = no->pai->meio;

				no->pai->kg = -1;
				free(no);
			}
			else //caso ninguem tenha 2 chaves
			{
				no->pai->esq->kg = no->pai->kp;
				no->pai->kp = -1;
				//checar se os filhos nao sao null para atualizar a ligacao com os pais
				no->pai->esq->dir = filho;
				if (filho)
					filho->pai = no->pai->esq->dir;

				no->pai->meio = NULL;
				if (no->pai == raiz)
				{
					raiz = no->pai->esq;
					free(no->pai);
					raiz->pai = NULL;
				}
				else
					raiz = retiraChave(no->pai, no->pai->esq, chave, raiz);
			}
		}
		else //se for filho a direita
		{
			if (no->pai->meio->kg != -1) //caso o irmao do meio tenha 2 chaves
			{
				no->kp = no->pai->kg;
				no->pai->kg = no->pai->meio->kg;
				no->pai->meio->kg = -1;
				//checar se os filhos nao sao null para atualizar a ligacao com os pais
				no->meio = filho;
				if (filho)
					filho->pai = no->meio;

				no->esq = no->pai->meio->dir;
				if (no->pai->meio->dir)
					no->pai->meio->dir->pai = no->esq;
			}
			else if (no->pai->meio->kg == -1)//caso o irmao do meio so tenha 1 chave
			{
				no->pai->meio->kg = no->pai->kg;
				no->pai->kg = -1;
				//checar se os filhos nao sao null para atualizar a ligacao com os pais
				no->pai->meio->dir = filho;
				if (filho)
					filho->pai = no->pai->meio->dir;

				no->pai->dir = NULL;
				free(no);

			}
		}
	}
	else //se for raiz
	{
		if (no->esq)
		{
			no->esq->kg = no->meio->kp;
			raiz = no->esq;
			no->esq->pai = NULL;
			free(no);
		}
		else
		{
			free(no);
			return NULL;
		}
	}
	return raiz;
}

ArvB23 *sucessordenaofolhas(ArvB23 *raiz,int k)
{
	if (raiz->kg != -1)
		return raiz;
	else
		return raiz->meio;
}
ArvB23* a23Retira(ArvB23* raiz, int chave)
{
	if (raiz)
	{
		ArvB23 *no = a23Busca(raiz, chave);
		if (no)
		{
			if (no->esq == NULL)
				raiz = retiraChave(no, NULL, chave, raiz);
			else
			{
				ArvB23 *suc = sucessordenaofolhas(no, chave);
				int aux;
				aux = no->kp;
				no->kp = suc->kp;
				suc->kp = aux;
				raiz = retiraChave(suc, NULL, chave, raiz);
			}
		}
		return raiz;
	}
	else
		return NULL;
}

void a23Mostra(char* titulo, ArvB23* raiz)
{
	if (titulo)
		printf("%s", titulo);
	if (raiz == NULL)
		return;
	printf("<");
	printf("%d", raiz->kp);
	if (raiz->kg == -1)
		printf(".");
	else
		printf("%d", raiz->kg);
	a23Mostra(NULL, raiz->esq);
	a23Mostra(NULL, raiz->meio);
	a23Mostra(NULL, raiz->dir);
	printf(">");
}