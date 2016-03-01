#include <windows.h>
#include <process.h>
#include <stdlib.h>
#include <stdio.h>
#include <TCHAR.h>
#include <iostream>
using namespace std;

class base
{
public:
	base(int a, int b)
	{
		x = a; y = b;
	}
	int x, y;
};

class CShareMessage
{
public:
	CShareMessage(LPCTSTR lpszFileName, int nFileSize, int bServer);
	~CShareMessage();
private:
	HANDLE m_hFileHandle;
	LPVOID m_pBuffer;
	base* m_pBase;
};

CShareMessage::CShareMessage(LPCTSTR lpszFileName, int nFileSize, int bServer)
{
	if (bServer)
	{
		m_hFileHandle = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, nFileSize, lpszFileName);

	}
	else
	{
		m_hFileHandle = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, lpszFileName);
	}

	m_pBuffer = (LPVOID)MapViewOfFile(m_hFileHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	
	if (bServer){
		*(DWORD*)m_pBuffer = 100;
	}
	else{
		base* pbase = (base*)m_pBuffer;
		//printf("m_pBuffer=%d\n", *(DWORD*)m_pBuffer);
		printf("x=%d,y=%d\n", pbase->x, pbase->y);
		pbase++;
		printf("x=%d,y=%d\n", pbase->x, pbase->y);
	}
}

CShareMessage::~CShareMessage()
{
	UnmapViewOfFile(m_pBuffer);
	CloseHandle(m_hFileHandle);
}

int main(int argc, char **argv[]){
	CShareMessage ser(_T("MyData"), sizeof(DWORD), 0);
}