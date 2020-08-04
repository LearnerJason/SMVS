// dllmain.h : 模块类的声明。

class CNailAlgoModule : public ATL::CAtlDllModuleT< CNailAlgoModule >
{
public :
	DECLARE_LIBID(LIBID_NailAlgoLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_NAILALGO, "{E35EE235-1F6F-40B3-8F0B-B2EB6833DE86}")
};

extern class CNailAlgoModule _AtlModule;
