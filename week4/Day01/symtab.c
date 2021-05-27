/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.1
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

void freeObject(Object *obj);
void freeScope(Scope *scope);
void freeObjectList(ObjectNode *objList);
void freeReferenceList(ObjectNode *objList);

SymTab *symtab;
Type *intType;
Type *charType;

/******************* Type utilities ******************************/

Type *makeIntType(void)
{
  Type *type = (Type *)malloc(sizeof(Type));
  type->typeClass = TP_INT;
  return type;
}

Type *makeCharType(void)
{
  Type *type = (Type *)malloc(sizeof(Type));
  type->typeClass = TP_CHAR;
  return type;
}

Type *makeArrayType(int arraySize, Type *elementType)
{
  Type *type = (Type *)malloc(sizeof(Type));
  type->typeClass = TP_ARRAY;
  type->arraySize = arraySize;
  type->elementType = elementType;
  return type;
}

Type *duplicateType(Type *type)
{
  Type *new;
  switch (type->typeClass)
  {
  case TP_ARRAY:
    new = makeArrayType(type->arraySize, type->elementType);
    break;
  case TP_INT:
    new = makeIntType();
    break;
  case TP_CHAR:
    new = makeCharType();
    break;
  default:
    break;
  }
  return new;
}

int compareType(Type *type1, Type *type2)
{
  if (type1->arraySize != type2->arraySize || type1->typeClass != type2->typeClass)
  {
    return 0;
  }
  compareType(type1->elementType, type2->elementType);
  return 1;
}

void freeType(Type* type) {
  // TODO
  switch (type->typeClass) {
  case TP_INT:
  case TP_CHAR:
    free(type);
    break;
  case TP_ARRAY:
    freeType(type->elementType);
    freeType(type);
    break;
  }
}

/******************* Constant utility ******************************/

ConstantValue *makeIntConstant(int i)
{
  ConstantValue *convertValue = (ConstantValue *)malloc(sizeof(ConstantValue));
  convertValue->type = TP_INT;
  convertValue->intValue = i;
  return convertValue;
}

ConstantValue *makeCharConstant(char ch)
{
  ConstantValue *convertValue = (ConstantValue *)malloc(sizeof(ConstantValue));
  convertValue->type = TP_CHAR;
  convertValue->charValue = ch;
  return convertValue;
}

ConstantValue *duplicateConstantValue(ConstantValue *v)
{
  ConstantValue *value = (ConstantValue *)malloc(sizeof(ConstantValue));
  value->type = v->type;
  if (v->type == TP_INT)
    value->intValue = v->intValue;
  else
    value->charValue = v->charValue;
  return value;
}

/******************* Object utilities ******************************/

Scope *CreateScope(Object *owner, Scope *outer)
{
  Scope *scope = (Scope *)malloc(sizeof(Scope));
  scope->objList = NULL;
  scope->owner = owner;
  scope->outer = outer;
  return scope;
}

Object *CreateProgramObject(char *programName)
{
  Object *program = (Object *)malloc(sizeof(Object));
  strcpy(program->name, programName);
  program->kind = OBJ_PROGRAM;
  program->progAttrs = (ProgramAttributes *)malloc(sizeof(ProgramAttributes));
  program->progAttrs->scope = CreateScope(program, NULL);
  symtab->program = program;

  return program;
}

Object *CreateConstantObject(char *name)
{
  Object *convertValue = (Object *)malloc(sizeof(Object));
  convertValue->kind = OBJ_CONSTANT;
  convertValue->constAttrs = (ConstantAttributes *)malloc(sizeof(ConstantAttributes));
  strcpy(convertValue->name, name);
  return convertValue;
}

Object *CreateTypeObject(char *name)
{
  Object *convertValue = (Object *)malloc(sizeof(Object));
  convertValue->kind = OBJ_TYPE;
  convertValue->typeAttrs = (TypeAttributes *)malloc(sizeof(TypeAttributes));
  strcpy(convertValue->name, name);
  return convertValue;
}

Object *CreateVariableObject(char *name)
{
  Object *obj = (Object *)malloc(sizeof(Object));
  strcpy(obj->name, name);
  obj->kind = OBJ_VARIABLE;
  obj->varAttrs = (VariableAttributes *)malloc(sizeof(VariableAttributes));
  obj->varAttrs->type = NULL;
  obj->varAttrs->scope = NULL;
  // obj->varAttrs->localOffset = 0;
  return obj;
}

Object *CreateFunctionObject(char *name)
{
	  Object* obj = (Object*) malloc(sizeof(Object));	
  strcpy(obj->name, name);	
  obj->kind = OBJ_FUNCTION;	
  obj->funcAttrs = (FunctionAttributes*) malloc(sizeof(FunctionAttributes));	
  obj->funcAttrs->returnType = NULL;	
  obj->funcAttrs->paramList = NULL;	
  // obj->funcAttrs->paramCount = 0;	
  // obj->funcAttrs->codeAddress = DC_VALUE;	
  obj->funcAttrs->scope = CreateScope(obj,symtab->currentScope);	
  return obj;
}
Object *CreateProcedureObject(char *name)
{
  Object* obj = (Object*) malloc(sizeof(Object));	
  strcpy(obj->name, name);	
  obj->kind = OBJ_PROCEDURE;	
  obj->procAttrs = (ProcedureAttributes*) malloc(sizeof(ProcedureAttributes));	
  obj->procAttrs->paramList = NULL;	
  // obj->procAttrs->paramCount = 0;	
  // obj->procAttrs->codeAddress = DC_VALUE;	
  obj->procAttrs->scope = CreateScope(obj,symtab->currentScope);	
  return obj;
}

