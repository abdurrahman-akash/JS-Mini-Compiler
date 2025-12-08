#include "parser.h"
#include "lexer.h"
#include "util.h"
#include <string.h>
#include <stdlib.h>

static ASTNode *expression();
static ASTNode *logical_or();
static ASTNode *logical_and();
static ASTNode *comparison();
static ASTNode *term();
static ASTNode *factor();
static ASTNode *primary();
static ASTNode *statement();
static ASTNode *block();

ASTNode *parse_statement() {
    return statement();
}

static ASTNode *statement() {
    Token t = current_tok();

    // If statement
    if (t.type == TOKEN_IF) {
        advance_token();
        expect(TOKEN_LPAREN, "Expected '(' after if");
        ASTNode *condition = expression();
        expect(TOKEN_RPAREN, "Expected ')' after condition");
        
        ASTNode *then_branch = statement();
        ASTNode *else_branch = NULL;
        
        if (current_tok().type == TOKEN_ELSE) {
            advance_token();
            else_branch = statement();
        }
        
        return new_if(condition, then_branch, else_branch);
    }

    // While loop
    if (t.type == TOKEN_WHILE) {
        advance_token();
        expect(TOKEN_LPAREN, "Expected '(' after while");
        ASTNode *condition = expression();
        expect(TOKEN_RPAREN, "Expected ')' after condition");
        
        ASTNode *body = statement();
        return new_while(condition, body);
    }

    // Block statement
    if (t.type == TOKEN_LBRACE) {
        return block();
    }

    // Let statement
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

    // Check for assignment (identifier = expression)
    if (t.type == TOKEN_IDENTIFIER) {
        Token next_tok = peek_token();
        
        if (next_tok.type == TOKEN_ASSIGN) {
            // It's an assignment
            char name[64];
            strcpy(name, t.lexeme);
            advance_token();  // consume identifier
            advance_token();  // consume =
            ASTNode *expr = expression();
            expect(TOKEN_SEMI, "Expected ';'");
            return new_assign(name, expr);
        }
    }

    // Expression statement
    ASTNode *expr = expression();
    expect(TOKEN_SEMI, "Expected ';'");
    return expr;
}

static ASTNode *block() {
    expect(TOKEN_LBRACE, "Expected '{'");
    
    ASTNode **statements = malloc(sizeof(ASTNode*) * 256);
    int count = 0;
    
    while (current_tok().type != TOKEN_RBRACE && current_tok().type != TOKEN_EOF) {
        statements[count++] = statement();
        if (count >= 256) fatal("Too many statements in block");
    }
    
    expect(TOKEN_RBRACE, "Expected '}'");
    return new_block(statements, count);
}

static ASTNode *expression() {
    return logical_or();
}

static ASTNode *logical_or() {
    ASTNode *n = logical_and();
    
    while (current_tok().type == TOKEN_OR) {
        advance_token();
        ASTNode *r = logical_and();
        n = new_logical("||", n, r);
    }
    return n;
}

static ASTNode *logical_and() {
    ASTNode *n = comparison();
    
    while (current_tok().type == TOKEN_AND) {
        advance_token();
        ASTNode *r = comparison();
        n = new_logical("&&", n, r);
    }
    return n;
}

static ASTNode *comparison() {
    ASTNode *n = term();
    Token t = current_tok();

    while (t.type == TOKEN_EQ || t.type == TOKEN_NE || 
           t.type == TOKEN_LT || t.type == TOKEN_GT ||
           t.type == TOKEN_LE || t.type == TOKEN_GE ||
           t.type == TOKEN_PLUS || t.type == TOKEN_MINUS) {
        
        if (t.type == TOKEN_PLUS || t.type == TOKEN_MINUS) {
            char op = (t.type == TOKEN_PLUS) ? '+' : '-';
            advance_token();
            ASTNode *r = term();
            n = new_binop(op, n, r);
        } else {
            char op[3];
            if (t.type == TOKEN_EQ) strcpy(op, "==");
            else if (t.type == TOKEN_NE) strcpy(op, "!=");
            else if (t.type == TOKEN_LT) strcpy(op, "<");
            else if (t.type == TOKEN_GT) strcpy(op, ">");
            else if (t.type == TOKEN_LE) strcpy(op, "<=");
            else if (t.type == TOKEN_GE) strcpy(op, ">=");
            
            advance_token();
            ASTNode *r = term();
            n = new_comparison(op, n, r);
        }
        
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
    
    if (t.type == TOKEN_NOT) {
        advance_token();
        return new_logical("!", factor(), NULL);
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

    if (t.type == TOKEN_TRUE) {
        advance_token();
        return new_boolean(1);
    }

    if (t.type == TOKEN_FALSE) {
        advance_token();
        return new_boolean(0);
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
