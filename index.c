/****************************************************
2 *                                                 *
3 * Moisés Silva de Azevedo                         *
4 * RGA 2022.0743.004-6                             *
5 * Implementação 3                                 *
6 * Disciplina: Estruturas de Dados e Programação I *
7 * Professor: Ronaldo Fiorilo                      *
8 *                                                 *
9 ***************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 10
#define FILENAME "data.txt"

typedef char * string;

typedef struct listNode {
    string data;
    struct listNode *next;
} ListNode;

typedef struct treeNode {
    string word;
    int fb;
    ListNode *list;
    struct treeNode *left, *right;
} TreeNode;

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
    printf("%s\n", list->data);
    showList(list->next);
  }
}

int countListNodes(ListNode *list) {
  if(list) {
    return 1 + countListNodes(list->next);
  } else return 0;
}

/* ARVORE AVL */

TreeNode *createTreeNode(string word) {
  TreeNode *node = (TreeNode *) malloc(sizeof(TreeNode));
  if(node) {
    node->word = (string) malloc(strlen(word) + 1);
    if(node->word) {
      strcpy(node->word, word);
      node->fb = 0;
      node->list = NULL;
      node->left = NULL;
      node->right = NULL;
    }
  }
  return node;
}

/* =============== ROTAÇÕES =============== */
TreeNode *simpleRightRotation(TreeNode *root) {
  TreeNode *u, *t2;
  u = root->left;
  t2 = u->right;

  u->right = root;
  root->left = t2;

  root->fb = 0;
  u->fb = 0;

  return u;
}

TreeNode *doubleRightRotation(TreeNode *root) {
  TreeNode *u, *v, *t2, *t3;
  u = root->left;
  v = u->right;

  t2 = v->left;
  t3 = v->right;

  v->left = u;
  v->right = root;

  root->left =t3;
  u->right = t2;

  if(v->fb == -1) {
    u->fb = 1;
  } else {
    u->fb =0;
  }
  if(v->fb == 1) {
    root->fb = -1;
  } else {
    root->fb = 0;
  }
  v->fb = 0;

  return v;
}

TreeNode *simpleLeftRotation(TreeNode *root) {
  TreeNode *u, *t2;
  u =  root->right;
  t2 = u->left;

  u->left = root;
  root->right = t2;

  root->fb = 0;
  u->fb = 0;

  return u;
}

TreeNode *doubleLeftRotation(TreeNode *root) {
  TreeNode *u, *z, *t2, *t3;
  u = root->right;
  z = u->left;

  t2 = z->left;
  t3 = z->right;

  z->right = u;
  z->left =root;

  u->left = t3;
  root->right = t2;

  if(z->fb == 1) {
    u->fb = -1;
  } else {
    u->fb = 0;
  }
  if(z->fb == -1) {
    root->fb = 1;
  } else {
    root->fb = 0;
  }
  z->fb = 0;

  return z;
}
/* =============== ROTAÇÕES =============== */


TreeNode *insertNodeInTree(TreeNode *root, string word, int *h) {
  if(root == NULL) {
    *h = 1;
    return createTreeNode(word);
  } else if(strcmp(word,root->word) == 0) {
    return root;
  } else if(strcmp(word,root->word) < 0) {
    root->left = insertNodeInTree(root->left, word, h);
    if(*h == 1) {
      if(root->fb == 1) {
        root->fb = 0;
        *h = 0;
      } else if(root->fb == 0) root->fb = -1;
      else {
        root->fb = -2;
        if(root->left->fb == -1) {
          root = simpleRightRotation(root);
        } else if(root->left->fb == 1) {
          root = doubleRightRotation(root);
        }
      }
    }
    return root;
  } else {
    root->right = insertNodeInTree(root->right,word, h);
    if(*h == 1) {
      if(root->fb == -1) {
        *h = 0;
        root->fb = 0;
      }
      else if(root->fb == 0) root->fb = 1;
      else {
        root->fb = 2;
        if(root->right->fb == 1) {
          root = simpleLeftRotation(root);
        } else if(root->right->fb == -1) {
          root = doubleLeftRotation(root);
        }
      }
    }
    return root;
  }
}

TreeNode *searchNodeInTree(TreeNode *root, string word) {
  if(root == NULL) return NULL;
  else if(strcmp(word, root->word) < 0) {
    return searchNodeInTree(root->left, word);
  } else if(strcmp(word, root->word) == 0) {
    return root;
  } else {
    return searchNodeInTree(root->right, word);
  }
}

