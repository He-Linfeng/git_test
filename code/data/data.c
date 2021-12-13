#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
Dhead *data_create()
{
    int flag = 1;
    Dhead *head = (void *)malloc(sizeof(Dhead));
    head->next=NULL;
    printf("create the data list\n");
    do
    {
        data_add(head);
        printf("continue ? 1-yes,0-no.\n");
        scanf("%d",&flag);
    } while (flag == 1);

    return head;
}

void data_print(Dhead *head)
{
    Data *p;
    if (head->sum == 0)
    {
        printf("the data list is empty，please press any key to exit!\n");
        getchar();
        return;
    }

    p = head->next;
    printf("the data list:\n");
    do
    {
        printf("data %d: %s\n", p->num, p->name);
        p = p->next;
    } while (p != NULL);
}

void data_add(Dhead *head)
{
    Data *p = (void *)malloc(sizeof(Data));
    printf("input the data's num:");
    scanf("%d", &(p->num));
    printf("input the data's mame:");
    scanf("%s", p->name);
    p->next = head->next;
    head->next = p;
}

void data_remove(Dhead *head)
{
}

void data_find(Dhead *head,int num)
{
    Data *p;
    if (head->sum == 0)
    {
        printf("the data list is empty，please press any key to exit!\n");
        getchar();
        return;
    }
    p = head->next;
    printf("the data is:\n");
    do
    {
        if (p->num == num)
        {
            printf("data %d: %s\n", p->num, p->name);
            return;
        }
        p = p->next;
    } while (p != NULL);

    printf("no find\n");
}

void data_modfiy(Dhead *head)
{
}

void data_read(Dhead *head, char *path)
{
}

void data_write(Dhead *head, char *path)
{
}