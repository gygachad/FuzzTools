#pragma once

#define VA_TO_RAW(Base, Va) (PVOID)((PBYTE)(Base) + (UINT)(Va))

#define PAGE_ALIGN(Addr) ((PVOID)(Addr) & ~7)

PVOID
SetImportTableHook(
	char* ImportDllName,
	char* ExportDllName,
	char* FunctionName,
	PVOID	pHook
);