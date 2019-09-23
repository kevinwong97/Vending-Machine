/******************************************************************************
** Student name: Kevin Wong   ...
** Student number: s3603055 ...
** Course:          Advanced Programming Techniques - S1 2018
******************************************************************************/

#include "vm_coin.h"

/**
 * Implement functions here for managing coins and the
 * "cash register" contained in the VmSystem struct.
 **/

/**
 * Some example functions:
 * init coins array, insert coin, change coin count for Denomination,
 * convert Denomination to coin value, deducting coins from register, etc...
 */

void showCoinsList(Coin * cashRegister) 
{
    printf("Coins Summary\n");
    printf("-------------\n");
    printf("%-12s | Count\n", "Denomination");
    printf("\n");
    printf("%-12s | %u\n", "5 cents", cashRegister[FIVE_CENTS].count);
    printf("%-12s | %u\n", "10 cents", cashRegister[TEN_CENTS].count);
    printf("%-12s | %u\n", "20 cents", cashRegister[TWENTY_CENTS].count);
    printf("%-12s | %u\n", "50 cents", cashRegister[FIFTY_CENTS].count);
    printf("%-12s | %u\n", "1 dollar", cashRegister[ONE_DOLLAR].count);
    printf("%-12s | %u\n", "2 dollar", cashRegister[TWO_DOLLARS].count);
    printf("%-12s | %u\n", "5 dollar", cashRegister[FIVE_DOLLARS].count);
    printf("%-12s | %u\n", "10 dollar", cashRegister[TEN_DOLLARS].count);
    printf("\n\n");
}

Boolean totalChange(Coin * cashRegister, int * change, int amount) 
{
    
    int i;
    unsigned value;

    if (amount == 0) return TRUE;
    /* iterate through each denomination and see what is remaining */
    for (i = TEN_DOLLARS; i >= 0; i--) 
    {
        if (cashRegister[i].count != 0)
         {
            if ((value = valueOfCoin(i)) <= amount) 
            {
              /* Increment amount of change at that denomination */
              change[i]++;
              /* Decrement amount of change in the cash register */
              cashRegister[i].count--;
              /* Repeat calculation */
              return (totalChange(cashRegister, change, amount - value));
            }
        }
    }
    return FALSE;
}

/*convert coin values into string to show the change that makes up the total change */
void coinString(char * string, int * coins) 
{
    int i;
    char array[BUFFER_ARRAY_SIZE];
    strcpy(string, " ");
    
    for (i = 0; i < NUM_DENOMS; i++)
     {
        if (coins[i] == 0) 
            continue;
        if (coins[i] > 1)
        {
            sprintf(array, "%dx", coins[i]);
            strcat(string, array);
        }
        switch (i) 
        {
            case FIVE_CENTS:
                strcat(string, "5c ");
                break;
            case TEN_CENTS:
                strcat(string, "10c ");
                break;
            case TWENTY_CENTS:
                strcat(string, "20c ");
                break;
            case FIFTY_CENTS:
                strcat(string, "50c ");
                break;
            case ONE_DOLLAR:
                strcat(string, "$1 ");
                break;
            case TWO_DOLLARS:
                strcat(string, "$2 ");
                break;
            case FIVE_DOLLARS:
                strcat(string, "$5 ");
                break;
            case TEN_DOLLARS:
                strcat(string, "$10 ");
                break;
        }
    }
}

/*value of coin from denomination*/
unsigned valueOfCoin(Denomination denomination) 
{
    switch (denomination) 
    {
        case FIVE_CENTS:
            return 5;
        case TEN_CENTS:
            return 10;
        case TWENTY_CENTS:
            return 20;
        case FIFTY_CENTS:
            return 50;
        case ONE_DOLLAR:
            return 100;
        case TWO_DOLLARS:
            return 200;
        case FIVE_DOLLARS:
            return 500;
        case TEN_DOLLARS:
            return 1000;
    }
    return 0;
}

/* converts denomination to coin value */
Denomination coinDenomination(Coin * cashRegister, int amount) 
{
    switch (amount) 
    {        
        case 5:
            return FIVE_CENTS;
        case 10:
            return TEN_CENTS;    
        case 20:
            return TWENTY_CENTS;
        case 50:
            return FIFTY_CENTS;
        case 100:
            return ONE_DOLLAR;
        case 200:
            return TWO_DOLLARS;
        case 500:
            return FIVE_DOLLARS;
        case 1000:
            return TEN_DOLLARS;
    }
    return -1;
}

/*insert coisn into cash register*/
void insertCoins(Coin * cashRegister, int * coins) 
{
    int i;
    for (i = 0; i < NUM_DENOMS; i++)
    {
        cashRegister[i].count += coins[i];
    } 
}

/*remove coins from cash register*/
void deductCoins(Coin * cashRegister, int * coins) 
{
    int i;
    for (i = 0; i < NUM_DENOMS; i++) 
    {
        cashRegister[i].count -= coins[i];
    } 
}

/*initialise coins into cash register*/
void initialiseCoinsArray(Coin * cashRegister) 
{
    int i;
    for (i = 0; i < NUM_DENOMS; i++) 
    {
        cashRegister[i].denom = i;
        cashRegister[i].count = 0;
    }
}


