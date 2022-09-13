#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern char* strdup(const char*);
#include <ctype.h>
#include <readline/readline.h>
#include "struct_b.h"


void freeTable(Table* table){
  for(int i = 0; i < table->size; ++i){
    KeySpace* elem = table->ks[i];
    KeySpace* temp;
    while(elem != NULL){
      temp = elem;
      elem = elem->next;
      free(temp);
    }
  }
  free(table->ks);
  free(table->ks_name);
  free(table->i_name);
  free(table);
}

int hash(int size, char* key) {
  int hash = 2139062143, len;
  len = strlen(key);
  for (int i = 0; i < len; ++i)
    hash = hash * 37 + key[i];
  hash = abs(hash) % size;
    return hash;
}
void output(Table* table){
  int count = 0, key_len, info_len;
  char* key, *info;
  table->ks_file = fopen(table->ks_name, "r+b");
  table->i_file = fopen(table->i_name, "r+b");
  printf("\nkey:    info:");
  printf("\n-------------");
  for(int i = 0; i < table->size; ++i){
    KeySpace* elem = table->ks[i];
    if(table->ks[i]->key_of == -1)
      elem = elem->next;
    while(elem != NULL){
      fseek(table->ks_file, elem->key_of, SEEK_SET);
      fseek(table->i_file, elem->info_of, SEEK_SET);
      fread(&key_len, sizeof(int), 1, table->ks_file);
      fread(&info_len, sizeof(int), 1, table->i_file);
      key = (char*) calloc(key_len + 1, sizeof(char));
      info = (char*) calloc(info_len + 1, sizeof(char));
      fread(key, sizeof(char), key_len, table->ks_file);
      fread(info, sizeof(char), info_len, table->i_file);
      printf("\n%d) %s    %4s", ++count, key, info);
      elem = elem->next;
      free(key);
      free(info);
    }
  }
  fclose(table->i_file);
  fclose(table->ks_file);
  printf("\n-------------");
}
int check_exist(Table* table, char* key_g){
  int key_len;
  char* key;
  int i = hash(table->size, key_g);
  KeySpace* elem = table->ks[i];
  table->ks_file = fopen(table->ks_name, "r+b");
  if(elem->key_of == -1)
    elem = elem->next;
  while(elem != NULL){
    fseek(table->ks_file, elem->key_of, SEEK_SET);
    fread(&key_len, sizeof(int), 1, table->ks_file);
    key = (char*) calloc(key_len + 1, sizeof(char));
    fread(key, sizeof(char), key_len, table->ks_file);
    if(!strcmp(key_g, key)){
      free(key);
      fclose(table->ks_file);
      return 0;
    }
    free(key);
    elem = elem->next;
  }
  return 1;
}

Item* get_item(){
  Item* item = (Item*) malloc(sizeof(Item));
  item->key = readline("\nEnter key:  ");
  item->info = readline("\nEnter info:  ");
  return item;
}
void PrintMainMenu(){
  printf("\nEnter 1 to add new element");
  printf("\nEnter 2 to find and out element by key");
  printf("\nEnter 3 to delete element by key");
  printf("\nEnter 4 to output table");
  printf("\nEnter 0 to end\n");
  printf("\nYour input:  ");
}

