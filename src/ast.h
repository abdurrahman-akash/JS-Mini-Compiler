#ifndef AST_H
#define AST_H

typedef enum {
    NODE_NUMBER,
    NODE_VAR,
    NODE_BINOP,
    NODE_ASSIGN,
    NODE_PRINT,
    NODE_BOOLEAN,
    NODE_COMPARISON,
    NODE_LOGICAL,
    NODE_IF,
    NODE_WHILE,
    NODE_BLOCK
} NodeType;

typedef struct ASTNode ASTNode;

struct ASTNode {
    NodeType type;
    ASTNode *left;
    ASTNode *right;
    ASTNode *condition;
    ASTNode *else_branch;
    ASTNode **statements;
    int statement_count;
    char name[64];
    double number;
    char op;
    int bool_value;
};

ASTNode *new_number(double v);
ASTNode *new_var(const char *name);
ASTNode *new_binop(char op, ASTNode *l, ASTNode *r);
ASTNode *new_assign(const char *name, ASTNode *expr);
ASTNode *new_print(ASTNode *expr);
ASTNode *new_boolean(int value);
ASTNode *new_comparison(char *op, ASTNode *l, ASTNode *r);
ASTNode *new_logical(char *op, ASTNode *l, ASTNode *r);
ASTNode *new_if(ASTNode *condition, ASTNode *then_branch, ASTNode *else_branch);
ASTNode *new_while(ASTNode *condition, ASTNode *body);
ASTNode *new_block(ASTNode **statements, int count);
void free_ast(ASTNode *n);

#endif
