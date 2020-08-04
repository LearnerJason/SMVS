// dllmain.h : 模块类的声明。

class CCrownCapAlgoModule : public ATL::CAtlDllModuleT< CCrownCapAlgoModule >
{
public :
	DECLARE_LIBID(LIBID_CrownCapAlgoLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_EOEALGO, "{AE0046CF-B7AE-4C39-BEB2-AD6C64DC9353}")
};

extern class CCrownCapAlgoModule _AtlModule;
