#pragma once

#include <atlcom.h>
#include <atlbase.h>
#include <string>
#include <map>
#include <vector>
#include <regex>
#include <boost\algorithm\string.hpp>
#include <boost\lexical_cast.hpp>
#include <TlHelp32.h>
#include "TypeDef.h"

using namespace std;
using namespace boost;



class utils
{
public:

	static VARENUM GetType(wstring strType)
	{
		VARENUM ret;
		ret = VT_EMPTY;
		if(strType == L"B")
			return VT_UI1;
		if(strType == L"LP")
			return VT_I4;
		if(strType == L"L")
			return VT_I4;
		if(strType == L"F" || strType == L"FP")
			return VT_R4;
		if(strType.find(L"SP") != wstring::npos)
			return VT_BSTR;

		return ret;
	}
	static string ws2s(wstring& inputws)
	{ 
		USES_CONVERSION;
		return W2CA(inputws.c_str()); 
	}

	static std::wstring s2ws(const string& s)
	{
		USES_CONVERSION;
		return A2CW(s.c_str());
	}
	static CString GetDayOfWeekStr(int nLangID,WORD dayOfWeek)
	{
		CString ret;
		if(nLangID == LANG_CHS)
		{
			if(dayOfWeek == 0) ret = _T("星期日");
			if(dayOfWeek == 1) ret = _T("星期一");
			if(dayOfWeek == 2) ret = _T("星期二");
			if(dayOfWeek == 3) ret = _T("星期三");
			if(dayOfWeek == 4) ret = _T("星期四");
			if(dayOfWeek == 5) ret = _T("星期五");
			if(dayOfWeek == 6) ret = _T("星期六");
		}
		else
		{
			if(dayOfWeek == 0) ret = _T("Sun.");
			if(dayOfWeek == 1) ret = _T("Mon.");
			if(dayOfWeek == 2) ret = _T("Tues.");
			if(dayOfWeek == 3) ret = _T("Wed.");
			if(dayOfWeek == 4) ret = _T("Thur.");
			if(dayOfWeek == 5) ret = _T("Fri.");
			if(dayOfWeek == 6) ret = _T("Sat.");
		}
		return ret;
	}

	static bool ParseThresholdSpec(THRESHOLD& threshold, wstring str)
	{
		if(SYMBOL_TYPE.empty())
			return false;

		std::vector<wstring> vStr = SplitString(str, L"$");
		size_t sz = vStr.size();
		if(sz <= 0 || sz != 2)
			return false;

		TrimString(vStr);
		wstring strComp = vStr[0];
		threshold.m_compType = GetCompType(strComp);
		if(threshold.m_compType == COMP_INVALID)
			return false;
		if(threshold.m_compType > LESS_EQUAL) // spec range
		{
			std::vector<wstring> minMax = SplitString(vStr[1], L"~");
			if(minMax.size() != 2)
				return false;
			TrimString(minMax);
			threshold.m_lcl = boost::lexical_cast<float>(minMax[0]);
			threshold.m_ucl = boost::lexical_cast<float>(minMax[1]);
		}
		else
		{
			threshold.m_specV = boost::lexical_cast<float>(vStr[1]);
		}
		return true;
	}
	static bool GetExePath(wstring& strExePath)
	{

		TCHAR exeFullPath[MAX_PATH]; 
		memset(exeFullPath,0,MAX_PATH);  
		GetModuleFileName(NULL,exeFullPath,MAX_PATH); 
		CString exePath;
		exePath.Format(_T("%s"),exeFullPath);  

		std::wstring wstrPath = CT2W(exePath);

		if(wstrPath == L"")
		{
			return false;
		}
		wstring strTmp = wstrPath;
		size_t index = strTmp.find_last_of(L"\\");
		if(index != string::npos)
		{
			strExePath = strTmp.substr(0, index);
		}
		return true;
	}

	static bool CompareWithThreshold(THRESHOLD& threshold, CComVariant &score)
	{
		HRESULT hr = score.ChangeType(VT_R4);
		if(SUCCEEDED(hr))
		{
			float fScore = score.fltVal;
			if (fScore < 0.0f)
			{
				if (fScore < -10.0f)
				{
					fScore +=10;
				}
				else
				{
					return false;
				}
			}

			if(threshold.m_compType == GREAT)
				return fScore > threshold.m_specV;
			if(threshold.m_compType == GREAT_EQUAL)
				return fScore >= threshold.m_specV;

			if(threshold.m_compType == LESS)
				return fScore < threshold.m_specV;
			if(threshold.m_compType == LESS_EQUAL)
				return fScore <= threshold.m_specV;

			if(threshold.m_compType == EQUAL)
				return fScore == threshold.m_specV;
			if(threshold.m_compType == NO_EQUAL)
				return fScore != threshold.m_specV;

			if(threshold.m_compType == IN_RANGE_CLOSE)
				return (fScore <= threshold.m_ucl && fScore  >= threshold.m_lcl);

			if(threshold.m_compType == IN_RANGE_OPEN)
				return (fScore < threshold.m_ucl && fScore > threshold.m_lcl);

			if(threshold.m_compType == IN_RANGE_LO_RC)
				return (fScore > threshold.m_lcl && fScore <= threshold.m_ucl);

			if(threshold.m_compType == IN_RANGE_LC_RO)
				return (fScore >= threshold.m_lcl && fScore < threshold.m_ucl);


			if(threshold.m_compType == OUT_RANGE_CLOSE)
				return (fScore > threshold.m_ucl || fScore  < threshold.m_lcl);

			if(threshold.m_compType == OUT_RANGE_OPEN)
				return (fScore >= threshold.m_ucl || fScore <= threshold.m_lcl);

			if(threshold.m_compType == OUT_RANGE_LO_RC)
				return (fScore <= threshold.m_lcl || fScore > threshold.m_ucl);

			if(threshold.m_compType == OUT_RANGE_LC_RO)
				return (fScore < threshold.m_lcl || fScore >= threshold.m_ucl);
		}
		return true;
	}

