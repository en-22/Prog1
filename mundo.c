#include <stdio.h>
#include <stdlib.h>
#include "liblef.h"
#include "libconjunto.h"
#include "libfila.h"
#include <math.h>
#include <time.h>

#define FIMTEMPO 34944
#define CHEGADA 0
#define SAIDA 1
#define MISSAO 2
#define FIM 3

struct Heroi{
    int id;
    int paciencia;
    int idade;
    int exp;
    conjunto_t *habilidades;
};typedef struct Heroi H;

struct Local{
    int id;
    int maxLocal;
    conjunto_t *publico;
    fila_t *fila;
    int xy[2];
};typedef struct Local L;

struct Mundo{
    int tempoAtual;
    int numHerois;
    int numLocais;
    int tamMundo;
    conjunto_t *hablidadesAll;
    H Herois[10*5];
    L Locais[10*5/6];
};typedef struct Mundo M;

int max(int x, int y){
    if(x > y){
        return x;
    }else{
        return y;
    }
}

int sortear(int a, int b){
    return rand() % (b+1-a) + a;
}
       
void criarheroi(M* m, int id, lef_t* LE){
    (m->Herois[id]).id = id;
    (m->Herois[id]).paciencia = sortear(0, 100);
    (m->Herois[id]).idade = sortear(18, 100);
    (m->Herois[id]).exp = 0;    
    (m->Herois[id]).habilidades = cria_subcjt_cjt(m->hablidadesAll, sortear(2, 5));
    
    evento_t evento;
    evento.tempo = sortear(0,96*7);
    evento.tipo = CHEGADA;
    evento.dado1 = id;
    evento.dado2 = sortear(0, m->numLocais-1);
    adiciona_ordem_lef(LE, &evento);
}

void criarlocal(M* m, int id){
    (m->Locais[id]).id = id;
    (m->Locais[id]).maxLocal = sortear(5, 30);
    (m->Locais[id]).publico = cria_cjt ((m->Locais[id]).maxLocal);
    (m->Locais[id]).fila = cria_fila();
    (m->Locais[id]).xy[0] = sortear(0, m->tamMundo - 1);
    (m->Locais[id]).xy[1] = sortear(0, m->tamMundo - 1);
}

M* criarmundo(lef_t* lef){
    int i;
    M* m;
    if(!(m = malloc(sizeof(M))))
        return NULL;
    int numHab = 10; 
    m->tempoAtual = 0;
    m->numHerois = numHab*5;
    m->numLocais = m->numHerois/6;
    m->tamMundo = 2000;
    m->hablidadesAll = cria_cjt(numHab);

    for(i = 0;i < numHab; i++)
       insere_cjt(m->hablidadesAll, sortear(0, numHab - 1));    

    for(i = 0;i < m->numHerois; i++)
        criarheroi(m,i,lef);

    for(i = 0; i < m->numLocais; i++)
        criarlocal(m,i);

    return m;
}


void chegada(M* m, int idH, int idL, lef_t* LE){
    evento_t evento;
    
    printf("%6d: CHEGA HEROI %2d LOCAL %d (%2d/%2d), ", m->tempoAtual, idH, idL, cardinalidade_cjt((m->Locais[idL]).publico), (m->Locais[idL]).maxLocal);

    if(cardinalidade_cjt((m->Locais[idL]).publico) >= (m->Locais[idL]).maxLocal)/*Se o local esta lotado*/
        if((m->Herois[idH]).paciencia/4 - tamanho_fila((m->Locais[idL]).fila) > 0){/*Se o herói entra na fila*/
            insere_fila((m->Locais[idL]).fila, idH);
            printf("FILA %d\n", tamanho_fila((m->Locais[idL]).fila));
        }
        else{/*Se o herói não entra na fila, cria um evento de saida*/
            printf("DESISTE\n");
            evento.tempo = m->tempoAtual;
            evento.tipo = SAIDA;
            evento.dado1 = idH;
            evento.dado2 = idL;
            adiciona_ordem_lef(LE, &evento);
            }
    else{/*Se o local não esta lotado, calcula o tempo permanencia no local e cria um evento de saida*/
            printf("ENTRA\n");
            insere_cjt((m->Locais[idL]).publico, idH);
            int TPL = max(1, m->Herois[idH].paciencia/10 + sortear(-2,6));
            evento.tempo = m->tempoAtual + TPL;
            evento.tipo = SAIDA;
            evento.dado1 = idH;
            evento.dado2 = idL;
            adiciona_ordem_lef(LE, &evento);
    }
}

void saida(M* m, int idH, int idL, lef_t* LE){
    evento_t evento;
    int heroiFila;
    int novoLocal = sortear(0, m->numLocais - 1);
    int distCartesiana = sqrt( pow((m->Locais[idL]).xy[0] - (m->Locais[novoLocal]).xy[0], 2) + pow((m->Locais[idL]).xy[1] - (m->Locais[novoLocal]).xy[1], 2) );
    int tdl = distCartesiana/(100-max(0, m->Herois[idH].idade - 40));

    printf("%6d: SAIDA HEROI %2d LOCAL %d (%2d/%2d)", m->tempoAtual, idH, idL, cardinalidade_cjt((m->Locais[idL]).publico), (m->Locais[idL]).maxLocal);

    if(pertence_cjt((m->Locais[idL]).publico, idH)){
        retira_cjt((m->Locais[idL]).publico, idH);/*Retira o Heroi idH do local, apenas se ele estava dentro*/
        if(!vazia_fila((m->Locais[idL]).fila)){/*Se a fila do local não está fazia, e o herói idH estava no local, o heroi no incio da fila cria um evento para entrar no local*/         
            retira_fila((m->Locais[idL]).fila, &heroiFila);
            printf(", REMOVE FILA HEROI %2d", heroiFila);
            evento.tempo = m->tempoAtual;
            evento.tipo = CHEGADA;
            evento.dado1 = heroiFila;
            evento.dado2 = idL;
            adiciona_inicio_lef(LE, &evento);
        }
    }/*Cria um novo evento de chegada para o herói que saiu*/
        evento.tempo = m->tempoAtual + tdl/15;
        evento.tipo = CHEGADA;
        evento.dado1 = idH;
        evento.dado2 = novoLocal;
        adiciona_ordem_lef(LE, &evento);

        printf("\n");
}

