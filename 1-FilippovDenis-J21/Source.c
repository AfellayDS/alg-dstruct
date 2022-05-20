#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable:4996)

#define TRUE 1
#define FALSE 0

#define FREE 2
#define NOTFREE 4
#define DELETED 3
#define SIZE 546378

typedef struct {
    char* str;
    int status;
} Hashtable;

Hashtable* Create()
{
    Hashtable* myHash;
    myHash = (Hashtable*)malloc(sizeof(Hashtable) * SIZE);
    if (myHash == NULL)
        return NULL;

    for (int i = 0; i < SIZE; i++)
        myHash[i].status = DELETED;

    return myHash;

}

static int hash(char* str)
{
    int index = 0;
    for (int i = 0; str[i] != 0; i++)
        index += str[i];

    return index % SIZE;

}

int Add(Hashtable* myHash, char* str) {
    int pos0 = hash(str);
    int pos = pos0, respos = -1, i = 0;
    do {
        if (myHash[pos].status == NOTFREE && strcmp(myHash[pos].str, str) == 0)
            return FALSE;

        if (myHash[pos].status == FREE || myHash[pos].status == DELETED) {
            respos = pos;
            break;
        }
        i++;
        pos = (pos0 + i * 1) % SIZE;
    } while (pos != pos0);
    if (respos != -1) {
        myHash[respos].str = malloc((strlen(str) + 1) * sizeof(char));
        strcpy(myHash[respos].str, str);
        myHash[respos].status = NOTFREE;
        return TRUE;
    }
    return FALSE;
}

int Find(Hashtable* myHash, char* str) {
    int pos0 = hash(str), pos = pos0, i = 0;
    do {
        if (myHash[pos].status == FREE) {
            return FALSE;
        }
        if (myHash[pos].status == NOTFREE && strcmp(myHash[pos].str, str) == 0)
            return TRUE;
        i++;
        pos = (pos0 + i * 1) % SIZE;
    } while (pos != pos0);
    return FALSE;
}

int Remove(Hashtable* myHash, char* str) {
    int p0 = hash(str), pos = p0, i = 0;
    do {
        if (myHash[pos].status == FREE) {
            return FALSE;
        }
        if (myHash[pos].status == NOTFREE && strcmp(myHash[pos].str, str) == 0) {
            free(myHash[pos].str);
            myHash[pos].status = DELETED;
            return TRUE;
        }
        i++;
        pos = (p0 + i * 1) % SIZE;
    } while (pos != p0);
    return FALSE;
}

void Delete(Hashtable* myHash) {

    for (int i = 0; i < SIZE; i++)
        if (myHash[i].status == NOTFREE)
            free(myHash[i].str);
    free(myHash);

}

int main()
{
    Hashtable* table = Create();
    int flag;
    char command;
    char value[10000];
    while (scanf("%c", &command) > 0)
    {
        if (command == 'a')
        {
            flag = scanf("%s", &value);
            Add(table, value);
        }

        if (command == 'f')
        {
            flag = scanf("%s", &value);
            if (Find(table, value))
                printf("%s", "yes\n");
            else
                printf("%s", "no\n");
        }

        if (command == 'r')
        {
            flag = scanf("%s", &value);
            Remove(table, value);
        }
    }
    Delete(table);

    return 0;
}