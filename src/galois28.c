unsigned char EXP_LUT[256];
unsigned char LOG_LUT[256];

void
init_galois_lut(void){
    int nx;   
    unsigned char x = 1;

    int i; 
    for (i = 0; i < 255; i++){
        EXP_LUT[i] = x;
        LOG_LUT[x] = (unsigned char)i;
        
        nx = (signed int)(x << 1);
        if (nx < 256){
            x = (unsigned char)nx;
        } else {
            x = (unsigned char)(nx ^ 285);
        }
    } 
}

unsigned char
gmul(unsigned char x, unsigned char y){
    if (x == 0 || y == 0){
        return 0;
    }
    return EXP_LUT[(LOG_LUT[x] + LOG_LUT[y]) % 255];
}

unsigned char
gdiv(unsigned char x, unsigned char y){
    if (x == 0 || y == 0){
        return 0;
    }
    return EXP_LUT[(LOG_LUT[x] + 255 - LOG_LUT[y]) % 255];
}

unsigned char
g2pow(unsigned char x){
    if (x <= 7){
        return 1 << x;
    }
    return EXP_LUT[x];
}

unsigned char
ginv(unsigned char x){
    return EXP_LUT[(255 - LOG_LUT[x]) % 255];
}

