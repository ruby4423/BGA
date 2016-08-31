// Base.cpp: implementation of the CBase class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Base.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBase::CBase()
{

}

CBase::~CBase()
{

}

D3DXVECTOR3 CBase::GetTriangeNormal(D3DXVECTOR3* vVertex1, D3DXVECTOR3* vVertex2, D3DXVECTOR3* vVertex3)
{
	D3DXVECTOR3 vNormal;
	D3DXVECTOR3 v1;
	D3DXVECTOR3 v2;

	D3DXVec3Subtract(&v1, vVertex2, vVertex1);
	D3DXVec3Subtract(&v2, vVertex3, vVertex1);

	D3DXVec3Cross(&vNormal, &v1, &v2);

	D3DXVec3Normalize(&vNormal, &vNormal);

	return vNormal;
}

bool CBase::m_fEnableLogging = false;

void CBase::LogError(char *lpszText, ...)
{
	
}

void CBase::LogInfo(char *lpszText, ...)
{
	
}

void CBase::LogWarning(char *lpszText, ...)
{
	
}

void CBase::StartLogging()
{
	
}

void CBase::StopLogging()
{
	
}

DWORD CBase::GetMemoryUsage()
{
	//This is slow. Don't use it inside the game loop!

	DWORD dwProcessID	= GetCurrentProcessId();
	HANDLE hSnapshot	= CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST | TH32CS_SNAPMODULE, dwProcessID);
	DWORD dwSize		= 0;		// Will contain the total memory usage when we're done!

	MODULEENTRY32 module;
	module.dwSize = sizeof(module);


	//Get memory used by modules (dlls etc)
	if (Module32First(hSnapshot, &module))
	{
  		do
  		{
   			dwSize += module.modBaseSize;
			//LogInfo("<li>%s", module.szModule);	//Log the module name that we are using
  		}
		while (Module32Next(hSnapshot, &module));
	}


	HEAPLIST32 heap;
	heap.dwSize = sizeof(heap); 

	//Get all memory used by the heap
	if (Heap32ListFirst(hSnapshot, &heap))
	{
		do
  		{
   			HEAPENTRY32 heapentry;
			heapentry.dwSize = sizeof(heapentry);
   			
			if (Heap32First(&heapentry,heap.th32ProcessID,heap.th32HeapID))
   			{
    			do
    			{
     				if (heapentry.dwFlags != LF32_FREE)		// If the block is currently used
					{
      					dwSize += heapentry.dwBlockSize;
					}
    			}
				while (Heap32Next(&heapentry));
   			}
  		}
		while (Heap32ListNext(hSnapshot,&heap));
	}

	CloseHandle(hSnapshot);

	return dwSize;
}


void CBase::LogMemoryUsage()
{
	DWORD dwMemoryUsage = GetMemoryUsage();
	float rMemoryUsage = 0.0f;
	
	if(dwMemoryUsage < 1024)
	{
		LogInfo("<li>Memory Usage: %d Bytes", dwMemoryUsage);
	}
	else if(dwMemoryUsage < 1048576)
	{
		rMemoryUsage = ((float)dwMemoryUsage / 1024.0f);
		LogInfo("<li>Memory Usage: %f KB", rMemoryUsage);
	}
	else if(dwMemoryUsage < 1073741824)
	{
		rMemoryUsage = ((float)dwMemoryUsage / 1048576.0f);
		LogInfo("<li>Memory Usage: %f MB", rMemoryUsage);
	}
	else
	{
		rMemoryUsage = ((float)dwMemoryUsage / 1073741824.0f);
		LogInfo("<li>Memory Usage: %f GB", rMemoryUsage);
	}
}
