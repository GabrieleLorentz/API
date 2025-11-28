#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//lista macchine
typedef struct nodo_a{
    int dist;
    struct nodo_a* next;
}lista;
//per percorso
typedef struct nodo_b{
    int stazione;
    int max;
    int tappa;
    struct nodo_b* next;
    struct nodo_b* pre;
}lista2;
//per albero
typedef struct nodo_t{
    int stazione;
    int n;
    int max;
    struct nodo_t* dx;
    struct nodo_t* sx;
    struct nodo_a* macchine;
}tree;

tree* aggiungiStazione(tree* t, int stazione);
tree* demolisciStazione(tree* t, int stazione);
tree* aggiungiAuto(tree* t, int stazione, int dist);
tree* rottamaAuto(tree* t, int stazione, int dist);
lista* pianificaPercorso1(tree* t, lista* percorso, int start, int finish);
lista* pianificaPercorso2(tree* t, lista* percorso, int start, int finish);
tree* trovaMin(tree* t);
tree* stazioneSuccessiva(tree* t, int x);
lista2* stazioniPercorso(tree* t, lista2* x, int finish, int start);
tree* cerca(tree* t, int stazione);
void eliminaLista(lista **x);

int main() {
    tree* root = NULL;
    char comando[18];
    int stazione;
    int dist;
    int start, finish;
    int y = -1;
    while(scanf("%s", comando) != EOF && y != 0) {
        if(strcmp(comando, "aggiungi-stazione") == 0){
            y = scanf("%d", &stazione);
            root = aggiungiStazione(root, stazione);
        }
        else if(strcmp(comando, "demolisci-stazione") == 0){
            y = scanf("%d", &stazione);
            root = demolisciStazione(root, stazione);
        }
        else if(strcmp(comando, "aggiungi-auto") == 0){
            y = scanf("%d%d", &stazione, &dist);
            root = aggiungiAuto(root, stazione, dist);
        }
        else if(strcmp(comando, "rottama-auto") == 0){
            y = scanf("%d%d", &stazione, &dist);
            root = rottamaAuto(root, stazione, dist);
        }
        else if(strcmp(comando, "pianifica-percorso") == 0){
            y = scanf("%d%d", &start, &finish);
            lista* percorso = malloc(sizeof(struct nodo_a));
            percorso->dist = finish;
            percorso->next = NULL;
            if(start < finish) {
                percorso = pianificaPercorso1(root, percorso, start, finish);
            }
            else{
                percorso = pianificaPercorso2(root, percorso, start, finish);
            }
            if(start == percorso->dist){
                int x = 0;
                while (percorso != NULL){
                    if(x == 0){
                        printf("%d", percorso->dist);
                        x = 1;
                    }else{
                        printf(" %d", percorso->dist);
                    }
                    percorso = percorso->next;
                }
                printf("\n");
            }
        }
    }
    return 0;
}

tree* aggiungiStazione(tree* t, int stazione){
    if(t!=NULL){
        if(stazione == t->stazione){
            printf("non aggiunta\n");
        }
        else if(stazione > t->stazione){
            t->dx = aggiungiStazione(t->dx, stazione);
        }
        else if(stazione < t->stazione){
            t->sx = aggiungiStazione(t->sx, stazione);
        }
    }
    else{
        t = (tree*)malloc(sizeof(struct nodo_t));
        t->dx = NULL;
        t->sx = NULL;
        t->stazione = stazione;
        t->macchine = NULL;
        printf("aggiunta\n");

        int dist;
        int y = -1;
        t->max = 0;
        y = scanf("%d", &t->n);
        lista* elem = NULL;
        for(int i=0; i < t->n && y != 0; i++) {
            y = scanf("%d", &dist);
            elem = malloc(sizeof(struct nodo_a));
            elem->dist = dist;
            elem->next = t->macchine;
            t->macchine = elem;
            if(dist > t->max){
                t->max = dist;
            }
        }
    }
    return t;
}

tree* demolisciStazione(tree* t, int stazione){
    if(t == NULL){
        printf("non demolita\n");
        return t;
    }
    else if(stazione > t->stazione){
        t->dx = demolisciStazione(t->dx, stazione);
    }
    else if(stazione < t->stazione){
        t->sx = demolisciStazione(t->sx, stazione);
    }
    else{
        if(t->sx == NULL && t->dx == NULL){
            eliminaLista(&t->macchine);
            free(t);
            printf("demolita\n");
            t = NULL;
        }
        else if(t->sx == NULL){
            tree* temp = t;
            eliminaLista(&temp->macchine);
            t = t->dx;
            free(temp);
            printf("demolita\n");
        }
        else if(t->dx == NULL){
            tree* temp = t;
            t = t->sx;
            eliminaLista(&temp->macchine);
            free(temp);
            printf("demolita\n");
        }
        else{
            tree* temp = trovaMin(t->dx);
            t->stazione = temp->stazione;
            t->n = temp->n;
            t->max = temp->max;
            eliminaLista(&t->macchine);
            if(t->n > 0){
                t->macchine = malloc(sizeof(struct nodo_a));
                lista* elem = NULL;
                lista *cur = t->macchine;
                lista* cur2 = temp->macchine;
                t->macchine->dist = temp->macchine->dist;
                t->macchine->next = NULL;
                while(cur2->next != NULL){
                    cur2 = cur2->next;
                    elem = malloc(sizeof(struct nodo_a));
                    elem->dist = cur2->dist;
                    elem->next = NULL;
                    cur->next = elem;
                    cur = cur->next;
                }
            }
            t->dx = demolisciStazione(t->dx, temp->stazione);
        }
    }
    return t;
}

