#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int number;
    struct Node *next;
} Node;

Node *head;
Node *gNptr;
int *gptr;

void print_menu()
{

    printf("\nYou have following options::\n");
    printf("\t1. Add a node to the front of the list.\n");
    printf("\t2. Add a node to the back of the list.\n");
    printf("\t3. Add a node in the nth position of the list.\n");
    printf("\t4. Remove the front item on the list.\n");
    printf("\t5. Remove the last item from the list.\n");
    printf("\t6. Remove a number from the list.\n");
    printf("\t7. Print the list.\n");
    printf("\t8. Quit.\n");
    return;
}

void add_node(int number)
{
    Node *temp = (Node *)malloc(sizeof(Node));
    temp->number = number;
    temp->next = head;
    head = temp;
    return;
}

void add_node_last(int number)
{
    if (head == NULL)
    {
        add_node(number);
    }
    else
    {
        Node *temp = (Node *)malloc(sizeof(Node));
        temp->number = number;
        temp->next = NULL;
        Node *temp1 = head;
        while (temp1->next != NULL)
        {
            temp1 = temp1->next;
        }
        temp1->next = temp;
    }
}

void add_node_position(int number, int position)
{
    if (position == 1)
    {
        add_node(number);
    }
    else if (position == -1)
    {
        add_node_last(number);
    }
    else
    {
        Node *temp = (Node *)malloc(sizeof(Node));
        temp->number = number;
        temp->next = NULL;

        Node *temp1 = head;
        int i;
        for (i = 0; i < (position - 2); i++)
        {
            temp1 = temp1->next;
        }
        temp->next = temp1->next;
        temp1->next = temp;
    }
}

void remove_first()
{
    if (head == NULL)
    {
        printf("\nThe list is already empty. Cannot remove further.");
        return;
    }
    head = head->next;
}

void remove_last()
{
    if (head == NULL)
    {
        printf("\nThe list is already empty. Cannot remove further.");
        return;
    }
    Node *temp = head;
    if (temp->next == NULL)
    {
        head = NULL;
        return;
    }
    while (temp->next->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = NULL;
}

void remove_value(int number)
{
    if (head == NULL)
    {
        printf("\nThe list is empty");
        return;
    }
    Node *temp = head;
    while (temp->next != NULL)
    {
        if (temp->next->number == number)
        {
            temp->next = temp->next->next;
        }
        temp = temp->next;
        if (temp == NULL)
            break;
    }

    if (head != NULL && head->number == number)
    {
        remove_first();
    }
}
void print_list()
{
    if (head == NULL)
    {
        printf("\nThe list is empty");
        return;
    }
    Node *temp = head;
    while (temp != NULL)
    {
        printf("%d->", temp->number);
        temp = temp->next;
    }
    printf("\n");
}

// void check()
// {
//     int *ptr;
//     Node *Nptr;
//     if (ptr == NULL)
//     {
//         printf("Ponter is NULL");
//     }
//     if (Nptr == NULL)
//     {
//         printf("Node pointer is NULL");
//     }
//     if (gNptr == NULL)
//     {
//         printf("Global Node pointer is NULL");
//     }
//     if (gptr == NULL)
//     {
//         printf("\nGlobal integer pointer is NULL");
//     }
// }

void free_list()
{
    Node *temp = head;
    Node *next;

    while (temp != NULL)
    {
        next = temp->next;
        free(temp);
        temp = next;
    }

    head = NULL; // Reset the head to NULL after freeing all nodes
}

int main()
{
    int option;
    int number, position;
    do
    {
        print_menu();
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            printf("\nEnter the number you want to insert::");
            scanf("%d", &number);
            add_node(number);
            print_list();
            break;
        case 2:
            printf("\nEnter the number you want to insert to the back of the list::");
            scanf("%d", &number);
            add_node_last(number);
            print_list();
            break;
        case 3:
            printf("\nEnter the number followed by the position you want to add the number to::");
            scanf("%d %d", &number, &position);
            add_node_position(number, position);
            print_list();
            break;
        case 4:
            remove_first();
            print_list();
            break;
        case 5:
            remove_last();
            print_list();
            break;
        case 6:
            printf("\nEnter the number you want to remove from the list::");
            scanf("%d", &number);
            remove_value(number);
            print_list();
            break;
        case 7:
            print_list();
            break;
        case 8:
            printf("\nExiting the program.");
            break;
        default:
            printf("\nINVALID OPTION\n");
            break;
        }
    } while (option != 8);
    free_list();
    return 0;
}