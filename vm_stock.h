/******************************************************************************
** Student name: Kevin Wong   ...
** Student number: s3603055 ...
** Course:          Advanced Programming Techniques - S1 2018
******************************************************************************/

#ifndef VM_STOCK_H
#define VM_STOCK_H

#include "vm_coin.h"

/**
 * The default stock level that all new stock should start with and the value
 * to use when restting stock.
 **/
#define DEFAULT_STOCK_LEVEL 20

#define STOCK_DELIM "|"


/* creates a new link node */
Node * createLinkNode();

/* returns pointer to the list */
List * createLinkList();

/* frees any dynamic memory for the list */
void freeMemList(List * list);

/* returns pointer to the item's ID */
Node * getItemId(List * list, const char * id);

/* shows every item in the list */
void showItemList(List * list);

/* add a new item to the link list */
Boolean linkListAdd(List * list, Stock * item); 

/* remove the item by the item's ID */
Boolean linkListRemove(List * list, const char * id);



#endif
