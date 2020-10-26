#include "mymalloc.h"
#define malloc(x) mymalloc(x)
#define free(x) myfree(x)

//initiallizes the linked list to be the size of the size 4096 array
//sets linked list to free and the next entry to NULL
void create(){
  openLL->size = 4096;
  openLL->isfree = 'f';
  openLL->next = NULL;
}

/*since the singular node in the linked list is the size of the array, everytime a new allocation is called and there isnt a block that is already the size
of the asked memory this function splits up a block so we minimize the ammounts of free blocks by only taking what the user asks for
The first line finds the next metadata block by adding the position of first, the size of the data, and the size of a metadata block*/
void split(metadataLL *first, size_t size){
  metadataLL *insert = (void*)((void*)first + size + sizeof(metadataLL));

  short newSize = first->size - size - sizeof(metadataLL);
  insert->size = newSize;

  insert->isfree = 'f';
  insert->next = first->next;

  first->size = size;
  first->isfree = 'n';
  first->next = insert;
}

/*this function recursively merges all the free blocks together, since we initialize the first linked list block to be the entire size of the array and we split every time we need new memory
we merge all the free memory back to a large free block so we can split again*/
/*if the current metadata block is free and the next is free we merge the two, if the current metadata block is not free we go on to the next block*/
void merge(metadataLL *cur){
  if(cur == NULL){
    return;
  }else if(cur->next == NULL){
    return;
  }else if(cur->isfree == 'f' && cur->next->isfree == 'f'){
    short combSize = cur->size + cur->next->size + sizeof(metadataLL);
    cur->size = combSize;
    cur->next = cur->next->next;
    merge(cur->next);
  }else if(cur->isfree == 'n'){
    merge(cur->next);
  }else if(cur->next == NULL){
    return;
  }
}

/*find_free does what the name implies, it finds a free block when called in mymalloc it finds a free block to allocate the memory to
if the block is larger than what is asked, the code calls split to utilize in a cost effective way
when called in merge it simply finds a free block then merge checks if the next block is also free so a merge can be preformed*/
void *find_free(size_t givSize){
  metadataLL *ptr;
  ptr = openLL;

  while(ptr != NULL){
    if(ptr->isfree == 'n'){
      ptr = ptr->next;
      continue;
    }if(ptr->size < givSize){
      ptr = ptr->next;
      continue;
    }if(ptr->size >= givSize){
      return ptr;
    }if(ptr->next == NULL && ptr->isfree != 'f'){
      printf("No room left for allocation in file %s, on line %d\n", __FILE__, __LINE__ - 1);
      return NULL;
    }
  }

  return NULL;
}

//size_left is a function we call in mymalloc to ensure there is enough data for what the user is asking
//size_left loops through the linked list and adds all the sizes and the sizes of a new struct
int size_left(){
  metadataLL *cur;
  cur = openLL;
  int memUsed = 0;
  int memLeft = 0;

  while(cur != NULL){
    if(cur->isfree == 'n'){
      memUsed += cur->size + sizeof(metadataLL);
    }
    cur = cur->next;
  }
  memLeft = 4096 - memUsed;
  return memLeft;
}

void optList(metadataLL* ipt){
  metadataLL *cur;

  cur = ipt;

  do{
    printf("Size: %d, Free Status: %c\n",cur->size, cur->isfree);
    cur = cur->next;
  }while(cur != NULL);
}

//mymalloc takes an input of type size_t and does multiple checks, if the size is 0 (since size_t is an unsigned int it cannot be a negative number) then mymalloc outputs an error
//if the size is over the size of the memory it outputs an error
void *mymalloc(size_t iptSize){
  metadataLL *cur;
  void *opt;
  int remainingMem = 0;
  remainingMem = size_left();

  if(iptSize == 0){
    printf("Input to small in file %s, on line %d\n", __FILE__, __LINE__ - 1); //maybe push out an error later
    return NULL;
  }
  if(iptSize > 4096 - sizeof(metadataLL)){
    printf("Input to large in file %s, on line %d\n", __FILE__, __LINE__ - 1); //to much data being asked to be allocated
    return NULL;
  }
  if(iptSize > remainingMem - sizeof(metadataLL)){
    printf("Not enough space in file %s, on line %d\n", __FILE__, __LINE__ - 1); //to much data being asked to be allocated
    return NULL;
  }
  if(openLL->size == 0){
    create(); //if memory has not been initialized
  }
  cur = find_free(iptSize);

  if(cur->size == iptSize){
    cur->isfree = 'n';
    cur->size = iptSize;
    opt = (void*)(++cur);
    //printf("%s\n", "Allocation Successful");
    return opt;
  }else if((cur->size) > (iptSize+sizeof(openLL))){
    split(cur, iptSize);
    cur->size = iptSize;
    opt = (void*)(++cur);
    //printf("%s\n", "Allocation Successful, Split performed");
    return opt;
  }

  return NULL;

}

/*myFree is the free function that loops through the linked list looking for the pointer given to it
once this function has found the pointer it sets the metadata blocks isfree flag to 'f' indicating it is free*/
/*since the metadata is before the pointer to the actual space we need to increment the cur pointer and check if it is equal to the pointer free was given
if these two pointers are equal we decrement to get back to the metadata and set its isfree flag to free*/
/*lastly we then call merge to combine the newly freed block to the next block if and only if the next block is also free*/
void myfree(void* ptr){

  metadataLL *cur;
  cur = openLL;

  while(cur != NULL){

    if(++cur == ptr){
      --cur;
      if(cur->isfree == 'f'){
        printf("Attempting to free pointer that has already been freed in file %s, on line %d\n", __FILE__, __LINE__ - 1);
        break;
      }
      cur->isfree = 'f';
      //printf("Allocated memory is now free from call in file %s, on line %d\n", __FILE__, __LINE__ - 1);
      merge(openLL);
      return;
    }
    --cur;
    if(cur->next != NULL){
      cur = cur->next;
    }
    else if(cur->next == NULL){
      printf("Attempting to free non allocated space or address is not a pointer in file %s, on line %d\n", __FILE__, __LINE__ - 1);
      break;
    }
  }
}