	static float ConvertStr2Value(wstring strValue)
	{
		if(strValue == L"N/A")
			return 0.0f;
		try
		{
			return boost::lexical_cast<float>(strValue);
		}
		catch(...)
		{
			return 0.0f;
		}
		return 0.0f;
	}

	static float to_float(wstring strValue)
	{
		if(!IsNumericString(strValue))
		{
			return -9999999.9f;
		}
		try
		{
			return boost::lexical_cast<float>(strValue);
		}
		catch(...)
		{
			return -9999999.9f;
		}
		return-9999999.9f;
	}

	static float to_float(CString strValue)
	{
		wstring wstrValue = CT2W(strValue);
		return to_float(wstrValue);
	}

	static int ConvertStr2Value(CString strValue)
	{
		wstring wstrValue = CT2W(strValue);
		int nRet = wcstol(wstrValue.c_str(), nullptr, 10);
		return nRet;
	}

	template<class T>
	static string to_string(const T& t)
	{
		std::ostringstream oss;
		oss << t;
		return oss.str();
	}

	template<class T>
	static wstring to_wstring(const T& t)
	{
		std::wostringstream oss;
		oss << t;
		return oss.str();
	}

	//支持空字符分割
	static std::vector<wstring> SplitString(wstring strTarget, wstring strSeperator,algorithm::token_compress_mode_type type = token_compress_on, bool m_bTrim = true)
	{
		std::vector<wstring> vSplitRslt;
		boost::split(vSplitRslt, strTarget, boost::is_any_of(strSeperator),type);
		if (m_bTrim)
		{
			TrimString(vSplitRslt);
		}

		return vSplitRslt;
	}

	static std::vector<string> SplitString(string strTarget, string strSeperator, bool m_bTrim = true)
	{
		std::vector<string> vSplitRslt;
		boost::split(vSplitRslt, strTarget, boost::is_any_of(strSeperator),token_compress_on);
		return vSplitRslt;
	}

	static void TrimString(std::vector<wstring>& vStr)
	{
		for(size_t t = 0; t < vStr.size(); ++t)
		{
			boost::trim(vStr[t]);
		}
	}

	static void TrimString(std::vector<string>& vStr)
	{
		for(size_t t = 0; t < vStr.size(); ++t)
		{
			boost::trim(vStr[t]);
		}
	}

	static HRESULT GetIDispatchMethods(IDispatch * pDisp,std::map<long, std::wstring>& methodsMap, std::map<long, std::wstring>& dictFuncIdHelpStr)
	{
		HRESULT hr = S_OK;
		HRESULT ret = S_OK;

		CComPtr<IDispatch> spDisp(pDisp);
		if(!spDisp)
			return E_INVALIDARG;

		CComPtr<ITypeInfo> spTypeInfo;
		hr = spDisp->GetTypeInfo(0, 0, &spTypeInfo);

		if(SUCCEEDED(hr) && spTypeInfo)
		{
			TYPEATTR *pTatt = nullptr;
			hr = spTypeInfo->GetTypeAttr(&pTatt);
			if(SUCCEEDED(hr) && pTatt)
			{
				FUNCDESC * fd = nullptr;
				methodsMap.clear();
				for(int i = 0; i < pTatt->cFuncs; ++i)
				{
					hr = spTypeInfo->GetFuncDesc(i, &fd);
					if(SUCCEEDED(hr) && fd)
					{
						CComBSTR funcName;
						CComBSTR docStr;
						spTypeInfo->GetDocumentation(fd->memid, &funcName, &docStr, NULL, NULL);
						if(funcName.Length()>0)
						{
							CComBSTR helpFuncName = funcName;
							helpFuncName.Append(L"Help");
							MEMBERID retID;
							ret = spTypeInfo->GetIDsOfNames(&helpFuncName, 1, &retID);
							if(SUCCEEDED(ret))
							{
								methodsMap[fd->memid] = funcName;

								DISPPARAMS dispParams = {NULL, NULL, 0, 0};
								CComVariant strHelp; 
								hr = pDisp->Invoke(retID, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dispParams, &strHelp, NULL, NULL);
								if(SUCCEEDED(hr))
								{
									dictFuncIdHelpStr[fd->memid] = COLE2T(strHelp.bstrVal);
								}
							}
						}

						spTypeInfo->ReleaseFuncDesc(fd);
					}
				}
				spTypeInfo->ReleaseTypeAttr(pTatt);
			}
		}
		return hr;
	}

	static HRESULT GetInterfaceHelpString(IDispatch * pDisp,wstring funcName, wstring& retStr)
	{
		if(pDisp == nullptr)
			return S_FALSE;
		wstring helpFuncName = funcName + L"Help";
		DISPID dispID;
		CComBSTR tmp(helpFuncName.c_str());
		HRESULT hr = pDisp->GetIDsOfNames(IID_NULL, &tmp, 1, LOCALE_SYSTEM_DEFAULT, &dispID);
		if(SUCCEEDED(hr))
		{
			DISPPARAMS dispParams = {NULL, NULL, 0, 0};
			CComVariant strHelp; 
			hr = pDisp->Invoke(dispID, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dispParams, &strHelp, NULL, NULL);
			if(SUCCEEDED(hr))
			{
				retStr = COLE2T(strHelp.bstrVal);
				return S_OK;
			}
		}
		return S_FALSE;
	}

	static DISPID GetInterfaceDispID(IDispatch * pDisp, wstring strFuncName)
	{
		DISPID dispID = -1;
		if(pDisp == nullptr)
			return dispID;

		CComBSTR tmp(strFuncName.c_str());
	    pDisp->GetIDsOfNames(IID_NULL, &tmp, 1, LOCALE_SYSTEM_DEFAULT, &dispID);
		return dispID;
	}

