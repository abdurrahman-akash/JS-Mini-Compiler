#include "ast.h"
#include <stdlib.h>
#include <string.h>

static ASTNode *make(NodeType t) {
    ASTNode *n = malloc(sizeof(ASTNode));
    n->type = t;
    n->left = n->right = NULL;
    n->condition = n->else_branch = NULL;
    n->statements = NULL;
    n->statement_count = 0;
    n->name[0]=0;
    n->number = 0;
    n->op = 0;
    n->bool_value = 0;
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

ASTNode *new_boolean(int value) {
    ASTNode *n = make(NODE_BOOLEAN);
    n->bool_value = value;
    return n;
}

ASTNode *new_comparison(char *op, ASTNode *l, ASTNode *r) {
    ASTNode *n = make(NODE_COMPARISON);
    strncpy(n->name, op, 63);
    n->left = l;
    n->right = r;
    return n;
}

ASTNode *new_logical(char *op, ASTNode *l, ASTNode *r) {
    ASTNode *n = make(NODE_LOGICAL);
    strncpy(n->name, op, 63);
    n->left = l;
    n->right = r;
    return n;
}

ASTNode *new_if(ASTNode *condition, ASTNode *then_branch, ASTNode *else_branch) {
    ASTNode *n = make(NODE_IF);
    n->condition = condition;
    n->left = then_branch;
    n->else_branch = else_branch;
    return n;
}

ASTNode *new_while(ASTNode *condition, ASTNode *body) {
    ASTNode *n = make(NODE_WHILE);
    n->condition = condition;
    n->left = body;
    return n;
}

ASTNode *new_block(ASTNode **statements, int count) {
    ASTNode *n = make(NODE_BLOCK);
    n->statements = statements;
    n->statement_count = count;
    return n;
}

void free_ast(ASTNode *n) {
    if (!n) return;
    free_ast(n->left);
    free_ast(n->right);
    free_ast(n->condition);
    free_ast(n->else_branch);
    if (n->statements) {
        for (int i = 0; i < n->statement_count; i++) {
            free_ast(n->statements[i]);
        }
        free(n->statements);
    }
    free(n);
}
