//  first define the ASTNode, 
// and the NodeType(int, vrbl, binary op, assignment, print)
// finally the fct declarations: (parser_init, parser_statement, free_ast..)

/*fucntions:
    parser_init(): initializes the lexer and takes the first token
    advance() = moves forward
    p_match(type) = checks if token matches and moves if its true
    expect(type) = if token doesnt match, stops and exits 
*/

/* We build AST nodes for int, var, binop, 
    make_int_node() 
    make_var_node(), 
    make_binop_node(), 

/* Create nodes for assignments (like let..) and "print" statement
    make_assign_node(), 
    make_print_node(), 

/* Parse THE statement first, then expression, then term, then factor..
    parse_statement(), 

/* PARSING EXPRESSIONS
    parse_expr() = for + and -
    parse_term() = for * and /
    parse_factor() = for literals, vrbls, parentheses
*/

#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static Token t; // current token that we iterate

void parser_init(const char* input) {
  lexer_init(input);
  t = lexer_next();
}

void advance() {
  t = lexer_next();
}

int p_match(TokenType type) {
  if (t.type == type) {
    advance();
    return 1;
  }
  return 0;
}

void expect(TokenType type) {
  if (!p_match(type)) {
    printf("Unexpected token: %s\n", t.value);
    exit(1);
  }
}



// make nodes for integers, variables, op...

ASTNode* make_int_node(int value) {
  ASTNode* node = malloc(sizeof(ASTNode));
  node->type = NODE_INT;
  node->int_value = value;
  return node;
}

ASTNode* make_var_node(char* name) {
  ASTNode* node = malloc(sizeof(ASTNode));
  node->type = NODE_VAR;
  node->var_name = strdup(name);
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

//builds a node for an assignment
ASTNode* make_assign_node(char* name, ASTNode* value) {
  ASTNode* node = malloc(sizeof(ASTNode));
  node->type = NODE_ASSIGN;
  node->assign.var_name = strdup(name);
  node->assign.value = value;
  return node;
}

// build a node for a "print" statement
ASTNode* make_print_node(ASTNode* expr) {
  ASTNode* node = malloc(sizeof(ASTNode));
  node->type = NODE_PRINT;
  node->print_expr = expr;
  return node;
}

// Forward declarations
ASTNode* parse_expr();
ASTNode* parse_term();
ASTNode* parse_factor();

// Grammar:
// stmt: "let" IDENT "=" expr
//     | "print" "(" expr ")"
//     | expr


// to recognize and decide the statement were parsing, 
// is it identifier, is it expr, is it print,
ASTNode* parse_statement() {
  if (t.type == TOKEN_IDENTIFIER && strcmp(t.value, "let") == 0) {
    advance(); // consume 'let'
    if (t.type != TOKEN_IDENTIFIER) {
      printf("Expected variable name\n");
      exit(1);
    }
    char* name = strdup(t.value);
    advance();
    expect(TOKEN_ASSIGN); // expect '='
    ASTNode* val = parse_expr();
    return make_assign_node(name, val);
  }

  if (t.type == TOKEN_IDENTIFIER && strcmp(t.value, "print") == 0) {
    advance(); // consume 'print'
    expect(TOKEN_LPAREN);
    ASTNode* val = parse_expr();
    expect(TOKEN_RPAREN);
    return make_print_node(val);
  }

  return parse_expr(); // fallback
}

// parses expr wit + or -, and build binary operation nodes
ASTNode* parse_expr() {
  ASTNode* left = parse_term();
  while (t.type == TOKEN_PLUS || t.type == TOKEN_MINUS) {
    char op = t.value[0];  // we store the operand 
    advance();
    ASTNode* right = parse_term();
    left = make_binop_node(op, left, right);
  }
  return left;
}

// parses higher priority operators * and /
ASTNode* parse_term() {
  ASTNode* left = parse_factor();
  while (t.type == TOKEN_MUL || t.type == TOKEN_DIV) {
    char op = t.value[0];
    advance();
    ASTNode* right = parse_factor();
    left = make_binop_node(op, left, right);
  }
  return left;
}

// parses integers, vrbl names, grouped expressions
ASTNode* parse_factor() {
  if (t.type == TOKEN_INT) {
    int val = atoi(t.value);
    advance();
    return make_int_node(val);
  }

  if (t.type == TOKEN_IDENTIFIER) {
    char* name = strdup(t.value);
    advance();
    return make_var_node(name);
  }

  if (p_match(TOKEN_LPAREN)) {
    ASTNode* expr = parse_expr(); // we call back parse_expr() for parentheses
    expect(TOKEN_RPAREN);
    return expr;
  }

  printf("Unexpected token in factor: %s\n", t.value);
  exit(1);
}

void free_ast(ASTNode* node) {
  if (!node) return;
  switch (node->type) {
    case NODE_INT: break;
    case NODE_VAR: free(node->var_name); break;
    case NODE_BINOP:
      free_ast(node->binop.left);
      free_ast(node->binop.right);
      break;
    case NODE_ASSIGN:
      free(node->assign.var_name);
      free_ast(node->assign.value);
      break;
    case NODE_PRINT:
      free_ast(node->print_expr);
      break;
  }
  free(node);
}