	static bool IsInterfaceSupported(IDispatch * pDisp, wstring funcName)
	{
		if(pDisp == nullptr)
			return false;
		DISPID dispID;
		CComBSTR tmpFuncName(funcName.c_str());
		HRESULT hr = pDisp->GetIDsOfNames(IID_NULL, &tmpFuncName, 1, LOCALE_SYSTEM_DEFAULT, &dispID);
		if(SUCCEEDED(hr))
		{
			return true;
		}
		return false;
	}

	static bool CheckResult(HRESULT result)
	{
		if(result == S_FALSE)
			return false;
		else if(SUCCEEDED(result))
			return true;
		else
			return false;

		return false;
	}

	static bool IsNumericString(CString strValue, bool bIntegerOnly = false)
	{
		int len = strValue.GetLength();
		if(len <= 0)
			return false;
		wstring value = CT2W(strValue);

		std::wregex wrx;
		if(bIntegerOnly)
		{
			wrx = L"\\d+$";
		}
		else
		{
			wrx = L"^-?[0-9]+([.][0-9]+){0,1}$";
		}
		bool ret = std::regex_match(value, wrx);
		if(!ret)
			return false;

		if(value.find(L".") != wstring::npos)
		{
			std::vector<wstring> vItem = SplitString(value, L".");
			if(!vItem.empty())
			{
				wstring wstrInt = vItem[0];
				if(wstrInt.size() >= 2 && wstrInt[0] == '0')
				{
					return false;
				}
			}
		}
		else
		{
			if(value.size() >= 2 && value[0] == '0')
				return false;
		}
		return ret;
	}


	static bool CheckRangeParameter(CString strRange)
	{
		if(strRange.GetLength() <= 0) return false;

		wstring wstrRange = CT2W(strRange);
		if(wstrRange.find(L"~") == wstring::npos)
		{
			return false;
		}
		std::vector<wstring> vItem = utils::SplitString(wstrRange, L"~");
		if(vItem.size() == 2)
		{
			if(utils::IsNumericString(vItem[0]) && utils::IsNumericString(vItem[1]))
			{
				float v1 = boost::lexical_cast<float>(vItem[0]);
				float v2 = boost::lexical_cast<float>(vItem[1]);
				if(v1 < v2)
				{
					return true;
				}
			} 
		}
		return false;
	}

	static bool IsNumericString(wstring strValue,bool bIntegerOnly = false)
	{
		if(strValue.empty())
			return false;
		std::wregex wrx;
		if(bIntegerOnly)
		{
			wrx = L"\\d+$";
		}
		else
		{
			wrx = L"^[+-]?[0-9]+(?:\.[0-9]{1,6})?$";
		}
		bool ret = std::regex_match(strValue, wrx);
		return ret;
	}

	enum em_ParamElementType{
		Type_Number_Normal,			//数字
		Type_Positive_integer,		//正整数
		Type_NonNegative_integer,   //非负整数
	};
	static bool IsNumericStringEx(wstring strValue,em_ParamElementType IType = Type_Number_Normal)
	{
		if(strValue.empty())
			return false;
		std::wregex wrx;

		switch(IType)
		{
		case Type_Number_Normal:
			wrx = L"^[+-]?[0-9]+(?:\.[0-9]{1,6})?$";
			break;
		case Type_Positive_integer	:
			wrx = L"^[0-9]*[1-9][0-9]*$";
			break;
		case Type_NonNegative_integer:
			wrx = L"\\d+$";
			break;
		default:
			break;
		}
		bool ret = std::regex_match(strValue, wrx);
		return ret;
	}

	static bool IsNumericStringEx(CString strValue,em_ParamElementType IType = Type_Number_Normal)
	{
		int len = strValue.GetLength();
		if(len <= 0)
			return false;

		std::wregex wrx;
		wstring value = CT2W(strValue);

		switch(IType)
		{
		case Type_Number_Normal:
			wrx = L"^[+-]?[0-9]+(?:\.[0-9]{1,6})?$";
			break;
		case Type_Positive_integer	:
			wrx = L"^[0-9]*[1-9][0-9]*$";
			break;
		case Type_NonNegative_integer:
			wrx = L"\\d+$";
			break;
		default:
			break;
		}
		bool ret = std::regex_match(value, wrx);
		return ret;
	}


	static CString GetTimeMinuteString(long nTime)
	{
		CString strTime;
		int nMinute = nTime % 100;
		int nHour   = (nTime - nMinute) / 100;
		strTime.Format(_T("%02d"), nHour);
		strTime.Append(_T(" : "));

		CString strMinute;
		strMinute.Format(_T("%02d"), nMinute);
		strTime.Append(strMinute);

		return strTime;
	}

	static std::vector<COMPARATOR> SYMBOL_TYPE;

	static COMPARE_TYPE GetCompType(wstring strComp)
	{
		if(SYMBOL_TYPE.empty())
			return COMP_INVALID;
		boost::trim(strComp);
		for(auto it = SYMBOL_TYPE.begin(); it != SYMBOL_TYPE.end(); it++)
		{
			if(it->m_strComp == strComp)
			{
				return it->m_compType;
			}
		}

		return COMP_INVALID;
	}

	static std::vector<COMPARATOR> InitDictionary()
	{
		std::vector<COMPARATOR> ret;
		ret.push_back(COMPARATOR(L">",GREAT));
		ret.push_back(COMPARATOR(L">=", GREAT_EQUAL));
		ret.push_back(COMPARATOR(L"<", LESS));
		ret.push_back(COMPARATOR(L"<=", LESS_EQUAL));
		ret.push_back(COMPARATOR(L"=", EQUAL));
		ret.push_back(COMPARATOR(L"!=", NO_EQUAL));
		ret.push_back(COMPARATOR(L"[I]", IN_RANGE_CLOSE));
		ret.push_back(COMPARATOR(L"(I)", IN_RANGE_OPEN));
		ret.push_back(COMPARATOR(L"(I]", IN_RANGE_LO_RC));
		ret.push_back(COMPARATOR(L"[I)", IN_RANGE_LC_RO));
		ret.push_back(COMPARATOR(L"[O]", OUT_RANGE_CLOSE));
		ret.push_back(COMPARATOR(L"(O)", OUT_RANGE_OPEN));
		ret.push_back(COMPARATOR(L"(O]", OUT_RANGE_LO_RC));
		ret.push_back(COMPARATOR(L"[O)", OUT_RANGE_LC_RO));
		return ret;
	}

