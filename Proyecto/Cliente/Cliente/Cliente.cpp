#include "pch.h"
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#pragma comment(lib, "user32.lib")

#define BUF_SIZE 1024
#define STRING_SIZE 1024

struct Segmento {
	bool servidorFlag;
	bool clienteFlag;
	char mensajeS[STRING_SIZE];
	char mensajeC[STRING_SIZE];
};
TCHAR szName[] = TEXT("Global\\MyFileMappingObject");
char mensaje[255];
Segmento *Data;
DWORD ThreadId;
DWORD WINAPI Read(void*) {
	while (true) {
		Data->clienteFlag = true;
		while (Data->clienteFlag);
		std::cout << Data->mensajeC << std::endl;
		
	}	
	
}
DWORD ThreadId1;
DWORD WINAPI Write(void *) {
	while (true) {
		std::cout << ":>" << std::endl;
		std::cin.getline(mensaje, STRING_SIZE);
		CopyMemory((PVOID)Data->mensajeS, mensaje, sizeof(mensaje));		
		Data->servidorFlag = false;
	}
	
}

int _tmain()
{
	HANDLE ThreadHandle;
	HANDLE ThreadHandle1;	
	HANDLE hMapFile;

	hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,   // read/write access
		FALSE,                 
		szName);               

	if (hMapFile == NULL)
	{
		_tprintf(TEXT("Could not open file mapping object (%d).\n"),
			GetLastError());
		return 1;
	}
	//lee mensaje
	Data = (Segmento *)MapViewOfFile(hMapFile, // handle to map object
		FILE_MAP_ALL_ACCESS,  // read/write permission
		0,
		0,
		BUF_SIZE);

	if (Data == NULL)
	{
		_tprintf(TEXT("Could not map view of file (%d).\n"),
			GetLastError());

		CloseHandle(hMapFile);

		return 1;
	}
	ThreadHandle = CreateThread(NULL, 0, Read, 0, 0, &ThreadId);
	ThreadHandle1 = CreateThread(NULL, 0, Write, 0, 0, &ThreadId1);
	
	WaitForSingleObject(ThreadHandle, INFINITE);
	WaitForSingleObject(ThreadHandle1, INFINITE);

	UnmapViewOfFile(Data);
	CloseHandle(hMapFile);

	return 0;
}

