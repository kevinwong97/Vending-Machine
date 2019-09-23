/******************************************************************************
** Student name: Kevin Wong   ...
** Student number: s3603055 ...
** Course:          Advanced Programming Techniques - S1 2018
******************************************************************************/

#include "vm.h"

int main(int argc, char ** argv)
{
    MenuFunction select;
    MenuItem menu[MENU_SELECTION_SIZE];
    VmSystem * system = malloc(sizeof(VmSystem)); 

    if (!systemInit(system)) /*validation if system could not be initialised*/
    {
        printf("Error, VM System cannot be initialised.\n");
        return EXIT_FAILURE;
    }

    if (loadData(system, argv[1], argv[2]))  /*loads stock.dat and coins.dat*/
    {
        initMenu(menu);
        do 
        {
            select = getMenuChoice(menu);
            printf("\n");
            if (select == NULL) 
                continue;
            (*select)(system);	
        } 
        while (select != menu[2].function && select != menu[8].function);/* saveAndExit option and Abort Program option */
    } 
    else 
    {
        abortProgram(system);	
    }
    printf("Goodbye.\n\n");
    return EXIT_SUCCESS;
}
