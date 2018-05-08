/*This is a rb program
 *Trevor Horine
 *05/08/18
 */
#include <iostream>
#include <cstring>
#include <fstream>
#include <math.h>
#include <stdlib.h>

using namespace std;
// node struct
struct node{
  node* left = NULL;
  node* right = NULL;
  node* par = NULL;
  int color = -1; // 0 is red 1 is black
  int value = 0;
};
//functions to return things inrelation to node that is passed in
node* parent(struct node* n){
  return n -> par;
}
node* grandparent(struct node* current){
  if(parent(current) == NULL){
    return NULL;
  }
  return current -> par -> par;
}
node* sib(struct node* n){
  struct node* p = n -> par;
  if(p == NULL){
    return NULL;
  }
  if(n == p -> left){
    return p -> right;
  }
  else{
    return p -> left;
  }
}
node* uncle(struct node* n){
  struct node* p = parent(n);
  struct node* g = grandparent(n);
  if(g == NULL){
    return NULL;
  }
  return sib(p);
}
//rotate functions
void rotate_left(node* &root, node* n) {
  node* nnew = n -> right;
  if(nnew != NULL) {
    n -> right = nnew -> left;
    nnew -> left = n;
    nnew -> par = n -> par;
    n -> par = nnew;
    if(nnew -> par != NULL && nnew -> par -> left == n) {
      nnew -> par -> left = nnew;
    }
    else if(nnew -> par != NULL && nnew -> par -> right == n) {
      nnew -> par -> right = nnew;
    }
    if(n == root) {
      root = nnew;
      root -> color = 1;
    }
  }
}

void rotate_right(node* &root, node* n) {
  node* nnew = n -> left;
  if(nnew != NULL) {
    if(n -> par != NULL && n == n -> par -> left) {
      n -> par -> left = n -> left;
    }
    else if(n -> par != NULL && n == n -> par -> right) {
      n -> par -> right = n -> left;
    }
    n -> left = nnew -> right;
    if(nnew -> right != NULL) {
      nnew -> right -> par = n;
    }
    nnew -> right = n;
    nnew -> par = n -> par;
    n -> par = nnew;
    if(n == root) {
      root = nnew;
      root -> color = 1;
    }
  }
}
//prototypes for the oter functions
void print(struct node*, int);
struct node* insert(struct node*&,struct node*);
void insert_rec(struct node*&, struct node* );
void insert_repair_tree(struct node*&, struct node*);
void insert_case1(struct node*&, struct node*);
void insert_case2(struct node*&, struct node*);
void insert_case3(struct node*&, struct node*);
void insert_case4(struct node*&, struct node*);
void insert_case4part2(struct node*&, struct node*);

