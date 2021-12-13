#include "data.h"
#include <stdio.h>
int main()
{
    Dhead *head;
    head = data_create();
    data_print(head);
    return 0;
} 