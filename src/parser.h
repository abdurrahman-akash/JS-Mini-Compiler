#ifndef PARSER_H
#define PARSER_H

#include "ast.h"

ASTNode *parse_statement();
ASTNode *parse_expression();

#endif
