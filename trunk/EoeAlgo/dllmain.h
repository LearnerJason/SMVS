// dllmain.h : 模块类的声明。

class CEoeAlgoModule : public ATL::CAtlDllModuleT< CEoeAlgoModule >
{
public :
	DECLARE_LIBID(LIBID_EoeAlgoLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_EOEALGO, "{62F35416-308B-4981-91A9-081ECC489D3F}")
};

extern class CEoeAlgoModule _AtlModule;
