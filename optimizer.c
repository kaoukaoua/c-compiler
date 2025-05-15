#include "parser.h"
#include "optimizer.h"
#include <stdlib.h>


ASTNode* optimize(ASTNode* node) {
  if (!node) return NULL;

  switch (node->type) {
    case NODE_BINOP: {
      ASTNode* left = optimize(node->binop.left);
      ASTNode* right = optimize(node->binop.right);

      if (left->type == NODE_INT && right->type == NODE_INT) {
        int result;
        switch (node->binop.op) {
          case '+': result = left->int_value + right->int_value; break;
          case '-': result = left->int_value - right->int_value; break;
          case '*': result = left->int_value * right->int_value; break;
          case '/': result = right->int_value != 0 ? left->int_value / right->int_value : 0; break;
          default: return node;
        }
        free_ast(node);
        return make_int_node(result);
      }

      node->binop.left = left;
      node->binop.right = right;
      return node;
    }

    case NODE_ASSIGN:
      node->assign.value = optimize(node->assign.value);
      return node;

    case NODE_PRINT:
      node->print_expr = optimize(node->print_expr);
      return node;

    default:
      return node;
  }
}
