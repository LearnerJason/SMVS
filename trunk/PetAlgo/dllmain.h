// dllmain.h : 模块类的声明。

class CPetAlgoModule : public ATL::CAtlDllModuleT< CPetAlgoModule >
{
public :
	DECLARE_LIBID(LIBID_PetAlgoLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_PETALGO, "{7D763E8F-5754-42C7-8044-31493DAED5CA}")
};

extern class CPetAlgoModule _AtlModule;