int testarequipes(M* m, conjunto_t *missao, int idmissao){
	int i;
	int num_equipe = -1;
	int menorcard = m->numHerois;
	int id, poder;
	for(i = 0; i < m->numLocais; i++){/*Analisa cada local do mundo*/
		conjunto_t* uniao = cria_cjt(10);
		inicia_iterador_cjt((m->Locais[i]).publico);
        while(incrementa_iterador_cjt((m->Locais[i]).publico, &id)){/*Olha cada heroi de um local i*/
            inicia_iterador_cjt ((m->Herois[id]).habilidades);
            while(incrementa_iterador_cjt((m->Herois[id]).habilidades, &poder))/*Olha cada poder de cada heroi no local i de insere na uniao*/
                insere_cjt(uniao, poder);
		}
        printf("%6d: MISSAO %2d HAB_EQL%2d:", m->tempoAtual, idmissao, i);
        imprime_cjt(uniao);

		if(contido_cjt(missao, uniao) && menorcard > cardinalidade_cjt((m->Locais[i]).publico)){/*Se a uniao possui as habilidades requeridas e tem card menor que a da uniao interior, o id desse local é o novo num_equipe*/
			num_equipe = i;
			menorcard = cardinalidade_cjt((m->Locais[i]).publico);
		}
		uniao = destroi_cjt(uniao);
	}
    printf("%6d: MISSAO %2d ", m->tempoAtual, idmissao);
    if(num_equipe!= -1){
        printf("HAB_EQS%2d:", num_equipe);
        imprime_cjt((m->Locais[num_equipe]).publico);
    }else{
        printf("IMPOSSIVEL\n");
    }
	return num_equipe;
}

void missao(M* m, int idmissao, lef_t* LE){
    int id;
    int id_local = -1;

    conjunto_t* missao = cria_subcjt_cjt(m->hablidadesAll, sortear(3,6));
    printf("%6d: MISSAO %2d HAB_REQ ", m->tempoAtual, idmissao);
    imprime_cjt(missao);
    id_local = testarequipes(m, missao, idmissao);
    destroi_cjt(missao);

    if(id_local != -1){
        inicia_iterador_cjt((m->Locais[id_local]).publico);
	while(incrementa_iterador_cjt((m->Locais[id_local]).publico, &id))/*Atualiza exp dos herois da equipe*/
        (m->Herois[id]).exp++;
    }else{/*Se a a missão é impossível recria a missao idmissao em outro tempo*/
        evento_t evento;
        evento.tempo = sortear(m->tempoAtual, FIMTEMPO);
        evento.tipo = MISSAO;
        evento.dado1 = idmissao;
        adiciona_ordem_lef(LE, &evento);
    }
}

void criarmissoes(M* m, lef_t* LE){
    int i;
    evento_t evento;
    for(i = 0; i < FIMTEMPO/100; i++){
        evento.tempo = sortear(0, FIMTEMPO);
        evento.tipo = MISSAO;
        evento.dado1 = i;
        adiciona_ordem_lef(LE, &evento);
    }
}
void definirfim(lef_t* LE){
    evento_t evento;
    evento.tempo = FIMTEMPO;
    evento.tipo = FIM;
    adiciona_ordem_lef(LE, &evento);
}
void destruirmundo(M* m, lef_t* lef){
    int i;
    destroi_lef(lef);
    for(i = 0; i < m->numLocais; i++)
        destroi_fila((m->Locais[i]).fila);
    for(i = 0; i < m->numLocais; i++)
        destroi_cjt((m->Locais[i]).publico);
    for(i = 0; i < m->numHerois; i++)
        destroi_cjt((m->Herois[i]).habilidades);
    destroi_cjt(m->hablidadesAll);
    free(m);
}

int main(){
    srand(time(NULL));
    int i; 
    evento_t *primeEvento;

    lef_t* listaEventos = cria_lef();
    M* mundo = criarmundo(listaEventos);   
    criarmissoes(mundo, listaEventos);
    definirfim(listaEventos);

    while(mundo->tempoAtual != FIMTEMPO){
        primeEvento = obtem_primeiro_lef(listaEventos);
        mundo->tempoAtual = primeEvento->tempo;
        switch(primeEvento->tipo){
            case CHEGADA:
                chegada(mundo, primeEvento->dado1, primeEvento->dado2, listaEventos);
            break;
            case SAIDA:
                saida(mundo, primeEvento->dado1, primeEvento->dado2, listaEventos);
            break;
            case MISSAO:
                missao(mundo, primeEvento->dado1, listaEventos);
            break;
            case FIM:
                printf("%6d: FIM\n", FIMTEMPO);
                for(i = 0; i < mundo->numHerois; i++)
                    printf("HEROI %2d EXPERIENCIA %2d\n", i, (mundo->Herois[i]).exp );
            break;
        }    
        free(primeEvento);
    }
    destruirmundo(mundo, listaEventos);
	return 0;
}
