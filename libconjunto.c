#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "libconjunto.h"

conjunto_t *cria_cjt (int max){
    conjunto_t* cjt_ptr;	
    if(!(cjt_ptr = malloc(sizeof(conjunto_t))))
        return NULL;
    if(!(cjt_ptr->v = malloc(sizeof(int)*max)))
        return NULL;
    cjt_ptr->max = max;
    cjt_ptr->card = 0;	
    return cjt_ptr;
}

conjunto_t *destroi_cjt (conjunto_t *c){
	free(c->v);
	free(c);
	return NULL;
}

int vazio_cjt (conjunto_t *c){
	if(c->card == 0)
		return 1;
	return 0;
}

int cardinalidade_cjt (conjunto_t *c){
	return c->card;
}

int insere_cjt (conjunto_t *c, int elemento){
	if(c->card + 1 <= c->max){
		if(!(pertence_cjt(c, elemento))){
			c->card++;
			int j;
			j = c->card - 1;
			while((j > 0) && (elemento < *(c->v + j-1))){
				*(c->v+j) = *(c->v+j-1);
				j = j - 1;
			}
			*(c->v + j) = elemento;
		}
		return 1;
	}
	return 0;
}

int posicao_no_vetor(conjunto_t *c, int elemento){
	int x = 0;
	while(*(c->v+x) != elemento)
		x++;
	return x;
}

int retira_cjt (conjunto_t *c, int elemento){
	if(!(pertence_cjt(c, elemento)))
		return 0;
	int x;
	for(x = posicao_no_vetor(c, elemento); x < c->card-1; x++){
		*(c->v + x) = *(c->v + x+1);
	}
	c->card--;
	return 1;
}

int pertence_cjt (conjunto_t *c, int elemento){
	int x;
	for(x = 0; x < c->card; x++){
		if(*(c->v + x) == elemento)
			return 1;
	}
	return 0;
}

int contido_cjt (conjunto_t *c1, conjunto_t *c2){
	int x;
	for(x = 0; x < c1->card; x++)
		if(!(pertence_cjt(c2, *(c1->v + x))))
			return 0;
	return 1;
}

int sao_iguais_cjt (conjunto_t *c1, conjunto_t *c2){
	if(c1->card == c2->card && contido_cjt(c1, c2) && contido_cjt(c2, c1))
		return 1;
	return 0;
}

conjunto_t *diferenca_cjt (conjunto_t *c1, conjunto_t *c2){
	conjunto_t* c3 = cria_cjt(c1->max);
	if(c3 == NULL)
		return NULL;
	if(sao_iguais_cjt(c1, c2))
	 	return c3;
	int x;
	for(x = 0; x < c1->card; x++)
		if(!(pertence_cjt(c2, *(c1->v + x))))
			insere_cjt(c3, *(c1->v + x));
	return c3;
}

conjunto_t *interseccao_cjt (conjunto_t *c1, conjunto_t *c2){
	if(sao_iguais_cjt(c1, c2))
	 	return copia_cjt(c1);
	conjunto_t* c3 = cria_cjt(c1->max);
	if(c3 == NULL)
		return NULL;
	int x;
	for(x = 0; x < c1->card; x++)
		if(pertence_cjt(c2, *(c1->v + x)))
			insere_cjt(c3, *(c1->v + x));
	return c3;
}

conjunto_t *uniao_cjt (conjunto_t *c1, conjunto_t *c2){
	if(sao_iguais_cjt(c1, c2))
		return copia_cjt(c1);
	conjunto_t* c3 = cria_cjt(c1->max + c2->max);
	if(c3 == NULL)
		return NULL;
	int x;
	for(x = 0; x < c1->card; x++)
		insere_cjt(c3, *(c1->v + x));
	for(x = 0; x < c2->card; x++)
		insere_cjt(c3, *(c2->v + x));
	return c3;
}

conjunto_t *copia_cjt (conjunto_t *c){
	conjunto_t* c2 = cria_cjt(c->max);
	if(c2 == NULL)
		return NULL;
	int x;
	for(x = 0; x < c->card; x++)
		insere_cjt(c2, *(c->v + x));
	return c2;
}

conjunto_t *cria_subcjt_cjt (conjunto_t *c, int n){
	if(n == c->card || c->card == 0)
		return copia_cjt(c);
	conjunto_t* subc = cria_cjt(c->max);
	while(n > subc->card){
		insere_cjt(subc, *(c->v + (rand() % c->card) ));
	}
	return subc;	
}

void imprime_cjt (conjunto_t *c){
	if(c->card > 0){
		int x;
		for(x = 0; x < c->card-1; x++){
			printf("%d ",*(c->v + x));
		}
		printf("%d\n", *(c->v + c->card-1));
	}else{
		printf("conjunto vazio\n");
	}
}


void inicia_iterador_cjt (conjunto_t *c){
	c->ptr = 0;
}

int incrementa_iterador_cjt (conjunto_t *c, int *ret_iterador){
	if(c->ptr < c->card){
		*ret_iterador = *(c->v+c->ptr);
		c->ptr++;
		return 1;
	}
	return 0;
}

int retira_um_elemento_cjt (conjunto_t *c){
	int x = *(c->v+(rand() % (c->card - 1)));
	retira_cjt(c, x);
	return x;
}

