#ifndef _DATA_H_
#define _DATA_H_

typedef struct data
{
    int num;
    char name[20];
    struct data *next;
} Data;

typedef struct head
{
    int sum;
    struct data *next;
}Dhead;

Dhead* data_create();
void data_print(Dhead *head);
void data_add(Dhead *head);
void data_remove(Dhead *head);
void data_find(Dhead *head);
void data_modfiy(Dhead *head);
void data_read(Dhead *head, char *path);
void data_write(Dhead *head, char *path);

#endif