//main mentod
int main(){
  struct node* root = new node();
  root = NULL;
  int input;
  char in[500];
  bool w = true;
  int f[400];
  while(w == true){
    //what do you want to do
    cout << "do you want to add, or quit?" << endl;
    for (int i = 0; i < 10; i++){
      in[i] = NULL;
    }
    cin >> in;
    for (int i = 0; i < 10; i++){
      in[i] = toupper(in[i]);
    }
    //to add this happens
    if(strcmp(in, "ADD") == 0){
      cout << "enter a number between 1 and 1000 to add to the tree or a file name including the .txt" << endl;
      input = NULL;
      for(int i = 0; i < 400; i++){
 	in[i] = NULL;
      }
      cin >> in;
      //if is file
      if (isalpha(in[0])){
 	ifstream file;
 	file.open(in);
 	file.getline(in, 400);
 	file.close();
 	for(int i = 0; i < 400; i++){
	  //if not number replace , with space
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
      //separate by space
      char *s = strtok(in, " ");
      while (s != NULL) {
 	if (i < 400) {
 	  //cout << s << endl;
 	  f[i] = atoi(s);
 	  struct node* temp = new node();
	  temp -> value = f[i];
	  //root = NULL;
	  root = insert(root, temp);
 	  //cout << f[i] << endl;
 	}
	s = strtok(NULL, " ");
      }
      //print
      //cout << root -> value << endl;
      print(root, 0);
    }
    //to quit this happens
    else if(strcmp(in, "QUIT") == 0){
      w = false;
    }
  }
}

//method to add nodes to tree
struct node* insert(struct node* &root, struct node* n){
  insert_rec(root, n);
  insert_repair_tree(root, n);
  return root;
}
//another part of adding
void insert_rec(struct node* &root, struct node* n){
  if (root == NULL){
    root = n;
    root -> color = 1;
    }
  else if (root != NULL && n -> value < root -> value){
    if(root -> left != NULL){
      insert_rec(root -> left, n);
      return;
    }
    else{
      root -> left = n;
      n -> par = root;
      n -> left = NULL;
      n -> right = NULL;
      n -> color = 0;
    }
  }
  else{
    if(root -> right != NULL){
      insert_rec(root -> right, n);
      return;
    }
    else{
      root -> right = n;
      n -> par = root;
      n -> left = NULL;
      n -> right = NULL;
      n -> color = 0;
    }
  }
}
//to fix tree do the following
void insert_repair_tree(struct node* &root, struct node* n){
  if (parent(n) == NULL) {
    insert_case1(root, n);
  } else if (parent(n) -> color == 1) {
    insert_case2(root, n);
  } else if (uncle(n) != NULL && uncle(n) -> color == 0) {
    insert_case3(root, n);
  } else {
    insert_case4(root, n);
  }
}
//if case one do following
void insert_case1(struct node* &root, struct node* n){
  if (parent(n) == NULL){
    n -> color = 1;
  }
}
//if case two it is in right spot
void insert_case2(struct node* &root, struct node* n){
  return;
}
//if case three do following
void insert_case3(struct node* &root, struct node* n){
  parent(n) -> color = 1;
  uncle(n) -> color = 1;
  if(grandparent(n) != root){
    grandparent(n) -> color = 0;
  }
  insert_repair_tree(root, grandparent(n));
}
//if case four do the following
void insert_case4(node* &root, node* n) {
  node* p = parent(n);
  node* g = grandparent(n);
  
  if(g -> left != NULL && g - >left -> right != NULL && n == g -> left -> right) {
    rotate_left(root, p);
  }
  else if(g -> right != NULL && g -> right -> left != NULL && n == g -> right -> left) {
    rotate_right(root, p);
    n = n -> right;
    insert_case4part2(root, n);
    return;
  }
  else if(g -> right != NULL && g -> right -> right !=NULL && n == g -> right -> right) {
    if(g == root) {
      if(p-> left != NULL) {
	g -> right = p -> left;
	p -> left -> par = g;
	g -> par = p;
      }
      else {
	g -> par = p;
      }
      g -> right = p -> left;
      p -> left = g;
      root = p;
      p -> right = n;
      n -> par = p;
      p -> color = 1;
      g -> color = 0;
    }
    else if(g -> par != NULL && g -> par -> right != NULL && g == g -> par -> right) {
      g -> par -> right = p;
      p -> left = g;
      p -> par = g -> par;
      g -> par = p;
      g -> right = NULL;
      p -> color = 1;
      g -> color = 0;
    }
    else if(g -> par != NULL) {
      g -> par -> left = p;
      p -> left = g;
      p -> par = g -> par;
      g -> par = p;
      g -> right = NULL;
      p -> color = 1;
      g -> color = 0;
    }
    return;
  }
  else if(g -> left != NULL && g -> left -> left != NULL && n == g -> left -> left && n -> color == 0 && n -> par -> color == 0) {
    insert_case4part2(root, n);
    return;
  }
  insert_case4part2(root, n->left);
}
// part two of case four
void insert_case4part2(node* &root, node* n) {
  node* p = parent(n);
  node* g = grandparent(n);
  
  if(n -> par -> left != NULL && n == p -> left) {
    rotate_right(root, g);
  }
  else {
    rotate_left(root, g);
  }
  p -> color = 1;
  g -> color = 0;
}

//method to print tree
void print(struct node* current, int d){
  //cout << "TREE" << endl;
  //cout << current << endl;
  if(current -> left != NULL){
    print(current -> left, d+1);
  }
  int t = d;
  for(t; t>0; t--){
    cout << "\t";
  }
  cout << current -> value;
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
