#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static TokenNode* current;
//the helpers
Token peek() {
    return current->token;
}

static void advance() {
    if (current->next)
        current = current->next;
}

int match(TokenType type) {
    if (peek().type == type) {
        advance();
        return 1;
    }
    return 0;
}

// Forward declarations
static ASTNode* primary();
static ASTNode* T();
static ASTNode* E(); 
static ASTNode* F();


//fcts to make nodes depending on the type
ASTNode* make_int_node(int value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_INT;
    node->int_lit.value = value;
    return node;
}

ASTNode* make_var_node(const char* name) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_VAR;
    node->var.name = strdup(name);
    return node;
}

ASTNode* make_binop_node(char op, ASTNode* left, ASTNode* right) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_BINOP;
    node->binop.op = op;
    node->binop.left = left;
    node->binop.right = right;
    return node;
}

ASTNode* make_assign_node(const char* name, ASTNode* value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_ASSIGN;
    node->assign.name = strdup(name);
    node->assign.value = value;
    return node;
}


//what primary does is checking (using match()) the given token type and make a node of its type
//check is by match(), which advances if the pointer to the given node matches a type, and returns 0 if not
static ASTNode* primary() {
    Token tok = peek(); // peek is basically the pointer pointing to the current token 

    if (match(TOKEN_INT)) {
        return make_int_node(tok.value);
    } else if (match(TOKEN_IDENTIFIER)) {
        char* name = strdup(tok.lexeme);
        if (match(TOKEN_ASSIGN)) {
            ASTNode* val = E();
            return make_assign_node(name, val);
        }
        return make_var_node(name);
    } else if (match(TOKEN_LPAREN)) {
        ASTNode* node = E();
        match(TOKEN_RPAREN);
        return node;
    }

    return NULL;
}

static ASTNode* T() {
    ASTNode* node = primary();

    while (peek().type == TOKEN_MUL || peek().type == TOKEN_DIV) {
        char op = peek().lexeme[0];
        advance();
        ASTNode* right = primary();
        node = make_binop_node(op, node, right);
    }

    return node;
}

static ASTNode* E() {
    ASTNode* node = T();

    while (peek().type == TOKEN_PLUS || peek().type == TOKEN_MINUS) {
        char op = peek().lexeme[0];
        advance();
        ASTNode* right = T();
        node = make_binop_node(op, node, right);
    }

    return node;
}
//the parse function that makes the parsing, and class the other fcts

ASTNode* parse(TokenList* tokens) {
    current = tokens->head;
    ASTNode** stmts = NULL;
    int count = 0;

    while (peek().type != TOKEN_SEMICOLON) {
        ASTNode* stmt = E();  // or call statement() if you have one
        stmts = realloc(stmts, sizeof(ASTNode*) * (count + 1));
        stmts[count++] = stmt;
        match(TOKEN_SEMICOLON); // optional semicolon match
    }

    return stmts;
}


void free_ast(ASTNode* node) {

    if (!node) return;
    switch (node->type) {
        case NODE_BINOP:
            free_ast(node->binop.left);
            free_ast(node->binop.right);
            break;
        case NODE_ASSIGN:
            free(node->assign.name);
            free_ast(node->assign.value);
            break;
        case NODE_VAR:
            free(node->var.name);
            break;
        default: break;
    }
    free(node);
}
