#include <string.h> /* memcpy */
#include "aqua/galois28.h" /* gmul, g2pow, ginv */
#include "aqua/galois28_poly.h" /* gpoly_add, gpoly_eval, gpoly_scale */
#include "aqua/types.h" /* Polynomial */

void
syndromes(Polynomial *m, int symn, Polynomial *r){
    r->order = symn + 1; 
    r->data[0] = 0;

    int i;
    for (i = 0; i < symn; i++){
        r->data[i+1] = gpoly_eval(m, g2pow(i));
    }
}

void
forney_syndromes(Polynomial *syn, Polynomial *errp, int order, Polynomial *r){
    Polynomial errpr = {{0}, 0};
    errpr.order = errp->order;

    int i;
    for (i = 0; i < errp->order; i++){    
        errpr.data[i] = order - 1 - errp->data[i];
    }

    r->order = syn->order - 1;
    memcpy(r->data, syn->data + 1, r->order * sizeof(char));    

    int j;
    for (i = 0; i < errp->order; i++){
        int x = g2pow(errpr.data[i]);
        for (j = 0; j < (r->order - 1); j++){
            r->data[j] = gmul(r->data[j], x) ^ r->data[j+1];
        }
    }
}

void
find_locator(Polynomial *syn, int errpo, int symn, Polynomial *r){
    Polynomial ol = {{0}, 0};

    r->data[0] = 1;
    ol.data[0] = 1;
    r->order = 1;
    ol.order = 1;

    int shift = 0;
    if (syn->order > symn){
        shift = syn->order - symn;
    }

    int i;
    for (i = 0; i < (symn - errpo); i++){
        int k = i + shift;
        unsigned char delta = syn->data[k];
        int j;
        for (j = 1; j < r->order; j++){
            delta = delta ^ gmul(r->data[r->order-(j+1)], syn->data[k-j]);
        }

        ol.data[ol.order] = 0;
        ol.order++; 
        
        Polynomial nl = {{0}, 0};
        nl.order = 0;
        
        Polynomial nl2 = {{0}, 0};
        nl.order = 0;

        Polynomial nl3 = {{0}, 0};
        nl.order = 0;
        
        if (delta != 0){
            if (ol.order > r->order){
                gpoly_scale(&ol, delta, &nl);
                gpoly_scale(r, ginv(delta), &ol);
                memcpy(r->data, nl.data, nl.order * sizeof(char));
                r->order = nl.order;
            }
            gpoly_scale(&ol, delta, &nl2); 
            gpoly_add(r, &nl2, &nl3);
            memcpy(r->data, nl3.data, nl3.order * sizeof(char));
            r->order = nl3.order;
        }
    }
}

void
find_errors(Polynomial *l, int order, Polynomial *r){
    Polynomial li = {{0}, 0};
    li.order = l->order;

    int i;
    for (i = 0; i < l->order; i++){
        li.data[l->order-i-1] = l->data[i];
    }

    r->order = 0;
    for (i = 0; i < order; i++){
        if (gpoly_eval(&li, g2pow(i)) == 0){
            r->data[r->order] = order - 1 - i;
            r->order++;
        }
    }
}

