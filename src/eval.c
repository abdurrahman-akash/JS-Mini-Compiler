#include "eval.h"
#include "env.h"
#include <stdio.h>
#include <stdlib.h>

double eval(ASTNode *n) {
    switch (n->type) {

        case NODE_NUMBER:
            return n->number;

        case NODE_VAR:
            return get_var(n->name);

        case NODE_BINOP: {
            double l = eval(n->left);
            double r = eval(n->right);
            switch (n->op) {
                case '+': return l+r;
                case '-': return l-r;
                case '*': return l*r;
                case '/':
                    if (r==0) { fprintf(stderr,"Division by zero\n"); exit(1); }
                    return l/r;
            }
            break;
        }

        case NODE_ASSIGN: {
            double v = eval(n->left);
            set_var(n->name, v);
            return v;
        }

        case NODE_PRINT: {
            double v = eval(n->left);
            printf("%g\n", v);
            return v;
        }
    }

    fprintf(stderr,"Unknown AST node\n");
    exit(1);
}
