#ifndef AST_H
#define AST_H

typedef enum {
    NODE_NUMBER,
    NODE_VAR,
    NODE_BINOP,
    NODE_ASSIGN,
    NODE_PRINT
} NodeType;

typedef struct ASTNode ASTNode;

struct ASTNode {
    NodeType type;
    ASTNode *left;
    ASTNode *right;
    char name[64];
    double number;
    char op;
};

ASTNode *new_number(double v);
ASTNode *new_var(const char *name);
ASTNode *new_binop(char op, ASTNode *l, ASTNode *r);
ASTNode *new_assign(const char *name, ASTNode *expr);
ASTNode *new_print(ASTNode *expr);
void free_ast(ASTNode *n);

#endif
