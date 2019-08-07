#ifndef AQUA_GALOIS28
#define AQUA_GALOIS28
extern unsigned char EXP_LUT[256];
extern unsigned char LOG_LUT[256];

void
init_galois_lut(void);

unsigned char
gmul(unsigned char x, unsigned char y);

unsigned char
gdiv(unsigned char x, unsigned char y);

unsigned char
g2pow(unsigned char x);

unsigned char
ginv(unsigned char x);
#endif
