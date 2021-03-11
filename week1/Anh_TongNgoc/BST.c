#include "BST.h"
void Empty(LIST *T)
{
  *T = NULL;
}
NODE *CreatNODE(DT x)
{
  NODE *P = (NODE *)malloc(sizeof(NODE));
  P->x = x;
  P->Left = P->Right = NULL;
  return P;
}
void Insert(LIST *T, DT x, int line)
{
  if (*T)
  {
    if (strcmp((*T)->x.x, x.x) == 0)
    {
      printf("Insert here\n");
      int check = 0;
      int excuted = 1;
      printf("insert %s here in line: %d\n", x.x, line);
      while ((*T)->x.line[check] != NULL)
      {
        if ((*T)->x.line[check] == line)
        {
          (*T)->x.timeOccur++;
          excuted = 0;
          break;
        }
        else
        {
          check = check + 1;
        }
      }
      (*T)->x.line[check] = line;
      if (excuted == 1)
      {
        (*T)->x.timeOccur++;
      }
      printf("when save in bst: %s %d\n", x.x, (*T)->x.timeOccur);
    }
    else if (strcmp((*T)->x.x, x.x) > 0)
    {
      Insert(&(*T)->Left, x, line);
    }
    else
    {
      printf("insert %s here in line: %d\n", x.x, line);
      Insert(&(*T)->Right, x, line);
    }
  }
  else
  {
    printf("insert %s here in line: %d\n", x.x, line);
    x.timeOccur = 1;
    x.line[0] = line;
    *T = CreatNODE(x);
  }
}
NODE *Search(LIST *T, DT x)
{
  if (*T)
  {
    if (strcmp((*T)->x.x, x.x) == 0)
      return *T;
    if (strcmp((*T)->x.x, x.x) > 0)
      return Search(&(*T)->Left, x);
    return Search(&(*T)->Right, x);
  }
  return NULL;
}
void NODE_2child(LIST *X, LIST *Y)
{
  if ((*Y)->Left)
    NODE_2child(X, &(*Y)->Left);
  else
  {
    strcpy((*X)->x.x, (*Y)->x.x);
    *X = *Y;
    *Y = (*Y)->Right;
  }
}
NODE *Delete(LIST *T, DT x)
{
  if (*T == NULL)
  {
    printf("Not found\n");
    return NULL;
  }
  if (strcmp((*T)->x.x, x.x) > 0)
    Delete(&(*T)->Left, x);
  else if (strcmp((*T)->x.x, x.x) < 0)
    Delete(&(*T)->Right, x);
  else
  {
    NODE *X = *T;
    if ((*T)->Left == NULL)
      *T = (*T)->Right;
    else if ((*T)->Right == NULL)
      *T = (*T)->Left;
    else
      NODE_2child(&X, &(*T)->Right);
    return X;
  }
}
void Free(LIST *T)
{
  if (*T)
  {
    Free(&(*T)->Left);
    Free(&(*T)->Right);
    free(*T);
  }
}
