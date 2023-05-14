/*
Instituição: IFSul Câmpus Passo Fundo
Curso: Bacharelado em Ciência da Computação
Disciplina: Compiladores
Atividade: Analisador Léxico para Tiny
Nome(s): Darlan Noetzold e Jakelyny Sousa de Araújo
Data: 09/05/2023

Resumo do Trabalho

Esta é uma implementação parcial de um analisador léxico (também conhecido como lexer ou scanner) para uma linguagem de programação. O lexer lê um fluxo de caracteres (de um arquivo, neste caso) e produz um fluxo de tokens, que são os blocos básicos de construção da linguagem. Os tokens podem então ser passados ​​para um analisador, que os usará para construir uma árvore de sintaxe abstrata (AST) que representa a estrutura do programa.

O código define uma enumeração TokenType, que contém valores para vários tipos de tokens, como palavras reservadas, identificadores, constantes numéricas e operadores. Ele também define uma estrutura Token, que contém o tipo e o valor de um token.

A função getNextToken é a parte principal do lexer. Ele lê os caracteres do arquivo de entrada um por vez e alterna entre diferentes estados com base no caractere que encontra. Dependendo do estado, ele define o tipo do token atual e adiciona o caractere ao seu valor. Depois de ler um token completo, ele o retorna.

A função isReservedWord verifica se uma determinada string é uma palavra reservada no idioma. Se for, retorna 1, caso contrário, retorna 0.
*/

#include <stdio.h>
#include <ctype.h>

#define MAX_TOKEN_LENGTH 20  // Tamanho Máximo de um Token

// Definindo as classes dos tokens
typedef enum {
  TOKEN_RESERVED_WORD,
  TOKEN_IDENTIFIER,
  TOKEN_NUMERIC_CONSTANT,
  TOKEN_ALPHANUMERIC_CONSTANT,
  TOKEN_ARITHMETIC_OPERATOR,
  TOKEN_COMPARISON_OPERATOR,
  TOKEN_DELIMITER,
  TOKEN_OPEN_PAREN,
  TOKEN_CLOSE_PAREN,
  TOKEN_ASSIGNMENT,
  TOKEN_SPECIAL,
  TOKEN_COMMENT,
  TOKEN_UNKNOWN,
  TOKEN_DELIMITER_WITH_BAR,
  TOKEN_EOF  
} TokenType;

// Estrutura de um token
typedef struct {
  TokenType type;
  char value[MAX_TOKEN_LENGTH + 1];
} Token;

// Função que grava os erros e Tokens desconhecidos em um arquivo texto
int writeOnError(char string[]){
  FILE *outputFile;
  outputFile = fopen("tiny_err_output.txt", "a");

  if (outputFile == NULL) {
      printf("Erro ao abrir o arquivo.\n");
      return 1;
  }
  fprintf(outputFile, "%s", string);

  fclose(outputFile);

  return 0;
}

// Função que grava os tokens reconhecidos em um arquivo de texto
int writeOnOk(char string[]){
  FILE *outputFile;
  outputFile = fopen("tiny_ok_output.txt", "a");

  if (outputFile == NULL) {
      printf("Erro ao abrir o arquivo.\n");
      return 1;
  }
  fprintf(outputFile, "%s", string);

  fclose(outputFile);

  return 0;
}

int newLine = 1;

