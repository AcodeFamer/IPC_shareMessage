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
	base(int a,int b)
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
	void addData(base b);
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
	/*
	m_pBuffer = (LPVOID)MapViewOfFile(m_hFileHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (bServer){
		//*(DWORD*)m_pBuffer = 300;
		//base* m_pBuffer = new base(1, 2);
		base b(1, 2);
		//(*(base*)m_pBuffer).x = 300;
		//(*(base*)m_pBuffer).y = 200;
		(*(base*)m_pBuffer) = b;
	}
	else{
		printf("m_pBuffer=%d\n", *(DWORD*)m_pBuffer);
	}
	*/
	//m_pBuffer = (LPVOID)MapViewOfFile(m_hFileHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	m_pBuffer = (LPVOID*)MapViewOfFile(m_hFileHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0); m_pBuffer = (LPVOID)MapViewOfFile(m_hFileHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	m_pBase = (base*)m_pBuffer;

}

CShareMessage::~CShareMessage()
{
	UnmapViewOfFile(m_pBuffer);
	CloseHandle(m_hFileHandle);
}

void CShareMessage::addData(base b)
{
	//m_pBuffer = (base*)m_pBuffer;
		//(*(base*)m_pBuffer).x = 300;
		//(*(base*)m_pBuffer).y = 200;
		(*m_pBase) = b;
		m_pBase++;
	
}

int main(int argc, char **argv[]){
	CShareMessage ser(_T("MyData"), sizeof(DWORD), 1);
	base b1(1, 2),b2(2,3);
	ser.addData(b1);
	ser.addData(b2);
	Sleep(10000);
}