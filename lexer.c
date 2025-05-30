#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"
#include <string.h>
#include <stdlib.h>

//strdup() & strndup() used to duplicate a string. 
//strdup duplicates the whole input its given
//strndup specifies how much of the input to copy

char* strndup(const char* s, size_t n) {
    char* out = (char*)malloc(n + 1);
    if (!out) return NULL;
    strncpy(out, s, n);
    out[n] = '\0';
    return out;
}

static Token make_token(TokenType type, const char* lexeme, int value) {
    Token tok;
    tok.type = type;
    tok.lexeme = strdup(lexeme);
    tok.value = value;
    return tok;
}

static void add_token(TokenList* list, Token token) {
    TokenNode* node = malloc(sizeof(TokenNode));
    node->token = token;
    node->next = NULL;

    if (!list->head) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }
}

//input is the first character of the input string
TokenList* lex(const char* input) {

    TokenList* list = malloc(sizeof(TokenList));
    list->head = NULL;
    list->tail = NULL;

    while (*input) {
          if (isspace(*input)) {
            input++;
        }

        if (isdigit(*input)) {
            int value = 0; // we define a variable "value" thatll hold the value of the digit/number
            const char* start = input; //we define "start" as the pointer to the first digit of the full number (if its a number)
            while (isdigit(*input)) { // walks through the input 
                value = value * 10 + (*input - '0'); // stores value of the digit
                input++;
            }
            int len = input - start; 
            char* text = strndup(start, len);
            add_token(list, make_token(TOKEN_INT, text, value));
            free(text);
            printf("Int lexing ok.\n");
        }

        if (isalpha(*input) || *input == '_') {
            const char* start = input;
            while (isalnum(*input) || *input == '_') {
                input++;
                int len = input - start;
                char* id = strndup(start, len);
                char strings[50];
                char* text = strncpy(strings, id, len);
            add_token(list, make_token(TOKEN_IDENTIFIER, text, 0));
            free(id);
            }
        }

        printf("alpha lexing ok.\n");

        if (*input == '"') {
            input++; // skip "
            const char* start = input;
            while (*input && *input != '"') {
                input++;
                int len = input - start;
                char* str = strndup(start, len);
                add_token(list, make_token(TOKEN_STRING, str, 0));
                free(str);
                if (*input == '"') input++; //we skip closing
            }
        }
        printf("string lexing ok.\n");

        if (*input == '<') {
            input++;
            if (*input == '=') { 
                input++; 
                add_token(list, make_token(TOKEN_LE, "<=", 0)); 
            } 
            else add_token(list, make_token(TOKEN_LT, "<", 0));
        }
        if (*input == '>') {
            input++;
            if (*input == '=') { 
                input++; 
                add_token(list, make_token(TOKEN_GE, ">=", 0)); 
            } 
            else add_token(list, make_token(TOKEN_GT, ">", 0));
        }
        if (*input == '{') { 
            input++; 
            add_token(list, make_token(TOKEN_LBRACE, "{", 0)); 
        }
        if (*input == '}') { 
            input++; 
            add_token(list, make_token(TOKEN_RBRACE, "}", 0)); 
        }
        printf("ops lexing ok.\n");

        // Also handle keywords like "if", "else", "while" in identifier logic

        switch (*input) {
            case '+': add_token(list, make_token(TOKEN_PLUS, "+", 0)); break;
            case '-': add_token(list, make_token(TOKEN_MINUS, "-", 0)); break;
            case '*': add_token(list, make_token(TOKEN_MUL, "*", 0)); break;
            case '/': add_token(list, make_token(TOKEN_DIV, "/", 0)); break;
            case '(': add_token(list, make_token(TOKEN_LPAREN, "(", 0)); break;
            case ')': add_token(list, make_token(TOKEN_RPAREN, ")", 0)); break;
            case ';': add_token(list, make_token(TOKEN_SEMICOLON, ";", 0)); break;
            case '=':
                if (*(input + 1) == '=') {
                    add_token(list, make_token(TOKEN_EQ, "==", 0));
                    input++;
                } else {
                    add_token(list, make_token(TOKEN_ASSIGN, "=", 0));
                }
                break;
            case '!':
                if (*(input + 1) == '=') {
                    add_token(list, make_token(TOKEN_NEQ, "!=", 0));
                    input++;
                } else {
                    add_token(list, make_token(TOKEN_INVALID, "!", 0));
                }
                break;
            default:
                add_token(list, make_token(TOKEN_INVALID, (char[]){*input, '\0'}, 0));
                break;
        }

        input++;
        add_token(list, make_token(TOKEN_EOF, "[EOF]", 0));
        return list;
        printf("lexerrr OK\n");
    }
}


void free_tokens(TokenList* list) {
    TokenNode* node = list->head;
    while (node) {
        free(node->token.lexeme);
        TokenNode* next = node->next;
        free(node);
        node = next;
    }
    free(list);
}

/*
 let a = 2 + 3 / 5
our main function in the lexer is lex: it takes the inputs, iterates through it, makes token of each lexeme (maketoken), and then add it to a list
(addtoken), which is called by the parser to parse; finally it frees the tokens(freetoken)






"make token": will tokenize the lexeme, one lexeme at a time, by deciding :
    - its type - its value -storing the lexeme

we will iterate through the input string (input), and then store the characters to another string (str), as long as the next character is not null
if its null, we loop back to the function
after making each token, we move to the function that  it will add it to a list (called list) which is the output of 





lets say the string 
fucntion to tokenize: 
    char strtok(char str, char delimiter)
int i = pos;
while(i != EOF){
    while(next_char !== NULL){
        add i to str = 
        str
    }
    strtok(i)
    next_char ++;
    i++;

    char str = (char) malloc(n*sizeof(char));
    for(int i=0, i<=NULL, i++){
        char str[i] += i;
    }
*/