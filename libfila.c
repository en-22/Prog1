#include <stdio.h>
#include <stdlib.h>
#include "libfila.h"

fila_t * cria_fila (){
	fila_t* fila;	
	if(!(fila = malloc(sizeof(fila_t))))
		return NULL;
	fila->tamanho = 0;
	return fila;
}

fila_t * destroi_fila (fila_t *f){
	nodo_f_t* nodoaux;
	int i;
	for(i=0; i<f->tamanho; i++){
        nodoaux = f->ini;
		f->ini = nodoaux->prox;
		free(nodoaux);
	}
	free(f);
	return NULL;
}

int vazia_fila(fila_t *f){
	if(f->tamanho == 0)
		return 1;
	return 0;
}

int tamanho_fila (fila_t *f){
	return f->tamanho;
}

int insere_fila (fila_t *f, int elemento){
	nodo_f_t* nodo;
	if(!(nodo = malloc(sizeof(nodo_f_t))))
		return 0;
	if(f->tamanho == 0){       
		nodo->elem = elemento;
		f->ini = nodo;
		f->fim = nodo;
		f->tamanho++;
		return 1;
	}   
	nodo->elem = elemento;
	(f->fim)->prox = nodo;
	f->fim = nodo;
	f->tamanho++;
	return 1;	
}

int retira_fila (fila_t *f, int *elemento){
	if(f->tamanho == 0)
			return 0;
	nodo_f_t *nodoaux;
	*elemento = (f->ini)->elem;
	nodoaux = f->ini;
	f->ini = (f->ini)->prox;
	free(nodoaux);
	f->tamanho--;
	return 1;
}

void imprime_fila (fila_t *f){
    nodo_f_t* nodo;
	int i;
	nodo = f->ini;
	for(i = 0; i < f->tamanho; i++){
		printf("%d ", nodo->elem);
        nodo = nodo->prox;
	}
}
