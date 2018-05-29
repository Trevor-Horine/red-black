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
    if(nnew -> left != NULL) {
      nnew -> left -> par = n;
    }
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
void search(struct node*, int, bool&);
node* find(struct node*, int);
struct node* insert(struct node*&,struct node*);
void insert_rec(struct node*&, struct node* );
void insert_repair_tree(struct node*&, struct node*);
void insert_case1(struct node*&, struct node*);
void insert_case2(struct node*&, struct node*);
void insert_case3(struct node*&, struct node*);
void insert_case4(struct node*&, struct node*);
void insert_case4part2(struct node*&, struct node*);
node* replace_node(node* n, node* child);
node* remove(node* &root, int removing);
void removal(node*&, node* n, int side);
bool isBlack(node* n);

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
    cout << "do you want to add, search, delete, or quit?" << endl;
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
    else if(strcmp(in, "SEARCH") == 0){
      bool found = false;
      cout << "What number do you want to search for?" << endl;
      int num = 0;
      cin >> num;
      search(root, num, found);
      if (found == true){
        cout << "That number is in the tree." << endl;
      }
      else{
        cout << "That number is not in the tree." << endl;
      }
    }
    else if(strcmp(in, "DELETE") == 0){
      if(root == NULL || root -> value == 0) {
	  cout << "The tree is empty, nothing to delete" << endl;
	}
	else {
	  int num;
	  bool found = false;
	  cout << "What number do you want to delete?" << endl;
	  cin >> num;
	  if(find(root, num) == NULL) {
	    cout << "That number is not in the tree" << endl;
	  }
	  else {
	    // cout << "banana" << endl;
	    // cout << root << endl;
	    root = remove(root, num);
	    // node* temp = remove(root, num);
	    //print(temp, 0);
	    //root = temp;
	    //root = remove(root, num);
	    //cout << temp << endl;
	    //cout << root << endl;
	    if(root == NULL) {
	      delete root;
	      root = NULL;
	      break;
	    }
	  }
	}
      print(root, 0);
    }
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
  
  if(g -> left != NULL && g ->left -> right != NULL && n == g -> left -> right) {
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

void search(struct node* current, int n, bool &found){
  if(current -> left != NULL){
    search(current -> left, n, found);
  }
  if(current -> value == n){
    found = true;
    return;
  }
  else{
    found == false;
  }
  if(current -> right != NULL){
    search(current -> right, n, found);
  }
}
node* find(node* root, int number){
//If the number is bigger than head go right
  if(number > root -> value) {
    //If heads right is not null then recursively go there
    if(root -> right != NULL) {
      return find(root -> right, number);
    }
    //if it is null then the number is not in the tree
    else {
      return NULL;
    }
  }
  else if(number == root -> value) {
    return root;
  }
  else if(number < root -> value) {
    if(root -> left != NULL) {
      return find(root -> left, number);
    }
    else {
      return NULL;
    }
  }
}
node* replace_node(node* old, node* newnode) {
  //if the node being replaced is not the root
  if(parent(old) != NULL) {
    newnode -> par = parent(old);

    if(parent(old) -> right == old) {
      parent(old) -> right = newnode;
    }
    else {
      parent(old) -> left = newnode;
    }
    old -> par = NULL;
    return old;
  }
  //If the node being replaced is the root, change the root's value
  else {
    old -> value = newnode -> value;
    old -> left = newnode -> left;
    old -> right = newnode -> right;
    if(old -> left != NULL) {
      old -> left -> par = old;
    }
    if(old -> right != NULL) {
      old -> right -> par = old;
    }
    return newnode;
  }
}

//Finds if the node is black or not
bool isBlack(node* n) {
  if(n != NULL && n->color == 1) {
    return true;
  }
  else if(n != NULL && n->color == 0) {
    return false;
  }
  else {
    return true;
  }
}

//removes the node from the tree
void removal(node* &root, node* n, int side) {
  //if then node being removed is the root, special case
  if(side != 0) {
    //if the sibling is 0
    if(isBlack(sib(n)) == false) {
      //switch the parent and sibling colors
      sib(n)->color = 1;
      parent(n)->color = 0;
      
      //roteat around parent, making sibling the grandparent
      if(side == 2) {
	rotate_right(root, parent(n));
      }
      else {
	rotate_left(root, parent(n));
      }
    }

    //sibling is black
    //if siblings children are black

    if(isBlack(sib(n)->right) && isBlack(sib(n)->left)) {
      //if n is black
      if(isBlack(parent(n)) == true) { //USED TO BE ISBLACK(N)
	//balance tree by making sibling red
	sib(n)->color = 0;

	//basically this section of the tree is unbalanced relative to the entire tree
	//so call recursively on n's parent
	if(!parent(parent(n))) {
	  removal(root, parent(n), 0);
	}
	else if(parent(parent(n))->right == parent(n)) {
	  removal(root, parent(n), 2);
	}
	else {
	  removal(root, parent(n), 1);
	}
	return;
      }
      
      //the parent is red
      else {
	parent(n)->color = 1;
	sib(n)->color = 0;
	return;
      } 
    }
    
    

    //n sibling has to have one red child
    if(side == 2 && (sib(n)->left == NULL || isBlack(sib(n)->left) == true) && (isBlack(sib(n)->right) == false)) {
      sib(n)->right->color = 1;
      sib(n)->color = 0;
      rotate_left(root, sib(n));
    }
    else if(side == 1 && (sib(n)->right == NULL || isBlack(sib(n)->right) == true) && (isBlack(sib(n)->left) == false)) {
      if(sib(n)->left != NULL) {
	sib(n)->left->color = 1;
      }
      sib(n)->color = 0;
      rotate_right(root, sib(n));
    }
    
    //outside child of n sibling has to be red now

    //finish balancing tree
    sib(n)->color = parent(n)->color;
    parent(n)->color = 1;
    if(side == 2) {
      sib(n)->left->color = 1;
      rotate_right(root, parent(n));
    }
    else {
      if(sib(n)->right != NULL) {
	sib(n)->right->color = 1;
      }
      rotate_left(root, parent(n));
    }
  }
}

//returns the tree after removing
node* remove(node* &root, int removing) {
  // cout << "testing" << endl;
  bool found = false;
  node* deleting = find(root, removing);
  
    
  if(deleting != NULL) {
    //If there are two children for the node being deleted
    if(deleting->right != NULL && deleting->left != NULL) {
      //Find the next smallest number
      node* replacement = deleting->left;
      while(replacement->right != NULL) {
	replacement = replacement->right;
      }
      //replace the node
      deleting->value = replacement->value;
      deleting = replacement;
    }
    cout << "1" << endl;
    int side = 0;
    cout << "1.1" << endl;
    //find if deleting is left or right child (2 = left, 1 = right)
    if(parent(deleting) != NULL) {
      cout << "1.2" << endl;
      if(deleting == parent(deleting) -> right) {
	cout << "1.3" << endl;
	side = 2;
	cout << "1.4" << endl;
      }
      else {
	cout << "1.5" << endl;
	side = 1;
	cout << "2" << endl;
      }
    }
    cout << "1.6" << endl;
    //if deleting is red, it can be deleted
    if(isBlack(deleting) == false) {
      cout << "1.7" << endl;
      if(side == 2) {
	cout << "1.7" << endl;
	parent(deleting) -> right = NULL;
	cout << "1.8" << endl;
      }
      else if(side == 1) {
	cout << "1.9" << endl;
	parent(deleting) -> left = NULL;
      }
      cout << "3" << endl;
    }
    // cout << "5" << endl;
    //deleting is black with two kids
    else if(deleting -> right != NULL || deleting -> left != NULL) {
      cout << "1.11" << endl;
      //child is whichever child of deleting isn't null
      node* child = new node;
      if(deleting -> right != NULL) {
	child = deleting->right;
	cout << "4" << endl;
      }
      else {
	child = deleting->left;
      }
      cout << "5" << endl;
      child->color = 1;
      deleting = replace_node(deleting, child);
      cout << "6" << endl;
    }
    //cout << "1.12" << endl;
    //deleting is black with no kids
    else {
      removal(root, deleting, side);
    }
    cout << "7" << endl;

    //If its the root then special case
    if(parent(deleting) == NULL) {
      deleting->value = 0;
      // cout << "this" << endl;
    }
    //if its not you can delete it
    else {
      //cout << "that" << endl;
      if(side == 2 && parent(deleting) != NULL) {
	parent(deleting)->right = NULL;
      }
      else if(side == 1 && parent(deleting) != NULL) {
	parent(deleting)->left = NULL;
      }
      delete deleting;
    }
  }
  else {
    cout << "That number wasn't in the tree" << endl;
  }
  //cout << "t" << endl;
  // while(parent(root) != NULL) {
  // root = parent(root);
  // cout << "w" << endl;
  // }
  //cout << "3" << endl;
  return root;
}
