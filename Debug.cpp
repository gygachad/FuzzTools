#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <psapi.h>

LPWSTR
GetCurrentProcessName()
{
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, GetCurrentProcessId());

	if (!hProcess)
		return NULL;

	LPWSTR pProcessName = NULL;
	DWORD dwSize = 2;

	BOOL bSuccess = false;

	do
	{
		if (pProcessName)
			free(pProcessName);

		pProcessName = (LPWSTR)malloc(dwSize * sizeof(wchar_t));

		if (!pProcessName)
			break;

		if (GetProcessImageFileName(hProcess, pProcessName, dwSize))
		{
			CloseHandle(hProcess);
			return pProcessName;
		}

		dwSize = dwSize * 2;

	} while (dwSize < 1024 * 10 && GetLastError() == ERROR_INSUFFICIENT_BUFFER);

	if (pProcessName)
		free(pProcessName);

	CloseHandle(hProcess);
	return NULL;
}

BOOL
AddToFile(
    const wchar_t* pFileName,
    const wchar_t* pRecord
)
{
    FILE* f = NULL;
    if (_wfopen_s(&f, pFileName, L"at+, ccs=UNICODE"))
        return false;

	size_t len = wcslen(pRecord);
    fwrite(pRecord, sizeof(wchar_t), len, f);
    fclose(f);

    return true;
}

wchar_t* FormatDebugString(const TCHAR* pszFormat, va_list _args)
{
    wchar_t* pBuffer;
    int nOutputLength;

    nOutputLength = _vscwprintf(pszFormat, _args);

    pBuffer = (wchar_t*)malloc(nOutputLength * 2 + 4);

    _vsntprintf_s(pBuffer, nOutputLength + 2, _TRUNCATE, pszFormat, _args);

    return pBuffer;
}

void
DebugLogToFile(const wchar_t* pFileName, const TCHAR* pszFormat, ...)
{
	va_list _args;
	va_start(_args, pszFormat);
	wchar_t* pLogRecord = FormatDebugString(pszFormat, _args);
	va_end(_args);

	if (pLogRecord)
	{
		AddToFile(pFileName, pLogRecord);
		AddToFile(pFileName, L"\x0D\x00\x0A\x00");
		free(pLogRecord);
	}
}

void
DebugLog(const TCHAR* pszFormat, ...)
{
	va_list _args;
	va_start(_args, pszFormat);
	wchar_t* pLogRecord = FormatDebugString(pszFormat, _args);
	va_end(_args);

	if (pLogRecord)
	{
		OutputDebugString(pLogRecord);
		OutputDebugString(L"\x0D\x00\x0A\x00");
		free(pLogRecord);
	}
}
