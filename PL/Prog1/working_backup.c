/**********************************************************************
cs3723p1.c by Long Trac
Purpose:	
    This program will create heap storage management functions based on using reference counts and
	understanding metadata. 
	These functions will manage free memory lists and the reference counts for user data nodes:
		• When a node is created, it sets the count to 1 (this is like a variable referencing data causing the count to be 1).
		• A node (i.e., from node) can be associated with another node (i.e., to node) by changing a pointer attribute in
			the from node. This will cause the to node to have its reference count increased. If the from node was already
			referencing another node via that particular pointer attribute, that original referenced node should have its
			reference count decreased. If the new to node value is NULL, don't attempt to increase its reference count.
		• We can also add references to nodes similar to a node having another variable referencing it.

Command Parameters:
   
Input:

Results:

Returns:

Notes:
	This program will run using the driver provided: cs 3723p1Driver.c
**********************************************************************/

/**********************************************************************
include "cs3723.h"
Purpose:
	The header file that contains all structures information for this program
Notes:
**********************************************************************/
#include "cs3723p1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************** userAllocate **********************************
void * userAllocate(StorageManager * pMgr, short shUserDataSize, short shNodeType , char sbUserData[], SMResult *psmResult)
Purpose:  
	Allocates an AllocNode from our heap, copies the binary sbUserData into that node, and returns a 
	pointer to the user data portion of the node. 

Parameters:
	I	StorageManager * 	pMgr 				– a pointer to the StorageManager structure. 
	I	short				shUserDataSize 		– the size of the user data. This does not include the four storage management prefix
													attributes. The size of the allocated node will actually be this plus at least the pMgr->shPrefixSize.
	I	short 				shNodeType			– this is a subscript into the storage manager's nodeTypeM array.
	I	char				sbUserData 			– this is storage buffer containing the actual user data which needs to be placed in the
													allocated node. This data can contain integer and double values; therefore, it is not a zero-terminated string.
	O	SMResult			psmResult 			– a pointer to a SMResult structure. 

Notes:

Return Values:
	NULL					-There isn't any free memory to handle alloc request.
	&(pAlloc->sbData)		-Allocate the data successfully

*********************************************************************/
void * userAllocate(StorageManager * pMgr, short shUserDataSize, short shNodeType , char sbUserData[], SMResult *psmResult)
{
	/*
		short 			shRealSize				- The real size of an allocNode: data size + prefix
		AllocNode *		pAlloc					- An AllocNode pointer that point to the node that will be allocated
 	*/
	short shRealSize = shUserDataSize + pMgr->shPrefixSize;			
	AllocNode *pAlloc;													

	//check if we have a free node from pMgr-> pFreeHeadM
	// if yes then use it for pAlloc
	if (pMgr->pFreeHeadM[shNodeType] != NULL)
	{
		pAlloc = (AllocNode *)(pMgr->pFreeHeadM[shNodeType]); 
		pMgr->pFreeHeadM[shNodeType] = pMgr->pFreeHeadM[shNodeType]->pNextFree;  
	}
	//if not then use the driver provided: utilAllocate function to allocate the top of the heap 
	else
	{
		pAlloc = utilAlloc(pMgr, shRealSize);
	}

	// if palloc is allocated, initialize the AllocNode
	if (pAlloc != NULL) 
	{
		pAlloc->shRefCount = 1;										//set reference count to 1
		pAlloc->shNodeType = shNodeType;							//set the nodeType of the allocated node to the parameter shNodeType
		pAlloc->shAllocSize = shRealSize;				 			//set the size to be the data of user + prefix
		pAlloc->cAF = 'A';											//set the flag to allocated
		memcpy(pAlloc->sbData,sbUserData,shUserDataSize);
		psmResult->rc = 0;											//set rc to 0 when run normally

		return &(pAlloc->sbData);
	}
	//if palloc is not allocated, set the rc infomation and return null
	else
	{
		char szErrorMes[MAX_MESSAGE_SIZE] = "ERROR: There isn't any free memory to handle alloc request.";
		psmResult->rc = RC_NOT_AVAIL;
		strcpy(psmResult->szErrorMessage,szErrorMes);

		return NULL;
	}	
}

