// dllmain.h : 模块类的声明。

class CPccAlgoModule : public ATL::CAtlDllModuleT< CPccAlgoModule >
{
public :
	DECLARE_LIBID(LIBID_PccAlgoLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_PCCALGO, "{5A285493-8F78-44B5-AA21-7683F454B071}")
};

extern class CPccAlgoModule _AtlModule;
