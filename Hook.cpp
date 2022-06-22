#include "stdafx.h"

PVOID
SetImportTableHook(
	char*	ImportDllName,
	char*	ExportDllName,
	char*	FunctionName,
	PVOID	pHook
)
{
	HMODULE hImporterDll;

	PVOID	pOriginalFuncPtr = NULL;

	hImporterDll = LoadLibraryA(ImportDllName);

	if (!hImporterDll	||
		!ExportDllName	||
		!FunctionName)
		return NULL;

	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)hImporterDll;
	PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)((PBYTE)pDosHeader + pDosHeader->e_lfanew);

	DWORD dwImportDirectoryVA = pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;

	PVOID pImportDirectoryRAW = VA_TO_RAW(hImporterDll,dwImportDirectoryVA);

	PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)pImportDirectoryRAW;

	while (pImportDescriptor->Name != NULL)
	{
		if (_strcmpi(ExportDllName, (char*)VA_TO_RAW(hImporterDll, pImportDescriptor->Name)) == 0x0)
		{
#ifdef X86
			if (IMAGE_SNAP_BY_ORDINAL32(pImportDescriptor->Characteristics))
			{
				__debugbreak();
			}
			else
			{
				PDWORD pdwIATVA = (PDWORD)VA_TO_RAW(hImporterDll, pImportDescriptor->OriginalFirstThunk);
				PIMAGE_THUNK_DATA32	pImportThunk = (PIMAGE_THUNK_DATA32)VA_TO_RAW(hImporterDll, pImportDescriptor->FirstThunk);

				while (*pdwIATVA != 0x0)
				{
					PIMAGE_IMPORT_BY_NAME pImportByNameThunk = (PIMAGE_IMPORT_BY_NAME)VA_TO_RAW(hImporterDll, *pdwIATVA);

					if (strcmp(pImportByNameThunk->Name, FunctionName) == 0x0)
					{
						pOriginalFuncPtr = (PVOID)pImportThunk->u1.AddressOfData;
						
						DWORD dwOldProtect;

						VirtualProtect(pImportThunk, 0x1000, PAGE_READWRITE, &dwOldProtect);
						pImportThunk->u1.AddressOfData = (DWORD)pHook;
						VirtualProtect(pImportThunk, 0x1000, dwOldProtect, &dwOldProtect);

						break;
					}

					pdwIATVA++;
					pImportThunk++;
 				}

				if (pOriginalFuncPtr)
					break;
			}
#else
			if (IMAGE_SNAP_BY_ORDINAL32(pImportDescriptor->Characteristics))
			{
				__debugbreak();
			}
			else
			{
				PULONG64 pdwIATVA = (PULONG64)VA_TO_RAW(hImporterDll, pImportDescriptor->OriginalFirstThunk);
				PIMAGE_THUNK_DATA64	pImportThunk = (PIMAGE_THUNK_DATA64)VA_TO_RAW(hImporterDll, pImportDescriptor->FirstThunk);

				while (*pdwIATVA != 0x0)
				{
					PIMAGE_IMPORT_BY_NAME pImportByNameThunk = (PIMAGE_IMPORT_BY_NAME)VA_TO_RAW(hImporterDll, *pdwIATVA);

					if (strcmp(pImportByNameThunk->Name, FunctionName) == 0x0)
					{
						pOriginalFuncPtr = (PVOID)pImportThunk->u1.AddressOfData;

						DWORD dwOldProtect;

						VirtualProtect(pImportThunk, 0x1000, PAGE_READWRITE, &dwOldProtect);
						pImportThunk->u1.AddressOfData = (ULONGLONG)pHook;
						VirtualProtect(pImportThunk, 0x1000, dwOldProtect, &dwOldProtect);

						break;
					}

					pdwIATVA++;
					pImportThunk++;
				}

				if (pOriginalFuncPtr)
					break;
			}
#endif
		}

		pImportDescriptor++;
	}

	return pOriginalFuncPtr;
}