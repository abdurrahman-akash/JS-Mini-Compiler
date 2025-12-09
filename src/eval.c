#include "eval.h"
#include "env.h"
#include "value.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Value *return_value = NULL;
static Value *exception_value = NULL;

Value *eval(ASTNode *n) {
    if (!n) return new_null_val();
    
    // Check if we have an exception
    if (exception_value) {
        return copy_value(exception_value);
    }
    
    // Check if we have a return value from a function
    if (return_value && n->type != NODE_FUNCTION && n->type != NODE_BLOCK) {
        return copy_value(return_value);
    }
    
    switch (n->type) {
        case NODE_NUMBER:
            return new_number_val(n->number);

        case NODE_STRING:
            return new_string_val(n->string_value);

        case NODE_BOOLEAN:
            return new_boolean_val(n->bool_value);

        case NODE_VAR:
            return copy_value(get_var(n->name));

        case NODE_BINOP: {
            Value *l = eval(n->left);
            Value *r = eval(n->right);
            Value *result = NULL;
            
            // String concatenation with +
            if (n->op == '+' && (l->type == VAL_STRING || r->type == VAL_STRING)) {
                char *ls = value_to_string(l);
                char *rs = value_to_string(r);
                char *combined = malloc(strlen(ls) + strlen(rs) + 1);
                strcpy(combined, ls);
                strcat(combined, rs);
                result = new_string_val(combined);
                free(ls);
                free(rs);
                free(combined);
            } else if (l->type == VAL_NUMBER && r->type == VAL_NUMBER) {
                switch (n->op) {
                    case '+': result = new_number_val(l->as.number + r->as.number); break;
                    case '-': result = new_number_val(l->as.number - r->as.number); break;
                    case '*': result = new_number_val(l->as.number * r->as.number); break;
                    case '/':
                        if (r->as.number == 0) {
                            fprintf(stderr, "Division by zero\n");
                            exit(1);
                        }
                        result = new_number_val(l->as.number / r->as.number);
                        break;
                    default:
                        result = new_null_val();
                }
            } else {
                result = new_null_val();
            }
            
            free_value(l);
            free_value(r);
            return result;
        }

        case NODE_COMPARISON: {
            Value *l = eval(n->left);
            Value *r = eval(n->right);
            int result = 0;
            
            if (l->type == VAL_NUMBER && r->type == VAL_NUMBER) {
                double lv = l->as.number;
                double rv = r->as.number;
                if (strcmp(n->name, "==") == 0) result = (lv == rv);
                else if (strcmp(n->name, "!=") == 0) result = (lv != rv);
                else if (strcmp(n->name, "<") == 0)  result = (lv < rv);
                else if (strcmp(n->name, ">") == 0)  result = (lv > rv);
                else if (strcmp(n->name, "<=") == 0) result = (lv <= rv);
                else if (strcmp(n->name, ">=") == 0) result = (lv >= rv);
            } else if (l->type == VAL_STRING && r->type == VAL_STRING) {
                int cmp = strcmp(l->as.string, r->as.string);
                if (strcmp(n->name, "==") == 0) result = (cmp == 0);
                else if (strcmp(n->name, "!=") == 0) result = (cmp != 0);
                else if (strcmp(n->name, "<") == 0)  result = (cmp < 0);
                else if (strcmp(n->name, ">") == 0)  result = (cmp > 0);
                else if (strcmp(n->name, "<=") == 0) result = (cmp <= 0);
                else if (strcmp(n->name, ">=") == 0) result = (cmp >= 0);
            }
            
            free_value(l);
            free_value(r);
            return new_boolean_val(result);
        }

        case NODE_LOGICAL: {
            if (strcmp(n->name, "!") == 0) {
                Value *v = eval(n->left);
                int result = !value_is_truthy(v);
                free_value(v);
                return new_boolean_val(result);
            }
            if (strcmp(n->name, "&&") == 0) {
                Value *l = eval(n->left);
                if (!value_is_truthy(l)) {
                    free_value(l);
                    return new_boolean_val(0);
                }
                free_value(l);
                Value *r = eval(n->right);
                int result = value_is_truthy(r);
                free_value(r);
                return new_boolean_val(result);
            }
            if (strcmp(n->name, "||") == 0) {
                Value *l = eval(n->left);
                if (value_is_truthy(l)) {
                    free_value(l);
                    return new_boolean_val(1);
                }
                free_value(l);
                Value *r = eval(n->right);
                int result = value_is_truthy(r);
                free_value(r);
                return new_boolean_val(result);
            }
            return new_null_val();
        }

        case NODE_ASSIGN: {
            Value *v = eval(n->left);
            set_var(n->name, v);
            return copy_value(v);
        }

        case NODE_PRINT: {
            Value *v = eval(n->left);
            char *str = value_to_string(v);
            printf("%s\n", str);
            free(str);
            Value *result = copy_value(v);
            free_value(v);
            return result;
        }

        case NODE_IF: {
            Value *cond = eval(n->condition);
            Value *result;
            if (value_is_truthy(cond)) {
                result = eval(n->left);
            } else if (n->else_branch) {
                result = eval(n->else_branch);
            } else {
                result = new_null_val();
            }
            free_value(cond);
            return result;
        }

        case NODE_WHILE: {
            Value *result = new_null_val();
            while (1) {
                Value *cond = eval(n->condition);
                int truthy = value_is_truthy(cond);
                free_value(cond);
                if (!truthy) break;
                
                free_value(result);
                result = eval(n->left);
                
                if (return_value || exception_value) break;  // Return or exception
            }
            return result;
        }

        case NODE_BLOCK: {
            Value *result = new_null_val();
            for (int i = 0; i < n->statement_count; i++) {
                free_value(result);
                result = eval(n->statements[i]);
                if (return_value || exception_value) break;  // Early return or exception
            }
            return result;
        }

        case NODE_FUNCTION: {
            return new_function_val(n->params, n->param_count, n->left);
        }

        case NODE_CALL: {
            Value *func = get_var(n->name);
            if (func->type != VAL_FUNCTION) {
                fprintf(stderr, "Not a function: %s\n", n->name);
                exit(1);
            }
            
            // Evaluate arguments
            Value **arg_values = malloc(sizeof(Value*) * n->arg_count);
            for (int i = 0; i < n->arg_count; i++) {
                arg_values[i] = eval(n->args[i]);
            }
            
            // Check argument count
            if (n->arg_count != func->as.function.param_count) {
                fprintf(stderr, "Function %s expects %d arguments, got %d\n",
                        n->name, func->as.function.param_count, n->arg_count);
                exit(1);
            }
            
            // Create new scope for function
            push_scope();
            
            // Bind parameters
            for (int i = 0; i < func->as.function.param_count; i++) {
                set_var(func->as.function.params[i], arg_values[i]);
            }
            
            free(arg_values);  // Free the array but not the values (owned by scope now)
            
            // Execute function body
            // Save the current return_value (in case we're in a nested call)
            Value *saved_return = return_value;
            return_value = NULL;
            
            Value *result = eval(func->as.function.body);
            
            // Check if function returned a value
            Value *ret;
            if (return_value) {
                ret = return_value;
                free_value(result);
            } else {
                ret = result;
            }
            
            // Restore the saved return_value
            return_value = saved_return;
            
            pop_scope();
            return ret;
        }

        case NODE_RETURN: {
            if (return_value) free_value(return_value);
            return_value = eval(n->left);
            return copy_value(return_value);
        }

        case NODE_ARRAY: {
            Value *arr = new_array_val();
            for (int i = 0; i < n->arg_count; i++) {
                array_push(arr, eval(n->args[i]));
            }
            return arr;
        }

        case NODE_OBJECT: {
            Value *obj = new_object_val();
            for (int i = 0; i < n->param_count; i++) {
                Value *val = eval(n->args[i]);
                object_set(obj, n->params[i], val);
            }
            return obj;
        }

        case NODE_INDEX: {
            Value *obj = eval(n->left);
            Value *index = eval(n->right);
            Value *result = new_null_val();
            
            if (obj->type == VAL_ARRAY && index->type == VAL_NUMBER) {
                int idx = (int)index->as.number;
                result = copy_value(array_get(obj, idx));
            } else if (obj->type == VAL_OBJECT && index->type == VAL_STRING) {
                result = copy_value(object_get(obj, index->as.string));
            }
            
            free_value(obj);
            free_value(index);
            return result;
        }

        case NODE_MEMBER: {
            Value *obj = eval(n->left);
            Value *result = new_null_val();
            
            if (obj->type == VAL_OBJECT) {
                result = copy_value(object_get(obj, n->name));
            } else if (obj->type == VAL_ARRAY && strcmp(n->name, "length") == 0) {
                result = new_number_val(obj->as.array.length);
            }
            
            free_value(obj);
            return result;
        }

        case NODE_TRY: {
            // Execute try block
            Value *try_result = eval(n->try_block);
            
            // If exception occurred during try block
            if (exception_value && n->catch_block) {
                Value *caught_exception = exception_value;
                exception_value = NULL;
                
                // Create new scope for catch block
                push_scope();
                
                // Bind exception to catch parameter
                if (n->catch_param[0] != '\0') {
                    set_var(n->catch_param, caught_exception);
                } else {
                    free_value(caught_exception);
                }
                
                // Execute catch block
                free_value(try_result);
                try_result = eval(n->catch_block);
                
                pop_scope();
            }
            
            // Execute finally block if present
            if (n->finally_block) {
                Value *finally_result = eval(n->finally_block);
                free_value(finally_result);
            }
            
            return try_result;
        }

        case NODE_THROW: {
            Value *throw_val = eval(n->left);
            
            // If it's already an error, use it; otherwise create error
            if (throw_val->type == VAL_ERROR) {
                exception_value = throw_val;
            } else {
                char *msg = value_to_string(throw_val);
                exception_value = new_error_val(msg);
                free(msg);
                free_value(throw_val);
            }
            
            return copy_value(exception_value);
        }
    }

    fprintf(stderr, "Unknown AST node type: %d\n", n->type);
    exit(1);
}
