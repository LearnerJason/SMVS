#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"
#include "ui_decorator.h"
#include <vector>
#include <string>
#include <boost\algorithm\string.hpp>
#include <boost\lexical_cast.hpp>

using namespace std;
using namespace boost;

class CModelToolApp : public CWinApp
{
public:
	CModelToolApp();

public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()

	std::vector<wstring> SplitString(wstring strTarget, const wstring& strSeperator)
	{
		std::vector<wstring> vSplitRslt;
		boost::split(vSplitRslt, strTarget, boost::is_any_of(strSeperator),token_compress_on);

		for(size_t t = 0; t < vSplitRslt.size(); ++t)
		{
			boost::trim(vSplitRslt[t]);
		}
		return vSplitRslt;
	}
};

extern CModelToolApp  theApp;
extern ui_decorator   gUiDecorator;