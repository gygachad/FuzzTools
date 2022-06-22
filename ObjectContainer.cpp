#include "stdafx.h"

BOOL
InitEmptyContainer(
	POBJ_CONTAINER pContainer
)
{
	if (!pContainer)
		return FALSE;

	RtlZeroMemory(pContainer, sizeof(OBJ_CONTAINER));
	InitializeListHead(&pContainer->ListHead);
	InitializeCriticalSection(&pContainer->CriticalSection);

	return TRUE;
}

BOOL
AddObject(
	POBJ_CONTAINER pContainer,
	PVOID	pObject,
	DWORD	dwType
)
{
	PCONTAINER_ITEM pItem;

	pItem = (PCONTAINER_ITEM)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(CONTAINER_ITEM));

	if (!pItem)
		return FALSE;

	InitializeListHead(&pItem->ListEntry);

	pItem->pObject = pObject;
	pItem->dwType = dwType;

	EnterCriticalSection(&pContainer->CriticalSection);

	InsertTailList(&pContainer->ListHead, &pItem->ListEntry);

	pContainer->dwObjectCount++;

	LeaveCriticalSection(&pContainer->CriticalSection);

	return TRUE;
}

DWORD
GetObjectCount(
	POBJ_CONTAINER pContainer,
	DWORD	dwType
)
{
	DWORD	dwCount;
	PCONTAINER_ITEM pCurItem;

	dwCount = 0;

	EnterCriticalSection(&pContainer->CriticalSection);

	if (IsListEmpty(&pContainer->ListHead))
	{
		LeaveCriticalSection(&pContainer->CriticalSection);
		return 0;
	}

	for (	PLIST_ENTRY pCurEntry = pContainer->ListHead.Flink;
			pCurEntry->Flink != pContainer->ListHead.Flink;
			pCurEntry = pCurEntry->Flink)
	{
		pCurItem = CONTAINING_RECORD(pCurEntry, CONTAINER_ITEM, ListEntry);

		if (dwType == pCurItem->dwType)
			dwCount++;
	}

	LeaveCriticalSection(&pContainer->CriticalSection);

	return dwCount;

}

PVOID
GetRandomObject(
	POBJ_CONTAINER pContainer,
	DWORD	dwType
)
{
	DWORD	dwObjectCount;
	DWORD	dwObjectNum;

	PCONTAINER_ITEM pCurItem;
	PVOID	pObject;

	dwObjectCount = GetObjectCount(pContainer, dwType);

	if (dwObjectCount == 0x0)
		return NULL;

	dwObjectNum = getrand(0x1, dwObjectCount);

	pCurItem = NULL;
	pObject = NULL;

	EnterCriticalSection(&pContainer->CriticalSection);

	for (PLIST_ENTRY pCurEntry = pContainer->ListHead.Flink;
		pCurEntry->Flink != pContainer->ListHead.Flink;
		pCurEntry = pCurEntry->Flink)
	{
		pCurItem = CONTAINING_RECORD(pCurEntry, CONTAINER_ITEM, ListEntry);

		if (dwType == pCurItem->dwType)
			dwObjectNum--;

		if (dwObjectNum == 0x0		&& 
			(dwType == pCurItem->dwType))
		{
			pObject = pCurItem->pObject;
			break;
		}
	}

	LeaveCriticalSection(&pContainer->CriticalSection);

	return pObject;
}

PCONTAINER_ITEM
GetFirstObject(
	POBJ_CONTAINER pContainer
)
{
	EnterCriticalSection(&pContainer->CriticalSection);

	if (pContainer->dwObjectCount == 0x0)
	{
		LeaveCriticalSection(&pContainer->CriticalSection);
		return NULL;
	}

	PCONTAINER_ITEM pCurItem;

	pCurItem = CONTAINING_RECORD(pContainer->ListHead.Flink, CONTAINER_ITEM, ListEntry);

	LeaveCriticalSection(&pContainer->CriticalSection);

	return pCurItem;
}

BOOL
FreeObject(
	POBJ_CONTAINER pContainer,
	PVOID	pObject,
	DWORD	dwType
)
{
	PCONTAINER_ITEM pCurItem;

	BOOL	bStatus = FALSE;

	EnterCriticalSection(&pContainer->CriticalSection);

	if (IsListEmpty(&pContainer->ListHead))
	{
		EnterCriticalSection(&pContainer->CriticalSection);
		return bStatus;
	}

	pCurItem = NULL;

	for (PLIST_ENTRY pCurEntry = pContainer->ListHead.Flink;
		pCurEntry->Flink != pContainer->ListHead.Flink;
		pCurEntry = pCurEntry->Flink)
	{
		pCurItem = CONTAINING_RECORD(pCurEntry, CONTAINER_ITEM, ListEntry);

		if(pCurItem->pObject == pObject &&
			pCurItem->dwType == dwType)
		{
			RemoveEntryList(pCurEntry);

			HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, pCurItem);

			pContainer->dwObjectCount--;

			bStatus = TRUE;

			break;
		}
		
	}

	LeaveCriticalSection(&pContainer->CriticalSection);

	return bStatus;
}