#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int interpret(ASTNode* node) {
  switch (node->type) {
    case NODE_INT:
      return node->int_value;

    case NODE_BINOP: {
      int left = interpret(node->binop.left);
      int right = interpret(node->binop.right);
      switch (node->binop.op) {
        case '+': return left + right;
        case '-': return left - right;
        case '*': return left * right;
        case '/': return right != 0 ? left / right : 0;
        default: printf("Unknown operator\n"); exit(1);
      }
    }

    default:
      printf("Cannot evaluate this AST node type\n");
      exit(1);
  }
}
