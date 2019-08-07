#include "types.h"

#ifndef AQUA_ANALYSIS
#define AQUA_ANALYSIS
void
syndromes(Polynomial *m, int symn, Polynomial *r);

void
forney_syndromes(Polynomial *syn, Polynomial *errp, int order, Polynomial *r);

void
find_locator(Polynomial *syn, int errpo, int symn, Polynomial *r);

void
find_errors(Polynomial *l, int order, Polynomial *r);
#endif


