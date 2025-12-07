#include "lexer.h"
#include "util.h"
#include <string.h>
#include <ctype.h>

static const char *src = NULL;
static size_t pos = 0;
static char ch = 0;

static Token token;

static void advance_char() {
    if (ch) {
        pos++;
        ch = src[pos];
    }
}

static void skip_ws() {
    while (isspace((unsigned char)ch))
        advance_char();
}

static Token make(TokenType t) {
    Token x; x.type = t; x.lexeme[0]=0; x.number=0;
    return x;
}

static Token number_tok() {
    char buf[64]; size_t n=0;
    while (isdigit((unsigned char)ch) || ch=='.') {
        if (n<63) buf[n++]=ch;
        advance_char();
    }
    buf[n]=0;
    Token t = make(TOKEN_NUMBER);
    t.number = atof(buf);
    return t;
}

static Token ident_or_kw() {
    char buf[64]; size_t n=0;
    while (isalnum((unsigned char)ch) || ch=='_') {
        if (n<63) buf[n++]=ch;
        advance_char();
    }
    buf[n]=0;

    if (strcmp(buf,"let")==0)
        return make(TOKEN_LET);

    Token t = make(TOKEN_IDENTIFIER);
    strcpy(t.lexeme, buf);
    return t;
}

static Token next_token() {
    skip_ws();

    if (!ch) return make(TOKEN_EOF);
    if (isdigit((unsigned char)ch)) return number_tok();
    if (isalpha((unsigned char)ch) || ch=='_') return ident_or_kw();

    Token t;

    switch (ch) {
        case '+': t=make(TOKEN_PLUS);   advance_char(); return t;
        case '-': t=make(TOKEN_MINUS);  advance_char(); return t;
        case '*': t=make(TOKEN_STAR);   advance_char(); return t;
        case '/': t=make(TOKEN_SLASH);  advance_char(); return t;
        case '=': t=make(TOKEN_ASSIGN); advance_char(); return t;
        case ';': t=make(TOKEN_SEMI);   advance_char(); return t;
        case '(': t=make(TOKEN_LPAREN); advance_char(); return t;
        case ')': t=make(TOKEN_RPAREN); advance_char(); return t;
        case ',': t=make(TOKEN_COMMA);  advance_char(); return t;
    }

    fprintf(stderr,"Unexpected character '%c'\n", ch);
    exit(1);
}

void init_lexer(const char *s) {
    src = s;
    pos = 0;
    ch = src[0];
    advance_token();
}

void advance_token() {
    token = next_token();
}

Token current_tok() {
    return token;
}

void expect(TokenType t, const char *msg) {
    if (token.type != t) {
        fprintf(stderr,"Parse error: %s\n", msg);
        exit(1);
    }
    advance_token();
}
