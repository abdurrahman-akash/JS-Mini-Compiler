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

    // Function declaration
    if (t.type == TOKEN_FUNCTION) {
        advance_token();
        
        if (current_tok().type != TOKEN_IDENTIFIER) {
            fatal("Expected function name");
        }
        char func_name[256];
        strcpy(func_name, current_tok().lexeme);
        advance_token();
        
        expect(TOKEN_LPAREN, "Expected '(' after function name");
        
        char **params = malloc(sizeof(char*) * 256);
        int param_count = 0;
        
        while (current_tok().type != TOKEN_RPAREN && current_tok().type != TOKEN_EOF) {
            if (current_tok().type != TOKEN_IDENTIFIER) {
                fatal("Expected parameter name");
            }
            params[param_count] = malloc(256);
            strcpy(params[param_count], current_tok().lexeme);
            param_count++;
            advance_token();
            
            if (current_tok().type == TOKEN_COMMA) {
                advance_token();
            } else {
                break;
            }
        }
        expect(TOKEN_RPAREN, "Expected ')' after parameters");
        
        ASTNode *body = statement();  // Function body (usually a block)
        ASTNode *func = new_function(params, param_count, body);
        
        // Store function as variable
        return new_assign(func_name, func);
    }

    // Return statement
    if (t.type == TOKEN_RETURN) {
        advance_token();
        ASTNode *expr = NULL;
        if (current_tok().type != TOKEN_SEMI) {
            expr = expression();
        }
        expect(TOKEN_SEMI, "Expected ';' after return");
        return new_return(expr);
    }

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

    // Try-catch-finally
    if (t.type == TOKEN_TRY) {
        advance_token();
        ASTNode *try_block = statement();
        
        ASTNode *catch_block = NULL;
        char catch_param[256] = {0};
        
        if (current_tok().type == TOKEN_CATCH) {
            advance_token();
            expect(TOKEN_LPAREN, "Expected '(' after catch");
            
            if (current_tok().type == TOKEN_IDENTIFIER) {
                strcpy(catch_param, current_tok().lexeme);
                advance_token();
            }
            
            expect(TOKEN_RPAREN, "Expected ')' after catch parameter");
            catch_block = statement();
        }
        
        ASTNode *finally_block = NULL;
        if (current_tok().type == TOKEN_FINALLY) {
            advance_token();
            finally_block = statement();
        }
        
        return new_try(try_block, catch_param, catch_block, finally_block);
    }

    // Throw statement
    if (t.type == TOKEN_THROW) {
        advance_token();
        ASTNode *expr = expression();
        expect(TOKEN_SEMI, "Expected ';' after throw");
        return new_throw(expr);
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

    if (t.type == TOKEN_STRING) {
        char str[256];
        strcpy(str, t.lexeme);
        advance_token();
        return new_string(str);
    }

    if (t.type == TOKEN_TRUE) {
        advance_token();
        return new_boolean(1);
    }

    if (t.type == TOKEN_FALSE) {
        advance_token();
        return new_boolean(0);
    }

    // Function expression: function(params) { body }
    if (t.type == TOKEN_FUNCTION) {
        advance_token();
        
        expect(TOKEN_LPAREN, "Expected '(' after function keyword");
        
        char **params = malloc(sizeof(char*) * 256);
        int param_count = 0;
        
        while (current_tok().type != TOKEN_RPAREN && current_tok().type != TOKEN_EOF) {
            if (current_tok().type != TOKEN_IDENTIFIER) {
                fatal("Expected parameter name");
            }
            params[param_count] = malloc(256);
            strcpy(params[param_count], current_tok().lexeme);
            param_count++;
            advance_token();
            
            if (current_tok().type == TOKEN_COMMA) {
                advance_token();
            } else {
                break;
            }
        }
        expect(TOKEN_RPAREN, "Expected ')' after parameters");
        
        ASTNode *body = statement();  // Function body (usually a block)
        return new_function(params, param_count, body);
    }

    // Array literal
    if (t.type == TOKEN_LBRACKET) {
        advance_token();
        ASTNode **elements = malloc(sizeof(ASTNode*) * 256);
        int count = 0;
        
        while (current_tok().type != TOKEN_RBRACKET && current_tok().type != TOKEN_EOF) {
            elements[count++] = expression();
            if (current_tok().type == TOKEN_COMMA) {
                advance_token();
            } else {
                break;
            }
        }
        expect(TOKEN_RBRACKET, "Expected ']'");
        return new_array(elements, count);
    }

    // Object literal
    if (t.type == TOKEN_LBRACE) {
        advance_token();
        char **keys = malloc(sizeof(char*) * 256);
        ASTNode **values = malloc(sizeof(ASTNode*) * 256);
        int count = 0;
        
        while (current_tok().type != TOKEN_RBRACE && current_tok().type != TOKEN_EOF) {
            if (current_tok().type != TOKEN_IDENTIFIER && current_tok().type != TOKEN_STRING) {
                fatal("Expected property name");
            }
            
            keys[count] = malloc(256);
            strcpy(keys[count], current_tok().lexeme);
            advance_token();
            
            expect(TOKEN_COLON, "Expected ':' after property name");
            values[count] = expression();
            count++;
            
            if (current_tok().type == TOKEN_COMMA) {
                advance_token();
            } else {
                break;
            }
        }
        expect(TOKEN_RBRACE, "Expected '}'");
        return new_object(keys, values, count);
    }

    if (t.type == TOKEN_IDENTIFIER) {
        char name[256];
        strcpy(name, t.lexeme);
        advance_token();

        // Function call or print
        if (current_tok().type == TOKEN_LPAREN) {
            advance_token();
            ASTNode **args = malloc(sizeof(ASTNode*) * 256);
            int arg_count = 0;
            
            while (current_tok().type != TOKEN_RPAREN && current_tok().type != TOKEN_EOF) {
                args[arg_count++] = expression();
                if (current_tok().type == TOKEN_COMMA) {
                    advance_token();
                } else {
                    break;
                }
            }
            expect(TOKEN_RPAREN, "Expected ')'");
            
            // Special handling for print
            if (strcmp(name, "print") == 0 && arg_count > 0) {
                return new_print(args[0]);
            }
            
            return new_call(name, args, arg_count);
        }

        // Special handling for console.log()
        if (strcmp(name, "console") == 0 && current_tok().type == TOKEN_DOT) {
            advance_token();
            if (current_tok().type == TOKEN_IDENTIFIER && strcmp(current_tok().lexeme, "log") == 0) {
                advance_token();
                if (current_tok().type == TOKEN_LPAREN) {
                    advance_token();
                    ASTNode **args = malloc(sizeof(ASTNode*) * 256);
                    int arg_count = 0;
                    
                    while (current_tok().type != TOKEN_RPAREN && current_tok().type != TOKEN_EOF) {
                        args[arg_count++] = expression();
                        if (current_tok().type == TOKEN_COMMA) {
                            advance_token();
                        } else {
                            break;
                        }
                    }
                    expect(TOKEN_RPAREN, "Expected ')'");
                    
                    if (arg_count > 0) {
                        return new_print(args[0]);
                    }
                }
            }
        }

        // Member access or array indexing
        ASTNode *node = new_var(name);
        while (1) {
            if (current_tok().type == TOKEN_DOT) {
                advance_token();
                if (current_tok().type != TOKEN_IDENTIFIER) {
                    fatal("Expected property name after '.'");
                }
                char member[256];
                strcpy(member, current_tok().lexeme);
                advance_token();
                node = new_member(node, member);
            } else if (current_tok().type == TOKEN_LBRACKET) {
                advance_token();
                ASTNode *index = expression();
                expect(TOKEN_RBRACKET, "Expected ']'");
                node = new_index(node, index);
            } else {
                break;
            }
        }
        
        return node;
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
