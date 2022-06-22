#include "stdafx.h"

PMEMORY_LOG
CreateMemoryLog()
{
	PMEMORY_LOG pMemLog;

	pMemLog = (PMEMORY_LOG)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(MEMORY_LOG));

	if (!pMemLog)
		return NULL;

	InitializeCriticalSection(&pMemLog->CriticalSection);

	pMemLog->pLogBuffer = VirtualAlloc(0x0, MAXIMUM_LOG_SIZE, MEM_COMMIT, PAGE_READWRITE);

	if(!pMemLog->pLogBuffer)
	{
		FreeMemoryLog(pMemLog);

		return NULL;
	}

	*(PDWORD)pMemLog->pLogBuffer = MEM_TAG;

	pMemLog->dwLogBufferSize = MAXIMUM_LOG_SIZE;
	pMemLog->dwOffset = sizeof(MEM_TAG);

	return pMemLog;
}

void
AddToMemoryLog(
	PMEMORY_LOG pMemLog,
	const TCHAR * pszFormat, 
	...
)
{
	wchar_t* pBuffer;
	int nOutputLength;

	if (!pMemLog)
		return;

	va_list _args;
	va_start(_args, pszFormat);

	nOutputLength = _vscwprintf(pszFormat, _args);

	pBuffer = (wchar_t*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, nOutputLength * 2 + 4);

	_vsnwprintf_s(pBuffer, nOutputLength + 2, _TRUNCATE, pszFormat, _args);
	
	DWORD	dwRecordSize = nOutputLength * 2;

	EnterCriticalSection(&pMemLog->CriticalSection);

	if (dwRecordSize + pMemLog->dwOffset + strlen(LAST_RECORD) >= MAXIMUM_LOG_SIZE)
	{
		__debugbreak();

		pMemLog->dwOffset = sizeof(MEM_TAG);
		pMemLog->dwRecordsAmount = 0x0;
	}

	RtlCopyMemory((PBYTE)pMemLog->pLogBuffer + pMemLog->dwOffset, pBuffer, dwRecordSize);

	pMemLog->dwOffset += dwRecordSize;
	pMemLog->dwRecordsAmount++;

	RtlCopyMemory((PBYTE)pMemLog->pLogBuffer + pMemLog->dwOffset, LAST_RECORD, strlen(LAST_RECORD));

	LeaveCriticalSection(&pMemLog->CriticalSection);

	//OutputDebugString(pBuffer);

	HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, pBuffer);

	va_end(_args);
}

VOID
PrintfHexBuf(
	PMEMORY_LOG pMemLog,
	PBYTE pBuffer,
	DWORD dwSize
)
{
	AddToMemoryLog(pMemLog, L"\r\n{\r\n");

	for (size_t i = 0; i < dwSize; i++)
	{
		AddToMemoryLog(pMemLog, L"0x%02X, ", pBuffer[i]);

		if (!((i + 1) % 16))
			AddToMemoryLog(pMemLog, L"\r\n");
	}

	AddToMemoryLog(pMemLog, L"\r\n}\r\n");
}

VOID
FreeMemoryLog(
	PMEMORY_LOG pMemLog
)
{
	if(pMemLog)
	{
		if (pMemLog->pLogBuffer)
			VirtualFree(pMemLog->pLogBuffer, MAXIMUM_LOG_SIZE, MEM_DECOMMIT);

		HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, pMemLog);
	}
}