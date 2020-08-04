#pragma once
#include <map>
#include <vector>
#include "resource.h"
#include "TypeDef.h"
using namespace std;

#define SMVS_WARNING_MAXNUM 6
CString MyLoadString(int ID);

class SMVS_ERROR_INFO
{
public:
	SMVS_ERROR_INFO();


	~SMVS_ERROR_INFO()
	{
		m_mapErrorInfo.clear();
	}

	CString GetErrorInfo(int ErrorID);
public:
	std::map<int,CString> m_mapErrorInfo;
	vector<int>  m_CHErrorVec;
};