// dllmain.h : 模块类的声明。

class CBottleCapAlgoModule : public ATL::CAtlDllModuleT< CBottleCapAlgoModule >
{
public :
	DECLARE_LIBID(LIBID_BottleCapAlgoLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_BOTTLECAPALGO, "{62F35416-308B-4981-91A9-081ECC489D3F}")
};

extern class CBottleCapAlgoModule _AtlModule;
