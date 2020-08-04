// dllmain.h : 模块类的声明。

class CJellyAlgoModule : public ATL::CAtlDllModuleT< CJellyAlgoModule >
{
public :
	DECLARE_LIBID(LIBID_JellyAlgoLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_JELLYALGO, "{15EE5A61-B8DB-4948-BAD8-85914A3E7C65}")
};

extern class CJellyAlgoModule _AtlModule;
