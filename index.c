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
#include <time.h>

#define SIZE 4001
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

/* Dado uma string obtem o somatório do codigo ASCII do caractere atual multiplicado pela constante k elevado a i, onde i é a iteração, no final retorna o módulo desse valor pelo tamanho da tabela */
unsigned int hash(string data) {
  int k, i;
  unsigned long hashValue = 0;
  k = 33;
  for(i = 0; data[i] != '\0'; i++) {
    hashValue += data[i] * power(k, i);
  }
  return hashValue % SIZE;
}

/* função que aloca memória dinâmicamente para a criação de um ListNode */
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

/* função que insere uma string data na lista 'list' */
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

/* função que libera a memória alocada para um ListNode */
void freeListNode(ListNode *node) {
  if(node) {
    free(node->data);
    free(node);
  }
}

/* função que percorre e libera cada nó da lista 'list' */
void freeList(ListNode *list) {
  if(list) {
    ListNode  *next = list->next;
    freeListNode(list);
    freeList(next);
  }
}

/* função que busca por um nó na lista e caso exista remove o nó. */
ListNode *removeNodeFromList(ListNode *list, string data) {
  if(list == NULL) return NULL;
  else if(strcmp(list->data, data) == 0) {
    ListNode *next = list->next;
    freeListNode(list);
    return next;
  } else if(strcmp(data, list->data) > 0){
    list->next = removeNodeFromList(list->next, data);
    return list;
  } else { return list; }
}

/* função que mosta cada nó da lista em uma linha */
void showList(ListNode *list) {
  if(list) {
    printf("%s\n", list->data);
    showList(list->next);
  }
}

/* função que conta quantos nós uma lista tem */
int countListNodes(ListNode *list) {
  if(list) {
    return 1 + countListNodes(list->next);
  } else return 0;
}

/*  =============== ÁRVORE AVL =============== */

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

void freeTreeNode(TreeNode *node) {
  if(node) {
    free(node->word);
    freeList(node->list);
    free(node);
  }
}

/* ROTAÇÕES ------------------------------------ */

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

/* ROTAÇÕES ------------------------------------*/

/* função que insere uma palavra 'word' na árvore avl passada por parâmetro, caso não exista  */
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

/* função que busca por determinada palavra na árvore avl passada por parâmetro */
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

/* função que busca por determinada palavra na árvore avl passada por parâmetro e caso exista remove o nó.. */
TreeNode *removeNodeFromTree(TreeNode *root, string word, int *h) {
  if(root == NULL) return NULL;
  else if(strcmp(root->word, word) == 0) {
    /* nenhum filho */
    if(!root->left && !root->right) {
      *h = 1;
      freeTreeNode(root);
      return NULL;
    } else if(root->left && !root->right) {
      /* um filho (esquerdo) */
      TreeNode *temp = root->left;
      *h = 1;
      freeTreeNode(root);
      return temp;
    } else if(!root->left && root->right) {
      /* um filho (direito)*/
      TreeNode *temp = root->right;
      *h = 1;
      freeTreeNode(root);
      return temp;
    } else {
      /* dois filhos */
      TreeNode *substitute = root->left;
      while(substitute->right) substitute = substitute->right;
      /* liberar e realocar memoria para a string pois os tamanhos das strings podem ser diferentes */
      free(root->word);
      root->word = (string) malloc(strlen(substitute->word) + 1);
      strcpy(root->word, substitute->word);
      root->left = removeNodeFromTree(root->left, substitute->word, h);
      if(*h == 1) {
        if(root->fb == -1) {
          root->fb = 0;
        } else if(root->fb == 0) {
          root->fb = 1;
          *h = 0;
        } else {
          if(root->right->fb == 0) {
            root = simpleLeftRotation(root);
            root->left->fb = 1;
            root->fb = -1;
            *h = 0;
          } else if(root->right->fb == 1) {
            root = simpleLeftRotation(root);
            root->right->fb = 0;
            root->fb = 0;
          } else {
            root = doubleLeftRotation(root);
            root->fb = 0;
          }
        }
      }
      return root;
    }
  } else if(strcmp(word, root->word) < 0) {
    root->left = removeNodeFromTree(root->left, word, h);
    if(*h == 1) {
      if(root->fb == -1) {
        root->fb = 0;
      } else if(root->fb == 0) {
        root->fb = 1;
        *h = 0;
      } else {
        if(root->right->fb == 0) {
          root = simpleLeftRotation(root);
          root->right->fb = 1;
          root->fb = -1;
          *h = 0;
        } else if(root->right->fb == 1) {
          root = simpleLeftRotation(root);
          root->right->fb = 0;
          root->fb = 0;
        } else {
          root = doubleLeftRotation(root);
          root->fb = 0;
        }
      }
    }
    return root;
  } else {
    root->right = removeNodeFromTree(root->right, word, h);
    if(*h == 1) {
      if(root->fb == 1) {
        root->fb = 0;
      } else if(root->fb == 0) {
        root->fb = -1;
        *h = 0;
      } else{
        if(root->left->fb == 0) {
          root = simpleRightRotation(root);
          root->fb = -1;
          root->right->fb = 1;
          *h = 0;
        } else if(root->left->fb == -1) {
          root = simpleRightRotation(root);
          root->fb = 0;
          root->right->fb = 0;
        } else if(root->left->fb == 1) {
          root = doubleRightRotation(root);
          root->fb = 0;
        }
      }
    }
    return root;
  }
}

