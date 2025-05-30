#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpret.h"


int interpret(ASTNode* node) {
  switch (node->type) {
    case NODE_INT:
      return node->int_lit.value;

    case NODE_BINOP: {
      int left = interpret(node->binop.left);
      int right = interpret(node->binop.right);
      switch (node->binop.op) {
        case '+': return left + right;
        case '-': return left - right;
        case '*': return left * right;
        case '/': return right != 0 ? left / right : 0;
        case NODE_PRINT: {
        int val = interpret(node->assign.value); //in case we reuse
        printf("%d\n", val);
        return 0;
        }
        default: printf("Unknown operator\n"); exit(1);
      }
    }

    default:
      printf("ERROR.\n");
      exit(1);
  }
}

