#pragma once

typedef struct _OBJ_CONTAINER
{
	LIST_ENTRY	ListHead;
	DWORD		dwObjectCount;
	CRITICAL_SECTION	CriticalSection;
}OBJ_CONTAINER, *POBJ_CONTAINER;

typedef struct _CONTAINER_ITEM
{
	LIST_ENTRY	ListEntry;
	DWORD	dwType;
	PVOID	pObject;
}CONTAINER_ITEM, *PCONTAINER_ITEM;

BOOL
InitEmptyContainer(
	POBJ_CONTAINER pContainer
);

BOOL
AddObject(
	POBJ_CONTAINER pContainer,
	PVOID	pObject,
	DWORD	dwType
);

DWORD
GetObjectCount(
	POBJ_CONTAINER pContainer,
	DWORD	dwType
);

PVOID
GetRandomObject(
	POBJ_CONTAINER pContainer,
	DWORD	dwType
);

PCONTAINER_ITEM
GetFirstObject(
	POBJ_CONTAINER pContainer
);

BOOL
FreeObject(
	POBJ_CONTAINER pContainer,
	PVOID	pObject,
	DWORD	dwType
);