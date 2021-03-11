/* Scanner
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reader.h"
#include "charcode.h"
#include "token.h"
#include "error.h"

extern int lineNo;
extern int colNo;
extern int currentChar;
int i = 0;
extern CharCode charCodes[];
int getLparAndTimes = 0;
/***************************************************************/

void skipBlank()
{
  // have blank -> reset value to zero.
  while ((charCodes[currentChar] == CHAR_SPACE) && (currentChar != EOF))
  {
    readChar();
  }
}

void skipComment()
{
  int checkEndOfComment = 0;
  int stepInit = 1;
  int getLPAR = 0;
  int getCharRPAR = 0;
  int getTimes = 0;
  int getRparTimes = 0;
  // checkEndOfComment = 1- > char now get: *, char =2: ) => finish skip comment
  // Go to end of comment, have model: *)
  while ((currentChar != EOF) && (getRparTimes != getLparAndTimes))
  {
    switch (charCodes[currentChar])
    {
    case CHAR_LPAR:
    {
      getLPAR = 1;
    }
    case CHAR_TIMES:
    {
      if (getLPAR == 1)
      {
        // printf("debug LPAR\n");
        getLparAndTimes++;
        getLPAR = 0;
      }
      getTimes = 1;
      break;
    }
    case CHAR_RPAR:
    {
      if (getTimes == 1)
      {
        // printf("123\n");
        getRparTimes++;
        // printf("%d\n", getRparTimes);
      }
    }
    default:
      getLPAR = 0;
      getTimes = 0;
    }
    readChar();
  }
  if (getRparTimes != getLparAndTimes)
  {
    error(ERR_ENDOFCOMMENT, lineNo, colNo);
  }
}
void skipDoubleSlash(void)
{
  while ((currentChar != EOF) && (currentChar != '\n'))
  {
    readChar();
  }
}
Token *readIdentKeyword(void)
{
  Token *tokenReadIdentKeyword = makeToken(TK_CHAR, lineNo, colNo);
  char stringSaveValue[MAX_IDENT_LEN + 1];
  char s3[MAX_IDENT_LEN + 1];
  char convertCurrentCharToChar = currentChar;
  i = 0;
  stringSaveValue[i] = convertCurrentCharToChar;
  currentChar = readChar();
  while (charCodes[currentChar] == CHAR_LETTER || charCodes[currentChar] == CHAR_DIGIT)
  {
    i++;
    convertCurrentCharToChar = currentChar;
    stringSaveValue[i] = convertCurrentCharToChar;
    currentChar = readChar();
  }
  // strcpy(tokenReadIdentKeyword->string, stringSaveValue);
  for (int index = 0; index <= i; index++)
  {
    tokenReadIdentKeyword->string[index] = stringSaveValue[index];
  }
  if (i > MAX_IDENT_LEN)
  {
    error(ERR_IDENTTOOLONG, tokenReadIdentKeyword->lineNo, tokenReadIdentKeyword->colNo);
    return tokenReadIdentKeyword;
  }
  TokenType valueHere = checkKeyword(tokenReadIdentKeyword->string);
  if (valueHere != TK_NONE)
  {
    tokenReadIdentKeyword->tokenType = valueHere;
  }
  else
  {
    tokenReadIdentKeyword->tokenType = TK_IDENT;
  }
  return tokenReadIdentKeyword;
}

Token *readNumber(void)
{
  Token *tokenNumber = makeToken(TK_NUMBER, lineNo, colNo);
  int i = 0;
  char convertToNumber = currentChar;
  while ((currentChar != EOF) && (charCodes[currentChar] == CHAR_DIGIT))
  {
    tokenNumber->string[i] = convertToNumber;
    i++;
    readChar();
    convertToNumber = currentChar;
  }
  return tokenNumber;
}

// Token *readConstChar(void)
// {
//   if ((currentChar != EOF) &&)
//   {
//     /* code */
//   }

// }

