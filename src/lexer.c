#include "lexer.h"
#include "util.h"
#include <string.h>
#include <ctype.h>

static const char *src = NULL;
static size_t pos = 0;
static char ch = 0;

static Token token;
static Token next;
static int has_next = 0;

static void advance_char() {
    if (ch) {
        pos++;
        ch = src[pos];
    }
}

static void skip_ws() {
    while (1) {
        if (isspace((unsigned char)ch)) {
            advance_char();
        } else if (ch == '/') {
            size_t saved_pos = pos;
            char saved_ch = ch;
            advance_char();
            if (ch == '/') {
                // Single line comment - skip to end of line
                while (ch && ch != '\n') {
                    advance_char();
                }
                if (ch == '\n') advance_char();
            } else {
                // Not a comment, restore position
                pos = saved_pos;
                ch = saved_ch;
                break;
            }
        } else {
            break;
        }
    }
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
    if (strcmp(buf,"function")==0)
        return make(TOKEN_FUNCTION);
    if (strcmp(buf,"return")==0)
        return make(TOKEN_RETURN);
    if (strcmp(buf,"if")==0)
        return make(TOKEN_IF);
    if (strcmp(buf,"else")==0)
        return make(TOKEN_ELSE);
    if (strcmp(buf,"while")==0)
        return make(TOKEN_WHILE);
    if (strcmp(buf,"try")==0)
        return make(TOKEN_TRY);
    if (strcmp(buf,"catch")==0)
        return make(TOKEN_CATCH);
    if (strcmp(buf,"finally")==0)
        return make(TOKEN_FINALLY);
    if (strcmp(buf,"throw")==0)
        return make(TOKEN_THROW);
    if (strcmp(buf,"true")==0)
        return make(TOKEN_TRUE);
    if (strcmp(buf,"false")==0)
        return make(TOKEN_FALSE);

    Token t = make(TOKEN_IDENTIFIER);
    strcpy(t.lexeme, buf);
    return t;
}

static Token string_tok() {
    char quote = ch;  // ' or "
    advance_char();
    
    char buf[256]; 
    size_t n = 0;
    
    while (ch && ch != quote && n < 255) {
        if (ch == '\\') {
            advance_char();
            if (ch == 'n') buf[n++] = '\n';
            else if (ch == 't') buf[n++] = '\t';
            else if (ch == '\\') buf[n++] = '\\';
            else if (ch == quote) buf[n++] = quote;
            else buf[n++] = ch;
            advance_char();
        } else {
            buf[n++] = ch;
            advance_char();
        }
    }
    buf[n] = 0;
    
    if (ch == quote) advance_char();
    else {
        fprintf(stderr, "Unterminated string\n");
        exit(1);
    }
    
    Token t = make(TOKEN_STRING);
    strcpy(t.lexeme, buf);
    return t;
}

static Token next_token() {
    skip_ws();

    if (!ch) return make(TOKEN_EOF);
    if (isdigit((unsigned char)ch)) return number_tok();
    if (ch == '"' || ch == '\'') return string_tok();
    if (isalpha((unsigned char)ch) || ch=='_') return ident_or_kw();

    Token t;

    switch (ch) {
        case '+': t=make(TOKEN_PLUS);   advance_char(); return t;
        case '-': t=make(TOKEN_MINUS);  advance_char(); return t;
        case '*': t=make(TOKEN_STAR);   advance_char(); return t;
        case '/': t=make(TOKEN_SLASH);  advance_char(); return t;
        case ';': t=make(TOKEN_SEMI);   advance_char(); return t;
        case '(': t=make(TOKEN_LPAREN); advance_char(); return t;
        case ')': t=make(TOKEN_RPAREN); advance_char(); return t;
        case '{': t=make(TOKEN_LBRACE); advance_char(); return t;
        case '}': t=make(TOKEN_RBRACE); advance_char(); return t;
        case '[': t=make(TOKEN_LBRACKET); advance_char(); return t;
        case ']': t=make(TOKEN_RBRACKET); advance_char(); return t;
        case ',': t=make(TOKEN_COMMA);  advance_char(); return t;
        case ':': t=make(TOKEN_COLON);  advance_char(); return t;
        case '.': t=make(TOKEN_DOT);    advance_char(); return t;
        case '=':
            advance_char();
            if (ch == '=') {
                t = make(TOKEN_EQ);
                advance_char();
            } else {
                t = make(TOKEN_ASSIGN);
            }
            return t;
        case '!':
            advance_char();
            if (ch == '=') {
                t = make(TOKEN_NE);
                advance_char();
            } else {
                t = make(TOKEN_NOT);
            }
            return t;
        case '<':
            advance_char();
            if (ch == '=') {
                t = make(TOKEN_LE);
                advance_char();
            } else {
                t = make(TOKEN_LT);
            }
            return t;
        case '>':
            advance_char();
            if (ch == '=') {
                t = make(TOKEN_GE);
                advance_char();
            } else {
                t = make(TOKEN_GT);
            }
            return t;
        case '&':
            advance_char();
            if (ch == '&') {
                t = make(TOKEN_AND);
                advance_char();
                return t;
            }
            break;
        case '|':
            advance_char();
            if (ch == '|') {
                t = make(TOKEN_OR);
                advance_char();
                return t;
            }
            break;
    }

    fprintf(stderr,"Unexpected character '%c'\n", ch);
    exit(1);
}

void init_lexer(const char *s) {
    src = s;
    pos = 0;
    ch = src[0];
    has_next = 0;
    advance_token();
}

void advance_token() {
    if (has_next) {
        token = next;
        has_next = 0;
    } else {
        token = next_token();
    }
}

Token current_tok() {
    return token;
}

Token peek_token() {
    if (!has_next) {
        next = next_token();
        has_next = 1;
    }
    return next;
}

void expect(TokenType t, const char *msg) {
    if (token.type != t) {
        fprintf(stderr,"Parse error: %s\n", msg);
        exit(1);
    }
    advance_token();
}
