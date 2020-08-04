#pragma once

#include <string>

using namespace std;

template<class T = char,unsigned int SIZE = 128>
class CBaseShareMem
{
public:
	CBaseShareMem()
	{
		m_hMem   = NULL;
		m_pBuff  = NULL;
		m_hMutex = NULL;
	}
	CBaseShareMem(LPCWSTR szName)
	{
		m_strName = szName;
		m_hMem = ::CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,sizeof(T)* SIZE,szName);
		if (GetLastError() == ERROR_ALREADY_EXISTS) 
		{
			m_hMem = ::OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,szName);
		} 
		if(m_hMem == NULL)
		{
			//TLOG_ERROR(_T("CreateFileMapping Failed!"));
		}
		m_pBuff = (T*)::MapViewOfFile(m_hMem, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(T) * SIZE);
		//事件与互斥对象
		wstring tmpStr = szName;
		m_hMutex=::CreateMutexW(NULL,FALSE,(tmpStr + _T("-Mutex")).c_str());
		if(m_hMutex == NULL)
		{
			//TLOG_ERROR(_T("CreateMutexW %s Failed"),szName);
		}
	}
	virtual ~CBaseShareMem()
	{
		ResetShareMemory();
	}
	void Init(LPCWSTR szName)
	{
		m_strName = szName;
		m_hMem = ::CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,sizeof(T)* SIZE,szName);
		if (GetLastError() == ERROR_ALREADY_EXISTS) 
		{
			m_hMem = ::OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,szName);
		} 
		if (m_hMem == NULL)
		{
			//TODO:
		}
		m_pBuff = (T*)::MapViewOfFile(m_hMem, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(T) * SIZE);

		wstring tmpStr = szName;
		m_hMutex=::CreateMutexW(NULL,FALSE,(tmpStr + _T("-Mutex")).c_str());
		if (m_hMutex == NULL)
		{
			//TODO
		}
	}
	void ResetShareMemory()
	{
		if(m_hMem != NULL)
		{
			::CloseHandle(m_hMem);
			m_hMem = NULL;
		}
		if(m_pBuff != NULL)
		{
			::UnmapViewOfFile(m_pBuff);
			m_pBuff = NULL;
		}
		if(m_hMutex != NULL)
		{
			::CloseHandle(m_hMutex);
			m_hMutex = NULL;
		}

	}
	void Lock()
	{
		::WaitForSingleObject(m_hMutex, INFINITE);
	}
	void Unlock()
	{
		::ReleaseMutex(m_hMutex);
	}
protected:
	T*      m_pBuff;
	HANDLE	m_hMem;
	HANDLE	m_hMutex;
	wstring m_strName;
};

template<class T = char, unsigned int SIZE = 256> // 
class CFileMappingProxy : public CBaseShareMem<T,SIZE>
{
public:
	CFileMappingProxy()
	{
	}
	CFileMappingProxy(LPCWSTR szName):CBaseShareMem<T,SIZE>(szName)
	{
		::memset((void*)m_infoBuffer, 0x00, sizeof(T) * SIZE);
	}
	~CFileMappingProxy()
	{
	}
	void EnableWriteBuff()
	{
		Lock();
		::memset((void*)m_pBuff, 0x00, SIZE);
		Unlock();
	}
	void DisableWriteBuff()
	{
		Lock();
		::memset((void*)m_pBuff, 0x00, SIZE);
		Unlock();
	}
	bool Read(size_t typeSz, T* pDst, size_t szDst)
	{
		if (m_pBuff == nullptr || pDst == nullptr)
			return false;
		if(szDst <= 0)
			return false;
		Lock();
		::memcpy_s((void*)pDst, typeSz*szDst, (void*)m_pBuff, SIZE*typeSz);
		Unlock();
		return true;
	}
	bool Write(size_t typeSz,const T* pSrc, size_t sz)
	{
		if (m_pBuff == nullptr /*|| pSrc == nullptr*/)
			return false;
		if(sz <= 0) 
			return false;
		if(sz > SIZE)
			sz = SIZE;

		Lock();
		::memset((void*)m_pBuff, 0x00, typeSz*sz);
		::memcpy_s((void*)m_pBuff, sz*typeSz, (void*)pSrc, sz*sizeof(T));
		m_pBuff[sz] = T(0);
		Unlock();

		return true;
	}
private:
	T  m_infoBuffer[SIZE];
};
