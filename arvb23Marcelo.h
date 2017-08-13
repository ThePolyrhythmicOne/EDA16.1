typedef struct _arvb23 ArvB23;

ArvB23* a23Cria(int chave);
ArvB23* a23Libera(ArvB23* raiz);

ArvB23* a23Busca(ArvB23* raiz, int chave);
ArvB23* a23Insere(ArvB23* raiz, int chave);
ArvB23* a23Retira(ArvB23* raiz, int chave);

ArvB23* a23Mostra(char* titulo, ArvB23* raiz);

