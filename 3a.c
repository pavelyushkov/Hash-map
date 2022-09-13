#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern char* strdup(const char*);
#include <ctype.h>
#include <readline/readline.h>
#include "struct_a.h"

void output(Table* table){
  int count = 0;
  for(int i = 0 ; i < table->size; ++i){
    if(i == 0){
      printf("\nkey:    info:");
      printf("\n-------------");
    }
    KeySpace* item = table->ks[i];
    while(item != NULL){
      if(table->ks[i]->key != NULL)
        printf("\n%d) %s    %4s", ++count, item->key, item->data);
      item = item->next;
    }
  }
  printf("\n-------------\n");
}

int hash(int size, char* key) {
  int hash = 2139062143, len;
  len = strlen(key);
  for (int i = 0; i < len; ++i)
    hash = hash * 37 + key[i];
  hash = abs(hash) % size;
    return hash;
}

KeySpace* FindAndOut(Table* table, char* key){
  for(int i = 0; i < table->size; ++i){
    KeySpace* item = table->ks[i];
    while(item != NULL){
      if(item->key == NULL)
        break;
      if(!strcmp(item->key, key)){
        free(key);
        KeySpace* pass = (KeySpace*) malloc(sizeof(KeySpace));
        pass->key = strdup(item->key);
        pass->data = strdup(item->data);
        pass->next = NULL;
        return pass;
      }
      item = item->next;
    }
  }
  free(key);
  return NULL;
}
int check_duplicate(Table* table, char* key){
  int i = hash(table->size, key);
  KeySpace* item = table->ks[i];
  if(item->key == NULL)
    return 1;
  else
    while(item != NULL){
      if(strcmp(item->key, key) == 0)
        return 0;
      item = item->next;
    }
  return 1;
}
int get_integer(){
  int q, i, flag = 1, res = 0;
  char* input = NULL;
  while(flag == 1){
   if(input != NULL)
      free(input);
    input = readline("");
    if(input == NULL)
      return 0;
    q = strlen(input);
    for(i = 0; i < q; i++){
      if(!isdigit(input[i])){
        printf("\nWrong input. Try again:   ");
        flag = 1;
        break;
      }
      if(i == (q - 1))
        flag = 0;
    }
  }
  res = atoi(input);
  free(input);
  return res;
}

void PrintMainMenu(){
  printf("\nEnter 1 to add new element");
  printf("\nEnter 2 to find and out element by key");
  printf("\nEnter 3 to delete element by key");
  printf("\nEnter 4 to output table");
  printf("\nEnter 0 to end\n");
  printf("\nYour input:  ");
}
void freeTable(Table* table){
  for(int i = 0; i < table->size; ++i){
    KeySpace* item = table->ks[i];
    if(item->key != NULL){
      while(item != NULL){
        free(item->data);
        free(item->key);
        KeySpace* pass = item;
        item = item->next;
        free(pass);
      }
    }
    else
      free(item);
  }
  free(table->ks);
  free(table);
}

KeySpace* get_item(KeySpace* item){
  item = (KeySpace*) calloc(1, sizeof(KeySpace));
  char* k = readline("\nEnter key:  ");
  char* i = readline("\nEnter info:  ");
  item->key = strdup(k);
  item->data = strdup(i);
  free(k);
  free(i);
  return item;
}
void add_element(Table* table, KeySpace* item, int index){
  if(table->ks[index]->key == NULL){
    table->ks[index]->key = strdup(item->key);
    table->ks[index]->data = strdup(item->data);
    table->ks[index]->next = NULL;
  }
  else{
    KeySpace* elem = table->ks[index];
    while(elem->next != NULL)
      elem = elem->next;
    elem->next = (KeySpace*) calloc(1, sizeof(KeySpace));
    elem->next->key = strdup(item->key);
    elem->next->data = strdup(item->data);
    elem->next->next = NULL;
  }
}
void delete_element(Table* table, char* key){
  int i = hash(table->size, key);
  KeySpace* item = table->ks[i];
  while(item != NULL){
    if(item->key != NULL){
      if(strcmp(item->key, key) == 0){
        if(item == table->ks[i]){
          free(item->key);
          free(item->data);
          item->key = NULL;
          return;
        }
        else{
          KeySpace* pass = table->ks[i];
          while(pass->next != item)
            pass = pass->next;
          pass->next = item->next;
          free(item->key);
          free(item->data);
          free(item);
          return;
        }
      }
    }
    item = item->next;
  }
}

Table* create_table(Table* table, int n){
  table = (Table*) malloc(sizeof(Table));
  table->size = n;
  table->ks = (KeySpace**) calloc(table->size, sizeof(KeySpace*));
  for(int i = 0; i < table->size; ++i){
    table->ks[i] = (KeySpace*) calloc(1, sizeof(KeySpace));
    table->ks[i]->key = NULL;
  }
  return table;
}

void freeItem(KeySpace* item){
  free(item->key);
  free(item->data);
  free(item);
}
int main(){
  int check = 1, size;
  Table* table;
  printf("\nEnter size of table: ");
  size = get_integer();
  table = create_table(table, size);
  if(size == 0){
    free(table);
    printf("\nFinished!\n\n");
    return 0;
  }
  while(check != 0){
    PrintMainMenu();
    check = get_integer();
    if(check == 0){
      freeTable(table);
      printf("\nFinished!\n\n");
      break;
    }
    else if(check == 1){
      KeySpace* item = get_item(item);
      int index = check_duplicate(table, item->key);
      if(index == 0)
        printf("\nElement with this key already exist\n");
      else{
        index = hash(size, item->key);
        add_element(table, item, index);
      }
      freeItem(item);
    }
    else if(check == 2){
      char* key = readline("\nEnter key to find:  ");
      KeySpace* item = FindAndOut(table, key);
      if(item == NULL)
        printf("\nNo element with this key\n");
      else{
        printf("\nInfo about element with this key:   %s\n", item->data);
        freeItem(item);
      }
    }
    else if(check == 3){
      char* key = readline("\nEnter key to delete element:  ");
      delete_element(table, key);
      free(key);
    }
    else if(check == 4){
      output(table);
    }
    else if(check > 4 || check < 0)
      printf("\nWrong input. Try again:  ");
  }
  return 0;
}
