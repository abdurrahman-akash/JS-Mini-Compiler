#ifndef AST_H
#define AST_H

typedef enum {
    NODE_NUMBER,
    NODE_STRING,
    NODE_VAR,
    NODE_BINOP,
    NODE_ASSIGN,
    NODE_PRINT,
    NODE_BOOLEAN,
    NODE_COMPARISON,
    NODE_LOGICAL,
    NODE_IF,
    NODE_WHILE,
    NODE_BLOCK,
    NODE_FUNCTION,
    NODE_CALL,
    NODE_RETURN,
    NODE_ARRAY,
    NODE_OBJECT,
    NODE_INDEX,
    NODE_MEMBER,
    NODE_TRY,
    NODE_THROW
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
    ASTNode **args;
    int arg_count;
    char **params;
    int param_count;
    char name[256];
    char string_value[256];
    double number;
    char op;
    int bool_value;
    // For try-catch-finally
    ASTNode *try_block;
    ASTNode *catch_block;
    ASTNode *finally_block;
    char catch_param[256];
};

ASTNode *new_number(double v);
ASTNode *new_string(const char *s);
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
ASTNode *new_function(char **params, int param_count, ASTNode *body);
ASTNode *new_call(const char *name, ASTNode **args, int arg_count);
ASTNode *new_return(ASTNode *expr);
ASTNode *new_array(ASTNode **elements, int count);
ASTNode *new_object(char **keys, ASTNode **values, int count);
ASTNode *new_index(ASTNode *object, ASTNode *index);
ASTNode *new_member(ASTNode *object, const char *member);
ASTNode *new_try(ASTNode *try_block, const char *catch_param, ASTNode *catch_block, ASTNode *finally_block);
ASTNode *new_throw(ASTNode *expr);
void free_ast(ASTNode *n);

#endif
