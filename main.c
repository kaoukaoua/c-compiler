#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "interpret.h"
#include "optimizer.h"


/* // we print token list before parsing (for debugging)
void debug_print_tokens(const char* input) {
  lexer_init(input);
  Token t;
  printf("Tokens:\n");
  do {
    t = lexer_next();
    printf("type=%d, value=%s\n", t.type, t.value);
  } while (t.type != TOKEN_EOF);
  printf("\n");
}
 */
/* 
void print_ast(ASTNode* node) {
  if (!node) return;

  switch (node->type) {
    case NODE_INT:
      printf("%d", node->int_value);
      break;
    case NODE_VAR:
      printf("%s", node->var_name);
      break;
    case NODE_BINOP:
      printf("(");
      print_ast(node->binop.left);
      printf(" %c ", node->binop.op);
      print_ast(node->binop.right);
      printf(")");
      break;
    case NODE_ASSIGN:
      printf("%s = ", node->assign.var_name);
      print_ast(node->assign.value);
      break;
    case NODE_PRINT:
      printf("print(");
      print_ast(node->print_expr);
      printf(")");
      break;
    default:
      printf("Unknown AST node.\n");
  }
}
 */
int main() {
    // Declare a buffer to store user input
    char input[1024];

    printf("Enter a statement: ");
    fgets(input, sizeof(input), stdin);

    if (input[strlen(input) - 1] == '\n') {
        input[strlen(input) - 1] = '\0';
    }
    // Print the tokens for debugging
    //debug_print_tokens(input);

    // Initialize the parser
    parser_init(input);

    // Parse the statement
    ASTNode* stmt = optimize(parse_statement());

    // Print the resulting AST
    /* printf("Parsed AST:\n");
    print_ast(stmt);
 */
    int result = interpret(stmt);
    printf("result: %d\n", result);

    free_ast(stmt);

    return 0;
}
