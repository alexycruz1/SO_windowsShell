#include "pch.h"
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

#define BUF_SIZE 1024
#define STRING_SIZE 1024
TCHAR szName[] = TEXT("Global\\MyFileMappingObject");
char mensaje[STRING_SIZE];
struct Segmento {
	bool servidorFlag;
	bool clienteFlag;
	char mensajeS[STRING_SIZE];
	char mensajeC[STRING_SIZE];
};
Segmento *Data;
DWORD ThreadId;
DWORD ThreadId1;
//LEER
DWORD WINAPI Read(void*) {
	while (true) {
		Data->servidorFlag = true;
		while (Data->servidorFlag);
		std::cout << Data->mensajeS << std::endl;
	}
	
}
//ESCRIBIR
DWORD WINAPI Write(void*) {
	while (true) {
		std::cout << ":>" << std::endl;
		std::cin.getline(mensaje, STRING_SIZE);
		CopyMemory((PVOID)Data->mensajeC, mensaje, sizeof(mensaje));		
		Data->clienteFlag = false;			
	}
	
}
int _tmain()
{
	HANDLE ThreadHandle;
	HANDLE ThreadHandle1;
	HANDLE hMapFile;
	
	//crea memoria compartida
	hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    
		PAGE_READWRITE,          
		0,                       // maximum object size (high-order DWORD)
		BUF_SIZE,                // maximum object size (low-order DWORD)
		szName);                 

	if (hMapFile == NULL)
	{
		_tprintf(TEXT("Could not create file mapping object (%d).\n"),
			GetLastError());
		return 1;
	}
	Data = (Segmento *)MapViewOfFile(hMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
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

	ThreadHandle = CreateThread(NULL, 0, Read, 0,0, &ThreadId);
	ThreadHandle1 = CreateThread(NULL, 0, Write,0, 0, &ThreadId1);
	
	WaitForSingleObject(ThreadHandle, INFINITE);
	WaitForSingleObject(ThreadHandle1, INFINITE);
	
	UnmapViewOfFile(Data);
	CloseHandle(hMapFile);

	return 0;
}
