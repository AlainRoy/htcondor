/***************************Copyright-DO-NOT-REMOVE-THIS-LINE**
  *
  * Condor Software Copyright Notice
  * Copyright (C) 1990-2004, Condor Team, Computer Sciences Department,
  * University of Wisconsin-Madison, WI.
  *
  * This source code is covered by the Condor Public License, which can
  * be found in the accompanying LICENSE.TXT file, or online at
  * www.condorproject.org.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  * AND THE UNIVERSITY OF WISCONSIN-MADISON "AS IS" AND ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  * WARRANTIES OF MERCHANTABILITY, OF SATISFACTORY QUALITY, AND FITNESS
  * FOR A PARTICULAR PURPOSE OR USE ARE DISCLAIMED. THE COPYRIGHT
  * HOLDERS AND CONTRIBUTORS AND THE UNIVERSITY OF WISCONSIN-MADISON
  * MAKE NO MAKE NO REPRESENTATION THAT THE SOFTWARE, MODIFICATIONS,
  * ENHANCEMENTS OR DERIVATIVE WORKS THEREOF, WILL NOT INFRINGE ANY
  * PATENT, COPYRIGHT, TRADEMARK, TRADE SECRET OR OTHER PROPRIETARY
  * RIGHT.
  *
  ****************************Copyright-DO-NOT-REMOVE-THIS-LINE**/
#ifndef _BUILD_TABLE_H_
#define _BUILD_TABLE_H_

#define INTRANGE   0
#define FLOATRANGE 1
#define NONRANGE   2

#include "condor_ast.h"

/*
class EntryNode {

    public:

        EntryNode(ExprTree *expr) { value = expr; next = NULL; }
        ~EntryNode() { delete value; delete next; }
        friend class StatEntry;
        void PrintMe() { value->Display(); }

    private:

        ExprTree *value;
        class EntryNode *next;
};

class StatEntry {
 public:
  StatEntry(char *name)
    { strcpy(entryName, name); entryValue = NULL; next = NULL; }
  ~StatEntry() { delete entryValue; delete next; }
  friend class StatTable;
  int IsEntry(char *identifier) { return !strcmp(entryName, identifier); }
  void AddValue(Expression *);
  void DelValue() { delete entryValue; entryValue = NULL; }
  void PrintVal();
  int IntVal() { return ((Integer *)(entryValue->value))->value; }
  float FloatVal() { return ((Float *)(entryValue->value))->value; }
  int noValue() { return !entryValue; }
  lexeme_type MyType() { return entryValue->value->MyType(); }
 private:
  char entryName[100];
  EntryNode *entryValue;
  class StatEntry *next;
};

class StatTable {
 public:
  StatTable() { entries = NULL; numOfEntries = 0; }
  ~StatTable() { delete entries; }
  StatEntry *FindEntry(char *);
  StatEntry *AddEntry(char *);
  lexeme_type EntryType(char *entry) { return FindEntry(entry)->MyType(); }
  void PrintEntry(char *entry) { FindEntry(entry)->PrintVal(); }
  void PrintTable();
 private:
  int numOfEntries;
  StatEntry *entries;
};

*/

enum {RANGE, VTAB_FIXED};            // used by VarTypeTable and Sel_best_mach()

class VTableEntry {

 public:

  VTableEntry(char *varName, int varType)
  {
    var = new char[strlen(varName) + 1];
    strcpy(var, varName);
    type = varType;
    next = NULL;
  }
  ~VTableEntry() { delete var; delete next; }
  char *MyName() { return var; }
  int MyType() { return type; }
  friend class VarTypeTable;

 private:
  char *var;
  int type;
  class VTableEntry *next;
};

class VarTypeTable {

 public:

  VarTypeTable() { table = NULL; }
  ~VarTypeTable() { delete table; }
  int FindType(char *);
  void AddEntry(char *, int);
  void PrintTable(FILE *);

 private:

  VTableEntry *table;
};

/*
void Build_table(FILE *, StatTable *);
*/
void BuildVarTypeTable(FILE *, VarTypeTable *);

#endif
