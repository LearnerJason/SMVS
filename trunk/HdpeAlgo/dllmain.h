// dllmain.h : 模块类的声明。

class CHdpeAlgoModule : public ATL::CAtlDllModuleT< CHdpeAlgoModule >
{
public :
	DECLARE_LIBID(LIBID_HdpeAlgoLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_HDPEALGO, "{83D03FF3-1F39-45A8-8938-D1BB7BDAE3CD}")
};

extern class CHdpeAlgoModule _AtlModule;
