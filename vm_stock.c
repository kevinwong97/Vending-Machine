/******************************************************************************
** Student name: Kevin Wong   ...
** Student number: s3603055 ...
** Course:          Advanced Programming Techniques - S1 2018
******************************************************************************/

#include "vm_stock.h"

/**
 * vm_stock.c this is the file where you will implement the
 * interface functions for managing the stock list.
 **/

/**
 * Some example functions:
 * create list, free list, create node, free node, insert node, etc...
 */


/* create a new node   */
Node * createLinkNode() 
{
    Node * node = malloc(sizeof(Node));
    node->data = NULL;
    node->next = NULL;
    return node;
}

/* create the list and return it used to link to other lists */
List * createLinkList() 
{
    List * list = malloc(sizeof(List));
    list->head = NULL;
    list->size = 0;
    return list;
}

/* frees dynamic memory for any linked list which contain nodes */
void freeMemList(List * list) 
{
    Node * current = list->head;
    Node * buffer;
    while (current != NULL)
     {
        buffer = current;
        current = current->next;
        free(buffer->data);
        free(buffer);
    }
    free(list);
}

/*returns null given the ID was not found or it will return the specific item ID */
Node * getItemId(List * list, const char * id) 
{
    Node * item;
    item = list->head;
    while (item != NULL && (strcmp(item->data->id,id) != 0)) 
    {
        item = item->next;
    }
    return item;
}

/* shows all the item ID, name, description, stock level and price */
void showItemList(List * list) 
{
    
    int buffer;
    int idRange = strlen("ID"), nameRange = strlen("Name"), stockRange = strlen("Available");
    Node * current = list->head;

    while(current != NULL) 
    {
        /* item ID */
        buffer = strlen(current->data->id);
        idRange = (idRange > buffer ? idRange : buffer);
        /* item Name */ 
        buffer = strlen(current->data->name);
        nameRange = (nameRange > buffer ? nameRange : buffer); 
        /* item stock */ 
        buffer = (int) countChar(current->data->onHand);
        stockRange = (stockRange > buffer ? stockRange : buffer);  
        current = current->next;
    } 
    current = list->head;

    printf("Items Menu\n\n");
    printf("%-*s | %-*s | %-*s | %s\n", idRange,"ID", nameRange,"Name", stockRange,"Available", "Price");
    printf("-----------------------------------------------\n");
    while (current != NULL) {
       printf("%-*s | %-*s | %-*u | $ %u.%02u\n", idRange,current->data->id, nameRange,current->data->name, 
               stockRange,current->data->onHand, current->data->price.dollars, current->data->price.cents);
               current = current->next;
       }
       printf("\n");
}

/* add a new item to the item List */
Boolean linkListAdd(List * list, Stock * item) 
{

    Node * current = list->head;
    Node * previous = NULL;
    Node * new = createLinkNode(); 

    switch(list->size) 
    {
        case 0:
            list->head = new;
            if (list->head == NULL) 
              return FALSE;
            list->head->data = item;
            list->head->next = NULL;/* list is null therefore emptied */
            break;
        case 1:
            if (strcmp(item->name, current->data->name) > 0) 
            {	
                current->next = new;/* add to the end of the list */
                new->data = item;
                new->next = NULL;
            } else {
                list->head = new;/* add at the start of the list */
                new->data = item;
                new->next = current;
            }
            break;
            
        default:
            while (current->next != NULL) /* add in between the start and end of list */
            {
                if (strcmp(item->name, current->data->name) < 0) 
                {
                    break;
                }
                previous = current;
                current = current->next;
            }
            previous->next = new;
            new->next = current;
            new->data = item;
            break;
    }
    list->size++;
    return TRUE;
}

/* remove a item by entering item ID */
Boolean linkListRemove(List * list, const char * id)
 { 

    Node * current = NULL;
    Node * previous = NULL;
    current = list->head;

    while (current != NULL && (strcmp(current->data->id,id) != 0))
    {
        previous = current;
        current = current->next;
    }

    if (current == NULL) 
        return FALSE;

    if (previous == NULL) 
    {
        list->head = current->next; 
    } 
    else 
    {
        previous->next = current->next;	
    }
    list->size--;
    free(current->data);
    free(current);
    return TRUE; 
} 