	static wstring Utf8ToUnicode(const string& str)
	{
		wstring tmpStr(L"");
		int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, str.c_str(), (int)strlen(str.c_str()), NULL, 0);
		if(wcsLen <=0)
			return tmpStr;
		wstring retWstr(wcsLen, '\0');
		::MultiByteToWideChar(CP_UTF8, NULL, str.c_str(), (int)strlen(str.c_str()), &retWstr[0], wcsLen);

		return retWstr;
	}

	static string Unicode2Utf8(const wstring& unicodeStr)
	{
		std::string retStr("");
		int strLen = (int)::WideCharToMultiByte(CP_UTF8, NULL, unicodeStr.c_str(), (int)wcslen(unicodeStr.c_str()), NULL, 0, NULL, NULL);
		if(strLen == 0)
			return retStr;
		std::vector<char> strBuffer(strLen, '\0');
		::WideCharToMultiByte(CP_UTF8, NULL, unicodeStr.c_str(), (int)wcslen(unicodeStr.c_str()), &strBuffer[0], strLen, NULL, NULL);

		retStr.assign(strBuffer.begin(), strBuffer.end());
		return retStr;
	}

	static wstring AnsiToUnicode(const string& str)
	{
		wstring tmpStr(L"");
		int wcsLen = ::MultiByteToWideChar(CP_ACP, NULL, str.c_str(), (int)strlen(str.c_str()), NULL, 0);
		if(wcsLen <=0)
			return tmpStr;
		wstring retWstr(wcsLen, '\0');
		::MultiByteToWideChar(CP_ACP, NULL, str.c_str(), (int)strlen(str.c_str()), &retWstr[0], wcsLen);
		return retWstr;
	}

	static string Unicode2Ansi(const wstring& unicodeStr)
	{
		std::string retStr("");
		int strLen = (int)::WideCharToMultiByte(CP_ACP, NULL, unicodeStr.c_str(), (int)wcslen(unicodeStr.c_str()), NULL, 0, NULL, NULL);
		if(strLen == 0)
			return retStr;
		std::vector<char> strBuffer(strLen, '\0');
		::WideCharToMultiByte(CP_ACP, NULL, unicodeStr.c_str(), (int)wcslen(unicodeStr.c_str()), &strBuffer[0], strLen, NULL, NULL);

		retStr.assign(strBuffer.begin(), strBuffer.end());
		return retStr;
	}

	static string TChar2Utf8(const CString& tStr)
	{
		wstring wstr = CT2W(tStr);
		return Unicode2Utf8(wstr);
	}
	static CString DateTime2CString(const SYSTEMTIME& dateTime)
	{
		CString str;
		str.Format(_T("%.4d-%.2d-%.2d %.2d:%.2d:%.2d:%.3d"),
			dateTime.wYear,
			dateTime.wMonth,
			dateTime.wDay,
			dateTime.wHour,
			dateTime.wMinute,
			dateTime.wSecond,
			dateTime.wMilliseconds);
		return str;
	}

	static long long DateTime2Int64(const SYSTEMTIME& dateTime)
	{
		long long ret = -1;
		ret = dateTime.wYear   * 10000000000LL 
			+ dateTime.wMonth  * 100000000 
			+ dateTime.wDay    * 1000000 
			+ dateTime.wHour   * 10000 
			+ dateTime.wMinute * 100 
			+ dateTime.wSecond;

		return ret;
	}

	static SYSTEMTIME Integer2Datetime(long long nDatetime)
	{
		SYSTEMTIME sysDt;

		sysDt.wYear = WORD(nDatetime / 10000000000LL);
		nDatetime = nDatetime % 10000000000LL ;

		sysDt.wMonth = WORD(nDatetime / 100000000);
		nDatetime = nDatetime % 100000000;

		sysDt.wDay = WORD(nDatetime / 1000000);
		nDatetime = nDatetime % 1000000;

		sysDt.wHour = WORD(nDatetime / 10000);
		nDatetime = nDatetime % 10000;

		sysDt.wMinute = WORD(nDatetime / 100);
		sysDt.wSecond = WORD(nDatetime % 100);

		return sysDt;
	}

	static long long ParseDateTimeFromByteArray(std::vector<BYTE> vBytes)
	{
		if(vBytes.empty()) return -1;
		string strProductDt;
		
		for(auto i = 0;i< vBytes.size();i++)
		{
			if(vBytes[i] == 0xFF)
			{
				continue;
			}
			std::vector<char> tmp(3,0);
			size_t sz = sprintf(&tmp[0],"%02d",vBytes[i]);
			string strTmp;
			strTmp.assign(tmp.begin(),tmp.begin() + sz);
			strProductDt.append(strTmp);
		}
		if(strProductDt.empty())
		{
			return 0;
		}
		long long nProductTime = boost::lexical_cast<long long>(strProductDt);
		return nProductTime;
	}

	static std::vector<BYTE> DateTime2Bytes(const SYSTEMTIME& dateTime)
	{
		std::vector<BYTE> vRet;

		vRet.push_back(BYTE(dateTime.wYear / 100));
		vRet.push_back(BYTE(dateTime.wYear % 100));
		vRet.push_back(BYTE(dateTime.wMonth));
		vRet.push_back(BYTE(dateTime.wDay));
		vRet.push_back(BYTE(dateTime.wHour));
		vRet.push_back(BYTE(dateTime.wMinute));
		vRet.push_back(BYTE(dateTime.wSecond));

		return vRet;
	}

	static long long GetValueFromBytes(std::vector<BYTE>& vByte)
	{
		long long nRet = -1;
		string strValue;
		for(int t = vByte.size() - 1; t >= 0; t--)
		{
			std::vector<char> tmp(2,0);
			if(vByte[t] != 0xFF)
			{
				size_t sz = sprintf(&tmp[0],"%d",vByte[t]);
				string strTmp;
				strTmp.assign(tmp.begin(), tmp.begin() + sz);
				strValue.append(strTmp);
			}
			else
			{
				strValue.push_back('0');
			}
		}
		nRet = boost::lexical_cast<long long>(strValue);
		return nRet;
	}

	static std::vector<BYTE> ConvertValue2ByteArray(long long nTimeSpan)
	{
		std::vector<BYTE> vRet;
		while(nTimeSpan != 0)
		{
			vRet.push_back(BYTE(nTimeSpan % 10));
			nTimeSpan = nTimeSpan / 10;
		}
		return vRet;
	}

	static void PrintDebugSymbol(Hobject& img)
	{
		Hobject ImageOneChannel;
		Hobject imgEx;

		select_obj(img,&imgEx,1);
		access_channel (imgEx, &ImageOneChannel, 1);

		set_grayval(ImageOneChannel, 0, 0, (int)'d');
		set_grayval(ImageOneChannel, 1, 0, (int)'e');
		set_grayval(ImageOneChannel, 2, 0, (int)'b');
		set_grayval(ImageOneChannel, 3, 0, (int)'u');
		set_grayval(ImageOneChannel, 4, 0, (int)'g');
	}

	static void RemoveDebugSymbol(Hobject& img)
	{
		Hobject ImageOneChannel;
		Hobject imgEx;
		select_obj(img,&imgEx,1);
		access_channel (imgEx, &ImageOneChannel, 1);
		set_grayval(ImageOneChannel, 0, 0, (int)'n');
	}

	static string ExtractTimestampString(Hobject& img)
	{
		Hlong nMillSecond = 0;
		Hlong nSecond  = 0;
		Hlong nMinute  = 0;
		Hlong nHour    = 0;
		Hlong nDay     = 0;
		Hlong nYDay    = 0;
		Hlong nMonth   = 0;
		Hlong nYear    = 0;
		Hlong numberObj=0;
		count_obj(img,&numberObj);
		if(numberObj>1)
		{
			Hobject img0;
			select_obj(img,&img0,1);
			get_image_time(img0,&nMillSecond,&nSecond,&nMinute,&nHour,&nDay,&nYDay,&nMonth,&nYear);
		}
		else
			get_image_time(img,&nMillSecond,&nSecond,&nMinute,&nHour,&nDay,&nYDay,&nMonth,&nYear);

		string retStr = "";

		std::ostringstream oss;
		oss << std::setfill('0') << std::setw(4) << (int)nYear << "-";
		oss << std::setfill('0') << std::setw(2) << (int)nMonth << "-";
		oss << std::setfill('0') << std::setw(2) << (int)nDay << "  ";
		oss << std::setfill('0') << std::setw(2) << (int)nHour << ":";
		oss << std::setfill('0') << std::setw(2) << (int)nMinute << ":";
		oss << std::setfill('0') << std::setw(2) << (int)nSecond << ":";
		oss << std::setfill('0') << std::setw(3) << nMillSecond;
		retStr = oss.str();
		return retStr;
	}

	static long long ExtractTimestamp(Hobject img)
	{
		if(img.Id() == H_EMPTY_REGION)
		{
			return 0;
		}
		long long ret = 0;

		Hlong nMillSecond = 0;
		Hlong nSecond  = 0;
		Hlong nMinute  = 0;
		Hlong nHour    = 0;
		Hlong nDay     = 0;
		Hlong nYDay    = 0;
		Hlong nMonth   = 0;
		Hlong nYear    = 0;

		get_image_time(img,&nMillSecond,&nSecond,&nMinute,&nHour,&nDay,&nYDay,&nMonth,&nYear);

		ret = (long long)nYear * 10000000000000;
		ret += (long long)nMonth * 100000000000;
		ret += (long long)nDay * 1000000000;
		ret += (long long)nHour * 10000000;
		ret += (long long)nMinute * 100000;	
		ret += (long long)nSecond * 1000;
		ret += (long long)nMillSecond ;

		return ret;
	}

	static CTime Int64ToDateTime(long long nTimestamp)
	{
		long long nSecond = nTimestamp % 100;
		nTimestamp /= 100;
		
		long long nMinute = nTimestamp % 100;
		nTimestamp /= 100;

		long long nHour = nTimestamp % 100;
		nTimestamp /= 100;

		long long nDay = nTimestamp % 100;
		nTimestamp /= 100;

		long long nMonth = nTimestamp % 100;
		nTimestamp /= 100;

		long long nYear = nTimestamp;

		CTime ret((int)nYear, (int)nMonth, (int)nDay, (int)nHour, (int)nMinute, (int)nSecond);
		return ret;
	}

	static long long TimeSpan(long long bgnTime, long long endTime) // return value is the time span(seconds)
	{
		CTime bgn = Int64ToDateTime(bgnTime);
		CTime end = Int64ToDateTime(endTime);
		CTimeSpan timeSpan = end - bgn;
		
		return timeSpan.GetTotalSeconds();
	}

	static float RoundOffValue(float value, float fStep)
	{
		float ret = value;
		int nValue = int(value * 100.0f);
		int nStep  = int(fStep * 100.0f);
		if(nValue % nStep == 0)
		{
			return ret;
		}
		else
		{
			int nRet = nValue - (nValue % nStep) + nStep;
			ret = float(nRet / 100.0f);
		}
		return ret;
	}

	static float RoundOffValue2Integer(float value)
	{
		float ret = value;
		int nValue = (int)value;
		float delta = value - nValue;
		if(delta < 0.5f)
			ret = nValue * 1.0f;
		else
			ret = nValue + 1.0f;

		return ret;
	}

	static bool CheckSpecialCharacter(wstring str)
	{
		if(str.empty())
			return false;

		bool bGet = false;
		wstring wstrSpecial(L"~!@#$%^&*()-+={}[]|'\\:;'\",。——.<>?\/《》");
		std::for_each(str.begin(), str.end(), [&](wchar_t ch)
		{
			auto it = wstrSpecial.find(ch);
			if(it != wstring::npos)
			{
				bGet = true;
			}
		});
		if(bGet)
		{
			return true;
		}

		std::wregex expression;
		expression = L"^[a-zA-Z0-9_\u4e00-\u9fa5]+$";
		bool ret = std::regex_match(str, expression);
		if(!ret)
		{
			return true;
		}

		wchar_t wch[2];
		wch[0] = str.front();
		wch[1] = 0;
		wstring strFirst = wch;
		return IsNumericString(strFirst);
	}

	static DWORD GetProcessIdFromName(LPCWSTR name)
	{   
		PROCESSENTRY32   pe;
		DWORD   id   =   0;   
		HANDLE   hSnapshot   =   CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);   
		pe.dwSize = sizeof(PROCESSENTRY32);   
		if(!Process32First(hSnapshot,&pe))
		{
			::CloseHandle(hSnapshot);
			hSnapshot = NULL;
			return  0;
		}
		while(1)   
		{   
			pe.dwSize   =   sizeof(PROCESSENTRY32);   
			if(Process32Next(hSnapshot,&pe)==FALSE)   
				break;  
			if(_wcsicmp(pe.szExeFile,name) == 0)   
			{   
				id   =   pe.th32ProcessID;   
				break;   
			}
		};   
		CloseHandle(hSnapshot); 
		hSnapshot = NULL;
		return   id;   
	}

	static bool CheckFileExists(const wstring& path)
	{
		if(path == L"")
		{
			return false;
		}
		if(INVALID_FILE_ATTRIBUTES == (::GetFileAttributes(path.c_str())))
		{
			return false;
		}
		return true;
	}

	static bool CheckFileExists(const CString& path)
	{
		if(path.GetLength() <= 0)
		{
			return false;
		}
		if(INVALID_FILE_ATTRIBUTES == (::GetFileAttributes(path)))
		{
			return false;
		}
		return true;
	}

	static bool  CreateFullDirectory(std::wstring wstrDir)
	{
		std::vector<wstring> vItems = SplitString(wstrDir, L"\\");

		size_t sz = vItems.size();
		wstring wstrCrtPath;
		for(size_t t = 0; t < sz; t++)
		{
			wstrCrtPath.append(vItems[t]);
			if(!CheckFileExists(wstrCrtPath))
			{
				::_wmkdir(wstrCrtPath.c_str());
			}
			if(t != sz - 1)
				wstrCrtPath.append(L"\\");
		}
		return CheckFileExists(wstrDir);
	}

	static bool DoRemoveDirectory(CString chrDirName)
	{
		CString strDirName = chrDirName;

		bool result;
		HANDLE Handle;
		WIN32_FIND_DATA fData;
		CString strTemp;
		DWORD errorcode;

		Handle = FindFirstFile(strDirName + _T("\\*.*"), &fData);

		if (Handle == INVALID_HANDLE_VALUE)
			return false;

		do 
		{
			errorcode = GetLastError();
			if (fData.cFileName[0] == '.')
				continue;

			if (fData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			{
				if (strDirName[strDirName.GetLength()-1] != '\\')
				{
					TCHAR chA[MAX_PATH];
					CString strA = strDirName+'\\'+ fData.cFileName;
					lstrcpy(chA, strA);
					DoRemoveDirectory(chA);
				}
				else
				{
					TCHAR chB[MAX_PATH];
					CString strB = strDirName + fData.cFileName;
					lstrcpy(chB, strB);
					DoRemoveDirectory(chB);
				}

				strTemp = strDirName + _T("\\") + fData.cFileName;
				SetFileAttributes(strTemp, ~FILE_ATTRIBUTE_READONLY);
				if (!RemoveDirectory(strTemp))
					result = false;
				else
					result = true;
			}
			else
			{
				strTemp = strDirName + _T("\\") + fData.cFileName;
				BOOL bl = SetFileAttributes(strTemp, ~FILE_ATTRIBUTE_READONLY);

				if (!DeleteFile(strTemp))
					result = false;
				else
					result = true;
			}

		}while(FindNextFile(Handle,&fData));

		errorcode = GetLastError();

		if (errorcode == ERROR_NO_MORE_FILES)
		{
			::RemoveDirectory(strDirName);
			result = true;
		}

		if (Handle)
			FindClose(Handle);

		return result;

	}
	static bool DeleteDirectory(CString chrDirName)
	{
		bool bDeleted  = DoRemoveDirectory(chrDirName);
		if(bDeleted)
		{
			::RemoveDirectory(chrDirName);
		}
		else
		{
			return false;
		}
		return true;
	}
	static bool CopyFiles(wstring wstrSrcPath, wstring wstrTargerPath)
	{
		if(!CheckFileExists(wstrSrcPath) || !CheckFileExists(wstrTargerPath))
		{
			return false;
		}
		SHFILEOPSTRUCT fop;
		::memset((void*)&fop, 0x00, sizeof(SHFILEOPSTRUCT));

		fop.wFunc = FO_COPY;
		fop.hwnd  = NULL;
		fop.hNameMappings = NULL;
		fop.lpszProgressTitle = NULL;

		std::vector<wchar_t> vFrom(wstrSrcPath.size() + 2, 0x00);
		vFrom.assign(wstrSrcPath.begin(), wstrSrcPath.end());
		std::vector<wchar_t> vTo(wstrTargerPath.size()+ 2, 0x00);
		vTo.assign(wstrTargerPath.begin(), wstrTargerPath.end());
		fop.pFrom   = (LPCWSTR)&vFrom[0];
		fop.pTo     = (LPCWSTR)&vTo[0];
		fop.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR;
		int nRet = SHFileOperation(&fop);
		return (nRet == 0);
	}

	static wstring CompType2String(COMPARE_TYPE compType)
	{
		wstring wstrRet;
		if(compType == GREAT)           return L">";
		if(compType == GREAT_EQUAL)     return L">=";
		if(compType == EQUAL)           return L"=";
		if(compType == NO_EQUAL)        return L"!=";
		if(compType == LESS)            return L"<";
		if(compType == LESS_EQUAL)      return L"<=";
		if(compType == IN_RANGE_OPEN)   return L"(I)";
		if(compType == IN_RANGE_CLOSE)  return L"[I]";
		if(compType == IN_RANGE_LO_RC)  return L"(I]";
		if(compType == IN_RANGE_LC_RO)  return L"[I)";
		if(compType == OUT_RANGE_OPEN)  return L"(O)";
		if(compType == OUT_RANGE_CLOSE) return L"[O]";
		if(compType == OUT_RANGE_LO_RC) return L"(O]";
		if(compType == OUT_RANGE_LC_RO) return L"[O)";
		return L" ";
	}

	static IT_PARAM GenerateItParam(const FUNC_INFO& fi)
	{
		IT_PARAM retIP;
		retIP.m_itIndex = boost::lexical_cast<wstring>(fi.m_funcIndex);
		retIP.m_itName  = fi.m_strFuncName;
		retIP.m_itNickname = fi.m_strCustomizedName;

		CString strV;
		if(fi.m_bTestFunc)
		{
			wstring wstrNewParam = utils::CompType2String(fi.m_threshold.m_compType);
			wstrNewParam.append(L"$");

			THRESHOLD threshold = fi.m_threshold;
			if(threshold.m_compType > LESS_EQUAL)
			{
				CString sRange;
				if(threshold.m_type == VT_R4)
				{
					sRange.Format(_T("%.2f~%.2f"),threshold.m_lcl,threshold.m_ucl);
				}
				else
				{
					sRange.Format(_T("%d~%d"),(int)threshold.m_lcl,(int)threshold.m_ucl);
				}
				wstrNewParam.append(T2CW(sRange));
			}
			else
			{
				if(threshold.m_type == VT_R4)
					strV.Format(_T("%.2f"), threshold.m_specV);
				else
					strV.Format(_T("%d"), (long)threshold.m_specV);
				wstrNewParam.append(T2CW(strV));
			}
			retIP.m_vParam.push_back(wstrNewParam);
		}
		else
		{
			retIP.m_vParam.push_back(L"N/A");
		}

		auto vParam = fi.m_vParam;
		for(int i = 0; i < vParam.size(); i++)
		{
			wstring wstrNewParam;
			if(vParam[i].m_type == VT_R4)
			{
				strV.Format(_T("%.2f"), vParam[i].m_value);
			}
			if(vParam[i].m_type == VT_I4 || vParam[i].m_type == VT_UI1)
			{
				strV.Format(_T("%d"), (long)vParam[i].m_value);
			}
			if(vParam[i].m_type == VT_BSTR)
			{
				strV = vParam[i].m_wstrFile.c_str();
				if(strV == _T(""))
				{
					strV = _T(" ");
				}
			}
			retIP.m_vParam.push_back(T2CW(strV));
		}
		return retIP;
	}

	static int ParseValue(char chValue)
	{
		int nValue = (int)chValue;
		if(nValue < 0)
		{
			nValue += 256;
		}
		return nValue;
	}

	static int ParseValue(char chHigh, char chLow)
	{
		return ParseValue(chHigh) * 256 + ParseValue(chLow);
	}

	static int ParseValue(char ch0, char ch1, char ch2, char ch3)
	{           
		int nV0 = ParseValue(ch0);
		int nV1 = ParseValue(ch1);
		int nV2 = ParseValue(ch2);
		int nV3 = ParseValue(ch3);
		return nV0 * 256 * 256 * 256 + nV1 * 256 * 256 + nV2 * 256 + nV3;
	}

	static int ParseValue(char ch0, char ch1, char ch2)
	{           
		int nV0 = ParseValue(ch0);
		int nV1 = ParseValue(ch1);
		int nV2 = ParseValue(ch2);
		return nV0 * 256 * 256+ nV1 * 256 + nV2;
	}

	static CRect GetTvPanelRect(int nTitleH, int nChIndex, int nChCount, CRect rcTvScreen)
	{
		rcTvScreen.right -= rcTvScreen.left;
		rcTvScreen.left = 0;
		int nDelta = 5;
		CRect rcRet(0,0,0,0);
		if(nChCount <= 2)
		{
			rcRet.left   = rcTvScreen.left + long((nChIndex - 1) * (1.0 * rcTvScreen.Width() / nChCount)) + nDelta;
			rcRet.top    = nTitleH + nDelta;
			rcRet.right  = rcRet.left + long(1.0 * rcTvScreen.Width() / nChCount) - nDelta;
			rcRet.bottom = rcTvScreen.bottom - nDelta;
			return rcRet;
		}
		else
		{
			int nTmpCount = nChCount + nChCount % 2;
			int nVerBlock = int(nTmpCount / 2.0);
			if(nChIndex <= nVerBlock)
			{
				rcRet.left   = rcTvScreen.left + long((nChIndex - 1) * (1.0 * rcTvScreen.Width() / nVerBlock)) + nDelta;
				rcRet.top    = nTitleH + nDelta;
				rcRet.right  = rcRet.left + long(1.0 * rcTvScreen.Width() / nVerBlock) - nDelta;
				rcRet.bottom = rcRet.top + long((rcTvScreen.Height() - nTitleH) / 2.0) - nDelta;
			}
			else
			{
				rcRet.left   = rcTvScreen.left + long((nChIndex - nVerBlock - 1) * (1.0 * rcTvScreen.Width() / nVerBlock)) + nDelta;
				rcRet.top    = nTitleH + long((rcTvScreen.Height() - nTitleH) / 2.0) + nDelta;
				rcRet.right  = rcRet.left + long(1.0 * rcTvScreen.Width() / nVerBlock) - nDelta;
				rcRet.bottom = rcTvScreen.bottom - nDelta;
			}
			return rcRet;
		}
		return rcRet;
	}

	static wstring GetRegionName(int nFuncIndex, const TASK& task)
	{
		wstring wstrRet(L"");
		if(nFuncIndex <= 0 || task.m_vItParam.size() < nFuncIndex)
		{
			return wstrRet;
		}
		while(--nFuncIndex)
		{
			auto funcItem = task.m_vItParam[nFuncIndex];
			if(!funcItem.IsTestAlgo())
			{
				return funcItem.m_itNickname;
			}
		}
		return wstrRet;
	}

	static std::vector<wstring> GetAllSerialPort()
	{
		std::vector<wstring> vRet;
		HKEY hKey;
		int nRet = ::RegOpenKeyExW(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DEVICEMAP\\SERIALCOMM"), NULL,KEY_READ, &hKey);
		if(nRet == ERROR_SUCCESS)
		{
			int i = 0; 
			while(1)  
			{  
				TCHAR portName[256],commName[256];  
				DWORD dwLong = sizeof(portName);
				DWORD dwSize = dwLong; 
				int nRslt = ::RegEnumValueW(hKey, i,portName, &dwLong,NULL, NULL, (PUCHAR)commName,&dwSize);
				if(nRslt == ERROR_NO_MORE_ITEMS)
					break; 
				CString sPort = commName;
				vRet.push_back(sPort.GetString());
				i++;
			}
			::RegCloseKey(hKey);  
		}
		return vRet;
	}

	static std::string bytes_to_hexstr(unsigned char* first, unsigned char* last)
	{
		std::ostringstream oss;
		oss << std::hex << std::setfill('0');
		while(first<last)
			oss << std::setw(2) << int(*first++);
		return oss.str();
	}

//#ifdef _DEBUG
//	static RECT NormalizeRect(RECT r)
//	{
//		int t;
//
//		if( r.left > r.right )
//		{
//			t = r.left;
//			r.left = r.right;
//			r.right = t;
//		}
//
//		if( r.top > r.bottom )
//		{
//			t = r.top;
//			r.top = r.bottom;
//			r.bottom = t;
//		}
//
//		return r;
//	}
//
//	static CvRect RectToCvRect(RECT sr)
//	{
//		sr = NormalizeRect( sr );
//		return cvRect( sr.left, sr.top, sr.right - sr.left, sr.bottom - sr.top );
//	}
//
//	static RECT CvRectToRect(CvRect sr)
//	{
//		RECT dr;
//		dr.left = sr.x;
//		dr.top = sr.y;
//		dr.right = sr.x + sr.width;
//		dr.bottom = sr.y + sr.height;
//
//		return dr;
//	}
//
//	static IplROI RectToROI(RECT r)
//	{
//		IplROI roi;
//		r = NormalizeRect( r );
//		roi.xOffset = r.left;
//		roi.yOffset = r.top;
//		roi.width = r.right - r.left;
//		roi.height = r.bottom - r.top;
//		roi.coi = 0;
//
//		return roi;
//	}
//
//	static void  FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin)
//	{
//		assert( bmi && width >= 0 && height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));
//
//		BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);
//
//		memset( bmih, 0, sizeof(*bmih));
//		bmih->biSize = sizeof(BITMAPINFOHEADER);
//		bmih->biWidth = width;
//		bmih->biHeight = origin ? abs(height) : -abs(height);
//		bmih->biPlanes = 1;
//		bmih->biBitCount = (unsigned short)bpp;
//		bmih->biCompression = BI_RGB;
//
//		if( bpp == 8 )
//		{
//			RGBQUAD* palette = bmi->bmiColors;
//			int i;
//			for( i = 0; i < 256; i++ )
//			{
//				palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
//				palette[i].rgbReserved = 0;
//			}
//		}
//	}
//
//	static void DisplayCvImage(CWnd* pWnd ,const CV_IMAGE& img)
//	{
//		if(!img.m_img.empty() && pWnd != nullptr)
//		{
//			CDC* pDC = pWnd->GetDC();
//			HDC hDC = pDC->GetSafeHdc();
//			CRect rc;
//			pWnd->GetWindowRect(rc);
//			::ReleaseDC(pWnd->m_hWnd,hDC);
//		}
//	}
//#endif
};

class chronograph
{
public:
	chronograph()
	{
		QueryPerformanceFrequency(&m_freq);
		QueryPerformanceCounter(&m_bgn);
	}
	void start()
	{
		QueryPerformanceCounter(&m_bgn);
	}

	double duration()
	{
		QueryPerformanceCounter(&m_end);
		return (m_end.QuadPart - m_bgn.QuadPart) * 1000.0 / m_freq.QuadPart;
	}
	long long duration_nan()
	{
		QueryPerformanceCounter(&m_end);
		return (m_end.QuadPart - m_bgn.QuadPart) * 1000000.0 / m_freq.QuadPart;
	}
	LARGE_INTEGER now()
	{
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return now;
	}
	double DoubleNow()
	{
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return now.QuadPart*1000.0/m_freq.QuadPart;
	}
	long long DoubleNowEx()
	{
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return (long long)(now.QuadPart*1000000.0/m_freq.QuadPart);
	}
private:
	LARGE_INTEGER m_freq;
	LARGE_INTEGER m_bgn;
	LARGE_INTEGER m_end;
};
