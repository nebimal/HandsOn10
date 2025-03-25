#include <stdio.h>
#include <stdlib.h>

typedef struct Node 
{
    int key, value;
    struct Node* next;
    struct Node* prev;
} Node;

typedef struct 
{
    Node* head;
    Node* tail;
} DoublyLinkedList;

typedef struct 
{
    DoublyLinkedList** table;
    int capacity;
    int size;
    const double A;
} HashTable;

void resize(HashTable* T, int newCapacity);

void initList(DoublyLinkedList* list) 
{
    list->head = list->tail = NULL;
}

void initHashTable(HashTable* T, int cap) 
{
    T->capacity = cap;
    T->size = 0;
    T->table = (DoublyLinkedList**)malloc(cap * sizeof(DoublyLinkedList*));
    for (int i = 0; i < cap; ++i) 
    {
        T->table[i] = (DoublyLinkedList*)malloc(sizeof(DoublyLinkedList));
        initList(T->table[i]);
    }
}

int hashFunction(HashTable* T, int key) 
{
    return (int)(T->capacity * (key * T->A - (int)(key * T->A))) % T->capacity;
}

void Hash_Insert(HashTable* T, int key, int value) 
{
    if (T->size >= T->capacity)
    { 
        resize(T, T->capacity * 2);
    }
    int index = hashFunction(T, key);
    
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    newNode->value = value;
    newNode->next = NULL;
    newNode->prev = T->table[index]->tail;
    
    if (T->table[index]->tail)
    { 
        T->table[index]->tail->next = newNode;
    }
    T->table[index]->tail = newNode;
    
    if (!T->table[index]->head)
    { 
        T->table[index]->head = newNode;
    }
    T->size++;
}


void Hash_Remove(HashTable* T, int key)
{
    int index = hashFunction(T, key);
    Node* temp = T->table[index]->head;
    while (temp) 
    {
        if (temp->key == key)
        {
            if (temp->prev)
            { 
                temp->prev->next = temp->next;
            }
            if (temp->next) 
            {
                temp->next->prev = temp->prev;
            }
            if (temp == T->table[index]->head) 
            {
                T->table[index]->head = temp->next;
            }
            if (temp == T->table[index]->tail) 
            {
                T->table[index]->tail = temp->prev;
            }
            
            free(temp); 
            T->size--;  

            if (T->size <= T->capacity / 4 && T->capacity > 8) 
                resize(T, T->capacity / 2);
            return;
        }
        temp = temp->next;
    }
}

void resize(HashTable* T, int newCapacity) 
{
    DoublyLinkedList** oldTable = T->table;
    int oldCapacity = T->capacity;
    
    T->capacity = newCapacity;
    T->table = (DoublyLinkedList**)malloc(newCapacity * sizeof(DoublyLinkedList*));
    for (int i = 0; i < newCapacity; ++i) 
    {
        T->table[i] = (DoublyLinkedList*)malloc(sizeof(DoublyLinkedList));
        initList(T->table[i]);
    }
    
    for (int i = 0; i < oldCapacity; ++i) 
    {
        Node* temp = oldTable[i]->head;
        while (temp) 
        {
            Hash_Insert(T, temp->key, temp->value);
            temp = temp->next;
        }
        free(oldTable[i]);
    }
    free(oldTable);
}

int Hash_Search(HashTable* T, int key)
{
    int index = hashFunction(T, key);
    Node* temp = T->table[index] -> head;
    while (temp)
    {
        if(temp->key == key)
        {
            return temp->value;
        }
    temp = temp->next;
    }
    return -1;
}


void display(HashTable* T) 
{
    for (int i = 0; i < T->capacity; ++i) {
        printf("Bucket %d: ", i);
        Node* temp = T->table[i]->head;
        while (temp) {
            printf("(%d, %d) -> ", temp->key, temp->value);
            temp = temp->next;
        }
        printf("NULL\n");
    }
}

int main() 
{
    HashTable T;
    initHashTable(&T, 8);
    
    int choice, key, value;
    
    while (1) 
    {
        printf("\nHash Table Operations:\n");
        printf("1. Insert\n");
        printf("2. Remove\n");
        printf("3. Search\n");
        printf("4. Display\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                printf("Enter key: ");
                scanf("%d", &key);
                printf("Enter value: ");
                scanf("%d", &value);
                Hash_Insert(&T, key, value);
                break;
                
            case 2:
                printf("Enter key to remove: ");
                scanf("%d", &key);
                Hash_Remove(&T, key);
                break;
                
            case 3:
                printf("Enter key to search: ");
                scanf("%d", &key);
                value = Hash_Search(&T, key);
                if (value != -1)
                {
                    printf("Value for key %d: %d\n", key, value);
                }    
                else
                {
                    printf("Key %d not found.\n", key);
                }
                break;
                
            case 4:
                display(&T);
                break;
                
            case 5:
                printf("Exiting...\n");
                exit(0);
                
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    
    return 0;
}
