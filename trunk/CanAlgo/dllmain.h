// dllmain.h : 模块类的声明。

class CCanAlgoModule : public ATL::CAtlDllModuleT< CCanAlgoModule >
{
public :
	DECLARE_LIBID(LIBID_CanAlgoLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_CANALGO, "{E67E9130-A7AA-48F8-9602-4F4C934A9F30}")
};

extern class CCanAlgoModule _AtlModule;
