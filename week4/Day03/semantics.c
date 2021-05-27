/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdlib.h>
#include <string.h>
#include "semantics.h"
#include "error.h"

extern SymTab *symtab;
extern Token *currentToken;

Object *lookupObject(char *name)
{
  // check in current Scope
  Scope *scope = symtab->currentScope;
  // Create Object
  Object *obj;
  while (scope != NULL)
  {
    obj = findObject(scope->objList, name);
    if (obj != NULL)
    {
      return obj;
    }
    // check in outer scope
    scope = scope->outer;
  }
  // find global
  obj = findObject(symtab->globalObjectList, name);
  if (obj != NULL)
  {
    return obj;
  }
  return NULL;
}
// check new ident = kiểm tra định danh mới
void checkFreshIdent(char *name)
{
  // just check in current scope
  Object *obj = findObject(symtab->currentScope->objList, name);
  if (obj != NULL)
  {
    // found same ident => duplicate
    error(ERR_DUPLICATE_IDENT, currentToken->lineNo, currentToken->colNo);
  }
}
// find in outer of outer and global to global
Object *checkDeclaredIdent(char *name)
{
  Object *obj = lookupObject(name);
  if (obj == NULL)
  {
    // check decalrd ex: s2 = s3; but s3 not declared => fail
    error(ERR_UNDECLARED_IDENT, currentToken->lineNo, currentToken->colNo);
  }
  return obj;
}

Object *checkDeclaredConstant(char *name)
{
  Object *obj = lookupObject(name);
  if (obj == NULL)
  {
    error(ERR_UNDECLARED_CONSTANT, currentToken->lineNo, currentToken->colNo);
  }
  // check type of object after find. type != const => error
  if (obj->kind != OBJ_CONSTANT)
  {
    error(ERR_INVALID_CONSTANT, currentToken->lineNo, currentToken->colNo);
  }
  return obj;
}

Object *checkDeclaredType(char *name)
{
  Object *obj = lookupObject(name);
  if (obj == NULL)
  {
    error(ERR_UNDECLARED_TYPE, currentToken->lineNo, currentToken->colNo);
  }
  if (obj->kind != OBJ_TYPE)
  {
    error(ERR_INVALID_TYPE, currentToken->lineNo, currentToken->colNo);
  }
  return obj;
}

Object *checkDeclaredVariable(char *name)
{
  Object *obj = lookupObject(name);
  if (obj == NULL)
  {
    error(ERR_UNDECLARED_VARIABLE, currentToken->lineNo, currentToken->colNo);
  }
  if (obj->kind != OBJ_VARIABLE)
  {
    error(ERR_INVALID_VARIABLE, currentToken->lineNo, currentToken->colNo);
  }
  return obj;
}

Object *checkDeclaredFunction(char *name)
{
  Object *obj = lookupObject(name);
  if (obj == NULL)
  {
    error(ERR_UNDECLARED_FUNCTION, currentToken->lineNo, currentToken->colNo);
  }
  if (obj->kind != OBJ_FUNCTION)
  {
    error(ERR_INVALID_FUNCTION, currentToken->lineNo, currentToken->colNo);
  }
  return obj;
}

Object *checkDeclaredProcedure(char *name)
{
  Object *obj = lookupObject(name);
  if (obj == NULL)
  {
    error(ERR_UNDECLARED_PROCEDURE, currentToken->lineNo, currentToken->colNo);
  }
  if (obj->kind != OBJ_PROCEDURE)
  {
    error(ERR_INVALID_PROCEDURE, currentToken->lineNo, currentToken->colNo);
  }
  return obj;
}

Object *checkDeclaredLValueIdent(char *name)
{
  Object *obj = lookupObject(name);
  if (obj == NULL)
  {
    error(ERR_UNDECLARED_IDENT, currentToken->lineNo, currentToken->colNo);
  }
  // variable - param - function ok, but const => wrong
  switch (obj->kind)
  {
  case OBJ_VARIABLE:
    break;
  case OBJ_PARAMETER:
    break;
  case OBJ_FUNCTION:
    // scope : current scope
    if (obj != symtab->currentScope->owner)
    {
      error(ERR_INVALID_IDENT, currentToken->lineNo, currentToken->colNo);
    }
    break;
  default:
    error(ERR_INVALID_IDENT, currentToken->lineNo, currentToken->colNo);
  }
  return obj;
}