/******************** userRemoveRef **********************************
void userRemoveRef(StorageManager *pMgr, void *pUserData, SMResult *psmResult)
Purpose:  
	Removes a reference to the specified data.

Parameters:
	I	StorageManager * 	pMgr 				– a pointer to the StorageManager structure. 
	I/O	void*				pUserData			– a pointer to the user data within an allocated node.
	O	SMResult			psmResult 			– a pointer to a SMResult structure. 

Notes:

Return Values:
	
*********************************************************************/
void userRemoveRef(StorageManager *pMgr, void *pUserData, SMResult *psmResult)
{
	/*
		int 				iAt;				- control variable representing subscript in metaAttrM
		MetaAttr *			pAttr;				- slightly simplifies referencing item in metaAttrM
		void **				ppNode;				- pointer into user data if this attribute is a pointer
		AllocNode *			pAlloc;			    - a pointer to the begin of an allocNode
	*/
	int iAt;							
	MetaAttr *pAttr;					
	void **ppNode;						
	AllocNode *pAlloc;	

    pAlloc = (AllocNode *)(((char*)pUserData)- pMgr->shPrefixSize);    
    	
	//check to see if the pointer is even in the heap memory
	//if it is not within the heap: set error message and flag through psmResult
	if ((char *)(pAlloc) < pMgr->pBeginStorage || (char *)(pAlloc) >=pMgr->pEndStorage)
	{	
		char szMessage[MAX_MESSAGE_SIZE] = "Invalid address which isn't within heap"; 
		psmResult->rc = RC_INVALID_ADDR;
		strcpy(psmResult->szErrorMessage, szMessage);
		
		return;
	}	
	else 
	{	
		pAlloc->shRefCount -= 1;					//decrement the shRefCount for that node
	}
        
	// if shRefCount reaches 0 start remove shRefCount recursively
	if (pAlloc->shRefCount == 0)
	{
		// Loop through each of the user data's attributes. The subcript start with 
		// shBeginMetaAttr from nodeTypeM end when the correspoding metaAttr's node type
		// is different
		for (iAt = pMgr->nodeTypeM[pAlloc->shNodeType].shBeginMetaAttr; pMgr->metaAttrM[iAt].shNodeType == pAlloc->shNodeType; iAt++)
		{
			pAttr = &(pMgr->metaAttrM[iAt]); 		// simplify the reference in the metaAttrM array 
		
			//check to see if it is a pointer
			if (pAttr->cDataType == 'P')
			{
				//use offset to access that pointer
				ppNode = (void **) &(pAlloc->sbData[pAttr->shOffset]);
				
				// if still pointing to another node then traverse and remove ref on that node as well
				if (*ppNode != NULL)
					userRemoveRef(pMgr, *ppNode, psmResult);
			}
		} 	
		memFree(pMgr, pAlloc, psmResult);			// Free the node
	} 
	psmResult->rc = 0;								//if everything succeded set rc flag to 0

	return;
}

/********************** userAssoc ************************************
void userAssoc (StorageManager *pMgr, void *pUserDataFrom, char szAttrName[], void *pUserDataTo, SMResult *psmResult)
Purpose:  
	Logically, it is setting a pointer in a user node to point to another node (or NULL) and correspondingly
	updating reference counts.

Parameters:
	I	StorageManager * 	pMgr 				– a pointer to the StorageManager structure. 
	I	void*				pUserDataFrom		- a user data pointer to the from node which contains the pointer attribute. 
	I	char				szAttrName 			– the name of the attribute which is used to determine where 
													the pointer is located within the from node.
	I/O void*				pUserDataTo			– a user data pointer to the to node. 												This is the from node.
	O	SMResult			psmResult 			– a pointer to a SMResult structure. 

Notes:

Return Values:
	
*********************************************************************/
void userAssoc (StorageManager *pMgr, void *pUserDataFrom, char szAttrName[], void *pUserDataTo, SMResult *psmResult)
{
	/*
		int 				iAt;				- control variable representing subscript in metaAttrM
		MetaAttr *			pAttr;				- slightly simplifies referencing item in metaAttrM
		void **				ppNode;				- pointer into user data if this attribute is a pointer
		AllocNode *			pAlloc;			    - a pointer to the begin of an allocNode
	*/
	int iAt;						
	MetaAttr *pAttr;                
	void **ppNode;                  
	AllocNode *pAlloc;

	pAlloc = (AllocNode *)(((char *)pUserDataFrom)- pMgr->shPrefixSize);

	// Loop through each of the user data's attributes.  The subscripts start with
    // shBeginMetaAttr from nodeTypeM and end when the corresponding metaAttr's node type is
    // different.
  	for (iAt = pMgr->nodeTypeM[pAlloc->shNodeType].shBeginMetaAttr; pMgr->metaAttrM[iAt].shNodeType == pAlloc->shNodeType; iAt++)
    {
		pAttr = &(pMgr->metaAttrM[iAt]);   // slightly simplify the reference in the metaAttrM array
		//check for AttrName
		if(strcmp(pAttr->szAttrName , szAttrName) == 0)
		{
			//check to see if it is a pointer data type 
			if(pAttr->cDataType == 'P')
			{
				ppNode = (void **) &(pAlloc->sbData[pAttr->shOffset]);
				
				//if it is pointing to something we have to remove the references.
				if(*ppNode != NULL)
					userRemoveRef(pMgr, *ppNode, psmResult);

				//change the pointer after making sure all references are correct
				*ppNode = pUserDataTo;

				//increase the reference count of the new node it is pointing to now
				if(*ppNode != NULL)
					userAddRef(pMgr,*ppNode, psmResult); 

				psmResult->rc = 0;						//if everything succeded set rc flag to 0

				return;
			}
			//if cData was not a pointer then return an error
			else	
			{
				char szErrorMes[MAX_MESSAGE_SIZE] = "ERROR: Attribute name for ASSOC not a pointer attribute.";
				psmResult->rc = RC_ASSOC_ATTR_NOT_PTR;
				strcpy(psmResult->szErrorMessage, szErrorMes);

				return;
			}
		}
	} 

	//If the ref attribute can not be found during the for loop then return an error
	char szErrorMes [MAX_MESSAGE_SIZE] = "ERROR: Attribute name for ASSOC not found for the from node.";
	psmResult->rc = RC_ASSOC_ATTR_NOT_FOUND;
	strcpy(psmResult->szErrorMessage, szErrorMes);	

	return;
}

