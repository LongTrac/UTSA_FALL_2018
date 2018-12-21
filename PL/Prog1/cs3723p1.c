#include "cs3723p1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/************************************************************************************************************************
This function first checks to see if there is a node type-specific free node available in pMgr -> pFreeHeadM[shNodeType].
If there is, it is used and the free list is updated.
If there is not, it uses the driver-provided utilAllocate to allocate memory from the top of the heap
*************************************************************************************************************************/
void * userAllocate(StorageManager * pMgr, short shUserDataSize, short shNodeType , char sbUserData[], SMResult *psmResult)
{
	short shRealSize = shUserDataSize + pMgr->shPrefixSize;			//The real size of an allocNode: data size + prefix
	AllocNode *pAlloc;													

		//check if we have a free node from pMgr-> pFreeHeadM
  if (pMgr->pFreeHeadM[shNodeType] != NULL)
	{
		pAlloc = (AllocNode *)(pMgr->pFreeHeadM[shNodeType]); 
		pMgr->pFreeHeadM[shNodeType] = pMgr->pFreeHeadM[shNodeType]->pNextFree;  
	}
		//if not then use the driver provided: utilAllocate function  
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

/****************************************************************************************************************
Function: userRemoveRef
****************************************************************************************************************/
void userRemoveRef(StorageManager *pMgr, void *pUserData, SMResult *psmResult)
{
	int iAt;							// control variable representing subscript in metaAttrM
	MetaAttr *pAttr;					// slightly simplifies referencing item in metaAttrM
	void **ppNode;						// pointer into user data if this attribute is a pointer
	AllocNode *pAlloc;			        // simplify the referenced parameter
    pAlloc = (AllocNode *)(((char*)pUserData)- pMgr->shPrefixSize);    
    	
		//check to see if the pointer is even in the heap memory
		//if it is not within the heap: set error message and flag through psmResult
	if ((char *)(pAlloc) < pMgr->pBeginStorage || (char *)(pAlloc) >=pMgr->pEndStorage)
	{	
		char szMessage[MAX_MESSAGE_SIZE] = "Invalid address which isn't within heap"; //error message
		psmResult->rc = RC_INVALID_ADDR;
		strcpy(psmResult->szErrorMessage, szMessage);
		
		return;
	}	
	else 
	{
		pAlloc->shRefCount -= 1;
	}
        
		// if shRefCount reaches 0 start remove recursively
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
/****************************************************************************************************************
Function: userAssoc
****************************************************************************************************************/
void userAssoc (StorageManager *pMgr, void *pUserDataFrom, char szAttrName[], void *pUserDataTo, SMResult *psmResult)
{
	int iAt;						// control variable representing subscript in metaAttrM
	MetaAttr *pAttr;                // slightly simplifies referencing item in metaAttrM
	void **ppNode;                  // pointer into user data if this attribute is a pointer
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
				//check to see if it is a pointer
			if(pAttr->cDataType == 'P')
			{
				ppNode = (void **) &(pAlloc->sbData[pAttr->shOffset]);
					//if it is pointing to something we have to remove the references and
					//increase the reference count of the new node it is pointing to now
				if(*ppNode != NULL)
					userRemoveRef(pMgr, *ppNode, psmResult);

					//change the pointer after making sure all references are correct
				*ppNode = pUserDataTo;
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

/****************************************************************************************************************
Function: userAddRef
****************************************************************************************************************/
void userAddRef (StorageManager *pMgr, void* pUserDataTo, SMResult *psmResult)
{
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
/****************************************************************************************************************
Function: memFree
****************************************************************************************************************/
void memFree (StorageManager *pMgr, AllocNode *pAlloc, SMResult *psmResult)
{	
		
	FreeNode * pAllocFree;
	pAllocFree = (FreeNode*) pAlloc;					// This pointer is the one needed to be freed
		// Verify that the node is in fact an allocated node. If not return an error via psmResult.
	if (pAlloc->cAF == 'A')
	{	
		pAlloc->cAF = 'F';
		pAllocFree->pNextFree = pMgr->pFreeHeadM[pAlloc->shNodeType];				//Add pAllocFree to the front
		pMgr->pFreeHeadM[pAlloc->shNodeType] = pAllocFree;							//of the pFree array										
		psmResult->rc = 0;

		return;
	}
	else
	{
		char szErrorMes[MAX_MESSAGE_SIZE] = "ERROR: Attempted to free a node which isn't allocated.";
		psmResult->rc = RC_CANT_FREE;
		strcpy(psmResult->szErrorMessage, szErrorMes);

		return;
	}
}
