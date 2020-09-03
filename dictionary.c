#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct avlnode* position;
typedef struct avlnode* avltree;

// declaring avl tree
struct avlnode{
	avltree l;
	avltree r;
	int ht;
	char eng[100],meaning[100];
};

//function to return height
static int ht(position p){
	if(p == NULL)
		return -1;
	else
		return p->ht;
}

// function to return the maximum of two numbers
int max(int a,int b){
	if(a>b)
		return a;
	else
		return b;
}

// function for single rotate with left
position Rl(position k2){
	position k1;
	k1 = k2->l;
	k2->l = k1->r;
	k1->r = k2;
	k2->ht = max(ht(k2->l),ht(k2->r))+1;
	k1->ht = max(ht(k1->l),k2-> ht)+1;
	return k1;
}

// function for single rotate with right
position Rr(position k2){
	position k1;
	k1 = k2->r;
	k2->r = k1->l;
	k1->l = k2;
	k2->ht = max(ht(k2->l),ht(k2->r))+1;
	k1->ht = max(ht(k1->r),k2-> ht)+1;
	return k1;
}

// function for double rotate with left
position RRl(position k3){
	k3->l = Rr(k3->l);
	k3 = Rl(k3);
	return k3;
}

//function for double rotate with right
position RRr(position k3){
	k3->r = Rl(k3->r);
	k3 = Rr(k3);
	return k3;
}

// function to insert into avl tree
avltree insert(avltree t, char eng[], char meaning[]){
	if(t==NULL){
		t = (avltree) malloc(sizeof(struct avlnode));
		strcpy(t->eng,eng);
		strcpy(t->meaning,meaning);
		t->ht = 0;
		t->l = t->r = NULL;
	}
	else if(strcasecmp(eng,t->eng)<0){
		t->l = insert(t->l,eng,meaning);
		if(ht(t->l)-ht(t->r)==2){
			if(strcasecmp(eng,t->l->eng)<0)
				t = Rl(t);
			else
				t = RRl(t);
		}
	}
	else if(strcasecmp(eng,t->eng)>0){
		t->r = insert(t->r,eng,meaning);
		if(ht(t->r)-ht(t->l)==2){
			if(strcasecmp(eng,t->r->eng)>0)
				t = Rr(t);
			else
				t = RRr(t);
		}
	}
	t->ht = max(ht(t->l),ht(t->r)) + 1;
	return t;
}

// function to balanceavl tree
int getBalance(avltree N){
    if (N == NULL)
        return 0;
    return ht(N->l) - ht(N->r);
}

// function that returns minimum value
avltree minValueNode(avltree node){
    avltree curr = node;

    while (curr->l != NULL)
        curr = curr->l;

    return curr;
}

// function to delete node
avltree deleteNode(avltree root, char key[]){
    if (root == NULL)
        return root;

    if (strcasecmp(key,root->eng)<0)
        root->l = deleteNode(root->l, key);

    else if(strcasecmp(key,root->eng)>0)
        root->r = deleteNode(root->r, key);

    else{
        if( (root->l == NULL) || (root->r == NULL) ) {
            avltree temp = root->l ? root->l : root->r;
						if (temp == NULL){
                temp = root;
                root = NULL;
            }
            else
             *root = *temp;
            free(temp);
        }

        else{
            avltree temp = minValueNode(root->r);

            strcpy(root->eng,temp->eng);
            strcpy(root->meaning,temp->meaning);

            root->r = deleteNode(root->r, temp->eng);
        }
    }

    if (root == NULL)
      return root;

    root->ht = 1 + max(ht(root->l),ht(root->r));
    int balance = getBalance(root);
    if (balance > 1 && getBalance(root->l) >= 0)
        return Rr(root);
		if (balance > 1 && getBalance(root->l) < 0){
        root->l =  Rl(root->l);
        return Rr(root);
    }
		if (balance < -1 && getBalance(root->r) <= 0)
        return Rl(root);
		if (balance < -1 && getBalance(root->r) > 0){
        root->r = Rr(root->r);
        return Rl(root);
    }
		return root;
}

