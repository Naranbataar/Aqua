#include "types.h"

#ifndef AQUA_MAIN
#define AQUA_MAIN
void
init_aqua(void);

int
protect(Polynomial *m, int symn);

int
purify(Polynomial *m, Polynomial *errp, int symn); 
#endif
