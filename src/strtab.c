#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "strtab.h"

const char *dataTypeStr[3] = {"int", "char", "void"};
const char *symbolTypeStr[3] = {"", "[]", "()"};

table_node* current_scope = NULL;
param *working_list_head = NULL;
param *working_list_end = NULL;

/* The symbolTable, which will be implemented as a hash table
  */
struct strEntry strTable[MAXIDS];

/* Provided is a hash function that you may call to get an integer back. */
int hash(unsigned char *str) {
  unsigned long hash = 5381;
  int c;
  while ((c = *str++)) {
    hash += (hash << 5) + c;
  }
  return hash % MAXIDS; // MAXIDS defined in strtab.h
}

int ST_insert(char *id, int data_type, int symbol_type, int *scope){
  // TODO: Concatenate the scope and id and use that to create the hash key

  /* TODO: Use ST_lookup to check if the id is already in the symbol table.
    If yes, ST_lookup will return an index that is not -1.
    if index != -1, that means the variable is already in the hashtable.
    Hence, no need to insert that variable again.
    However, if index == -1,
      then use linear probing to find an empty spot and insert there.
    Then return that index.
    */
    //stores key and formats it as scope:id
    (void)scope;
    if(current_scope == NULL){
      return -1;
    }
    //checks if symbol already exists in the table
    for(int i = 0; i < MAXIDS; i++)
    {
      symEntry *e = current_scope->strTable[i];
      if(e != NULL && strcmp(e->id,id) ==0)
      {
         printf("Error: multiply declared identifier %s\n", id);
         return i;
      }
    }
    //computes hash key
    int idx = hash((unsigned char*)id);

    while (current_scope->strTable[idx] != NULL) {
       idx = (idx + 1) % MAXIDS;
    }

    //assign values
    symEntry* entry = malloc(sizeof(symEntry));
    if(entry == NULL){
      return -1;
    }
    entry->id = strdup(id);
    entry->scope = strdup("");
    entry->data_type = data_type;
    entry->symbol_type = symbol_type;
    entry->size = 0;
    entry->params = NULL;

    current_scope->strTable[idx] = entry;
    
    /*
    while (strTable[idx].id != NULL) {
        idx = (idx + 1) % MAXIDS;
    }
    //copy data and return index of the table
    
    strTable[idx].id = strdup(id);
    strTable[idx].scope = strdup(scope);
    strTable[idx].data_type = data_type;
    strTable[idx].symbol_type = symbol_type;
    */
   
    return idx;
}

symEntry* ST_lookup(char *id) {
  // TODO: Concatenate the scope and id and use that to create the hash key

  /* TODO: Use the hash value to check if the index position has the "id".
    If not, keep looking for id until you find an empty spot.
    If you find "id", return that index.
    If you arrive at an empty spot, that means "id" is not there.
    Then return -1.
    */
   /*
   char key[256];
   snprintf(key, sizeof(key), "%s:%s", scope, id);
   int h = hash((unsigned char*)key);

   int start = h;

  //need to search table until finding the symbol or finding an empty space
   while (strTable[h].id != NULL) {
      char currentKey[256];
         snprintf(currentKey, sizeof(currentKey), "%s:%s",
                 strTable[h].scope, strTable[h].id);

        if (strcmp(currentKey, key) == 0) {
           return h;
        }

      //prevents going out of bounds
      h++;
      if (h == MAXIDS) {
         h = 0;
      }
      //helps prevent infinite loops
      if (h == start) {
         break;
      }
    }
    return -1;
    */
   //OLD CODE ABOVE THIS

   table_node* node = current_scope;

   while (node != NULL) {
      for (int i = 0; i < MAXIDS; i++) {
         if (node->strTable[i] != NULL && strcmp(node->strTable[i]->id, id) == 0) {
            return node->strTable[i];
         }
      }
      node = node->parent;
   }

   return NULL;
}

void output_entry(int i){
  printf("%d: %s ", i, dataTypeStr[strTable[i].data_type]);
  printf("%s:%s%s\n",
    strTable[i].scope, strTable[i].id, symbolTypeStr[strTable[i].symbol_type]);
}

void new_scope() {
   table_node* node = malloc(sizeof(table_node));
   for (int i = 0; i < MAXIDS; i++) {
      node->strTable[i] = NULL;
   }
   node->parent = current_scope;
   current_scope = node;
}

void up_scope() {
   if (current_scope != NULL) {
      current_scope = current_scope->parent;
   }
}

void add_param(int data_type, int symbol_type) {
   param* parameter = malloc(sizeof(param));
   parameter->data_type = data_type;
   parameter->symbol_type = symbol_type;
   parameter->next = NULL;

   if (working_list_head == NULL) {
      working_list_head = parameter;
      working_list_end = parameter;
   }
   else {
      working_list_end->next = parameter;
      working_list_end = parameter;
   }
}

void connect_params(int index, int num_params) {
   if (current_scope == NULL || current_scope->parent == NULL) {
      return;
   }

   symEntry* func = current_scope->parent->strTable[index];

   if (func == NULL) {
      return;
   }

   func->params = working_list_head;
   func->size = num_params;

   working_list_head = NULL;
   working_list_end = NULL;
}

static void print_scope(table_node *node, int depth) {
    if (node == NULL) return;

    for (int i = 0; i < MAXIDS; i++) {
        symEntry *e = node->strTable[i];
        if (e != NULL) {
            for (int j = 0; j < depth; j++) printf("  ");
            printf("%s : %d %d %s\n", e->id, e->data_type, e->symbol_type, e->scope);
        }
    }

    print_scope(node->first_child, depth + 1);
    print_scope(node->next, depth);
}

void print_sym_tab(void) {
    print_scope(current_scope, 0);
}