Token *getToken(void)
{
  Token *token;
  int ln, cn;
  if (currentChar == EOF)
    return makeToken(TK_EOF, lineNo, colNo);
  switch (charCodes[currentChar])
  {
    // Get space
  case CHAR_SPACE:
    skipBlank();
    return getToken();
    // Check char letter and insert get Token=
  case CHAR_LETTER:
    return readIdentKeyword();
    // Check char is digit.
  case CHAR_DIGIT:
    return readNumber();
  case CHAR_PLUS:
    token = makeToken(SB_PLUS, lineNo, colNo);
    readChar();
    return token;
  case CHAR_LPAR:
    ln = lineNo;
    cn = colNo;
    readChar();
    if (currentChar == EOF)
    {
      return makeToken(TK_NONE, ln, cn);
    }
    switch (charCodes[currentChar])
    {
    case CHAR_TIMES:
      readChar();
      getLparAndTimes = 1;
      skipComment();
      // AFter skip comment, we again get token
      return getToken();
      break;
    default:
      return makeToken(CHAR_LPAR, lineNo, colNo);
      break;
    }

  case CHAR_SEMICOLON:
  {
    token = makeToken(SB_SEMICOLON, lineNo, colNo);
    readChar();
    return token;
  }
  case CHAR_COLON:
  {
    ln = lineNo;
    cn = colNo;
    readChar();
    if ((currentChar != EOF) && (charCodes[currentChar] == CHAR_EQ))
    {
      readChar();
      return makeToken(SB_ASSIGN, ln, cn);
    }
    else
    {
      return makeToken(SB_COLON, ln, cn);
    }
  }
  case CHAR_PERIOD:
  {
    ln = lineNo;
    cn = colNo;
    readChar();
    if ((currentChar != EOF) && (charCodes[currentChar] == CHAR_RPAR))
    {
      readChar();
      return makeToken(SB_RSEL, ln, cn);
    }
    else
      return makeToken(SB_PERIOD, ln, cn);
  }
  case CHAR_COMMA:
  {
    token = makeToken(SB_COMMA, lineNo, colNo);
    readChar();
    return token;
  }
  case CHAR_EQ:
  {
    token = makeToken(SB_EQ, lineNo, colNo);
    readChar();
    return token;
  }
  case CHAR_EXCLAIMATION:
  {
    ln = lineNo;
    cn = colNo;
    readChar();
    if ((currentChar != EOF) && (charCodes[currentChar] == CHAR_EQ))
    {
      return makeToken(SB_NEQ, ln, cn);
    }
    else
      return makeToken(TK_NONE, ln, cn);
  }
  case CHAR_LT:
  {
    ln = lineNo;
    cn = colNo;
    readChar();
    if ((currentChar != EOF) && charCodes[currentChar] == CHAR_EQ)
    {
      return makeToken(SB_LE, ln, cn);
    }
    else
    {
      return makeToken(SB_LT, ln, cn);
    }
  }
  case CHAR_GT:
  {
    ln = lineNo;
    cn = colNo;
    readChar();
    if ((currentChar != EOF) && charCodes[currentChar] == CHAR_EQ)
    {
      return makeToken(SB_GE, ln, cn);
    }
    else
    {
      return makeToken(SB_GT, ln, cn);
    }
  }
  case CHAR_MINUS:
  {
    ln = lineNo;
    cn = colNo;
    readChar();
    return makeToken(SB_MINUS, ln, cn);
  }
  case CHAR_TIMES:
  {
    readChar();
    return makeToken(SB_TIMES, lineNo, colNo);
  }
  case CHAR_SLASH:
  {
    cn = colNo;
    ln = lineNo;
    readChar();
    if ((currentChar != EOF) && charCodes[currentChar] == CHAR_SLASH)
    {
      skipDoubleSlash();
      return getToken();
    }
    else
    {
      return makeToken(SB_SLASH, cn, ln);
    }
  }
  // case CHAR_SINGLEQUOTE:{
  //   readChar();
  //   readConstChar();
  // }
  default:
    token = makeToken(TK_NONE, lineNo, colNo);
    error(ERR_INVALIDSYMBOL, lineNo, colNo);
    readChar();
    return token;
  }
}

/******************************************************************/

