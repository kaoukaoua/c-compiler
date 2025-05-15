#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef enum {
  NODE_INT,
  NODE_VAR,
  NODE_BINOP, //binary op
  NODE_ASSIGN,
  NODE_PRINT
} NodeType;

typedef struct ASTNode ASTNode;

struct ASTNode {
  NodeType type;
  union {
    int int_value;
    char* var_name;
    struct {
      char op;
      ASTNode* left;
      ASTNode* right;
    } binop;
    struct {
      char* var_name;
      ASTNode* value;
    } assign;
    ASTNode* print_expr;
  };
};

void parser_init(const char* input);
ASTNode* parse_statement();
void free_ast(ASTNode* node);

ASTNode* make_int_node(int value);
ASTNode* make_var_node(char* name);
ASTNode* make_binop_node(char op, ASTNode* left, ASTNode* right);
ASTNode* make_assign_node(char* name, ASTNode* value);
ASTNode* make_print_node(ASTNode* expr);


#endif
