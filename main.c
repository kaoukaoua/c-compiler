#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "interpret.h"
#include "optimizer.h"

//read file function, returns a buffer 
char* read_file(const char* filename) {
  FILE* file = fopen(filename, "r");
  if (!file) return NULL;

  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  rewind(file);

  char* buffer = malloc(size + 1);
  fread(buffer, 1, size, file);
  buffer[size] = '\0';

  fclose(file);
  return buffer;
}

void print_ast(ASTNode* node) {
    if (node->type == NODE_INT) {
        printf("%d", node->int_lit.value);
    } else if (node->type == NODE_BINOP) {
        printf("(");
        print_ast(node->binop.left);
        printf(" %c ", node->binop.op);
        print_ast(node->binop.right);
        printf(")");
    }
}





//main function
int main(int argc, char* argv[]) {
    //we check
    if (argc<2) {
      printf("Please specify the test file.\n");
      return 1;
    }

    const char* filename = argv[1];
    char* input = read_file(filename);
    //input is the pointer to the first character of the input content
    printf("MAIN OK\n");
    if (!input) {
        printf("Failed to read file: %s\n", filename);
        return 1;
    }

    
    // Step 1: Lexing
    TokenList* tokens = lex(input);
    printf("lexing OK\n");
    // Step 2: Parsing
    
    ASTNode* ast = parse(tokens);
    print_ast(ast);
    //printf("parsed ast: \n");
    printf("\n");

    // Step 3: Interpreting
    int result = interpret(ast);

    printf("Result: %d\n", result);

    free_tokens(tokens);
    free_ast(ast);
    free(tokens);
    return 0;
}
