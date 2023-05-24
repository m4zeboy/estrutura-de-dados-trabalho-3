#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define SIZE 4001
typedef char * string;

long power(int k, int i) {
  if(i == 0) {
    return 1;
  } else if(i == 1) {
    return k;
  } else {
    return k * power(k, i-1);
  }
}


/* Dado uma string obtem o somatório do codigo ascii do caractere atual multiplicado pela constante k elevado a i, onde i é a iteração, no final retorna o módulo desse valor pelo tamanho da tabela */
unsigned int hash(string data) {
  int k, i;
  unsigned long hashValue = 0;
  k = 33;
  for(i = 0; data[i] != '\0'; i++) {
    hashValue += data[i] * power(k, i);
  }
  return hashValue % SIZE;
}

/* Nó da lista de sinônimos, a chave e o valor é uma string que guarda o sinonimo */
typedef struct listNode {
  string data;
  struct listNode *next;
} ListNode;

ListNode *createListNode(string data) {
  ListNode *node = (ListNode *) malloc(sizeof(ListNode));
  if(node) {
    node->data = (string) malloc(strlen(data) + 1);
    if(node->data) {
      strcpy(node->data, data);
      node->next = NULL;
    }
  }
  return node;
}

ListNode *insertNodeInList(ListNode *list, string data) {
  if(list == NULL) {
    return createListNode(data);
  } else if(strcmp(data,list->data) > 0) {
    list->next = insertNodeInList(list->next, data);
    return list;
  }
   else if(strcmp(data,list->data) < 0) {
    ListNode *new = createListNode(data);
    new->next = list;
    list = new;
    return list;
   } else { return list; }
}

void showList(ListNode *list) {
  if(list) {
    printf("%s ", list->data);
    showList(list->next);
  }
}

/* Nó da árvore AVL de palavras, a chave e o valor é uma string que guarda uma palavra */
typedef struct treeNode {
  string word;
  int fb;
  struct treeNode *left, *right;
} TreeNode;

TreeNode *createTreeNode(string word) {
  TreeNode *node = (TreeNode *) malloc(sizeof(TreeNode));
  if(node) {
    node->word = (string) malloc(strlen(word) + 1);
    if(node->word) {      
      strcpy(node->word, word);
      node->fb = 0;
      node->left = NULL;
      node->right = NULL;
    }
  }
  return node;
}

int main(void) {
  TreeNode *table[SIZE];
  return 0;
}