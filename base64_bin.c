#include <stdio.h>
#include "file.h"
#include "base64.c"
#include <conio.h>

void help(char *name)
{
    fprintf(stderr, "base64 encoder and decoder by YX Hao (201503)\n");
    fprintf(stderr, "Usage:\n%s <-e|-d> [infile [outfile]]\n%s <-et|-dt> [intext [outfile]]\n", name, name);
    exit( 1 );
}

int main(int argc, char *argv[])
{
    size_t N, L;
    FILE *fp;
    char *str_in, *str_out;
    //
    if (stricmp( argv[1], "-et" ) == 0) {
        str_in = argv[2];
        N = strlen(str_in);
        str_out = base64encode(str_in, N, &L);
    }
    else if (stricmp( argv[1], "-dt" ) == 0) {
        str_in = argv[2];
        N = strlen(str_in);
        str_out = base64decode(str_in, N, &L);
    }
    else {
        switch (argc) {
        case 2:
            fp = stdin;
            break;
        case 3:
        case 4:
            fp = fopen(argv[2], "rb");
            break;
        default:
            help( argv[0] );
        }
        if (!fp) { return errno; }
        //
        if (stricmp( argv[1], "-e" ) == 0) {
            N = read_file(fp, &str_in);
            str_out = base64encode(str_in, N, &L);
        }
        else if (stricmp( argv[1], "-d" ) == 0) {
            N = read_file(fp, &str_in);
            str_out = base64decode(str_in, N, &L);
        }
        else {
            help( argv[0] );
        }
        if (fp != stdin) { fclose(fp); }
    }
    /* output */
    switch (argc) {
    case 2:
    case 3:
        fp = stdout;
        break;
    case 4:
        fp = fopen(argv[3], "wb");
        break;
    }
    if (!fp) { return errno; }
    fwrite(str_out, 1, L, fp);
    if (fp != stdout) { fclose(fp); }
    //
    free(str_in);
    free(str_out);
    return 0;
}
