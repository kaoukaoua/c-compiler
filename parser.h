#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"

typedef enum {
    NODE_INT,
    NODE_VAR,
    NODE_BINOP,
    NODE_ASSIGN,
    NODE_IF,
    NODE_WHILE,
    NODE_BLOCK,
    NODE_PRINT,
    NODE_BOOL,
    NODE_COMPARE,
    NODE_EQ,
    NODE_NEQ
} NodeType;


typedef struct ASTNode {
    NodeType type;
    union {
        struct { 
            int value; 
        } int_lit;
        struct { 
            char* name; 
        } var;
        struct {
            char op;
            struct ASTNode* left;
            struct ASTNode* right;
        } binop;
        struct {
            char* name;
            struct ASTNode* value;
        } assign; 
        struct { 
            struct ASTNode* expr; 
        } print;
        struct {
            struct ASTNode** stmts;
            int count;
        } block;
    }; 
} ASTNode;



ASTNode* parse(TokenList* tokens);
ASTNode* make_int_node(int value);
ASTNode* make_var_node(const char* name);
ASTNode* make_binop_node(char op, ASTNode* left, ASTNode* right);
ASTNode* make_assign_node(const char* name, ASTNode* value);

ASTNode* parse(TokenList* tokens);
void print_ast(ASTNode* node);
void free_ast(ASTNode* node);
Token peek();
int match(TokenType type);


#endif
