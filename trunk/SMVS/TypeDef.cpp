#include "stdafx.h"
#include "TypeDef.h"
#include <regex>

void PathAdaptToModelFiles(wstring &path,int cameraIndex)
{
	wostringstream oss;  
	oss<<L"#"<<cameraIndex;

	std::wstring wstr=oss.str(); 

	std::wregex expression;
	expression = L"^[a-zA-Z]:(((\\\\(?! )[^/:*?<>\\\"\"|\\\\]+)+\\\\?)|(\\\\)?)\\s*$";
	bool ret = std::regex_match(path, expression);
	if(ret)
	{
		if (path.find(L".shm")!= wstring::npos)
		{
			int i = path.rfind(L"\\");
			path.insert(i,wstr);
			int j = path.rfind(L".");
			path.insert(j,wstr);
		}
		if (path.find(L".reg")!= wstring::npos)
		{
	
			int i = path.rfind(L"\\");
			path.insert(i,wstr);
		}
		return;
	}
	return;
}

void ModelFilesAdaptToPath(wstring &path)
{
	CString str(path.c_str());

	if (path.find(L".shm")!= wstring::npos)
	{
		int i = str.ReverseFind('#');
		int j = str.ReverseFind('.');
		CString strpath = str.Left(i)+str.Mid(j);

		i = strpath.ReverseFind('#');
		if (i!=-1)
		{
			j = strpath.ReverseFind('\\');
			CString strpath2 = strpath.Left(i)+strpath.Mid(j);
			path = strpath2.GetString();
		}
		else
		{
			path = strpath.GetString();
		}
	}

	if (path.find(L".reg")!= wstring::npos)
	{
		int i = str.ReverseFind('#');
		int j = str.ReverseFind('\\');
		CString strpath = str.Left(i)+str.Mid(j);

		path = strpath.GetString();
	}


	return;
}