tree* aggiungiAuto(tree* t, int stazione, int dist){
    if(t == NULL){
        printf("non aggiunta\n");
        return t;
    }
    else if(stazione > t->stazione){
        t->dx = aggiungiAuto(t->dx, stazione, dist);
    }
    else if(stazione < t->stazione){
        t->sx = aggiungiAuto(t->sx, stazione, dist);
    }
    else{
        t->n ++;
        lista* elem = malloc(sizeof(struct nodo_a));
        elem->dist = dist;
        elem->next = t->macchine;
        t->macchine = elem;
        printf("aggiunta\n");
        if(dist > t->max){
            t->max = dist;
        }
    }
    return t;
}

tree* rottamaAuto(tree* t, int stazione, int dist){
    if(t == NULL){
        printf("non rottamata\n");
        return t;
    }
    else if(stazione > t->stazione){
        t->dx = rottamaAuto(t->dx, stazione, dist);
    }
    else if(stazione < t->stazione){
        t->sx = rottamaAuto(t->sx, stazione, dist);
    }
    else{
        lista* temp = t->macchine;
        lista* pre = t->macchine;
        while(temp != NULL && temp->dist != dist){
            pre = temp;
            temp = temp->next;
        }
        if(temp == NULL){
            printf("non rottamata\n");
        }
        else{
            if(pre == temp){
                t->macchine = t->macchine->next;
            }
            else{
                pre->next = temp->next;
            }
            free(temp);
            printf("rottamata\n");
            t->n --;
            if(dist == t->max){
                t->max = 0;
                lista* curr = t->macchine;
                while (curr != NULL){
                    if(curr->dist > t->max){
                        t->max = curr->dist;
                    }
                    curr = curr->next;
                }
            }
        }

    }
    return t;
}

lista* pianificaPercorso1(tree* t, lista* percorso, int start, int finish){
    tree* temp = cerca(t, start);
    while((finish - temp->stazione) > temp->max){
        temp = stazioneSuccessiva(t, temp->stazione);
    }
    if((finish - temp->stazione) <= temp->max && temp->stazione != finish){
        lista* elem = NULL;
        elem = malloc(sizeof(struct nodo_a));
        elem->dist = temp->stazione;
        elem->next = percorso;
        percorso = elem;

        if(temp->stazione > start){
            percorso = pianificaPercorso1(t, percorso, start, temp->stazione);
        }
    }
    else{
        printf("nessun percorso\n");
    }
    return percorso;
}

lista* pianificaPercorso2(tree* t, lista* percorso, int start, int finish){
    lista2* tabella = NULL;
    tabella = stazioniPercorso(t, tabella, finish, start);

    lista2* scorrimento = tabella->next;

    while(tabella->stazione != finish){
        while(scorrimento != NULL && tabella->max >= (tabella->stazione - scorrimento->stazione)){
            if(scorrimento->tappa == -1 || ((scorrimento->tappa > (tabella->tappa + 1)) ||
                                            ((scorrimento->tappa == (tabella->tappa + 1)) && scorrimento->pre->stazione > tabella->stazione))){
                scorrimento->tappa = tabella->tappa + 1;
                scorrimento->pre = tabella;
            }
            scorrimento = scorrimento->next;
        }
        if(tabella->next->tappa != -1){
            tabella = tabella->next;
            scorrimento = tabella->next;
        }else{
            break;
        }
    }
    if(tabella->stazione == finish && tabella->tappa != -1){
        while(tabella->pre != NULL){
            tabella = tabella->pre;
            lista *elem = malloc(sizeof(struct nodo_a));
            elem->dist = tabella->stazione;
            elem->next = percorso;
            percorso = elem;
        }
    }else{
        printf("nessun percorso\n");
    }
    return percorso;
}

tree* trovaMin(tree* t){
    while(t->sx != NULL){
        t = t->sx;
    }
    return t;
}

tree* stazioneSuccessiva(tree* t, int x){
    tree* successore = NULL;
    while(t !=NULL){
        if (x >= t->stazione){
            t = t->dx;
        }
        else{
            successore = t;
            t = t->sx;
        }
    }
    return successore;
}

lista2* stazioniPercorso(tree* t, lista2* x, int finish, int start){
    tree* temp = cerca(t, finish);
    x = malloc(sizeof(struct nodo_b));
    x->stazione = finish;
    x->max = temp->max;
    x->tappa = -1;
    x->next = NULL;
    x->pre = NULL;

    lista2* elem = NULL;

    temp = stazioneSuccessiva(t, finish);
    while(temp->stazione != start){
        elem = malloc(sizeof(struct nodo_b));
        elem->stazione = temp->stazione;
        elem->max = temp->max;
        elem->tappa = -1;
        elem->next = x;
        elem->pre = NULL;

        x->pre = elem;
        x = x->pre;

        temp = stazioneSuccessiva(t, temp->stazione);
    }
    elem = malloc(sizeof(struct nodo_b));
    elem->stazione = temp->stazione;
    elem->max = temp->max;
    elem->tappa = 0;
    elem->next = x;
    elem->pre = NULL;

    x->pre = elem;
    x = x->pre;

    return x;
}

tree* cerca(tree* t, int stazione){
    if(t==NULL || stazione==t->stazione){
        return t;
    }
    if(stazione < t->stazione){
        return cerca(t->sx, stazione);
    }
    else
        return cerca(t->dx, stazione);
}

void eliminaLista(lista **x){
    lista *curr = *x;
    lista *next;
    while (curr != NULL)
    {
        next = curr->next;
        free(curr);
        curr = next;
    }
    *x = NULL;
}

