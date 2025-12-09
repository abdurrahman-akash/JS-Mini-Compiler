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
    n->args = NULL;
    n->arg_count = 0;
    n->params = NULL;
    n->param_count = 0;
    n->name[0]=0;
    n->string_value[0]=0;
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

ASTNode *new_string(const char *s) {
    ASTNode *n = make(NODE_STRING);
    strncpy(n->string_value, s, 255);
    return n;
}

ASTNode *new_var(const char *name) {
    ASTNode *n = make(NODE_VAR);
    strncpy(n->name, name, 255);
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

ASTNode *new_function(char **params, int param_count, ASTNode *body) {
    ASTNode *n = make(NODE_FUNCTION);
    n->params = params;
    n->param_count = param_count;
    n->left = body;
    return n;
}

ASTNode *new_call(const char *name, ASTNode **args, int arg_count) {
    ASTNode *n = make(NODE_CALL);
    strncpy(n->name, name, 255);
    n->args = args;
    n->arg_count = arg_count;
    return n;
}

ASTNode *new_return(ASTNode *expr) {
    ASTNode *n = make(NODE_RETURN);
    n->left = expr;
    return n;
}

ASTNode *new_array(ASTNode **elements, int count) {
    ASTNode *n = make(NODE_ARRAY);
    n->args = elements;  // Reusing args field
    n->arg_count = count;
    return n;
}

ASTNode *new_object(char **keys, ASTNode **values, int count) {
    ASTNode *n = make(NODE_OBJECT);
    n->params = keys;  // Reusing params field
    n->args = values;
    n->param_count = count;
    return n;
}

ASTNode *new_index(ASTNode *object, ASTNode *index) {
    ASTNode *n = make(NODE_INDEX);
    n->left = object;
    n->right = index;
    return n;
}

ASTNode *new_member(ASTNode *object, const char *member) {
    ASTNode *n = make(NODE_MEMBER);
    n->left = object;
    strncpy(n->name, member, 255);
    return n;
}

ASTNode *new_try(ASTNode *try_block, const char *catch_param, ASTNode *catch_block, ASTNode *finally_block) {
    ASTNode *n = make(NODE_TRY);
    n->try_block = try_block;
    n->catch_block = catch_block;
    n->finally_block = finally_block;
    if (catch_param) {
        strncpy(n->catch_param, catch_param, 255);
    } else {
        n->catch_param[0] = '\0';
    }
    return n;
}

ASTNode *new_throw(ASTNode *expr) {
    ASTNode *n = make(NODE_THROW);
    n->left = expr;
    return n;
}

void free_ast(ASTNode *n) {
    if (!n) return;
    free_ast(n->left);
    free_ast(n->right);
    free_ast(n->condition);
    free_ast(n->else_branch);
    free_ast(n->try_block);
    free_ast(n->catch_block);
    free_ast(n->finally_block);
    if (n->statements) {
        for (int i = 0; i < n->statement_count; i++) {
            free_ast(n->statements[i]);
        }
        free(n->statements);
    }
    if (n->args) {
        for (int i = 0; i < n->arg_count; i++) {
            free_ast(n->args[i]);
        }
        free(n->args);
    }
    if (n->params) {
        for (int i = 0; i < n->param_count; i++) {
            if (n->type == NODE_OBJECT || n->type == NODE_FUNCTION) {
                free(n->params[i]);
            }
        }
        if (n->type == NODE_OBJECT || n->type == NODE_FUNCTION) {
            free(n->params);
        }
    }
    free(n);
}
