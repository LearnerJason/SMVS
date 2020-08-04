// dllmain.h : 模块类的声明。

class CPlugAlgoModule : public ATL::CAtlDllModuleT< CPlugAlgoModule >
{
public :
	DECLARE_LIBID(LIBID_PlugAlgoLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_PCCALGO, "{5A285493-8F78-44B5-AA21-7683F454B071}")
};

extern class CPlugAlgoModule _AtlModule;
