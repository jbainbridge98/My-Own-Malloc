#include<stdio.h>
#include<stddef.h>

//The storage to be malloced and freed to
static unsigned char myBlock[4096];

//Struct of linked list of metadata
typedef struct metadataLL{
  short size;
  char isfree; //n = not free f = free
  struct metadataLL *next;
}metadataLL;

//sets the linked list to the same point in memory as the array of 4096
metadataLL *openLL = (void*)myBlock;

void create();
void split(metadataLL *first, size_t size);
void *ourMalloc(size_t iptSize);
void merge();
void ourFree(void* ptr);
void optList(metadataLL* ipt);
void *find_free(size_t givSize);
int size_left();
