/******************************************************************************
** Student name: Kevin Wong   ...
** Student number: s3603055 ...
** Course:          Advanced Programming Techniques - S1 2018
******************************************************************************/

#include "vm_options.h"

/**
 * vm_options.c this is where you need to implement the system handling
 * functions (e.g., init, free, load, save) and the main options for
 * your program. You may however have some of the actual work done
 * in functions defined elsewhere.
 **/

/**
 * Initialise the system to a known safe state. Look at the structure
 * defined in vm_system.h.
 **/
Boolean systemInit(VmSystem * system)
{
    system->itemList = createLinkList();
    initialiseCoinsArray(system->cashRegister);
    system->stockFileName = NULL;
    system->coinFileName = NULL;
    return TRUE;
}

/**
 * Free all memory that has been allocated. If you are struggling to
 * find all your memory leaks, compile your program with the -g flag
 * and run it through valgrind.
 **/
void systemFree(VmSystem * system)
{ 
    free(system);
}

/**
 * Loads the stock and coin data into the system. You will also need to assign
 * the char pointers to the stock and coin file names in the system here so
 * that the same files will be used for saving. A key part of this function is
 * validation. A substantial number of marks are allocated to this function.
 **/
Boolean loadData(
        VmSystem * system, const char * stockFileName, const char * coinsFileName)
{
    return (loadStock(system, stockFileName) && loadCoins(system, coinsFileName));
}

/**
 * Loads the stock file data into the system.
 **/
Boolean loadStock(VmSystem * system, const char * fileName)
{ 
    char line[LINE_SIZE];
    char coin[COIN_TOKEN_SIZE];
    char * tok;
    
    FILE * stockfile;
    Stock * newStockItem; 

    system->stockFileName = fileName;
    stockfile = fopen(fileName, "r");

    if (stockfile == NULL) {
        printf("Error, Stock file cannot be opened.\n");
        return FALSE;
    }

    while (fgets(line, LINE_SIZE, stockfile)) {
        newStockItem = malloc(sizeof(Stock));

        tok = strtok(line, STOCK_DELIM);
        strcpy(newStockItem->id, tok);
        tok = strtok(NULL, STOCK_DELIM);
        strcpy(newStockItem->name, tok);
        tok = strtok(NULL, STOCK_DELIM);
        strcpy(newStockItem->desc, tok);
        tok = strtok(NULL, STOCK_DELIM);
        strcpy(coin, tok);
        tok = strtok(NULL, STOCK_DELIM);

        newStockItem->onHand = (unsigned) strtol(tok, NULL, 10);
        newStockItem->price.dollars = (unsigned) strtol(strtok(coin, "."), NULL, 10);
        newStockItem->price.cents = (unsigned) strtol(strtok(NULL, "."), NULL, 10);

        if (!linkListAdd(system->itemList, newStockItem)) {
            return FALSE;
        }
    }
    fclose(stockfile);

    return TRUE;
}


/**
 * Loads the coin file data into the system.
 **/
Boolean loadCoins(VmSystem * system, const char * fileName)
{
    unsigned quantity;
    int denomination;
    char coin[COIN_TOKEN_SIZE];
    
    FILE * coinfile;
    system->coinFileName = fileName;
    coinfile = fopen(fileName, "r");

    if (coinfile == NULL) {
        printf("Error, Coins file cannot be opened.\n");
        return FALSE;
    }

    while (fgets(coin, COIN_TOKEN_SIZE, coinfile)) {
        denomination = (int) strtol(strtok(coin, COIN_DELIM), NULL, 10);
        quantity = (unsigned) strtol(strtok(NULL, COIN_DELIM), NULL, 10);

        denomination = coinDenomination(system->cashRegister, denomination);
        system->cashRegister[denomination].count += quantity;
    }
    fclose(coinfile);

    return TRUE;
}

/**
 * Saves all the stock back to the stock file.
 **/
Boolean saveStock(VmSystem * system)
{
    FILE * stockfile;
    Node * currentStock;
    
    stockfile = fopen(system->stockFileName, "w");
    currentStock = system->itemList->head;

    while (currentStock != NULL) {
        fprintf(stockfile, "%s%s%s%s%s%s%u.%02u%s%u\n", 
          currentStock->data->id, STOCK_DELIM,
          currentStock->data->name, STOCK_DELIM,
          currentStock->data->desc, STOCK_DELIM,
          currentStock->data->price.dollars,
          currentStock->data->price.cents, STOCK_DELIM,
          currentStock->data->onHand);
          currentStock = currentStock->next;
    }
    fclose(stockfile);

    return TRUE;
}

