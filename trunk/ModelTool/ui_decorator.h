#pragma once
#include <afxwin.h>
#include <map>
#include <algorithm>
#include "Resource.h"

using namespace std;

enum eIMAGE_TYPE
{
	IMG_INVALID = -1,
	IMG_CLOSE_DLG = 0,
	IMG_RECTANGEL_SELECT,
	IMG_POLYGON_SELECT,
	IMG_CIRCLE_DRAW,
	IMG_RECTANGLE_DRAW,
	IMG_POLYGON_DRAW,
	IMG_UNION_ACTION,
	IMG_DIFFERENCE_ACTION,
	IMG_UNDO_ACTION
};

struct ui_decorator
{
public:
	ui_decorator()
	{
		HMODULE module = GetModuleHandle(0); 
		TCHAR pFileName[MAX_PATH]; 
		GetModuleFileName(module, pFileName, MAX_PATH); 
 
		CString csFullPath(pFileName); 
		int nPos = csFullPath.ReverseFind( _T('\\') ); 
		if(nPos <=0)
			return;

		CString path = csFullPath.Left(nPos); 
		m_imgs.clear();

		HRESULT hr = m_imgs[IMG_CLOSE_DLG].Load(path + _T("\\ui_pictures\\ch_close.png"));
		//hr = m_imgs[IMG_STOP].Load(path + _T("\\ui_pictures\\off.png"));
		//hr = m_imgs[IMG_WARNING].Load(path + _T("\\ui_pictures\\warning.png"));
		//hr = m_imgs[IMG_ERROR].Load(path + _T("\\ui_pictures\\error.png"));
		//hr = m_imgs[IMG_PARAM].Load(path + _T("\\ui_pictures\\parameter.png"));
		//hr = m_imgs[IMG_NEXT].Load(path + _T("\\ui_pictures\\next.png"));
		//hr = m_imgs[IMG_PRIOR].Load(path + _T("\\ui_pictures\\prior.png"));
		//hr = m_imgs[IMG_REFRESH].Load(path + _T("\\ui_pictures\\refresh.png"));
		//hr = m_imgs[IMG_CH_CAMERA].Load(path + _T("\\ui_pictures\\ch_camera.png"));
		//hr = m_imgs[IMG_CH_LIGHT].Load(path + _T("\\ui_pictures\\ch_light.png"));
		//hr = m_imgs[IMG_CH_REJECTOR].Load(path + _T("\\ui_pictures\\ch_rejector.png"));
	}
	~ui_decorator()
	{
		std::for_each(m_imgs.begin(),m_imgs.end(),[&](std::pair<eIMAGE_TYPE, CImage> pair)
		{
			pair.second.Destroy();
		});
		m_imgs.clear();
	}

	std::map<eIMAGE_TYPE, CImage> m_imgs;

private:
	ui_decorator(const ui_decorator&);
	ui_decorator& operator=(const ui_decorator&);
};