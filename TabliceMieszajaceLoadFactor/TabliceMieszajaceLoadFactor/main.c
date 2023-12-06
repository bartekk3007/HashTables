#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct ListNode
{
	int val;
	struct ListNode* next;
};

int hashFunction(int size, int n);

void traverseTable(struct ListNode** array, int size);

int countUsedMembers(struct ListNode** array, int size);

int countNode(struct ListNode** array, int size);

void freeNodes(struct ListNode** array, int* size);

void insertNode(struct ListNode** array, int* size, int n);

void shrinkArray(struct ListNode*** array, int* size);

void expandArray(struct ListNode*** array, int* size);

void expandArray(struct ListNode*** array, int* size)
{
	int bigSize = *size * 2;
	struct ListNode** array2 = (struct ListNode**) malloc(sizeof(struct ListNode*) * bigSize);

	for (int i = 0; i < bigSize; i++)
	{
		array2[i] = NULL;
	}

	for (int i = 0; i < *size; i++)
	{
		struct ListNode* temp = (*array)[i];
		while (temp != NULL)
		{
			insertNode(&array2, &bigSize, temp->val);
			temp = temp->next;
		}
	}

	freeNodes(*array, size);

	free(*array);
	*array = array2;
	*size = bigSize;
}

bool searchBool(struct ListNode** array, int n);

bool searchBool(struct ListNode** array, int n)
{
	if ((*array) == NULL)
	{
		return false;
	}
	else if ((*array)->val == n)
	{
		return true;
	}
	else
	{
		searchBool(&(*array)->next, n);
	}
}

bool searchNodeBool(struct ListNode** array, int* size, int n);

bool searchNodeBool(struct ListNode** array, int* size, int n)
{
	int index = hashFunction(*size, n);
	return searchBool(&array[index], n);
}

void insert(struct ListNode** array, int n);

void insert(struct ListNode** array, int n)
{
	struct ListNode* vertex = (struct ListNode*)malloc(sizeof(struct ListNode));
	if (vertex)
	{
		vertex->val = n;
		vertex->next = NULL;
		if (*array == NULL)
		{
			*array = vertex;
		}
		else
		{
			insert(&(*array)->next, n);
		}
	}
}

void insertNode(struct ListNode*** array, int *size, int n)
{
	float ratio = ((float)countNode((*array), *size) + 1 ) / (float)*size;
	if (ratio > 0.75)
	{
		expandArray(array, size);
	}
	int index = hashFunction(*size, n);
	insert(&(*array)[index], n);
}

void delete(struct ListNode** array, int n);

void delete(struct ListNode** array, int n)
{
	if (*array == NULL)
	{
		return;
	}
	else if ((*array)->val == n)
	{
		struct ListNode* temp = (*array)->next;
		free(*array);
		*array = temp;
	}
	else
	{
		delete(&(*array)->next, n);
	}
}

void deleteNode(struct ListNode*** array, int* size, int n);

void deleteNode(struct ListNode*** array, int *size, int n)
{
	if (searchNodeBool(*array, size, n))
	{
		float ratio = ((float)countNode((*array), *size) - 1) / (float)*size;
		if (ratio < 0.25)
		{
			shrinkArray(array, size);
		}
		int index = hashFunction(*size, n);
		delete(&(*array)[index], n);
	}
}

void search(struct ListNode** array, int n);

void search(struct ListNode** array, int n)
{
	if ((*array) == NULL)
	{
		printf("Nie znaleziono wartosc %d w tabeli mieszajacej\n", n);
	}
	else if ((*array)->val == n)
	{
		printf("Znaleziono wartosc %d w tabeli mieszajacej\n", n);
	}
	else
	{
		search(&(*array)->next, n);
	}
}

void searchNode(struct ListNode** array, int* size, int n);

void searchNode(struct ListNode** array, int *size, int n)
{
	int index = hashFunction(*size, n);
	search(&array[index], n);
}

int main(void)
{
	int size = 20;
	struct ListNode** array = (struct ListNode**)malloc(sizeof(struct ListNode*) * size);

	if (array)
	{
		for (int i = 0; i < size; i++)
		{
			array[i] = NULL;
		}

		insertNode(&array, &size, 5);
		insertNode(&array, &size, 8);
		insertNode(&array, &size, 13);
		insertNode(&array, &size, 21);
		insertNode(&array, &size, 25);
		deleteNode(&array, &size, 21);

		printf("\n");
		traverseTable(array, size);
		printf("\n");
		printf("%d\n", countUsedMembers(array, size));
		printf("%d\n", countNode(array, size));
		printf("%d\n", array[5]->next->val);

		insertNode(&array, &size, 34);
		printf("%d\n", array[5]->next->val);
		insertNode(&array, &size, 55);
		insertNode(&array, &size, 89);
		insertNode(&array, &size, 2);
		insertNode(&array, &size, 3);
		insertNode(&array, &size, 11);
		insertNode(&array, &size, 17);
		insertNode(&array, &size, 19);
		insertNode(&array, &size, 23);
		insertNode(&array, &size, 29);
		printf("\n");
		traverseTable(array, size);
		printf("\n");
		printf("%d\n", countUsedMembers(array, size));
		printf("%d\n", countNode(array, size));
		insertNode(&array, &size, 31);

		printf("\n");
		traverseTable(array, size);
		printf("\n");
		printf("%d\n", countUsedMembers(array, size));
		printf("%d\n", countNode(array, size));

		deleteNode(&array, &size, 34);
		printf("%p\n", array[25]->next);

		searchNode(array, &size, 25);
		searchNode(array, &size, 34);

		freeNodes(array, &size);
		free(array);
	}

	return 0;
}

int hashFunction(int size, int n)
{
	return n * 13 % size;
}

void traverseTable(struct ListNode** array, int size)
{
	for (int i = 0; i < size; i++)
	{
		printf("%d: ", i);
		struct ListNode* temp = array[i];
		while (temp != NULL)
		{
			printf("%d ", temp->val);
			temp = temp->next;
		}
		printf("\n");
	}
}

int countUsedMembers(struct ListNode** array, int size)
{
	int count = 0;
	for (int i = 0; i < size; i++)
	{
		if (array[i] != NULL)
		{
			count++;
		}
	}
	return count;
}

int countNode(struct ListNode** array, int size)
{
	int count = 0;
	for (int i = 0; i < size; i++)
	{
		struct ListNode* temp = array[i];
		while (temp != NULL)
		{
			count++;
			temp = temp->next;
		}
	}
	return count;
}

void freeNodes(struct ListNode** array, int *size)
{
	for (int i = 0; i < *size; i++)
	{
		while (array[i] != NULL)
		{
			struct ListNode* temp = array[i]->next;
			free(array[i]);
			array[i] = temp;
		}
	}
}

void shrinkArray(struct ListNode*** array, int* size)
{
	int smallSize = *size / 2;
	struct ListNode** array2 = (struct ListNode**)malloc(sizeof(struct ListNode*) * smallSize);

	for (int i = 0; i < smallSize; i++)
	{
		array2[i] = NULL;
	}

	for (int i = 0; i < *size; i++)
	{
		struct ListNode* temp = (*array)[i];
		while (temp != NULL)
		{
			insertNode(&array2, &smallSize, temp->val);
			temp = temp->next;
		}
	}

	freeNodes(*array, size);

	free(*array);
	*array = array2;
	*size = smallSize;
}