#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>

#define BUF_SIZE (100*1024*1024 +1)

int main(int argc, unsigned char *argv[]) {
    /*
    int opt;
    while ((opt = getopt(argc, argv, "l:f:")) != -1) {
        switch (opt) {
            case 'l':
                printf("Option a\n");
                break;
            case 'b':
                printf("Option b with value %s\n", optarg);
                break;
            case 'c':
                printf("Option c with value %s\n", optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s [-a] [-b value] [-c value]\n", argv[0]);
                return 1;
        }
    }
    */

    if (argc < 3) {
        printf("usage: fold4b {filepath} {length}\n");
        exit(1);
    }

    unsigned char *filepath = argv[1];
    int rec_len = atoi(argv[2]);

    FILE *fpi;
    if (NULL == (fpi = fopen(filepath, "rb"))) {
        perror("file open error.");
        exit(1);
    }

    unsigned char *ibufp, *obufp, *ip, *op;
    if (NULL == (ibufp = malloc(BUF_SIZE))) {
        perror("memory allocation error1.");
        exit(1);
    }
    if (NULL == (obufp = malloc(BUF_SIZE))) {
        perror("memory allocation error2.");
        exit(1);
    }

    int block_size = BUF_SIZE / (rec_len +2);

    while(feof(fpi) == 0) {
        int nblk = fread(ibufp, rec_len, block_size, fpi);

        ip=ibufp;
        for(int i=0; i < nblk * rec_len; i++, ip++) {
            if (*ip == 0x0d || *ip == 0x0a) *ip = '.';
        }

        ip = ibufp;
        op = obufp;
        for(int i=0; i<nblk; i++) {
            memcpy(op, ip, rec_len);
            *(op +rec_len) = 0x0d;
            *(op +rec_len +1) = 0x0a;
            ip += rec_len;
            op += rec_len +2;
        }
        fwrite(obufp, rec_len+2, nblk, stdout);
    }
    fflush(stdout);

    return 0;
}
