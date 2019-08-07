#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "aqua/aqua.h"

typedef enum { STREAM_TYPE_protect, STREAM_TYPE_purify } STREAM_TYPE;

int 
no_errata_purify(Polynomial* m, int symn){
    Polynomial errp = {{0}, 0};
    return purify(m, &errp, symn);
}

int
stream(STREAM_TYPE type, int length, int symn){
    Polynomial msg = {{0}, 0};

    FILE *fpin = freopen(NULL, "rb", stdin);
    FILE *fpout = freopen(NULL, "wb", stdout);

    int in_size = 0;
    int (*function)(Polynomial*, int) = NULL;
    switch (type) {
        case STREAM_TYPE_protect:
            in_size = length;
            function = protect;
        break;

        case STREAM_TYPE_purify:
            in_size = length + symn;
            function = no_errata_purify;
        break;
    }

    while (1){
        int bytes = fread(msg.data, 1, in_size, fpin);
        if (bytes == 0){
            break;
        }
        msg.order = bytes;
        int result = function(&msg, symn);

        if (result != 0){
            return result;
        }

        fwrite(msg.data, 1, msg.order, fpout);
    }
    fclose(fpin);
    fclose(fpout);
    return 0;
}
int
usage(void){
    fprintf(stderr, "Usage: aqua {MODE} [OPTIONS]...\n");
    fprintf(stderr, "Protect and recover protected files ");
    fprintf(stderr, "using Reed-Solomon\n");
    fprintf(stderr, "Avaliable modes\n");
    fprintf(stderr, "    protect        Encodes a stream\n");
    fprintf(stderr, "    purify         Decodes a stream\n");
    fprintf(stderr, "\nMessage size + number of symbols can't be more");
    fprintf(stderr, " than 255\n");
    fprintf(stderr, "    -c       message size, defaults to 245, or");
    fprintf(stderr, " (255 - s) if -s provided\n");
    fprintf(stderr, "    -s       number of symbols, defaults to 10, or");
    fprintf(stderr, " (255 - c) if -c provided\n");
    return 1;
}

int
argint(char* arg, int* out){
    char *ptr = arg;
    long i = strtol(arg, &ptr, 10);
    if (i > 255){
        return 1;
    }
    if (strlen(ptr) != 0){
        return 1;
    }
    *out = i;
    return 0;
}

int 
main(int argc, char** argv){
    init_aqua();
    int c = 0;
    int s = 0;

    if (argc < 2){
        return usage();
    }

    STREAM_TYPE type;
    if (strcmp(argv[1], "protect") == 0) {
        type = STREAM_TYPE_protect;
    } else if (strcmp(argv[1], "purify") == 0){
        type = STREAM_TYPE_purify;
    } else {
        return usage();
    }
   
    int i;
    while ((i = getopt (argc, argv, "c:s:")) != -1)
    switch (i){
        case 'c':
            if (argint(optarg, &c) != 0){
                return usage();
            }
        break;

        case 's':
            if (argint(optarg, &s) != 0){
                return usage();
            }
        break;

        case '?':
            return usage();
        break;
        
        default:
            return usage();
    }

    if (c == 0){
        c = (s != 0) ? (255 - s) : (245);
    }
            
    if (s == 0){
        s = (c != 0) ? (255 - c) : (10);
    }

    if ((c + s) > 255){
        return usage();
    }    

    int result = stream(type, c, s); 
    switch (result){
        case 3:
            fprintf(stderr, "I can't fix it, too many errors\n");
        break;

        case 4:
            fprintf(stderr, "I couldn't find all errors\n");
        break;

        case 5:
            fprintf(stderr, "The message can't be fixed\n");
        break;
    }

    return result;
}

