// dllmain.h : 模块类的声明。

class CBatteryShellAlgoModule : public ATL::CAtlDllModuleT< CBatteryShellAlgoModule >
{
public :
	DECLARE_LIBID(LIBID_BatteryShellAlgoLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_BatteryShellAlgo, "{214A6FFD-54E1-4867-ABA6-329560BA7D1B}")
};

extern class CBatteryShellAlgoModule _AtlModule;
