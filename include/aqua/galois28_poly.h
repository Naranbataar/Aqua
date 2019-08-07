#include "types.h"

#ifndef AQUA_GALOIS28_POLY
#define AQUA_GALOIS28_POLY

void
gpoly_add(Polynomial *p, Polynomial *q, Polynomial *r);

void
gpoly_mul(Polynomial *p, Polynomial *q, Polynomial *r);

void
gpoly_scale(Polynomial *p, int s, Polynomial *r);

void
gpoly_rsgen(int n, Polynomial *r);

unsigned char
gpoly_eval(Polynomial *p, int x);

#endif
