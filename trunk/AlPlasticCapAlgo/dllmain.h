// dllmain.h : 模块类的声明。

class CAlPlasticCapAlgoModule : public ATL::CAtlDllModuleT< CAlPlasticCapAlgoModule >
{
public :
	DECLARE_LIBID(LIBID_AlPlasticCapAlgoLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_AlPlasticCapAlgo, "{62F35416-308B-4981-91A9-081ECC489D3F}")
};

extern class CAlPlasticCapAlgoModule _AtlModule;
