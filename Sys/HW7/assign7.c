#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FILENAME "inventory.dat"
typedef struct
    {
        char itemName [64];
        char simpleName [16];
        int currentQuantity ;
        int maxQuantity ;
        char body [128];
    } Item ;
//char FILENAME [] = "inventory.dat";
void prompt ();
void creater (int);
void readr (int);
void deleter (int);
void updater (int);

int main()
{
    
    char szInputBuffer[101];                        //The buffer for input 
    char cMenuInput ;
    int iStructSize = sizeof(Item);
    int iCount;
    int iloopCount = 0;

    while (1)
    {   
        fflush(stdin);
        //print out the menu for the program
        prompt();

        //input character to process request
        fgets(szInputBuffer,sizeof(szInputBuffer),stdin);
        cMenuInput = szInputBuffer[0];
        cMenuInput = tolower(cMenuInput);
        if(feof(stdin))
            exit(1);

        switch(cMenuInput){
            case 'c':
                creater(iStructSize);
                break;
            case 'r':
                readr(iStructSize);
                break;
            case 'u':
                updater(iStructSize);
                break;
            case 'd':
                deleter(iStructSize);
                break;
            default:
                printf("INVALID INPUT\n");
                
                break;
        }
    }
    return 0;
}
/********************************************************************************************
Prompt function
*********************************************************************************************/
void prompt()
{
    // printing of the menu
    printf("\nEnter one of the following actions or press CTRL-D to exit.\n");
    printf("C - create a new item\n");
    printf("R - read an existing item\n") ;
    printf("U - update an existing item\n") ;
    printf("D - delete an existing item \n");
}

/********************************************************************************************
Create function
*********************************************************************************************/
void creater( int iSize)
{
    FILE *invenFile;                    //file ptr
    char szInputFileBuffer[101];        //input buffer from the file
    int iReadCount = 0;                 //The read count return number
    int iNum = 0;                       //Item number input from the client
    Item record;                        //The item that we gonna read into

    //creating file if it doesn't exist
    invenFile = fopen (FILENAME,"ab+"); 
    fclose(invenFile);

    //Enter the item number
    printf("\nEnter an item number: ");
    fgets(szInputFileBuffer,sizeof(szInputFileBuffer),stdin);
    sscanf(szInputFileBuffer,"%d",&iNum);
    //open file for actual reading
    invenFile = fopen(FILENAME, "rb+");
    //seek the position into the file
    fseek(invenFile, iNum*iSize, SEEK_SET);
    //read the record
    iReadCount = fread (&record,        //the address of the record that we will store
                    iSize,              // size of the structure
                    1L,                 // read 1 record
                    invenFile);         // file ptr

    if(iReadCount == 1 && record.maxQuantity != 0)
    {
        printf("\nERROR: item already exists");
    }
    else
    {
        //ask for all information
        printf("Simple name: ");
        fgets(record.simpleName,sizeof(record.simpleName),stdin);
        strtok(record.simpleName, "\n");
        
        printf("Item name: ");
        fgets(record.itemName,sizeof(record.itemName),stdin);
        strtok(record.itemName, "\n");
        
        printf("Current Quantity: ");
        fgets(szInputFileBuffer,sizeof(szInputFileBuffer),stdin);
        sscanf(szInputFileBuffer,"%i",&record.currentQuantity);
        
        printf("Max Quantity: ");
        fgets(szInputFileBuffer,sizeof(szInputFileBuffer),stdin);
        sscanf(szInputFileBuffer,"%i",&record.maxQuantity);
        
        printf("Body: ");
        fgets(record.body,sizeof(record.body),stdin);
        strtok(record.body, "\n");

        //write to the file
        fseek(invenFile, iNum*iSize, SEEK_SET);
        fwrite (&record,        //the address of the record that we will store
                iSize,              // size of the structure
                1L,                 // read 1 record
                invenFile);         // file ptr
    }
    fclose(invenFile);
    return;
}

/********************************************************************************************
READ function
*********************************************************************************************/
void readr(int iSize)
{
    FILE *invenFile;                    //file ptr
    char szInputFileBuffer[101];        //input buffer from the file
    int iReadCount = 0;                 //The read count return number
    int iNum = 0;                       //Item number input from the client
    Item record;                        //The item that we gonna read into

    //creating file if it doesn't exist
    invenFile = fopen (FILENAME,"ab+"); 
    fclose(invenFile);

    //Enter an input
    printf("\nEnter an item number: ");
    fgets(szInputFileBuffer,sizeof(szInputFileBuffer),stdin);
    sscanf(szInputFileBuffer,"%d",&iNum);

    invenFile = fopen (FILENAME,"rb");

    //If the file open unsuccessfully
    if (invenFile == NULL)
    {
        printf("\nERROR: FILE NOT FOUND");
        fclose(invenFile);
        return;
    }

    //if the file open successfully
    //seek the position into the file
    fseek(invenFile, iNum*iSize, SEEK_SET);
    //read the record
    iReadCount = fread (&record,            //the address of the record that we will store
                    iSize,              // size of the structure
                    1L,                 // read 1 record
                    invenFile);          // file ptr

    //check if read successfully
    //check for out of bound but because of expected output from the prof
    // I have to put item not found
    if(iReadCount != 1)
        printf("\nERROR: item not found");
    if(iReadCount == 1 && record.maxQuantity == 0)
    {
        printf("\nERROR: item not found");
    }
    else if(iReadCount == 1)
    {
        printf("\nItem name: %s", record.itemName);
        printf("\nSimple name: %s", record.simpleName);
        printf("\nItem Number: %i", iNum);
        printf("\nQty %i / %i",record.currentQuantity,record.maxQuantity);
        printf("\nDescription: %s\n",record.body);
    }


    fclose(invenFile);
    return;
}

