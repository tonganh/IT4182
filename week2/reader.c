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
  return currentChar;
} // doc ki tu qua getc, sau moi ky tu tang so cot them 1, neu gap \n thi tang so dong, so cot ve 0
// con tro file (inputStream) chuyen den ky tu tiep theo
// while(getc(inputStream)) == in ca file

int openInputStream(char *fileName) {
  inputStream = fopen(fileName, "rt");
  if (inputStream == NULL)
    return IO_ERROR;
  lineNo = 1;
  colNo = 0;
  readChar();
  return IO_SUCCESS;
} // mo file, set so dong ve 1, so cot ve 0

void closeInputStream() {
  fclose(inputStream);
}