/* função que busca por uma palavra str1 na tabela e caso encontre tenta remover o sinônimo str2 caso o encontre também */
void removeSynonym(TreeNode *table[], string str1, string str2) {
  int index;
  TreeNode *temp;
  index = hash(str1);
  temp = searchNodeInTree(table[index], str1);
  if(temp) {
    temp->list = removeNodeFromList(temp->list, str2);
    /* remover a palavra da estrutura se ela não possuir sinÔnimos */
    if(temp->list == NULL) {
      int h = 0;
      table[index] = removeNodeFromTree(table[index], str1, &h);
    }
  }
}

/* função que conta quantos nós uma árvore tem */
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

/* função que  insere a palavra str2 na tabela e insere str2 na lista de sinônimos de str1 */
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

/* função que conta quantas palavras tem na tabela - conta todos os nós de todas as árvores */
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

/* SALVAR ------------------------------------ */

/* função que escreve uma lista no arquivo file */
void saveList(FILE *file, ListNode *list) {
  if(list) {
    fprintf(file, "%s\n",list->data);
    saveList(file,list->next);
  }
}
/* função que escreve uma árvore  no arquivo file */
void saveTree(FILE *file, TreeNode *root) {
  if(root) {
    fprintf(file, "%s\n%d\n", root->word, countListNodes(root->list));
    saveList(file,root->list);
    saveTree(file, root->left);
    saveTree(file, root->right);
  }
}
/* função que escreve percorre a tabela escreve no arquivo  com nome filename */
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

/* RECUPERAR ------------------------------------ */

/* função que que lê 'count_total_synonyms' linhas e insere na lista 'list' */
ListNode *readList(FILE *file, ListNode *list, int count_total_synonyms) {
  int i;
  for(i = 0; i < count_total_synonyms; i++) {
    char synonym[40];
    fscanf(file,"%s\n", synonym);
    list = insertNodeInList(list, synonym);
  }
  return list;
}

/* função que lê os dados do arquivo e insere na tabela 'table' */
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
      clock_t inicio, fim;
      double cpu_time_used;
      int index;
      TreeNode *temp;
      inicio = clock();
      index = hash(str1);
      temp = searchNodeInTree(table[index], str1);
      fim = clock();
      cpu_time_used = ((double) (fim - inicio)) / CLOCKS_PER_SEC;

      if(temp) {
        showList(temp->list);
      } else {
        printf("hein?\n");
      }
      printf("\nbuscou em: %f segundos\n", cpu_time_used);
    } else if(strcmp(op, "remove") == 0) {
      if(input_length == 2) {
        int index, h;
        index = hash(str1);
        h = 0;
        table[index] = removeNodeFromTree(table[index], str1, &h);
      } else if(input_length == 3) {
        /* remove str1 str2 */
        removeSynonym(table, str1, str2);
        /* remove str2 str1 */
        removeSynonym(table, str2, str1);
      }
    }
    else if(strcmp(op, "fim") == 0) {
        exit = 1;
    }

  }
  save(FILENAME, table);
  return 0;
}