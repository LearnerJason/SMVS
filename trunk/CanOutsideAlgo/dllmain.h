// dllmain.h : 模块类的声明。

class CCanOutsideAlgoModule : public ATL::CAtlDllModuleT< CCanOutsideAlgoModule >
{
public :
	DECLARE_LIBID(LIBID_CanOutsideAlgoLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_CANOUTSIDEALGO, "{762CD1A7-F24F-4CA4-9238-5A09E436C197}")
};

extern class CCanOutsideAlgoModule _AtlModule;
