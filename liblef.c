#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "liblef.h"

lef_t * cria_lef (){
	lef_t* lef;	
	if(!(lef = malloc(sizeof(lef_t))))
		return NULL;
    lef->Primeiro = NULL;
	return lef;
}

lef_t *destroi_lef (lef_t *l){
    nodo_lef_t* nodoaux = l->Primeiro;
    while(l->Primeiro != NULL){
        nodoaux = l->Primeiro;
        l->Primeiro = nodoaux->prox;
        free(nodoaux->evento);
        free(nodoaux);
    }
    free(l);
    return 0;
}

int adiciona_inicio_lef (lef_t *l, evento_t *evento){
	nodo_lef_t* nodo;
	if(!(nodo = malloc(sizeof(nodo_lef_t))))
		return 0;
    evento_t* evento_novo;
    if(!(evento_novo = malloc(sizeof(evento_t))))
		return 0;
    memcpy(evento_novo, evento, sizeof(evento_t));
    nodo->evento = evento_novo;
	if(l->Primeiro == NULL){
		l->Primeiro = nodo;
        l->Primeiro->prox = NULL;
		return 1;
	}
	nodo_lef_t* nodoaux = l->Primeiro;
	l->Primeiro = nodo;
	l->Primeiro->prox = nodoaux;
	return 1;
}

int adiciona_ordem_lef  (lef_t *l, evento_t *evento){
	if(l->Primeiro == NULL || l->Primeiro->evento->tempo > evento->tempo){
		adiciona_inicio_lef(l, evento);
		return 1;
	}
    
    nodo_lef_t* nodoaux = l->Primeiro;
	nodo_lef_t* novonodo;
    evento_t* evento_novo;
	while( nodoaux->prox != NULL && nodoaux->prox->evento->tempo <= evento->tempo){
		nodoaux = nodoaux->prox;		
	}   
	if(!(novonodo = malloc(sizeof(nodo_lef_t))))
		return 0;
    if(!(evento_novo = malloc(sizeof(evento_t))))
		return 0;
    memcpy(evento_novo, evento, sizeof(evento_t));
	novonodo->evento = evento_novo;
    novonodo->prox = nodoaux->prox;
	nodoaux->prox = novonodo;
	return 1;
}

evento_t *obtem_primeiro_lef (lef_t *l){
    evento_t* evento = l->Primeiro->evento;
    nodo_lef_t* nodo = l->Primeiro; 
    l->Primeiro = l->Primeiro->prox;
    free(nodo);
    return evento;
}