// Função que reconhece as palavras reservadas do tiny
int isReservedWord(const char *value) {
  if (value[0] == 'e' && value[1] == 'n' && value[2] == 'd' && value[3] == '\0') {
    return 1;
  } else if (value[0] == 'i' && value[1] == 'f' && value[2] == '\0') {
    return 1;
  } else if (value[0] == 't' && value[1] == 'h' && value[2] == 'e' && value[3] == 'n' && value[4] == '\0') {
    return 1;
  } else if (value[0] == 'r' && value[1] == 'e' && value[2] == 'p' && value[3] == 'e' && value[4] == 'a' && value[5] == 't' && value[6] == '\0') {
    return 1;
  } else if (value[0] == 'd' && value[1] == 'o' && value[2] == '\0') {
    return 1;
  } else if (value[0] == 'e' && value[1] == 'l' && value[2] == 's' && value[3] == 'e' && value[4] == '\0') { 
    return 1;
  } else if (value[0] == 'r' && value[1] == 'e' && value[2] == 'a' && value[3] == 'd' && value[4] == '\0') { 
    return 1;
  } else if (value[0] == 'u' && value[1] == 'n' && value[2] == 't' && value[3] == 'i' && value[4] == 'l' && value[5] == '\0') { 
    return 1;
  } else if (value[0] == 'w' && value[1] == 'r' && value[2] == 'i' && value[3] == 't' && value[4] == 'e' && value[5] == '\0') { 
    return 1;
  } else {
    return 0;
  }
}

// Contador para garantir o tamanho de um TOKEN_ALPHANUMERIC_CONSTANT
int CONT_CONST = 0;


// Função que analisa cada token
Token getNextToken(FILE *inputFile) {
  Token token;
  char c;
  int state = 0; //Estes estados vão variar dependendo do token que estiver sendo processado
  int i = 0;
  while ((c = fgetc(inputFile)) != EOF) {
    switch (state) {
      case 0: // Estado padrão
        if(c == '\n') newLine += 1;
        if (c == '{') {
          state = 1;
          token.type = TOKEN_COMMENT;
        }else if (isalpha(c)) {
          state = 2;
          token.type = TOKEN_IDENTIFIER;
          token.value[i++] = c;
        } else if (isdigit(c)) {
          state = 3;
          token.type = TOKEN_NUMERIC_CONSTANT;
          token.value[i++] = c;
        } else if (c == '"') {
          CONT_CONST = 0;
          state = 4;
          token.type = TOKEN_ALPHANUMERIC_CONSTANT;
        } else if (c == '+'
            || c == '-'
            || c == '*'
            || c == '/') {
          state = 5;
          token.type = TOKEN_ARITHMETIC_OPERATOR;
          token.value[i++] = c;
        } else if (c == '='
            || c == '<') {
          state = 6;
          token.type = TOKEN_COMPARISON_OPERATOR;
          token.value[i++] = c;
        } else if (c == ';' || isspace(c)) {
          state = 7;
          token.type = TOKEN_DELIMITER;
          token.value[i++] = c;
        } else if (c == '(') {
          state = 8;
          token.type = TOKEN_OPEN_PAREN;
          token.value[i++] = c;
        } else if (c == ')') {
          state = 9;
          token.type = TOKEN_CLOSE_PAREN;
          token.value[i++] = c;
        } else if (c == '!') {
          state = 10;
          token.type = TOKEN_SPECIAL;
          token.value[i++] = c;
        } else if (c == ':') {
          state = 11;
          token.value[i++] = c;
        } else if (c == '\\') {
          state = 13;
          token.type = TOKEN_DELIMITER;
          token.value[i++] = c;
        } else {
          state = 12;
          token.type = TOKEN_UNKNOWN;
          token.value[i++] = c;
        }
      break;
      case 1: // Estado para validação de comentários
        if (c == '}') {
          token.value[i] = '\0';
          return token;
        }
        token.value[i++] = c;
      break;
      case 2: // Estado para identificar palavras reservadas e identificadores
        if (isalpha(c) || isdigit(c)) {
          token.value[i++] = c;
        } else {
          ungetc(c, inputFile);
          token.value[i] = '\0';
          if (isReservedWord(token.value)) {
            token.type = TOKEN_RESERVED_WORD;
          }else{
            token.type = TOKEN_IDENTIFIER;
          }
          
          return token;
        }
      break;
      case 3: // Estado para reconhecer constantes numéricas
        if (isdigit(c)) {
          token.value[i++] = c;
        } else {
          ungetc(c, inputFile);
          token.value[i] = '\0';
          return token;
        }
      break;
      case 4: // Estado para reconhecer constantes alfanuméricas, utilizando aspas duplas
        CONT_CONST = CONT_CONST +1;
        if (c == '"') {
          token.value[i] = '\0';
          return token;
        } else {
          if(CONT_CONST > MAX_TOKEN_LENGTH){ // Gera erro se for maior que o tamanho máximo de token
            char message[1000];
            printf("ERROR ON LENGHT: Token type: TOKEN_ALPHANUMERIC_CONSTANT, Value: %s\n", token.value);
            snprintf(message, sizeof(message), "ERROR ON LENGHT: Token type: TOKEN_ALPHANUMERIC_CONSTANT, Value: %s\n", token.value);
            writeOnError(message);
            return token;
          }
          token.value[i++] = c;
        }
      break;
      case 5: // Estado para reconhecer operadores aritméticos
        token.value[i] = '\0';
        ungetc(c, inputFile);
        return token;
      case 6: // Estado para reconhecer operadores de comparação
        if (c == '=') {
          token.value[i++] = c;
        } else {
          ungetc(c, inputFile);
        }
        token.value[i] = '\0';
        return token;
      case 7: // Estado para reconhecer ;
      case 8: // Estado para reconhecer (
      case 9: // Estado para reconhecer )
      case 10: // Estado para reconhecer !
        token.value[i] = '\0';
        ungetc(c, inputFile);
        return token;
      case 11: // Estado para reconhecer atribuição
        if (c == '=') { 
          state = 0;
          token.type = TOKEN_ASSIGNMENT;
          token.value[i] = c;
          i++;
          token.value[i] = '\0';
          return token;
        }else {
          token.type = TOKEN_UNKNOWN;
          return token;
        }
      case 12: // Estado para tokens não reconhecidos
          token.value[i++] = c;
          
      case 13: // Estado para delimitadores com \, como tabulação e quebra de linha
        if (c == 'n' || c == 't'){
          token.value[i++] = c;
        }else{
          ungetc(c, inputFile);
        }
        token.value[i] = '\0';
        return token;
      break;
      default:
        token.type = TOKEN_UNKNOWN;
        return token;
    }
  }
  token.type = TOKEN_EOF;
  return token;
}

