// Vous pouvez compiler tests.c comme ceci depuis le terminal:
// gcc -o tests tests.c mymalloc.c
// Ensuite vous pouvez exécuter avec : ./tests 

#include <stdio.h>
#include <signal.h>
#include <setjmp.h>
#include "mymalloc.h"

// Context pour gérer une segfault.
jmp_buf ctx;

// Free doit accepter le pointeur NULL
void test1(){
  myfree(NULL);
}

// Utilisation très simple de malloc et free
void test2(){
  int *a = mymalloc(sizeof(int));
  *a = 0;
  myfree(a);
}

// Test simple pour voir si free fonctionne
// Chaque demande entre dans un bloc mémoire de 4 Ko
// Alloue environ 9.5 Go is la mémoire n'est pas récupérée
void test3(){
  for (int i = 0; i < 5000000; i++){
    int *a = mymalloc(sizeof(int) * 512);
    for(int j = 0; j < 512; j ++){
      a[j]= j;
    }
    myfree(a);
  }
}

// Utilisation très simple de malloc avec une grande taille
void test4(){
  int *a = mymalloc(sizeof(int) * 1024 * 1024);
  for(int j = 0; j < 1024 * 1024; j ++){
    a[j]= j;
  }
  myfree(a);
}

// Test simple pour voir si free fonctionne avec un malloc de grande taille
// Il suffit de regarder la mémoire utiliser par le programme
// Alloue environ 40 Go si la mémoire n'est pas récupérée
void test5(){
  for (int i = 0; i < 10000; i++){
    int *a = mymalloc(sizeof(int) * 1024 * 1024);
    for(int j = 0; j < 1024 * 1024; j += 1000){
      a[j]= j;
    }
    myfree(a);
  }
}

typedef struct node node;
struct node {
  int valueInt;
  char valueChar;
  node *next;
  node *previous;
};

// Petit programme pour testé si malloc et free fonctionne bien
int test6(){
  // La racine
  node *root = mymalloc(sizeof(node));
  root->valueInt = 0;
  root->valueChar = 115;
  root->next = NULL;
  root->previous = NULL;

  // Création d'une liste doublement chaînée
  node *current = root;
  for(int i = 0; i < 4 * 1024; i ++){
  /*for(int i = 0; i < 4 * 1024 * 1024; i ++){*/
    node *p = mymalloc(sizeof(node));
    p->valueInt = i;
    p->valueChar = 115;
    p->next = NULL;
    p->previous = current;
    current->next = p;
    current = p;
  }

  // On efface quelques éléments
  current = root;
  int n = 0;
  while (current->next){
    n++;
    if (n % 23 != 0) {
      // Rien à faire
      current = current->next;
    }
    else {
      // On efface ce noeud
      node *tmp;
      current->previous->next = current->next;
      current->next->previous = current->previous;
      tmp = current->next;
      myfree(current);
      current = tmp;
    }
  }

  // Quelques appel à malloc voir si cela vous mélange un peu
  void *ptrArray[1024];
  for(int i = 0; i < 1024; i ++){
    int *d = mymalloc(sizeof(double));
    *d = 435;
    ptrArray[i] = d;
  }

  // On ajoute quelques éléments
  current = root;
  n = 0;
  while (current->next){
    n++;
    if (n % 27 != 0) {
      // Rien à faire
      current = current->next;
    }
    else {
      // On ajoute un noeud
      node *p = mymalloc(sizeof(node));
      p->valueInt = 2346;
      p->valueChar = 115;
      p->next = current->next;
      p->previous = current;
      current->next = p;
      current = p->next;
    }
  }

  // Calcul de la somme
  current = root;
  long sum = root->valueInt + root->valueChar;
  while (current->next){
    sum += current->valueInt + current->valueChar;
    current = current->next;
  }

  // Nettoyage de la mémoire
  current = root;
  while (current->next){
    node *tmp = current->next;
    myfree(current);
    current = tmp;
  }
  myfree(current);

  for(int i = 0; i < 1024; i ++){
    myfree(ptrArray[i]);
  }

  return sum;
}


void sigseg_handler(int signum) {
  printf("**Segfault\n");
  longjmp(ctx, 1);
}

#define TEST_NOSEGFAULT(test, number) \
  if(!setjmp(ctx)) {\
    test;\
    printf("Test "#number" OK\n");\
  } else \
    printf("Test "#number" KO\n")


int main(){
  signal(SIGSEGV, &sigseg_handler);
  TEST_NOSEGFAULT(test1(), 1);

  TEST_NOSEGFAULT(test2(), 2);

  TEST_NOSEGFAULT(test3(), 3);

  TEST_NOSEGFAULT(test4(), 4);

  TEST_NOSEGFAULT(test5(), 5);

  long res = 0;
  TEST_NOSEGFAULT(res=test6(), 6);
  /*if (res == 632474318)*/
  if (res == 8823938)
    printf("Test 6 bonne réponse %ld OK\n", res);
  else
    printf("Test 6 mauvaise réponse %ld KO\n", res);

  printf("Done\n");
  return 0;
}

