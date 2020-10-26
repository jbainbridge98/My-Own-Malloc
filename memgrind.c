#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "mymalloc.c"


double testA(){
  char* ptr = NULL;
  struct timeval start, end;

  gettimeofday(&start, NULL);

  for(int i = 0; i < 120; i++){
    ptr = malloc(1);
    free(ptr);
  }
  gettimeofday(&end, NULL);
  double result;

  result = (end.tv_sec - start.tv_sec) * 1e6;
  result = (result + (end.tv_usec - start.tv_usec)) * 1e-6;

  return result;
}

double testB(){
  char* ptr[120];

  struct timeval start, end;
  gettimeofday(&start, NULL);

  for(int i = 0; i < 120; i++){
    ptr[i] = malloc(1);
  }
  for(int j = 0; j < 120; j++){
    free(ptr[j]);
  }
  gettimeofday(&end, NULL);
  double result;

  result = (end.tv_sec - start.tv_sec) * 1e6;
  result = (result + (end.tv_usec - start.tv_usec)) * 1e-6;

  return result;
}

double testC(){
  int mallocs = 0;
  int frees = 0;
  char* ptr [120];

  for(int i = 0; i < 120; i++){
    ptr[i] = NULL;
  }

  struct timeval start, end;
  gettimeofday(&start, NULL);

  while(frees < 120){
    int pick = rand() % 2;
    if(mallocs == 120){
      pick = 1;
    } //0 for malloc, 1 for free
    if(pick == 0 && mallocs != 120){
      for(int i = 0; i < 120; i++){
        if(ptr[i] == NULL || mallocs < 1){
          ptr[i] = malloc(1);
          mallocs++;
          break;
        }
      }
    }
    if(pick == 1){
      for(int i = 0; i < 120; i++){
        if(ptr[i] != NULL){
          free(ptr[i]);
          frees++;
          ptr[i] = NULL;
          break;
        }
      }
    }
  }
  gettimeofday(&end, NULL);
  double result;

  result = (end.tv_sec - start.tv_sec) * 1e6;
  result = (result + (end.tv_usec - start.tv_usec)) * 1e-6;

  return result;
}

double testD(){
  struct timeval start, end;
  gettimeofday(&start, NULL);

  char *a = (char*)malloc(1);
  char *b = (char*)malloc(1);
  char *c = (char*)malloc(1);
  char *d = (char*)malloc(1);
  char *e = (char*)malloc(1);
  char *f = (char*)malloc(1);
  char *g = (char*)malloc(1);
  char *h = (char*)malloc(1);
  char *i = (char*)malloc(1);
  char *j = (char*)malloc(1);
  char *k = (char*)malloc(1);
  char *l = (char*)malloc(1);
  char *m = (char*)malloc(1);
  char *n = (char*)malloc(1);
  char *o = (char*)malloc(1);
  char *p = (char*)malloc(1);
  char *q = (char*)malloc(1);
  char *r = (char*)malloc(1);
  char *s = (char*)malloc(1);
  char *t = (char*)malloc(1);
  char *u = (char*)malloc(1);
  char *v = (char*)malloc(1);
  char *w = (char*)malloc(1);
  char *x = (char*)malloc(1);
  char *y = (char*)malloc(1);
  char *z = (char*)malloc(1);

  free(z);
  free(y);
  free(x);
  free(w);
  free(v);
  free(u);
  free(t);
  free(s);
  free(r);
  free(q);
  free(p);
  free(o);
  free(n);
  free(m);
  free(l);
  free(k);
  free(j);
  free(i);
  free(h);
  free(g);
  free(f);
  free(e);
  free(d);
  free(c);
  free(b);
  free(a);


  gettimeofday(&end, NULL);
  double result;

  result = (end.tv_sec - start.tv_sec) * 1e6;
  result = (result + (end.tv_usec - start.tv_usec)) * 1e-6;

  return result;
}

double testE(){
  struct timeval start, end;
  gettimeofday(&start, NULL);


  long wrong = 0;
  free((long*)wrong);

  char ptr = 'e';
  free(&ptr);

  char *p = (char*)malloc(1);
  free(p);
  free(p);

  char *a = (char*)malloc(1);
  free(a);
  free(a);
  free(a);
  free(a);
  free(a);

  char *z = (char*)malloc(4096);
  free(z);

  gettimeofday(&end, NULL);
  double result;

  result = (end.tv_sec - start.tv_sec) * 1e6;
  result = (result + (end.tv_usec - start.tv_usec)) * 1e-6;

  return result;

}


int main(int argc, char *argv[]){
  double totalA;
  double totalB;
  double totalC;
  double totalD;
  double totalE;

  for(int i = 0; i < 50; i++){
    totalA += testA();
    totalB += testB();
    totalC += testC();
    totalD += testD();
    totalE +=testE();
  }

  totalA /= 50;
  totalB /= 50;
  totalC /= 50;
  totalD /= 50;
  totalE /= 50;

  printf("Test A average: %f\nTest B average: %f\nTest C average: %f\nTest D average: %f\nTest E average: %f\n",totalA, totalB, totalC, totalD, totalE);



}
