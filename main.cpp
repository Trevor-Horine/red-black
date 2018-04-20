/*This is a rb program
 *Trevor Horine
 *3/15/18
 */
#include <iostream>
#include <cstring>
#include <fstream>
#include <math.h>
#include <stdlib.h>

using namespace std;

struct node{
  node* left = NULL;
  node* right = NULL;
  node* par = NULL;
  int color = -1; // 0 is red 1 is black
  int value = 0;
};
node* parent(struct node* n){
  return n -> par;
}
node* grandparent(struct node* current){
  return current -> par -> par;
}
node* sib(struct node* n){
  struct node* p = n -> par;
  if(p == NULL){
    return NULL;
    if(n == p -> left){
      return p -> right;
    }
    if(n == p -> right){
      return p -> left;
    }
  }
}
node* uncle(struct node* n){
  struct node* p = parent(n);
  struct node* g = grandparent(n);
}
void rotate_left(struct node* n){
  struct node* nnew = n -> left;
  n -> right = nnew -> left;
  nnew -> left = n;
  nnew -> par = n -> par;
  n -> par = nnew;
}
void rotate_right(struct node* n){
  struct node* nnew = n -> left;
  n -> left = nnew -> right;
  nnew -> right = n;
  nnew -> par = n -> par;
  n -> par = nnew;
}

void print(struct node*, int);
struct node* insert(struct node*,struct node*);
void insert_rec(struct node*, struct node* );
void insert_repair_tree(struct node*);
void insert_case1(struct node* n);
void insert_case2(struct node* n);
void insert_case3(struct node* n);
void insert_case4(struct node* n);

//main mentod
int main(){
  struct node* root = new node();
  root -> value = NULL;
  int input;
  char in[500];
  bool w = true;
  int f[400];
  while(w == true){
    cout << "do you want to add, remove, or quit?" << endl;
    for (int i = 0; i < 10; i++){
      in[i] = NULL;
    }
    cin >> in;
    for (int i = 0; i < 10; i++){
      in[i] = toupper(in[i]);
    }
    if(strcmp(in, "ADD") == 0){
      cout << "enter a number between 1 and 1000 to add to the tree or a file name including the .txt" << endl;
      input = NULL;
      for(int i = 0; i < 400; i++){
 	in[i] = NULL;
      }
      cin >> in;
      if (isalpha(in[0])){
 	ifstream file;
 	file.open(in);
 	file.getline(in, 400);
 	file.close();
 	for(int i = 0; i < 400; i++){
 	  if (!isdigit(in[i])){
 	    if (in[i] == ','){
 	      in[i] = ' ';
 	    }
 	    else{
 	      i++;
 	    }
 	  }
 	}
 	for (int i = 0; i < 400; i++){
 	  cout << in[i];
	}
 	cout << endl;
      }
      int i = 0;
      char *s = strtok(in, " ");
      while (s != NULL) {
 	if (i < 400) {
 	  //cout << s << endl;
 	  f[i] = atoi(s);
 	  struct node* temp = new node();
 	  temp -> value = f[i];
 	  insert(temp, root);
 	  //cout << f[i] << endl;
 	}
	s = strtok(NULL, " ");
      }
      print(root, 0);
    }
    else if(strcmp(in, "QUIT") == 0){
      w = false;
    }
  }
}

//method to add nodes to tree
struct node* insert(struct node* root, struct node* n){
  if (root == NULL){
    root = n;
    root -> color = 1;
  }
  else{
    insert_rec(root, n);
    insert_repair_tree(n);
    root = n;
    while(parent(root) != NULL){
      root = parent(root);
    }
  }
  return root;
}

void insert_rec(struct node* root, struct node* n){
  if (root != NULL && n -> value < root -> value){
    if(root -> left != NULL){
      insert_rec(root -> left, n);
      return;
    }
    else{
      root -> left = n;
    }
  }
  else if ( root != NULL){
    if ( root -> right != NULL){
      insert_rec(root -> right, n);
      return;
    }
    else{
      root -> right = n;
    }
  }
  n -> par = root;
  n -> left = NULL;
  n -> right = NULL;
  n -> color = 0;
}

void insert_repair_tree(struct node* n){
  if (parent(n) == NULL) {
    insert_case1(n);
  } else if (parent(n) -> color == 1) {
    insert_case2(n);
  } else if (uncle(n) -> color == 0) {
    insert_case3(n);
  } else {
    insert_case4(n);
  }
}

void insert_case1(struct node* n){
  if (parent(n) == NULL)
    n -> color = 1;
}

void insert_case2(struct node* n){
  return;
}

void insert_case3(struct node* n){
  parent(n) -> color = 1;
  uncle(n) -> color = 1;
  grandparent(n) -> color = 0;
  insert_repair_tree(grandparent(n));
}

void insert_case4(struct node* n){
  struct node* p = parent(n);
  struct node* g = grandparent(n);
  if (n == g -> left -> right) {
    rotate_left(p);
    n = n -> left;
  }
  else if (n == g -> right -> left) {
    rotate_right(p);
    n = n -> right; 
  }
  if (n == p -> left) {
    rotate_right(g);
  }
  else{
    rotate_left(g);
  }
  p -> color = 1;
  g -> color = 0;
}

//method to print tree
void print(struct node* current, int d){
  //cout << "TREE" << endl;
  if(current -> left != NULL){
    print(current-> left, d+1);
  }
  int t = d;
  for(t; t>0; t--){
    cout << "\t";
  }
  cout << current -> value << " ";
  if(current -> color == 0){
    cout << "R" << endl;
  }
  else if(current -> color == 1){
    cout << "B" << endl;
  }
  if(current -> right != NULL){
    print(current -> right, d+1);
  }
}
