#include "parser.h"
#include "lexer.h"
#include "util.h"
#include <string.h>

static ASTNode *expression();
static ASTNode *term();
static ASTNode *factor();
static ASTNode *primary();

ASTNode *parse_statement() {
    Token t = current_tok();

    if (t.type == TOKEN_LET) {
        advance_token();
        t = current_tok();
        if (t.type != TOKEN_IDENTIFIER)
            fatal("Expected identifier after let");

        char name[64];
        strcpy(name, t.lexeme);
        advance_token();

        expect(TOKEN_ASSIGN, "Expected '='");
        ASTNode *expr = expression();
        expect(TOKEN_SEMI, "Expected ';'");
        return new_assign(name, expr);
    }

    ASTNode *expr = expression();
    expect(TOKEN_SEMI, "Expected ';'");
    return expr;
}

static ASTNode *expression() {
    ASTNode *n = term();
    Token t = current_tok();

    while (t.type == TOKEN_PLUS || t.type == TOKEN_MINUS) {
        char op = (t.type == TOKEN_PLUS) ? '+' : '-';
        advance_token();
        ASTNode *r = term();
        n = new_binop(op, n, r);
        t = current_tok();
    }
    return n;
}

static ASTNode *term() {
    ASTNode *n = factor();
    Token t = current_tok();

    while (t.type == TOKEN_STAR || t.type == TOKEN_SLASH) {
        char op = (t.type == TOKEN_STAR) ? '*' : '/';
        advance_token();
        ASTNode *r = factor();
        n = new_binop(op, n, r);
        t = current_tok();
    }
    return n;
}

static ASTNode *factor() {
    Token t = current_tok();

    if (t.type == TOKEN_MINUS) {
        advance_token();
        return new_binop('-', new_number(0), factor());
    }
    return primary();
}

static ASTNode *primary() {
    Token t = current_tok();

    if (t.type == TOKEN_NUMBER) {
        double v = t.number;
        advance_token();
        return new_number(v);
    }

    if (t.type == TOKEN_IDENTIFIER) {
        char name[64];
        strcpy(name, t.lexeme);
        advance_token();

        if (strcmp(name,"print")==0 && current_tok().type==TOKEN_LPAREN) {
            advance_token();
            ASTNode *e = expression();
            expect(TOKEN_RPAREN,"Expected ')'");
            return new_print(e);
        }

        return new_var(name);
    }

    if (t.type == TOKEN_LPAREN) {
        advance_token();
        ASTNode *e = expression();
        expect(TOKEN_RPAREN,"Expected ')'");
        return e;
    }

    fatal("Unexpected token in primary");
    return NULL;
}

ASTNode *parse_expression() {
    return expression();
}
