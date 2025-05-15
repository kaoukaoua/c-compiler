#ifndef LEXER_H
#define LEXER_H

typedef enum {
  TOKEN_INT,
  TOKEN_IDENTIFIER,
  TOKEN_STRING,
  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_MUL,
  TOKEN_DIV,
  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_EQ, //equal
  TOKEN_NEQ, //not equal
  TOKEN_OPERATOR,
  TOKEN_EOF,
  TOKEN_INVALID,
  TOKEN_SEMICOLON,
  TOKEN_ASSIGN // FOR '='
} TokenType;

typedef struct {
  TokenType type;
  char* value;
} Token;

void lexer_init(const char* input);
Token lexer_next();
Token peek_token();

#endif