/**
 * Saves all the coins back to the coins file.
 **/
Boolean saveCoins(VmSystem * system)
{
    FILE * coinfile;
    int denomination;

    coinfile = fopen(system->coinFileName, "w");
    for (denomination = TEN_DOLLARS; denomination >= FIVE_CENTS; denomination--)
    {
        fprintf(coinfile, "%u,%u\n", valueOfCoin(denomination),system->cashRegister[denomination].count);
    }
    fclose(coinfile);

    return TRUE;
}

/**
 * This option allows the user to display the items in the system.
 * This is the data loaded into the linked list in the requirement 2.
 **/
void displayItems(VmSystem * system)
{
    showItemList(system->itemList);
}

/**
 * This option allows the user to purchase an item.
 * This function implements requirement 5 of the assignment specification.
 **/
void purchaseItem(VmSystem * system)
{ 
    int leftoverCost, denominationPutIn, coinPutIn;
    int coinsPutIn[NUM_DENOMS];
    int coinsTakeOut[NUM_DENOMS];
    char changeTemp[BUFFER_CHANGE_SIZE];
    char change[LINE_SIZE];
    char id[ID_LEN + EXTRA_SPACES];
    char * tempEnd;
    Node * item = NULL;

    printf("Purchase Item\n");
    printf("-------------\n");
    printf("Please enter the id of the item you wish to purchase: ");

    if (!getInputValidation(id, ID_LEN+EXTRA_SPACES)) 
        return;

    item = getItemId(system->itemList, id);

    if (item == NULL) {
        printf("Sorry, the requested item could not be found.\n");
        return;
    } 
    if (item->data->onHand == 0) {
        printf("Sorry, this item is out of stock.\n");
        return;
    }
    printf("You have selected \"%s - %s\". This will cost you $%u.%02u.\n", item->data->name, item->data->desc, item->data->price.dollars, item->data->price.cents);
    printf("Please hand over the money - type in the value of each note/coin in cents.\n");
    printf("Press enter on a new and empty line to cancel this purchase.\n");

    leftoverCost = item->data->price.dollars*100 + item->data->price.cents;
    /* stores coins going in and out to memory block */
    memset(coinsPutIn, 0, NUM_DENOMS * sizeof(int));
    memset(coinsTakeOut, 0, NUM_DENOMS * sizeof(int));
    while (leftoverCost > 0) {

        printf("You still need to give us $%d.%02d:", leftoverCost/100, leftoverCost%100);

        if (!getInputValidation(changeTemp, BUFFER_CHANGE_SIZE)) 
            return;
        coinPutIn = strtol(changeTemp, &tempEnd, 10);
        if (*tempEnd != '\0') {
            printf("Error, Please enter an amount in cents.\n");
            continue;
        }
        denominationPutIn = coinDenomination(system->cashRegister, coinPutIn);
        if (denominationPutIn != -1) {
            coinsPutIn[denominationPutIn]++;
            leftoverCost -= coinPutIn;
        } else {
            printf("Error: $%d.%02d is not a valid denomination of Money.\n", coinPutIn/100, coinPutIn%100);
        }
    }
    leftoverCost *= -1;

    insertCoins(system->cashRegister, coinsPutIn);

    
    if (totalChange(system->cashRegister, coinsTakeOut, leftoverCost)) {
        item->data->onHand--;
        coinString(change, coinsTakeOut);	
        printf("Thank you. Here is your %s, and your change of $%d.%02d:%s\n", item->data->name, leftoverCost/100, leftoverCost%100, change); 
        printf("Please come back soon.\n\n");
    } else {
        
        /* Remove the coins user puts in */
        deductCoins(system->cashRegister, coinsPutIn); 
        /* take out coins from register */
        insertCoins(system->cashRegister, coinsTakeOut);
        coinString(change, coinsPutIn);	
        printf("Sorry, the correct change cannot be provided. You will be given a refund:%s\n\n",
                change);
    }

}

/**
 * You must save all data to the data files that were provided on the command
 * line when the program loaded up, display goodbye and free the system.
 * This function implements requirement 6 of the assignment specification.
 **/
void saveAndExit(VmSystem * system)
{ 
    if (!saveStock(system)) {
        printf("Error, Stock data cannot be saved.\n");
    }
    if (!saveCoins(system)) {
        printf("Error, Coin data cannot be saved.\n");
    }
    freeMemList(system->itemList);
    systemFree(system);
}

/**
 * This option adds an item to the system. This function implements
 * requirement 7 of of assignment specification.
 **/
