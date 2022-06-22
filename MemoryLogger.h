#pragma once

#define	MEM_TAG				'GOLM'
#define LAST_RECORD			"LAST_RECORD"
#define MAXIMUM_LOG_SIZE	0x10000000

typedef struct _MEMORY_LOG
{
	CRITICAL_SECTION	CriticalSection;
	PVOID	pLogBuffer;
	DWORD	dwOffset;
	DWORD	dwLogBufferSize;
	DWORD	dwRecordsAmount;
}MEMORY_LOG, *PMEMORY_LOG;

PMEMORY_LOG
CreateMemoryLog();

void
AddToMemoryLog(
	PMEMORY_LOG pMemLog,
	const TCHAR * pszFormat,
	...
);

VOID
PrintfHexBuf(
	PMEMORY_LOG pMemLog,
	PBYTE pBuffer,
	DWORD dwSize
);

VOID
FreeMemoryLog(
	PMEMORY_LOG pMemLog
);