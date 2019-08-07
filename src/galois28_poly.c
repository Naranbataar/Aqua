#include <string.h> /* memcpy */
#include "aqua/galois28.h" /* g2pow, gmul */
#include "aqua/types.h" /* Polynomial */

void
gpoly_add(Polynomial *p, Polynomial *q, Polynomial *r){
    int length;
    if (p->order > q->order){
        length = p->order;
    } else {
        length = q->order;
    }

    int i;
    for (i = 0; i < 256; i++){
        r->data[i] = 0;
    }
    r->order = length;

    r->order = length;
    for (i = 0; i < p->order; i++){
        r->data[i + r->order - p->order] = p->data[i]; 
    }
    for (i = 0; i < q->order; i++){
        int loc = i + r->order - q->order;
        r->data[loc] = r->data[loc] ^ q->data[i]; 
    }
    
}

void
gpoly_mul(Polynomial *p, Polynomial *q, Polynomial *r){
    int length = (p->order+q->order-1);
    
    int i;
    for (i = 0; i < 256; i++){
        r->data[i] = 0;
    }
    r->order = length;

    int j;
    for (j = 0; j < q->order; j++){
        for (i = 0; i < p->order; i++){
            r->data[i+j] = r->data[i+j] ^ gmul(p->data[i], q->data[j]);
        }
    }
}

void
gpoly_scale(Polynomial *p, int s, Polynomial *r){
    int i;
    for (i = 0; i < p->order; i++){
        r->data[i] = gmul(p->data[i], s);
    }
    r->order = p->order;
}

Polynomial cache[256] = {{{0}, 0}};
void
gpoly_rsgen(int n, Polynomial *r){
    if (cache[n].order != 0 || n == 0){
        memcpy(r->data, cache[n].data, cache[n].order * sizeof(char));
        r->order = cache[n].order;
        return;
    }

    r->data[0] = 1;
    r->order = 1;
    
    Polynomial m = {{0}, 0};
    m.order = 2;

    Polynomial r2 = {{0}, 0};
    r2.order = r->order;
    
    int i;
    for (i = 0; i < n; i++){
        m.data[0] = 1;
        m.data[1] = g2pow(i); 
        gpoly_mul(r, &m, &r2);
        memcpy(r->data, r2.data, r2.order * sizeof(char));
        r->order = r2.order;
    }

    memcpy(cache[n].data, r->data, r->order * sizeof(char));
    cache[n].order = r->order;
}

unsigned char
gpoly_eval(Polynomial *p, int x){
    unsigned char y = p->data[0];

    int i;
    for (i = 1; i < p->order; i++){
        y = gmul(y, x) ^ p->data[i];
    }

    return y;
}


