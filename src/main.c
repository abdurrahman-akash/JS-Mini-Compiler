#include "../include/mini_js.h"
#include <stdio.h>
#include <stdlib.h>

static char *read_entire(const char *path) {
    FILE *f = fopen(path,"rb");
    if (!f) { perror("fopen"); exit(1); }

    fseek(f,0,SEEK_END);
    long len = ftell(f);
    fseek(f,0,SEEK_SET);

    char *buf = malloc(len+1);
    fread(buf,1,len,f);
    buf[len]=0;
    fclose(f);
    return buf;
}

int main(int argc, char **argv) {
    if (argc<2) {
        printf("Usage: %s file.js\n", argv[0]);
        return 1;
    }

    char *src = read_entire(argv[1]);
    init_lexer(src);

    while (current_tok().type != TOKEN_EOF) {
        ASTNode *st = parse_statement();
        eval(st);
        free_ast(st);
    }

    free(src);
    return 0;
}
