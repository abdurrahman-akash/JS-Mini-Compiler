#include "env.h"
#include "util.h"
#include <string.h>

typedef struct {
    char name[64];
    double value;
} Var;

static Var vars[256];
static int count = 0;

static int find(const char *name) {
    for (int i=0;i<count;i++)
        if (strcmp(vars[i].name,name)==0)
            return i;
    return -1;
}

double get_var(const char *name) {
    int i = find(name);
    if (i<0) {
        fprintf(stderr,"Undefined variable: %s\n", name);
        exit(1);
    }
    return vars[i].value;
}

void set_var(const char *name, double v) {
    int i = find(name);
    if (i<0) {
        if (count>=256) fatal("Too many variables");
        strcpy(vars[count].name, name);
        vars[count].value = v;
        count++;
    } else {
        vars[i].value = v;
    }
}
