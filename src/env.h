#ifndef ENV_H
#define ENV_H

#include "value.h"

void push_scope(void);
void pop_scope(void);
Value *get_var(const char *name);
void set_var(const char *name, Value *v);

#endif
