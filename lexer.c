#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

/*
//initialize lexer, returns the list of tokens
    lexer_init()

//just check the next token
    lexer_peek() 

//move position to next token
    char lexer_advance()

//character check
    int isletter(c)
    int isdigit(c)
    int match(e)

//returns next token
    void skip_whitespace()
    next_token()
    make_token()

//read names, integers, string literals, opeators..
    Token read_identifier()
    Token read_number()
    Token read_string()
    Token read_operator()

// finally check the next token
    lexer_next()
*/


static const char* src = NULL;
static int pos = 0;

//initialize lexer, returns the list of tokens
void lexer_init(const char* input) {
  src = input;
  pos = 0;
}

char lexer_peek() {
  return src[pos];
}

char lexer_advance() {
  return src[pos++];
}

int is_letter(char c) {
  return isalpha(c) || c == '_';
}

int is_digit(char c) {
  return c >= '0' && c <= '9';
}

int l_match(char e) {   //e is for expected character
  if (lexer_peek() == e) {
    lexer_advance();
    return 1;
  }
  return 0;
}

void skip_whitespace() {
  while (isspace(lexer_peek())) 
    lexer_advance();
}

// Simple string duplication function
char* str_duplicate(const char* start, int len) {
  char* str = malloc(len + 1); // +1 for the null terminator
  if (!str) return NULL;
  strncpy(str, start, len);
  str[len] = '\0';
  return str;
}

Token make_token(TokenType type, const char* value) {
  Token t;
  t.type = type;
  t.value = strdup(value);
  return t;
}


Token read_number() {
  int start = pos;
  while (is_digit(lexer_peek())) 
    lexer_advance();
  int len = pos - start;
  char* num = str_duplicate(src + start, len);  // Use simple str_duplicate
  return make_token(TOKEN_INT, num);
}

Token read_identifier() {
  int start = pos;
  while (is_letter(lexer_peek()) || is_digit(lexer_peek())) 
    lexer_advance();
  int len = pos - start;
  char* id = str_duplicate(src + start, len);
  return make_token(TOKEN_IDENTIFIER, id);
}

Token read_string() {
  lexer_advance(); // skip opening "
  int start = pos;
  while (lexer_peek() != '"' && lexer_peek() != '\0') 
    lexer_advance();
  int len = pos - start;
  char* str = str_duplicate(src + start, len); // 
  lexer_advance(); // skip closing "
  return make_token(TOKEN_STRING, str);
}

Token read_operator() {
  char c = lexer_peek();

  if (c == '=' && src[pos + 1] == '=') {
    lexer_advance(); 
    lexer_advance();
    return make_token(TOKEN_EQ, "==");
  }
  if (c == '!' && src[pos + 1] == '=') {
    lexer_advance(); 
    lexer_advance();
    return make_token(TOKEN_NEQ, "!=");
  }

  lexer_advance();
  char op[2] = { c, '\0' };
  return make_token(TOKEN_OPERATOR, op);
}

Token lexer_next() {
  skip_whitespace();

  char c = lexer_peek();
  if (c == '\0') 
    return make_token(TOKEN_EOF, "");

  if (is_letter(c)) 
    return read_identifier();
  if (is_digit(c)) 
    return read_number();
  if (c == '"') 
    return read_string();
  
    //to diffferentiate between assignm operator '=' and checking '=='
  if (c == '=' && src[pos + 1] == '=') {
    lexer_advance(); lexer_advance();
    return make_token(TOKEN_EQ, "==");
  }
  if (c == '=') {
    lexer_advance();
    return make_token(TOKEN_ASSIGN, "=");
  }


  switch (c) {
    case '+': lexer_advance(); return make_token(TOKEN_PLUS, "+");
    case '-': lexer_advance(); return make_token(TOKEN_MINUS, "-");
    case '*': lexer_advance(); return make_token(TOKEN_MUL, "*");
    case '/': lexer_advance(); return make_token(TOKEN_DIV, "/");
    case '(': lexer_advance(); return make_token(TOKEN_LPAREN, "(");
    case ')': lexer_advance(); return make_token(TOKEN_RPAREN, ")");
    case ';': lexer_advance(); return make_token(TOKEN_SEMICOLON, ";");

    default:
      lexer_advance();
      return make_token(TOKEN_INVALID, "?");
  }
}

Token peek_token() {
  int saved_pos = pos;
  Token t = lexer_next();
  pos = saved_pos;
  return t;
}
