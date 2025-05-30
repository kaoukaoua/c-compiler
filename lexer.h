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
  TOKEN_LBRACE, // {
  TOKEN_RBRACE, // }
  TOKEN_LT,     // <
  TOKEN_GT,     // >
  TOKEN_LE,     // <=
  TOKEN_GE,     // >=
  TOKEN_ASSIGN// for '='
} TokenType;

typedef struct {
    TokenType type;
    char* lexeme;
    int value;  // valid if type == TOKEN_INT
} Token;

typedef struct TokenNode {
    Token token;
    struct TokenNode* next;
} TokenNode;

typedef struct {
    TokenNode* head;
    TokenNode* tail;
} TokenList;

// Lexer functions
TokenList* lex(const char* input);
void print_tokens(TokenList* list);
void free_tokens(TokenList* list);

#endif