char* get_ks_name(char* name){
  char* res = (char*) calloc(strlen(name) + 4, 1);
  res[0] = 'k';
  res[1] = 's';
  res[2] = '_';
  for(int i = 3; i < strlen(name) + 3; ++i){
    res[i] = name[i-3];
  }
  return res;
}
char* get_i_name(char* name){
  char* res = (char*) calloc(strlen(name) + 3, 1);
  res[0] = 'i';
  res[1] = '_';
  for(int i = 2; i < strlen(name) + 2; ++i){
    res[i] = name[i-2];
  }
  return res;
}
int delete_element(Table* table, char* key_g) {
  int key_len;
  char* key;
  int i = hash(table->size, key_g);
  KeySpace* elem = table->ks[i];
  table->ks_file = fopen(table->ks_name, "r+b");
  if(elem->key_of == -1)
    elem = elem->next;
  while(elem != NULL){
    fseek(table->ks_file, elem->key_of, SEEK_SET);
    fread(&key_len, sizeof(int), 1, table->ks_file);
    key = (char*) calloc(key_len + 1, sizeof(char));
    fread(key, sizeof(char), key_len, table->ks_file);
    if(!strcmp(key_g, key)){
      free(key);
      if(elem == table->ks[i]){
        elem->key_of = -1;
        elem->info_of = -1;
      }
      else{
        KeySpace* temp = table->ks[i];
        while(temp->next != elem){
          temp = temp->next;
        }
        temp->next = elem->next;
        free(elem);
      }
      fclose(table->ks_file);
      return 0;
    }
    free(key);
    elem = elem->next;
  }
  fclose(table->ks_file);
  return 1;
}
Item* FindAndOut(Table* table, char* key_g){
  int key_len, info_len;
  char* key, *info;
  int i = hash(table->size, key_g);
  KeySpace* elem = table->ks[i];
  table->ks_file = fopen(table->ks_name, "r+b");
  table->i_file = fopen(table->i_name, "r+b");
  if(elem->key_of == -1)
    elem = elem->next;
  while(elem != NULL){
    fseek(table->ks_file, elem->key_of, SEEK_SET);
    fread(&key_len, sizeof(int), 1, table->ks_file);
    key = (char*) calloc(key_len + 1, sizeof(char));
    fread(key, sizeof(char), key_len, table->ks_file);
    if(!strcmp(key_g, key)){
      free(key);
      Item* item = (Item*) malloc(sizeof(Item));
      item->key = strdup(key_g);
      fseek(table->i_file, elem->info_of, SEEK_SET);
      fread(&info_len, sizeof(int), 1, table->i_file);
      info = (char*) calloc(info_len + 1, sizeof(char));
      fread(info, sizeof(char), info_len, table->i_file);
      item->info = strdup(info);
      free(info);
      return item;
    }
    free(key);
    elem = elem->next;
  }
  return NULL;
}
Table* create_table(Table* table, int size){
  char* name = readline("\nEnter filename:  ");
  table = (Table*) malloc(size * sizeof(Table));
  table->size = size;
  table->ks = (KeySpace**) malloc(size * sizeof(KeySpace*));
  for(int i = 0; i < table->size; ++i){
    table->ks[i] = (KeySpace*) malloc(sizeof(KeySpace));
    table->ks[i]->key_of = -1;
    table->ks[i]->info_of = -1;
    table->ks[i]->next = NULL;
  }
  table->ks_name = get_ks_name(name);
  table->i_name = get_i_name(name);
  free(name);
  table->ks_file = fopen(table->ks_name, "w+b");
  table->i_file = fopen(table->i_name, "w+b");
  fclose(table->i_file);
  fclose(table->ks_file);
  return table;
}
void add_element(Table* table, Item* item, int i){
  int key_len, info_len;
  if (table->ks[i]->key_of == -1) {
    table->ks_file = fopen(table->ks_name, "r+b");
    table->i_file = fopen(table->i_name, "r+b");
    fseek(table->ks_file, 0, SEEK_END);
    fseek(table->i_file, 0, SEEK_END);
    table->ks[i]->key_of = ftell(table->ks_file);
    table->ks[i]->info_of = ftell(table->i_file);
    key_len = strlen(item->key);
    info_len = strlen(item->info);
    fwrite(&key_len, sizeof(int), 1, table->ks_file);
    fwrite(&info_len, sizeof(int), 1, table->i_file);
    fwrite(item->key, sizeof(char), key_len, table->ks_file);
    fwrite(item->info, sizeof(char), info_len, table->i_file);
    fclose(table->i_file);
    fclose(table->ks_file);
  }
  else {
    KeySpace* elem = table->ks[i];
    while(elem->next != NULL){
      elem = elem->next;
    }
    elem->next = (KeySpace*) malloc(sizeof(KeySpace));
    table->ks_file = fopen(table->ks_name, "r+b");
    table->i_file = fopen(table->i_name, "r+b");
    fseek(table->ks_file, 0, SEEK_END);
    fseek(table->i_file, 0, SEEK_END);
    elem->next->key_of = ftell(table->ks_file);
    elem->next->info_of = ftell(table->i_file);
    key_len = strlen(item->key);
    info_len = strlen(item->info);
    fwrite(&key_len, sizeof(int), 1, table->ks_file);
    fwrite(&info_len, sizeof(int), 1, table->i_file);
    fwrite(item->key, sizeof(char), key_len, table->ks_file);
    fwrite(item->info, sizeof(char), info_len, table->i_file);
    elem->next->next = NULL;
    fclose(table->i_file);
    fclose(table->ks_file);
  }
}

void freeItem(Item* item){
  free(item->key);
  free(item->info);
  free(item);
}
int main(){
  int check = 1, size, index;
  Table* table;
  printf("\nEnter size of table: ");
  size = get_integer();
  table = create_table(table, size);
  if(size == 0){
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
    Item* item = get_item();
    index = check_exist(table, item->key);
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
    Item* item = FindAndOut(table, key);
    if(item == NULL)
      printf("\nNo element with this key\n");
    else{
      printf("\nInfo about element with this key:   %s\n", item->info);
      freeItem(item);
    }
    free(key);
  }
  else if(check == 3){
     char* key = readline("\nEnter key to delete element:  ");
     index = delete_element(table, key);
     if(index == 0)
       printf("\nElement deleted\n");
     else
       printf("\nNo element with this key\n");
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