// Função Principal
int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: ./lexer filename\n");
    return 1;
  }
  FILE *inputFile = fopen(argv[1], "r");
  if (inputFile == NULL) {
    printf("Error: Could not open file %s\n", argv[1]);
    return 1;
  }
  Token token;
  while ((token = getNextToken(inputFile)).type != TOKEN_EOF) {  // Loop que vai varrer todo o arquivo que foi passado como argumento
    char message[100];  // Mensagem de Tokens válidos
    char errorMessage[100]; // Mensagem de Tokens inválidos
    if(token.type == 0){  // Cada type é referente a uma classe de token
      snprintf(message, sizeof(message), "On Code Line: %d - Token type: TOKEN_RESERVED_WORD, Value: %s\n", newLine, token.value);
      printf("On Code Line: %d - Token type: TOKEN_RESERVED_WORD, Value: %s\n", newLine, token.value);      
    }else if(token.type == 1){
      snprintf(message, sizeof(message), "On Code Line: %d - Token type: TOKEN_IDENTIFIER, Value: %s\n", newLine, token.value);
      printf("On Code Line: %d - Token type: TOKEN_IDENTIFIER, Value: %s\n", newLine, token.value);
    }else if(token.type == 2){
      snprintf(message, sizeof(message), "On Code Line: %d - Token type: TOKEN_NUMERIC_CONSTANT, Value: %s\n", newLine, token.value);
      printf("On Code Line: %d - Token type: TOKEN_NUMERIC_CONSTANT, Value: %s\n", newLine, token.value);
    }else if(token.type == 3){
      snprintf(message, sizeof(message), "On Code Line: %d - Token type: TOKEN_ALPHANUMERIC_CONSTANT, Value: \"%s\"\n", newLine, token.value);
      printf("On Code Line: %d - Token type: TOKEN_ALPHANUMERIC_CONSTANT, Value: \"%s\"\n", newLine, token.value);
    }else if(token.type == 4){
      snprintf(message, sizeof(message), "On Code Line: %d - Token type: TOKEN_ARITHMETIC_OPERATOR, Value: %s\n", newLine, token.value);
      printf("On Code Line: %d - Token type: TOKEN_ARITHMETIC_OPERATOR, Value: %s\n", newLine, token.value);
    }else if(token.type == 5){
      snprintf(message, sizeof(message), "On Code Line: %d - Token type: TOKEN_COMPARISON_OPERATOR, Value: %s\n", newLine, token.value);
      printf("On Code Line: %d - Token type: TOKEN_COMPARISON_OPERATOR, Value: %s\n", newLine, token.value);
    }else if(token.type == 6){
      snprintf(message, sizeof(message), "On Code Line: %d - Token type: TOKEN_DELIMITER, Value: %s\n", newLine, token.value);
      printf("On Code Line: %d - Token type: TOKEN_DELIMITER, Value: %s\n", newLine, token.value);
    }else if(token.type == 7){
      snprintf(message, sizeof(message), "On Code Line: %d - Token type: TOKEN_OPEN_PAREN, Value: %s\n", newLine, token.value);
      printf("On Code Line: %d - Token type: TOKEN_OPEN_PAREN, Value: %s\n", newLine, token.value);
    }else if(token.type == 8){
      snprintf(message, sizeof(message), "On Code Line: %d - Token type: TOKEN_CLOSE_PAREN, Value: %s\n", newLine, token.value);
      printf("On Code Line: %d - Token type: TOKEN_CLOSE_PAREN, Value: %s\n", newLine, token.value);
    }else if(token.type == 9){
      snprintf(message, sizeof(message), "On Code Line: %d - Token type: TOKEN_ASSIGNMENT, Value: %s\n", newLine, token.value);
      printf("On Code Line: %d - Token type: TOKEN_ASSIGNMENT, Value: %s\n", newLine, token.value);
    }else if(token.type == 10){
      snprintf(message, sizeof(message), "On Code Line: %d - Token type: TOKEN_SPECIAL, Value: %s\n", newLine, token.value);
      printf("On Code Line: %d - Token type: TOKEN_SPECIAL, Value: %s\n", newLine, token.value);
    }else if(token.type == 11){
      snprintf(message, sizeof(message), "On Code Line: %d - Token type: TOKEN_COMMENT, Value: {%s}\n", newLine, token.value);
      printf("On Code Line: %d - Token type: TOKEN_COMMENT, Value: {%s}\n", newLine, token.value);
    }else if(token.type == 12){
      snprintf(errorMessage, sizeof(errorMessage), "TOn Code Line: %d - oken type: TOKEN_UNKNOWN, Value: %s\n", newLine, token.value);
      printf("On Code Line: %d - Token type: TOKEN_UNKNOWN, Value: %s\n", newLine, token.value);
    }else if(token.type == 13){
      snprintf(message, sizeof(message), "On Code Line: %d - Token type: TOKEN_DELIMITER, Value: %s\n", newLine, token.value);
      printf("On Code Line: %d - Token type: TOKEN_DELIMITER, Value: %s\n", newLine, token.value);
    }else if(token.type == 14){
      snprintf(message, sizeof(message), "On Code Line: %d - Token type: TOKEN_EOF, Value: %s\n", newLine, token.value);
      printf("On Code Line: %d - Token type: TOKEN_EOF, Value: %s\n", newLine, token.value);
    }else{
      snprintf(errorMessage, sizeof(errorMessage), "On Code Line: %d - Token type: TOKEN_UNKNOWN, Value: %s\n", newLine, token.value);
      printf("On Code Line: %d - Token type: TOKEN_UNKNOWN, Value: %s\n", newLine, token.value);
    }

    writeOnError(errorMessage);
    writeOnOk(message);
  }
  fclose(inputFile);
  return 0;
}
