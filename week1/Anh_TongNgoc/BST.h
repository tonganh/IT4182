#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define MAX 1000
typedef struct
{
  char x[MAX];    //word
  int timeOccur; //time word occured
  int line[MAX];
} DT;
typedef struct NODE
{
  DT x;
  struct NODE *Left;
  struct NODE *Right;
} NODE;
typedef struct NODE *LIST;
void Empty(LIST *T);
NODE *CreatNODE(DT x);
void Insert(LIST *T, DT x, int line);
void NODE_2child(LIST *X, LIST *Y);
NODE *Search(LIST *T, DT x);
NODE *Delete(LIST *T, DT x);
void Free(LIST *T);
