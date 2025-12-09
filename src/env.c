#include "env.h"
#include "util.h"
#include <string.h>
#include <stdlib.h>

typedef struct {
    char name[256];
    Value *value;
} Var;

typedef struct Scope {
    Var vars[256];
    int count;
    struct Scope *parent;
} Scope;

static Scope *current_scope = NULL;

void push_scope(void) {
    Scope *new_scope = malloc(sizeof(Scope));
    new_scope->count = 0;
    new_scope->parent = current_scope;
    current_scope = new_scope;
}

void pop_scope(void) {
    if (!current_scope) return;
    
    Scope *old = current_scope;
    current_scope = current_scope->parent;
    
    // Clean up values in the old scope
    for (int i = 0; i < old->count; i++) {
        free_value(old->vars[i].value);
    }
    free(old);
}

static int find_in_scope(Scope *scope, const char *name) {
    if (!scope) return -1;
    for (int i = 0; i < scope->count; i++) {
        if (strcmp(scope->vars[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

Value *get_var(const char *name) {
    // Search from current scope up to global
    Scope *scope = current_scope;
    while (scope) {
        int i = find_in_scope(scope, name);
        if (i >= 0) {
            return scope->vars[i].value;
        }
        scope = scope->parent;
    }
    
    fprintf(stderr, "Undefined variable: %s\n", name);
    exit(1);
}

void set_var(const char *name, Value *v) {
    // Initialize global scope if needed
    if (!current_scope) {
        push_scope();
    }
    
    // Try to find in current scope only (for let)
    int i = find_in_scope(current_scope, name);
    if (i >= 0) {
        free_value(current_scope->vars[i].value);
        current_scope->vars[i].value = v;
        return;
    }
    
    // Try to find in parent scopes (for reassignment)
    Scope *scope = current_scope->parent;
    while (scope) {
        i = find_in_scope(scope, name);
        if (i >= 0) {
            free_value(scope->vars[i].value);
            scope->vars[i].value = v;
            return;
        }
        scope = scope->parent;
    }
    
    // Not found anywhere, add to current scope
    if (current_scope->count >= 256) {
        fatal("Too many variables in scope");
    }
    strcpy(current_scope->vars[current_scope->count].name, name);
    current_scope->vars[current_scope->count].value = v;
    current_scope->count++;
}
