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
  ListNode *list;
  struct treeNode *left, *right;
} TreeNode;

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

/* rotações */
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

void showTree(TreeNode *root) {
  if(root) {
    showTree(root->left);
    printf("%s ", root->word);
    showTree(root->right);
  }
}

int main(void) {
  TreeNode *tree = NULL;
  int h = 0;
  tree = insertNodeInTree(tree, "moises", &h);  
  tree = insertNodeInTree(tree, "antonio", &h);  
  tree = insertNodeInTree(tree, "gabriel", &h);  
  tree = insertNodeInTree(tree, "maria", &h);  
  tree = insertNodeInTree(tree, "vilmar", &h);
  showTree(tree);  
  return 0;
}