Object *CreateParameterObject(char *name, enum ParamKind kind, Object *owner)
{
  printf("check in param\n");

  Object *obj = (Object *)malloc(sizeof(Object));
  strcpy(obj->name, name);
  obj->kind = OBJ_PARAMETER;

  obj->paramAttrs = (ParameterAttributes *)malloc(sizeof(ParameterAttributes));

  obj->paramAttrs->kind = kind;
  obj->paramAttrs->type = NULL;
  obj->paramAttrs->function = owner;
  // obj->paramAttrs->scope = NULL;
  // obj->paramAttrs->localOffset = 0;
  return obj;
}

void freeObject(Object* obj) {
  switch (obj->kind) {
  case OBJ_CONSTANT:
    free(obj->constAttrs->value);
    free(obj->constAttrs);
    break;
  case OBJ_TYPE:
    free(obj->typeAttrs->actualType);
    free(obj->typeAttrs);
    break;
  case OBJ_VARIABLE:
    free(obj->varAttrs->type);
    free(obj->varAttrs);
    break;
  case OBJ_FUNCTION:
    freeReferenceList(obj->funcAttrs->paramList);
    freeType(obj->funcAttrs->returnType);
    freeScope(obj->funcAttrs->scope);
    free(obj->funcAttrs);
    break;
  case OBJ_PROCEDURE:
    freeReferenceList(obj->procAttrs->paramList);
    freeScope(obj->procAttrs->scope);
    free(obj->procAttrs);
    break;
  case OBJ_PROGRAM:
    freeScope(obj->progAttrs->scope);
    free(obj->progAttrs);
    break;
  case OBJ_PARAMETER:
    freeType(obj->paramAttrs->type);
    free(obj->paramAttrs);
  }
  free(obj);
}

void freeScope(Scope *scope)
{
  if (scope->outer == NULL || scope == NULL)
  {
    return;
  }
  freeScope(scope->outer);
  freeObjectList(scope->objList);
  freeObject(scope->owner);
  free(scope);
}

void freeObjectList(ObjectNode *objList) {
  ObjectNode* list = objList;

  while (list != NULL) {
    ObjectNode* node = list;
    list = list->next;
    freeObject(node->object);
    free(node);
  }
}

void freeReferenceList(ObjectNode *objList)
{
  ObjectNode *tmp = objList;
  while (tmp->object != NULL)
  {
    freeObject(tmp->object);
    tmp = tmp->next;
  }
  free(tmp);
  free(objList);
}

void AddObject(ObjectNode **objList, Object *obj)
{
  ObjectNode *node = (ObjectNode *)malloc(sizeof(ObjectNode));
  node->object = obj;
  node->next = NULL;
  if ((*objList) == NULL)
    *objList = node;
  else
  {
    ObjectNode *n = *objList;
    while (n->next != NULL)
      n = n->next;
    n->next = node;
  }
}

Object *findObject(ObjectNode *objList, char *name)
{
  while (objList != NULL)
  {
    if (strcmp(objList->object->name, name) == 0)
      return objList->object;
    else
      objList = objList->next;
  }
  return NULL;
}

/******************* others ******************************/

void initSymTab(void)
{
  Object *obj;
  Object *param;

  symtab = (SymTab *)malloc(sizeof(SymTab));
  symtab->globalObjectList = NULL;

  obj = CreateFunctionObject("READC");
  obj->funcAttrs->returnType = makeCharType();
  AddObject(&(symtab->globalObjectList), obj);

  obj = CreateFunctionObject("READI");
  obj->funcAttrs->returnType = makeIntType();
  AddObject(&(symtab->globalObjectList), obj);
  obj = CreateProcedureObject("WRITEI");
  param = CreateParameterObject("i", PARAM_VALUE, obj);

  param->paramAttrs->type = makeIntType();

  AddObject(&(obj->procAttrs->paramList), param);
  AddObject(&(symtab->globalObjectList), obj);

  obj = CreateProcedureObject("WRITEC");
  param = CreateParameterObject("ch", PARAM_VALUE, obj);
  param->paramAttrs->type = makeCharType();
  AddObject(&(obj->procAttrs->paramList), param);
  AddObject(&(symtab->globalObjectList), obj);

  obj = CreateProcedureObject("WRITELN");
  AddObject(&(symtab->globalObjectList), obj);

  intType = makeIntType();
  charType = makeCharType();
}

void cleanSymTab(void) {
  freeObject(symtab->program);
  // freeObjectList(symtab->globalObjectList);
  free(symtab);
  freeType(intType);
  freeType(charType);
}

void enterBlock(Scope *scope)
{
  symtab->currentScope = scope;
}

void exitBlock(void)
{
  symtab->currentScope = symtab->currentScope->outer;
}

void declareObject(Object *obj)
{
  if (obj->kind == OBJ_PARAMETER)
  {
    Object *owner = symtab->currentScope->owner;
    switch (owner->kind)
    {
    case OBJ_FUNCTION:
      AddObject(&(owner->funcAttrs->paramList), obj);
      break;
    case OBJ_PROCEDURE:
      AddObject(&(owner->procAttrs->paramList), obj);
      break;
    default:
      break;
    }
  }

  AddObject(&(symtab->currentScope->objList), obj);
}
