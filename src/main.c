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

static ASTNode **function_asts = NULL;
static int function_count = 0;
static int function_capacity = 0;

static void save_function_ast(ASTNode *ast) {
    if (function_capacity == 0) {
        function_capacity = 16;
        function_asts = malloc(sizeof(ASTNode*) * function_capacity);
    }
    if (function_count >= function_capacity) {
        function_capacity *= 2;
        function_asts = realloc(function_asts, sizeof(ASTNode*) * function_capacity);
    }
    function_asts[function_count++] = ast;
}

static int is_function_declaration(ASTNode *n) {
    return n && n->type == NODE_ASSIGN && n->left && n->left->type == NODE_FUNCTION;
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
        Value *result = eval(st);
        free_value(result);
        
        // Don't free function declarations - they're needed later
        if (is_function_declaration(st)) {
            save_function_ast(st);
        } else {
            free_ast(st);
        }
    }

    // Clean up function ASTs at the end
    for (int i = 0; i < function_count; i++) {
        free_ast(function_asts[i]);
    }
    if (function_asts) free(function_asts);

    free(src);
    return 0;
}
