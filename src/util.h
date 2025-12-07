#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>

#define fatal(msg) do { fprintf(stderr, "%s\n", msg); exit(1); } while(0)

#endif