int countTreeNodes(TreeNode *root) {
  if(root) {
    return 1 + countTreeNodes(root->left) + countTreeNodes(root->right);
  } else return 0;
}

/* função que inicializa cada posição da tabela com NULL */
void initTable(TreeNode *table[]) {
  int i;
  for(i = 0; i < SIZE; i++) {
    table[i] = NULL;
  }
}

void insert(TreeNode *table[], string str1, string str2) {
  int index, h;
  TreeNode *temp;
  index = hash(str1);
  h = 0;
  table[index] = insertNodeInTree(table[index], str1, &h);
  temp = searchNodeInTree(table[index], str1);
  if(temp) {
    temp->list = insertNodeInList(temp->list, str2);
  }
}

int countTotalWordsInTable(TreeNode *table[]) {
  int i, count;
  count = 0;
  for(i = 0; i < SIZE; i++) {
    if(table[i]) {
      count += countTreeNodes(table[i]);
    }
  }
  return count;
}

/* =============== Persistencia =============== */

/* SALVAR */
void saveList(FILE *file, ListNode *list) {
  if(list) {
    fprintf(file, "%s\n",list->data);
    saveList(file,list->next);
  }
}

void saveTree(FILE *file, TreeNode *root) {
  if(root) {
    fprintf(file, "%s\n%d\n", root->word, countListNodes(root->list));
    saveList(file,root->list);
    saveTree(file, root->left);
    saveTree(file, root->right);
  }
}

void save(string filename, TreeNode *table[]) {
  FILE *file = fopen(filename, "w");
  if(file) {
    int count_total_words,i;
    count_total_words = countTotalWordsInTable(table);
    fprintf(file, "%d\n", count_total_words);
    for(i = 0; i < SIZE; i++) {
      if(!table[i]) continue;
      saveTree(file, table[i]);
    }
    fclose(file);
  } else {
    fprintf(stderr, "Erro ao abrir o arquivo %s.\n", filename);
  }
}

ListNode *readList(FILE *file, ListNode *list, int count_total_synonyms) {
  int i;
  for(i = 0; i < count_total_synonyms; i++) {
    char synonym[40];
    fscanf(file,"%s\n", synonym);
    list = insertNodeInList(list, synonym);
  }
  return list;
}


/* RECUPERAR */
void read(string filename, TreeNode *table[]) {
  /* considere que a tabela já está iniciada com valores nulos */
  FILE *file = fopen(filename, "r");
  if(file) {
    int count_total_words,i, test;
    test = fscanf(file, "%d\n", &count_total_words);
    if(test != 1) {
      fprintf(stderr, "Erro ao abrir o arquivo %s.\n", filename);
      return;
    }
    for(i = 0; i < count_total_words; i++) {
      int index, h, count_total_synonyms;
      char word[40];
      TreeNode *temp;
      fscanf(file, "%s\n%d\n", word, &count_total_synonyms);
      index = hash(word);
      h = 0;
      table[index] = insertNodeInTree(table[index], word, &h);
      temp = searchNodeInTree(table[index], word);
      temp->list = readList(file,temp->list, count_total_synonyms);
    }

    fclose(file);
  } else {
    fprintf(stderr, "Erro ao abrir o arquivo %s.\n", filename);
  }
}


int main(void) {
  TreeNode *table[SIZE];
  char input[100], op[12], str1[40], str2[40];
  int exit, input_length;
  initTable(table);
  read(FILENAME, table);
  exit = 0;
  while (exit == 0) {
    fgets(input, sizeof(input), stdin);
    input_length = sscanf(input, "%s %s %s", op, str1, str2);
    if(strcmp(op, "insere") == 0) {
      insert(table, str1, str2);
      insert(table, str2, str1);
    }
    else if(strcmp(op, "busca") == 0) {
      int index;
      TreeNode *temp;
      index = hash(str1);
      temp = searchNodeInTree(table[index], str1);
      if(temp) {
        showList(temp->list);
      } else {
        printf("hein?\n");
      }
    } else if(strcmp(op, "remove") == 0) {
      if(input_length == 2) {
        /* TODO remove str */
      } else if(input_length == 3) {
        /* TODO remove str1 str2 */
      }
    }
    else if(strcmp(op, "fim") == 0) {
        exit = 1;
    }

  }
  save(FILENAME, table);
  return 0;
}