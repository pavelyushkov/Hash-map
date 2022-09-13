#ifndef STRUCT_H
#define STRUCT_H

typedef struct KeySpace {
  char* key;
  char* data;
  struct KeySpace* next;
} KeySpace;

typedef struct Table {
  KeySpace** ks;
  int size;
} Table;

#endif //STRUCT_H
