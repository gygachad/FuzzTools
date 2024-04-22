#pragma once

LPWSTR
GetCurrentProcessName();

void
DebugLog(const TCHAR* pszFormat, ...);

void
DebugLogToFile(const wchar_t* pFileName, const TCHAR* pszFormat, ...);

#ifdef _DEBUG
#define DBG_LOG DebugLog
#define DBG_LOG_TO_FILE DebugLogToFile
#else
#define DBG_LOG(...) 
#define DBG_LOG_TO_FILE (...) 
#endif