/********************** userAddRefc ************************************
void userAddRef (StorageManager *pMgr, void* pUserDataTo, SMResult *psmResult)
Purpose:  
	Logically, it is adding a reference to the specified to node. This is similar to adding a variable's
	reference to the to node.

Parameters:
	I	StorageManager * 	pMgr 				– a pointer to the StorageManager structure. 
	I/O void*				pUserDataTo			– a user data pointer to the to node. 												This is the from node.
	O	SMResult			psmResult 			– a pointer to a SMResult structure. 

Notes:

Return Values:
	
*********************************************************************/
void userAddRef (StorageManager *pMgr, void* pUserDataTo, SMResult *psmResult)
{
	/*
		AllocNode *			pAlloc				- a pointer to the begin of an allocNode
	*/
	AllocNode *pAlloc;

	pAlloc = (AllocNode *)(((char *)pUserDataTo)- pMgr->shPrefixSize);	

	//check to see if the pointer is even in the heap memory
	//if it is not within the heap: set error message and flag through psmResult
	if((char *)(pAlloc) < pMgr->pBeginStorage || (char *)(pAlloc) >= pMgr->pEndStorage)
	{
		char szErrorMes [MAX_MESSAGE_SIZE] = "ERROR: Invalid address which isn't within heap.";
		psmResult->rc = RC_INVALID_ADDR;
		strcpy(psmResult->szErrorMessage, szErrorMes);

		return; 
	}
	// if it is a valid address : increase ref count
	else
	{
		pAlloc->shRefCount += 1;
		psmResult->rc = 0;

		return;
	}
	
}

/********************** memFree ************************************
void memFree (StorageManager *pMgr, AllocNode *pAlloc, SMResult *psmResult)
Purpose:  
	 This is adding the specified allocated node to the free list for that node's node type.

Parameters:
	I	StorageManager * 	pMgr 				– a pointer to the StorageManager structure. 
	I/O AllocNode*			pAlloc				– a pointer to an allocated node. This is NOT a user data pointer 												This is the from node.
	O	SMResult			psmResult 			– a pointer to a SMResult structure. 

Notes:

Return Values:
	
*********************************************************************/
void memFree (StorageManager *pMgr, AllocNode *pAlloc, SMResult *psmResult)
{	
	/*
		FreeNode *			pAlloc				- a pointer that needed to be freed 
	*/	
	FreeNode * pAllocFree;
	pAllocFree = (FreeNode*) pAlloc;

	// Verify that the node is in fact an allocated node. 
	if (pAlloc->cAF == 'A')
	{	
		pAlloc->cAF = 'F';
		pAllocFree->pNextFree = pMgr->pFreeHeadM[pAlloc->shNodeType];				//Add pAllocFree to the front
		pMgr->pFreeHeadM[pAlloc->shNodeType] = pAllocFree;							//of the pFree array										
		psmResult->rc = 0;

		return;
	}
	//If not return an error via psmResult.
	else
	{
		char szErrorMes[MAX_MESSAGE_SIZE] = "ERROR: Attempted to free a node which isn't allocated.";
		psmResult->rc = RC_CANT_FREE;
		strcpy(psmResult->szErrorMessage, szErrorMes);

		return;
	}
}
