// dllmain.h : 模块类的声明。

class CCapacitorAlgoModule : public ATL::CAtlDllModuleT< CCapacitorAlgoModule >
{
public :
	DECLARE_LIBID(LIBID_CapacitorAlgoLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_CapacitorAlgo, "{0D070125-602A-4CCD-8E1E-BFCF48C83519}")
};

extern class CCapacitorAlgoModule _AtlModule;
