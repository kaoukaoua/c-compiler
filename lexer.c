#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

/*
//initialize lexer, returns the list of tokens
    lexer_init()

//just check the next token
    src[i]() 

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


static const char* src = NULL; // using a pointer to the string to access it instead of copying 
static int i = 0;

// ------------------------ functions ----------------

//initialize lexer, returns the list of tokens
void lexer_init(const char* input) {
  src = input; // src points to the string input
  i = 0;
}

//moves one position ahead
char lexer_advance() {
  return src[i++];
}

// checks if its a letter
int is_letter(char c) {
  return isalpha(c);
}

// checks if its a digit
int is_digit(char c) {
  return c >= '0' && c <= '9';
}

//checks if current char matches an expected character
int l_match(char e) {   //e is for expected
  if (src[i] == e) {
    lexer_advance(); // if it does match it moves ahead
  }
  return 0; 
}

void skip_whitespace() {
  while (isspace(src[i])) 
    lexer_advance();
}

char* getSubstring(const char* str, int start, int length) {
    char* subStr = malloc(length + 1);
    strncpy(subStr, str + start, length);
    subStr[length] = '\0';
    return subStr;
}



//--------------------- tokens --------------------------

Token make_token(TokenType type, const char* value) {
  Token t;
  t.type = type;
  t.value = value;
  return t;
}


Token read_number() {
  int start = i;
  while (is_digit(src[i])) 
    lexer_advance();
  int len = i - start;
  char* num = getSubstring(src, start, len);  // Use simple getSubstring
  return make_token(TOKEN_INT, num);
}

Token read_identifier() {
  int start = i;
  while (is_letter(src[i]) || is_digit(src[i]))  // evaluate if i is a digit in case the identifier contains a digit (eg: x1)
    lexer_advance();
  int len = i - start;
  char* id = getSubstring(src, start, len);
  return make_token(TOKEN_IDENTIFIER, id);
}

Token read_string() {
  lexer_advance(); // skip opening "
  int start = i;
  while (src[i] != '"' && src[i] != '\0') 
    lexer_advance();
  int len = i - start;
  char* str = getSubstring(src, start, len); // 
  lexer_advance(); // skip closing "
  return make_token(TOKEN_STRING, str);
}

Token read_operator() {
  char c = src[i];

  if (c == '=' && src[i + 1] == '=') {
    lexer_advance(); 
    lexer_advance();
    return make_token(TOKEN_EQ, "==");
  }
  if (c == '!' && src[i + 1] == '=') {
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

  char c = src[i];

  while (c != '\0') { // while its not EOF, keep checking
    if (is_letter(c)) 
      return read_identifier();
    if (is_digit(c)) 
      return read_number();
    if (c == '"') 
      return read_string();
    
      //to diffferentiate between assignm operator '=' and checking '=='
    //case 1: equal op
    if (c == '=' && src[i + 1] == '=') {
      lexer_advance(); lexer_advance();
      return make_token(TOKEN_EQ, "==");
    }
    //case 2: assignment op
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
   return make_token(TOKEN_EOF, "");
  }
}



Token peek_token() {
  int saved_i = i;
  Token t = lexer_next();
  i = saved_i;
  return t;
}