// function to find word
position find(avltree a,char word[]){
	if(a==NULL)
		return NULL;
	else if(strcasecmp(word,a->eng)<0)
		return find(a->l,word);
	else if(strcasecmp(word,a->eng)>0)
		return find(a->r,word);
	else
		return a;
}
// function to print inorder
void inorder(avltree t,char op){
	if(t!=NULL){
		inorder(t->l,op);
		if(t->eng[0] == op)
			printf("%s : %s\n",t->eng,t->meaning);
		inorder(t->r,op);
	}
}
// function to find meaning
int findMeaning(avltree t, char s[]){
	position p = find(t,s);

	if(p!=NULL){
		printf("%s : %s\n",s,p->meaning);
		return 0;
	}
	else
		printf( "Sorry %s is not in the dictionary...\n",s);
	return -1;
}
// function to add element to avl tree
avltree add(avltree t, char word[]){
	position f = find(t,word);

	if(f==NULL){
		printf("\nEnter the meaning of the given word : ");
		char meaning[100];
		scanf(" %[^\n]",meaning);
		t = insert(t,word,meaning);
		printf("\nThe word was successfully added.\n");
	}
	else{
		printf("\nSorry the word already exists...\n");
	}
	return t;
}

// function to search in avl tree
avltree search(avltree t){
	char word[100];
	printf("Enter the word to be searched for : ");
	scanf("%s",word);
	int ch = findMeaning(t,word);
	if(ch == -1){
		printf("\n\nDo you want to add the word to the dictionary? (Yes-1/No-0)\n");
		scanf("%d",&ch);
		if(ch == 1)
			t = add(t,word);
	}
	return t;
}

// view avl tree
void view(avltree t){
	char op;
	printf("Enter the starting alphabet : ");
	scanf(" %c",&op);
	system("clear");
	inorder(t,op);
}

void viewall(avltree t){
	getchar();
	for(char op = 'a'; op <='z'; op++){
		system("clear");
		inorder(t,op);
		printf("\nthe words starting with %c are over",op);
		printf("\n\nPRESS ENTER TO CONTINUE...");
		getchar();
	}
}

avltree createdict(avltree t){
	FILE *fp;
	fp = fopen("dict.txt","r");
	char word[100],meaning[100];

	while(fscanf(fp,"%s",word) != EOF){
		fscanf(fp," %[^\n]",meaning);
		t = insert(t,word,meaning);
	}

	fclose(fp);
	return t;
}

void writeout(avltree t,FILE *fp){
	if(t!=NULL){
		writeout(t->l,fp);
		fprintf(fp,"%s %s\n",t->eng,t->meaning);
		writeout(t->r,fp);
	}
}

int main(void){
	avltree tree = NULL;
	tree = createdict(tree);
	int op = 1;
	while(op != 0){
		printf("\nMENU:");
		printf("\n1. Add a word\n2. Delete a word\n3. Search for a word\n");
		printf("4. Display words starting with a given alphabet\n5. Display all the words\n0. Exit the dictionary\n");
		printf("Choose an option : ");
		scanf("%d",&op);
		char word[100];
		printf("\n");
		switch(op){
			case 1 :
				printf("Enter the word to be added : ");
				scanf("%s",word);
				tree = add(tree,word);
			  break;
			case 2 :
				printf("Enter the word to be deleted : ");
				scanf("%s",word);
				position f = find(tree,word);
				if(f!=NULL){
					tree = deleteNode(tree,word);
					printf("\nThe word was successfully deleted.\n");
				}
				else{
					printf("Sorry %s is not in the dictionary...\n",word);
				}
			  break;
			case 3 :
				 	tree = search(tree);
					break;
			case 4 :
				view(tree);
			  break;
			case 5 :
				viewall(tree);
				break;
			case 0 :
			  break;
			default : printf("INCORRECT OPTION... CHOOSE AGAIN : ");
		}
		if(op !=5 ){
			printf("PRESS ENTER TO CONTINUE...");
			getchar();
			getchar();
		}
	}
	FILE *fp;
	fp = fopen("dict.txt","w");
	writeout(tree,fp);
	printf("\nTHE DICTIONARY IS SAVED...\n");
	printf("Thank you...");
	fclose(fp);
}
