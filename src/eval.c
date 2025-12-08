#include "eval.h"
#include "env.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double eval(ASTNode *n) {
    if (!n) return 0;
    
    switch (n->type) {

        case NODE_NUMBER:
            return n->number;

        case NODE_BOOLEAN:
            return n->bool_value ? 1.0 : 0.0;

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

        case NODE_COMPARISON: {
            double l = eval(n->left);
            double r = eval(n->right);
            if (strcmp(n->name, "==") == 0) return (l == r) ? 1.0 : 0.0;
            if (strcmp(n->name, "!=") == 0) return (l != r) ? 1.0 : 0.0;
            if (strcmp(n->name, "<") == 0)  return (l < r) ? 1.0 : 0.0;
            if (strcmp(n->name, ">") == 0)  return (l > r) ? 1.0 : 0.0;
            if (strcmp(n->name, "<=") == 0) return (l <= r) ? 1.0 : 0.0;
            if (strcmp(n->name, ">=") == 0) return (l >= r) ? 1.0 : 0.0;
            break;
        }

        case NODE_LOGICAL: {
            if (strcmp(n->name, "!") == 0) {
                double v = eval(n->left);
                return (v == 0.0) ? 1.0 : 0.0;
            }
            if (strcmp(n->name, "&&") == 0) {
                double l = eval(n->left);
                if (l == 0.0) return 0.0;
                double r = eval(n->right);
                return (r != 0.0) ? 1.0 : 0.0;
            }
            if (strcmp(n->name, "||") == 0) {
                double l = eval(n->left);
                if (l != 0.0) return 1.0;
                double r = eval(n->right);
                return (r != 0.0) ? 1.0 : 0.0;
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

        case NODE_IF: {
            double condition = eval(n->condition);
            if (condition != 0.0) {
                return eval(n->left);
            } else if (n->else_branch) {
                return eval(n->else_branch);
            }
            return 0.0;
        }

        case NODE_WHILE: {
            double result = 0.0;
            while (eval(n->condition) != 0.0) {
                result = eval(n->left);
            }
            return result;
        }

        case NODE_BLOCK: {
            double result = 0.0;
            for (int i = 0; i < n->statement_count; i++) {
                result = eval(n->statements[i]);
            }
            return result;
        }
    }

    fprintf(stderr,"Unknown AST node\n");
    exit(1);
}
