#pragma once
#include "resource.h"   
#include "JellyAlgo_i.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE Error"
#endif

using namespace ATL;
void MyHalconExceptionHandler(const Halcon::HException& except)
{
	throw except;
}
class ATL_NO_VTABLE CJellyAlgo :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CJellyAlgo, &CLSID_JellyAlgo>,
	public IDispatchImpl<IJellyAlgo, &IID_IJellyAlgo, &LIBID_JellyAlgoLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CJellyAlgo()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_JELLYALGO1)


BEGIN_COM_MAP(CJellyAlgo)
	COM_INTERFACE_ENTRY(IJellyAlgo)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		HException::InstallHHandler(&MyHalconExceptionHandler);
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:

};

OBJECT_ENTRY_AUTO(__uuidof(JellyAlgo), CJellyAlgo)
