#ifndef STRUCT_H
#define STRUCT_H

typedef struct Item {
  char* key;
  char* info;
} Item;

typedef struct KeySpace {
  int key_of;
  int info_of;
  struct KeySpace* next;
} KeySpace;

typedef struct Table {
  FILE* ks_file;
  FILE* i_file;
  char* ks_name;
  char* i_name;
  KeySpace** ks;
  int size;
} Table;

#endif //STRUCT_H