/********************************************************************************************
Delete function
*********************************************************************************************/
void deleter (int iSize)
{
    FILE *invenFile;                    //file ptr
    char szInputFileBuffer[101];        //input buffer from the file
    int iReadCount = 0;                 //The read count return number
    int iNum = 0;                       //Item number input from the client
    Item record;                        //The item that we gonna read into

    //creating file if it doesn't exist
    invenFile = fopen (FILENAME,"ab+"); 
    fclose(invenFile);

    //Enter an input
    printf("\nEnter an item number: ");
    fgets(szInputFileBuffer,sizeof(szInputFileBuffer),stdin);
    sscanf(szInputFileBuffer,"%d",&iNum);

    invenFile = fopen (FILENAME,"rb+");

    //If the file open unsuccessfully
    if (invenFile == NULL)
    {
        printf("\nERROR: FILE NOT FOUND");
        fclose(invenFile);
        return;
    }

    //if the file open successfully
    //seek the position into the file
    fseek(invenFile, iNum*iSize, SEEK_SET);
    //read the record
    iReadCount = fread (&record,        //the address of the record that we will store
                    iSize,              // size of the structure
                    1L,                 // read 1 record
                    invenFile);         // file ptr

    //check if read successfully
    //check for out of bound but because of expected output from the prof
    // I have to put item not found
    if(iReadCount != 1)
        printf("\nERROR: item not found");
    if(iReadCount == 1 && record.maxQuantity == 0)
    {
        printf("\nERROR: item not found");
    }
    else if (iReadCount == 1)
    {
        record.maxQuantity = 0;
        //write to the file
        fseek(invenFile, iNum*iSize, SEEK_SET);
        fwrite (&record,            //the address of the record that we will store
                iSize,              // size of the structure
                1L,                 // read 1 record
                invenFile);         // file ptr
        printf ("%s was successfully deleted.", record.simpleName);
    }
    fclose(invenFile);
    
    return;
}

/********************************************************************************************
Update function
*********************************************************************************************/
void updater(int iSize)
{
    FILE *invenFile;                    //file ptr
    char szInputFileBuffer[101];        //input buffer from the file
    int iReadCount = 0;                 //The read count return number
    int iNum = 0;                       //Item number input from the client
    Item record;                        //The item that we gonna read into
    Item record_new;                    //The new record that will be written into the file
    
    //assiging default value for checking later
    record_new.currentQuantity = 0;
    record_new.maxQuantity = 0;

    //creating file if it doesn't exist
    invenFile = fopen (FILENAME,"ab+"); 
    fclose(invenFile);

    //Enter an input
    printf("\nEnter an item number: ");
    fgets(szInputFileBuffer,sizeof(szInputFileBuffer),stdin);
    sscanf(szInputFileBuffer,"%d",&iNum);

    invenFile = fopen (FILENAME,"rb+");

    //If the file open unsuccessfully
    if (invenFile == NULL)
    {
        printf("\nERROR: FILE NOT FOUND");
        fclose(invenFile);
        return;
    }

    //if the file open successfully
    //seek the position into the file
    fseek(invenFile, iNum*iSize, SEEK_SET);
    //read the record
    iReadCount = fread (&record,        //the address of the record that we will store
                    iSize,              // size of the structure
                    1L,                 // read 1 record
                    invenFile);         // file ptr

    //check if read successfully
    //check for out of bound but because of expected output from the prof
    // I have to put item not found
    if(iReadCount != 1)
        printf("\nERROR: item not found");
    if(iReadCount == 1 && record.maxQuantity == 0)
    {
        printf("\nERROR: item not found");
    }
    else if(iReadCount == 1)
    {
        //ask for all information
        printf("Simple name: ");
        fgets(record_new.simpleName,sizeof(record_new.simpleName),stdin);
        strtok(record_new.simpleName, "\n");
        
        printf("Item name: ");
        fgets(record_new.itemName,sizeof(record_new.itemName),stdin);
        strtok(record_new.itemName, "\n");
        
        printf("Current Quantity: ");
        fgets(szInputFileBuffer,sizeof(szInputFileBuffer),stdin);
        sscanf(szInputFileBuffer,"%i",&record_new.currentQuantity);
        
        printf("Max Quantity: ");
        fgets(szInputFileBuffer,sizeof(szInputFileBuffer),stdin);
        sscanf(szInputFileBuffer,"%i",&record_new.maxQuantity);
        
        printf("Body: ");
        fgets(record_new.body,sizeof(record_new.body),stdin);
        strtok(record_new.body, "\n");

        //check if the input was null
        if (record_new.simpleName == NULL)
            sscanf(record_new.simpleName,"%s", record.simpleName);
        if (record_new.itemName == NULL)
            sscanf(record_new.itemName,"%s", record.itemName);
        if (record_new.currentQuantity == 0)
            record_new.currentQuantity = record.currentQuantity;
        if (record_new.maxQuantity == 0)
            record_new.maxQuantity = record.maxQuantity;
        if (record_new.body == NULL)
            sscanf(record_new.body,"%s", record.body);

        //write to the file
        fseek(invenFile, iNum*iSize, SEEK_SET);
        fwrite (&record_new,        //the address of the record that we will store
                iSize,              // size of the structure
                1L,                 // read 1 record
                invenFile);         // file ptr
    }
    fclose(invenFile);
    return;

}