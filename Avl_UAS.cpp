#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct tnode{
	int key;
	int height;
	char name[50];
	struct tnode *left, *right;
};

struct tnode *newnode(int x, char name[]){
	struct tnode *curr = (struct tnode*)malloc(sizeof(struct tnode));
	
	curr->key = x;
	strcpy(curr->name, name);
	curr->height = 1;
	curr->left = NULL;
	curr->right = NULL;
	
	return curr;
}

int max(int left, int right){
	if(left > right){
		return left;
	} else{
		return right;
	}
}

int getHeight(struct tnode *root){
	if(root == NULL){
		return 0;
	} else{
		return root->height;
	}
}

int getBF(struct tnode *root){
	if(root == NULL){
		return 0;
	} else{
		return getHeight(root->left)-getHeight(root->right);
	}
}

struct tnode *rightRotate(struct tnode *T){
	/*
		T
	S
		B
	*/
	struct tnode *S = T->left;
	struct tnode *B = S->right;
	
	//Rotate
	S->right = T;
	T->left = B;
	
	T->height = max(getHeight(T->left), getHeight(T->right))+1;
	S->height = max(getHeight(S->left), getHeight(S->right))+1;
	
	return S;
}

struct tnode *leftRotate(struct tnode *T){
	/*
	T
		S
	B
	*/
	struct tnode *S = T->right;
	struct tnode *B = S->left;
	
	//Rotate
	S->left = T;
	T->right = B;
	
	T->height = max(getHeight(T->left), getHeight(T->right))+1;
	S->height = max(getHeight(S->left), getHeight(S->right))+1;
	
	return S;
}

struct tnode *insert(struct tnode *root, int key, char name[]){
	if(root == NULL){
		return newnode(key, name);
	} else if(key < root->key){
		root->left = insert(root->left, key, name);
	} else{
		root->right = insert(root->right, key, name);
	}
	
	root->height = max(getHeight(root->left), getHeight(root->right))+1;
	int bFactor = getBF(root);
	
	if(bFactor > 1 && key < root->left->key){
		return rightRotate(root);
	}
	
	if(bFactor < -1 && key > root->right->key){
		return leftRotate(root);
	}
	
	if(bFactor > 1 && key > root->left->key){
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}
	
	if(bFactor < -1 && key < root->right->key){
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}
	
	return root;
}

struct tnode *predecessor(struct tnode *root){
	struct tnode *curr = root->left;
	
	while(curr->right != NULL){
		curr = curr->right;
	}
	
	return curr;
}

struct tnode *del(struct tnode *root, int key, char name[]){
	if(root == NULL){
		return NULL;
	} else if(key < root->key){
		root->left = del(root->left, key, name);
	} else if(key > root->key){
		root->right = del(root->right, key, name);
	} else{
		//1 or none
		if(root->left == NULL || root->right == NULL){
			struct tnode *temp;
			
			if(root->left != NULL){
				temp = root->left;
			} else{
				temp = root->right;
			}
			
			if(temp == NULL){
				temp = root;
				root = NULL;
			} else{
				*root = *temp;
			}
			
			free(temp);
			
			//2 node
		} else{
			struct tnode *temp = predecessor(root);
			
			root->key = temp->key;
			strcpy(root->name, temp->name);
			
			root->left = del(root->left, temp->key, temp->name);
		}
	}
	
	if(root == NULL){
		return root;
	}
	
	root->height = max(getHeight(root->left), getHeight(root->right))+1;
	int bFactor = getBF(root);
	
	if(bFactor > 1 && getBF(root->left) >= 0){
		return rightRotate(root);
	}
	
	if(bFactor < -1 && getBF(root->right) <= 0){
		return leftRotate(root);
	}
	
	if(bFactor > 1 && getBF(root->left) < 0){
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}
	
	if(bFactor < -1 && getBF(root->right) > 0){
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}
	
	return root;
}

int tempNum = -1;
void findName(struct tnode *root, char name[]){
	if(root == NULL){
		return;
	}
	
	if(strcmp(root->name, name) == 0){
		tempNum = root->key;
	}
	findName(root->left, name);
	findName(root->right, name);
}

void preorder(struct tnode *root){
	if(root == NULL){
		return;
	}
	
	printf("%d(%s) ", root->key, root->name);
	preorder(root->left);
	preorder(root->right);
}

int coLeader = 1;
void preorderHierarchy(struct tnode *root, int maxHeight){
	if(root == NULL){
		return;
	}
	
	//Conditions
	if(maxHeight - root->height == 0){
		printf("Clan Leader: %s | %d years\n", root->name, root->key);
	} else if(root->height == 1){
		printf("Member: %s | %d years\n", root->name, root->key);
	} else if(maxHeight - root->height == 1){
		if(coLeader == 1){
			printf("[West] Co-Leader: %s | %d years\n", root->name, root->key);
		} else{
			printf("[East] Co-Leader: %s | %d years\n", root->name, root->key);
		}
		coLeader++;
	} else{
		printf("Elder: %s | %d years\n", root->name, root->key);
	}
	
	preorderHierarchy(root->left, maxHeight);
	preorderHierarchy(root->right, maxHeight);
}

int main(){
	struct tnode *root = NULL;
	
	printf("================INSERT================\n");
	int t;
	printf("Test Cases: ");
	scanf("%d", &t);
	
	int num;
	char name[50];
	for(int i=0; i<t; i++){
		printf("[%d] Key_Name: ", i+1);
		scanf("%d %s", &num, name);
		
		root = insert(root, num, name);
	}
	preorder(root); printf("\n");
	
	printf("================DELETE================\n");
	int n;
	printf("Test Cases: ");
	scanf("%d", &n);
	
	for(int i=0; i<n; i++){
		tempNum = -1;
		printf("[%d]Name to Delete: ", i+1);
		scanf("%s", name);
		
		findName(root, name);
		root = del(root, tempNum, name);
	}
	preorder(root); printf("\n");
	
	printf("================HIERARCHY================\n");
	int maxHeight = root->height;
	
	preorderHierarchy(root, maxHeight);
}

/*
11
4 john
5 tree
6 klee
2 bomb
1 james
10 prep
16 rover
3 kaguya
30 oji
7 lylia
15 kem
2
rover
oji

16 30
*/
