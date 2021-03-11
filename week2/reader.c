/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include "reader.h"

FILE *inputStream;
int lineNo, colNo;
int currentChar;

int readChar(void) {
  currentChar = getc(inputStream);
  colNo ++;
  if (currentChar == '\n') {
    lineNo ++;
    colNo = 0;
  }
  // printf("current char, value end of file: %c %d %d\n", currentChar, lineNo, colNo);
  return currentChar;
}

int openInputStream(char *fileName) {
  inputStream = fopen(fileName, "r+");
  if (inputStream == NULL)
    return IO_ERROR;
  lineNo = 1;
  colNo = 0;
  readChar();
  printf("openInputStream\n");
  return IO_SUCCESS;
}

void closeInputStream() {
  fclose(inputStream);
}

