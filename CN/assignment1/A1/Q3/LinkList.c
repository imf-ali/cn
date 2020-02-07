#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct node
{
	int data;
	struct node* next;
	struct node* prev;
};

struct node* head = NULL;
struct node* tail = NULL;

struct node* getNewNode(int x) {
	struct node* newNode = (struct node*)malloc(sizeof(struct node));
	newNode->data = x;
	newNode->next = NULL;
	newNode->prev = NULL;
	return newNode;
}

void insert(int x) {
	struct node* temp = NULL;
    temp = getNewNode(x);
	if (head == NULL)
	{
		head = temp;
		tail = temp;
	}
	else
	{
		tail->next = temp;
		temp->prev = tail;
		tail = tail->next;
	}
}

void searchElement(int x) {
	struct node* temp = NULL;
	temp = head;
	while(temp != NULL) {
		if (temp->data == x)
		{
			printf("%d is present\n",x);
			break;
		}
		temp = temp->next;
	}
	if (temp == NULL)
	{
		printf("%d is not present\n",x);
	}
}

void deleteElement(int x) {
	struct node* temp = NULL;
	temp = head;
	while(temp != NULL) {
		if (temp->data == x)
		{
			if (temp == head)
			{
				head = head->next;
				head->prev = NULL;
				free(temp);
			}
			else if (temp == tail)
			{
				tail = tail->prev;
				tail->next = NULL;
				free(temp);
			}
			else
			{
				(temp->prev)->next = temp->next;
				(temp->next)->prev = temp->prev;
				free(temp);
			}
		}
		temp = temp->next;
	}

}

void printList() {
	struct node* temp = NULL;
	temp = head;
	while(temp != NULL) {
		printf("%d ",temp->data);
		temp = temp->next;
	}
	printf("\n");
}

void printreverse() {
	struct node* temp = NULL;
	temp = tail;
	while(temp != NULL) {
		printf("%d ",temp->data);
		temp = temp->prev;
	}
	printf("\n");
}

int main()
{
	insert(5);
	insert(15);
	insert(25);
	insert(35);
    printList();
	searchElement(15);
	deleteElement(15);
	printList();
	printreverse();


	return 0;
}
