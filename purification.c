#include <string.h> /* memcpy */
#include "aqua/galois28.h" /* gmul, gdiv, g2pow, ginv */
#include "aqua/galois28_poly.h" /* gpoly_add, gpoly_mul, gpoly_eval */
#include "aqua/types.h" /* Polynomial */

int
purification(Polynomial *m, Polynomial *syn, Polynomial *errp, Polynomial *l){
    int i;

    Polynomial coefp = {{0}, 0};
    coefp.order = errp->order;
    for (i = 0; i < errp->order; i++){
        coefp.data[i] = m->order - 1 - errp->data[i];
    }

    Polynomial synr = {{0}, 0};
    synr.order = syn->order;
    for (i = 0; i < syn->order; i++){
        synr.data[syn->order-i-1] = syn->data[i];
    }

    Polynomial eval = {{0}, 0};
    gpoly_mul(&synr, l, &eval);
    for (i = 0; i < l->order; i++){
        eval.data[i] = eval.data[i+(eval.order-l->order)];
    }
    eval.order = l->order;

    Polynomial x = {{0}, 0};
    x.order = coefp.order;

    int li; 
    for (i = 0; i < coefp.order; i++){
        li = coefp.data[i];
        x.data[i] = g2pow(li);
    }
    
    Polynomial xl = {{0}, 0};
    xl.order = x.order;
    for (i = 0; i < x.order; i++){
        xl.data[i] = LOG_LUT[x.data[i]];
    }

    Polynomial e = {{0}, 0};
    e.order = m->order;

    int j;
    for (i = 0; i < x.order; i++){
        unsigned char xi = x.data[i];
        unsigned char xii = ginv(xi);
        unsigned char xiil = LOG_LUT[xii];   
 
        unsigned char mul;
        unsigned char prime = 1;
        for (j = 0; j < x.order; j++){
            if (j != i){
                mul = 1 ^ EXP_LUT[(xl.data[j] + xiil) % 255];
                prime = gmul(prime, mul);
            }
        }

        if (prime == 0){
            return 1;
        }  

        unsigned char y = gpoly_eval(&eval, xii);
        y = gmul(xi, y);

        e.data[errp->data[i]] = gdiv(y, prime);       
    }

    Polynomial m2 = {{0}, 0};
    gpoly_add(m, &e, &m2);    
    
    memcpy(m->data, m2.data, m2.order * sizeof(char));
    m->order = m2.order;
    return 0;
}

