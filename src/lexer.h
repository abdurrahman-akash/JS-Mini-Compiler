#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>

typedef enum {
    TOKEN_EOF,
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_LET,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_ASSIGN,
    TOKEN_SEMI,
    TOKEN_COMMA
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[64];
    double number;
} Token;

void init_lexer(const char *source);
void advance_token(void);
Token current_tok(void);
void expect(TokenType type, const char *msg);

#endif
