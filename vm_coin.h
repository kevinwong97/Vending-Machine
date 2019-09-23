/******************************************************************************
** Student name: Kevin Wong   ...
** Student number: s3603055 ...
** Course:          Advanced Programming Techniques - S1 2018
******************************************************************************/

#ifndef VM_COIN_H
#define VM_COIN_H

#include "vm_system.h"

/**
 * The default coin level used when resetting values.
 **/
#define DEFAULT_COIN_COUNT 20

#define COIN_DELIM ","

#define BUFFER_ARRAY_SIZE (EXTRA_SPACES + 3)

Boolean totalChange(Coin * cashRegister, int * change, int amount);
void showCoinsList(Coin * cashRegister);
void coinString(char * string, int * coins);
unsigned valueOfCoin(Denomination denomination);
Denomination coinDenomination(Coin * cashRegister, int amount);
void insertCoins(Coin * cashRegister, int * coins);
void deductCoins(Coin * cashRegister, int * coins);
void initialiseCoinsArray(Coin * cashRegister);



#endif