void addItem(VmSystem * system)
{ 
    Boolean validPrice;
    char newNumberID[ID_LEN  + NULL_SPACE];
    char coin[COIN_TOKEN_SIZE + NULL_SPACE];
    char * charpointer;
    unsigned numberID = 0;
    Stock * newStockItem;

    printf("Add Item\n");
    printf("-------------\n\n");

    newStockItem = malloc(sizeof(Stock));

    do {
        numberID++;
        sprintf(newNumberID, "I%04u", numberID);
    } while(getItemId(system->itemList, newNumberID) != NULL);

    printf("This new meal item will have the Item id of %s.\n", newNumberID);
    strcpy(newStockItem->id, newNumberID);
    printf("Enter the item name: ");
    if (!getInputValidation(newStockItem->name, NAME_LEN)) 
        return;
    printf("Enter the item description: ");
    if (!getInputValidation(newStockItem->desc, DESC_LEN)) 
        return;
    validPrice = FALSE;
    while (!validPrice) {
        printf("Enter the price for this item: ");
        if (!getInputValidation(coin, COIN_TOKEN_SIZE)) 
            return;

        newStockItem->price.dollars = (unsigned) strtol(strtok(coin, "."), &charpointer, 10);
        if (*charpointer != '\0') {
            printf("Error, Please enter a valid price.\n");
            continue;
        }
        newStockItem->price.cents = (unsigned) strtol(strtok(NULL, "."), &charpointer, 10);
        if (*charpointer != '\0') {
            printf("Error, Please enter a valid price.\n");
            continue;
        }

        if ((newStockItem->price.dollars*100 + newStockItem->price.cents)%5 != 0) {
            printf("Error, Please enter a valid price.\n");
            continue;
        }

        newStockItem->onHand = DEFAULT_STOCK_LEVEL;
        validPrice = TRUE;
    }

    if (!linkListAdd(system->itemList, newStockItem)) {
        printf("Error, The new item cannot be added.\n");
        return;
    }
    printf("This item \"%s - %s\" has now been added to the menu.\n\n", newStockItem->name, newStockItem->desc);
}

/**
 * Remove an item from the system, including free'ing its memory.
 * This function implements requirement 8 of the assignment specification.
 **/
void removeItem(VmSystem * system)
{ 
    char id[ID_LEN + NULL_SPACE];
    char name[NAME_LEN + NULL_SPACE];
    Node * item;

    printf("Remove Item\n");
    printf("-------------\n\n");

    printf("Enter the item id of the item to remove from the menu: ");   
    if (!getInputValidation(id, ID_LEN+EXTRA_SPACES)) 
        return;

    if ((item = getItemId(system->itemList, id)) == NULL) {
        printf("Error, that item id does not exist.\n");
        return;
    }

    strcpy(name, item->data->name);
    if (!linkListRemove(system->itemList, id)) {
        printf("Error, that item cannot be removed.\n");
    }
    printf("\"%s - %s\" has been removed from the system.\n", id, name);
}

/**
 * This option will require you to display the coins from lowest to highest
 * value and the counts of coins should be correctly aligned.
 * This function implements part 4 of requirement 18 in the assignment
 * specifications.
 **/
void displayCoins(VmSystem * system)
{ 
    showCoinsList(system->cashRegister);
}

/**
 * This option will require you to iterate over every stock in the
 * list and set its onHand count to the default value specified in
 * the startup code.
 * This function implements requirement 9 of the assignment specification.
 **/
void resetStock(VmSystem * system)
{ 
    Node * node = system->itemList->head;

    while (node != NULL) {
        node->data->onHand = DEFAULT_STOCK_LEVEL;
        node = node->next;
    }
    printf("All stock has been reset to the default level of %u.\n", DEFAULT_STOCK_LEVEL);
}

/**
 * This option will require you to iterate over every coin in the coin
 * list and set its 'count' to the default value specified in the
 * startup code.
 * This requirement implements part 3 of requirement 18 in the
 * assignment specifications.
 **/
void resetCoins(VmSystem * system)
{ 
    Denomination denomination;

    for (denomination = FIVE_CENTS; denomination <= TEN_DOLLARS; denomination++) {
        system->cashRegister[denomination].count = DEFAULT_COIN_COUNT;
    }
    printf("All coins have been reset to the default level of %u.\n", DEFAULT_COIN_COUNT);
}

/**
 * This option will require you to display goodbye and free the system.
 * This function implements requirement 10 of the assignment specification.
 **/
void abortProgram(VmSystem * system)
{ 
    freeMemList(system->itemList);
    systemFree(system);
}
 