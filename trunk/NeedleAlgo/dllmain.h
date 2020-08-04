// dllmain.h : 模块类的声明。

class CNeedleAlgoModule : public ATL::CAtlDllModuleT< CNeedleAlgoModule >
{
public :
	DECLARE_LIBID(LIBID_NeedleAlgoLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_NEEDLEALGO, "{B1C6A254-5F8B-4BDF-A5E1-2911A95E69A4}")
};

extern class CNeedleAlgoModule _AtlModule;
