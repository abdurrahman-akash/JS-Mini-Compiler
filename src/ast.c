#include "ast.h"
#include <stdlib.h>
#include <string.h>

static ASTNode *make(NodeType t) {
    ASTNode *n = malloc(sizeof(ASTNode));
    n->type = t;
    n->left = n->right = NULL;
    n->name[0]=0;
    n->number = 0;
    n->op = 0;
    return n;
}

ASTNode *new_number(double v) {
    ASTNode *n = make(NODE_NUMBER);
    n->number = v;
    return n;
}

ASTNode *new_var(const char *name) {
    ASTNode *n = make(NODE_VAR);
    strncpy(n->name, name, 63);
    return n;
}

ASTNode *new_binop(char op, ASTNode *l, ASTNode *r) {
    ASTNode *n = make(NODE_BINOP);
    n->op = op;
    n->left = l; n->right = r;
    return n;
}

ASTNode *new_assign(const char *name, ASTNode *expr) {
    ASTNode *n = make(NODE_ASSIGN);
    strncpy(n->name, name, 63);
    n->left = expr;
    return n;
}

ASTNode *new_print(ASTNode *expr) {
    ASTNode *n = make(NODE_PRINT);
    n->left = expr;
    return n;
}

void free_ast(ASTNode *n) {
    if (!n) return;
    free_ast(n->left);
    free_ast(n->right);
    free(n);
}