void printToken(Token *token)
{

  printf("%d-%d:", token->lineNo, token->colNo);

  switch (token->tokenType)
  {
  case TK_NONE:
    printf("TK_NONE\n");
    break;
  case TK_IDENT:
    printf("TK_IDENT(%s)\n", token->string);
    break;
  case TK_NUMBER:
    printf("TK_NUMBER(%s)\n", token->string);
    break;
  case TK_CHAR:
    printf("TK_CHAR(\'%s\')\n", token->string);
    break;
  case TK_EOF:
    printf("TK_EOF\n");
    break;

  case KW_PROGRAM:
    printf("KW_PROGRAM\n");
    break;
  case KW_CONST:
    printf("KW_CONST\n");
    break;
  case KW_TYPE:
    printf("KW_TYPE\n");
    break;
  case KW_VAR:
    printf("KW_VAR\n");
    break;
  case KW_INTEGER:
    printf("KW_INTEGER\n");
    break;
  case KW_CHAR:
    printf("KW_CHAR\n");
    break;
  case KW_ARRAY:
    printf("KW_ARRAY\n");
    break;
  case KW_OF:
    printf("KW_OF\n");
    break;
  case KW_FUNCTION:
    printf("KW_FUNCTION\n");
    break;
  case KW_PROCEDURE:
    printf("KW_PROCEDURE\n");
    break;
  case KW_BEGIN:
    printf("KW_BEGIN\n");
    break;
  case KW_END:
    printf("KW_END\n");
    break;
  case KW_CALL:
    printf("KW_CALL\n");
    break;
  case KW_IF:
    printf("KW_IF\n");
    break;
  case KW_THEN:
    printf("KW_THEN\n");
    break;
  case KW_ELSE:
    printf("KW_ELSE\n");
    break;
  case KW_WHILE:
    printf("KW_WHILE\n");
    break;
  case KW_DO:
    printf("KW_DO\n");
    break;
  case KW_FOR:
    printf("KW_FOR\n");
    break;
  case KW_TO:
    printf("KW_TO\n");
    break;

  case SB_SEMICOLON:
    printf("SB_SEMICOLON\n");
    break;
  case SB_COLON:
    printf("SB_COLON\n");
    break;
  case SB_PERIOD:
    printf("SB_PERIOD\n");
    break;
  case SB_COMMA:
    printf("SB_COMMA\n");
    break;
  case SB_ASSIGN:
    printf("SB_ASSIGN\n");
    break;
  case SB_EQ:
    printf("SB_EQ\n");
    break;
  case SB_NEQ:
    printf("SB_NEQ\n");
    break;
  case SB_LT:
    printf("SB_LT\n");
    break;
  case SB_LE:
    printf("SB_LE\n");
    break;
  case SB_GT:
    printf("SB_GT\n");
    break;
  case SB_GE:
    printf("SB_GE\n");
    break;
  case SB_PLUS:
    printf("SB_PLUS\n");
    break;
  case SB_MINUS:
    printf("SB_MINUS\n");
    break;
  case SB_TIMES:
    printf("SB_TIMES\n");
    break;
  case SB_SLASH:
    printf("SB_SLASH\n");
    break;
  case SB_LPAR:
    printf("SB_LPAR\n");
    break;
  case SB_RPAR:
    printf("SB_RPAR\n");
    break;
  case SB_LSEL:
    printf("SB_LSEL\n");
    break;
  case SB_RSEL:
    printf("SB_RSEL\n");
    break;
  }
}

int scan(char *fileName)
{
  Token *token;

  if (openInputStream(fileName) == IO_ERROR)
    return IO_ERROR;
  token = getToken();
  // Read to end of file.
  while (token->tokenType != TK_EOF)
  {
    printToken(token);
    free(token);
    token = getToken();
  }

  free(token);
  closeInputStream();
  return IO_SUCCESS;
}

/******************************************************************/

int main(int argc, char *argv[])
{
  // Check input argc have enough file
  // if (argc <= 1) {
  //   printf("scanner: no input file.\n");
  //   return -1;
  // }

  if (scan("db.txt") == IO_ERROR)
  {
    printf("Can\'t read input file!\n");
    return -1;
  }

  return 0;
}
