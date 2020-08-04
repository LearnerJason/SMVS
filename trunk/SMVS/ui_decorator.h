#pragma once
#include <afxwin.h>
#include <map>
#include <algorithm>
#include "Resource.h"

using namespace std;

enum eIMAGE_TYPE
{
	IMG_INVALID = -1,
	IMG_RUN = 0,
	IMG_WARNING,
	IMG_STOP,
	IMG_ERROR, // Channel Error
	IMG_PARAM,
	IMG_PRIOR,
	IMG_NEXT,
	IMG_REFRESH,
	IMG_REFRESH_GRAY,
	IMG_CH_CAMERA_RUN,
	IMG_CH_CAMERA_RUN_GRAY,
	IMG_CH_CAMERA_PAUSE,
	IMG_CH_CAMERA_PAUSE_GRAY,
	IMG_CH_CAMERA,
	IMG_CH_LIGHT,
	IMG_CH_REJECTOR,
	IMG_CH_ALGO,
	IMG_CH_DEVICE_INFO,
	IMG_CH_UI_CONFIG,
	IMG_CH_IMG_SOURCE,
	IMG_CH_IMG_SOURCE_GRAY,
	IMG_CH_FIRST,
	IMG_CH_PREVIOUS,
	IMG_CH_NEXT,
	IMG_CH_LAST,
	IMG_CH_LOCKED,
	IMG_CH_LOCKED_GRAY,
	IMG_CH_UNLOCKED,
	IMG_CH_UNLOCKED_GRAY,
	IMG_CH_ZOOM_IN,
	IMG_CH_ZOOM_IN_GRAY,
	IMG_CH_ZOOM_OUT,
	IMG_CH_ZOOM_OUT_GRAY,
	IMG_CH_SAVE_IMG,
	IMG_CH_SAVE_IMG_GRAY,
	IMG_CH_SAVE_ALL_IMG,
	IMG_CH_SAVE_ALL_IMG_GRAY,
	IMG_CH_DELETE_MODEL_IMG,
	IMG_CH_DELETE_MODEL_IMG_GRAY,
	IMG_CH_ADD_MODEL_IMG,
	IMG_CH_ADD_MODEL_IMG_GRAY,
	IMG_CH_RESET_STATS_DATA,
	IMG_CH_RESET_STATS_DATA_GRAY,
	IMG_CH_SINGLE_MODE,
	IMG_CH_SINGLE_MODE_GRAY,
	IMG_CH_CONTINOUS_MODE,
	IMG_CH_CONTINOUS_MODE_GRAY,
	IMG_CH_LEFT_ARROW,
	IMG_CH_LEFT_ARROW_GRAY,
	IMG_CH_RIGHT_ARROW,
	IMG_CH_RIGHT_ARROW_GRAY,
	IMG_CLOSE_DLG,
	IMG_CLOSE_DLG_GRAY,
	IMG_SYS_TASK,
	IMG_SYS_TASK_SELECTED,
	IMG_SYS_MODEL,
	IMG_SYS_MODEL_SELECTED,
	IMG_SYS_CAMERA_SELECTED,
	IMG_SYS_LIGHT_SELECTED,
	IMG_SYS_REJECTOR_SELECTED,
	IMG_SYS_CHART_MGR,
	IMG_SYS_CHART_MGR_SELECTED,
	IMG_SYS_CONFIG_FILE,
	IMG_SYS_CONFIG_FILE_SELECTED,
	IMG_SYS_SHIFT,
	IMG_SYS_SHIFT_SELECTED,
	IMG_MAIN_LOGIN,
	IMG_MAIN_LOGGED_IN,
	IMG_MAIN_SETTING,
	IMG_MAIN_SETTING_GRAY,
	IMG_MAIN_UAC,
	IMG_MAIN_UAC_GRAY,
	IMG_MAIN_HELP,
	IMG_MAIN_LANGUAGE,
	IMG_MAIN_LANGUAGE_GRAY,
	IMG_SPC_QUERY,
	IMG_SPC_EXPORT,
	IMG_SPC_EXPORT_GRAY,
	IMG_WARN_WARNED,
	IMG_WARN_NORMAL,
	IMG_MAIN_TASK_CHANGE,
	IMG_START_GRAB,
	IMG_PAUSE_GRAB
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

		HRESULT hr = m_imgs[IMG_RUN].Load(path + _T("\\ui_pictures\\on.png"));
		hr = m_imgs[IMG_STOP].Load(path + _T("\\ui_pictures\\off.png"));
		hr = m_imgs[IMG_WARNING].Load(path + _T("\\ui_pictures\\warning.png"));
		hr = m_imgs[IMG_ERROR].Load(path + _T("\\ui_pictures\\error.png"));
		hr = m_imgs[IMG_NEXT].Load(path + _T("\\ui_pictures\\next.png"));
		hr = m_imgs[IMG_PRIOR].Load(path + _T("\\ui_pictures\\prior.png"));
		hr = m_imgs[IMG_REFRESH].Load(path + _T("\\ui_pictures\\refresh.png"));
		hr = m_imgs[IMG_REFRESH_GRAY].Load(path + _T("\\ui_pictures\\refresh_gray.png"));
		hr = m_imgs[IMG_CH_CAMERA_RUN].Load(path + _T("\\ui_pictures\\ch_camera_run.png"));
		hr = m_imgs[IMG_CH_CAMERA_RUN_GRAY].Load(path + _T("\\ui_pictures\\ch_camera_run_gray.png"));
		hr = m_imgs[IMG_CH_CAMERA_PAUSE].Load(path + _T("\\ui_pictures\\ch_camera_pause.png"));
		hr = m_imgs[IMG_CH_CAMERA_PAUSE_GRAY].Load(path + _T("\\ui_pictures\\ch_camera_pause_gray.png"));
		hr = m_imgs[IMG_CH_CAMERA].Load(path + _T("\\ui_pictures\\ch_camera.png"));
		hr = m_imgs[IMG_CH_LIGHT].Load(path + _T("\\ui_pictures\\ch_light.png"));
		hr = m_imgs[IMG_CH_REJECTOR].Load(path + _T("\\ui_pictures\\ch_rejector.png"));
		hr = m_imgs[IMG_CH_ALGO].Load(path + _T("\\ui_pictures\\ch_algorithm.png"));
		hr = m_imgs[IMG_CH_DEVICE_INFO].Load(path + _T("\\ui_pictures\\ch_device_info.png"));
		hr = m_imgs[IMG_CH_UI_CONFIG].Load(path + _T("\\ui_pictures\\ch_ui_config.png"));
		hr = m_imgs[IMG_CH_IMG_SOURCE].Load(path + _T("\\ui_pictures\\ch_image_source.png"));
		hr = m_imgs[IMG_CH_IMG_SOURCE_GRAY].Load(path + _T("\\ui_pictures\\ch_image_source_gray.png"));
		hr = m_imgs[IMG_CH_FIRST].Load(path + _T("\\ui_pictures\\ch_first.png"));
		hr = m_imgs[IMG_CH_PREVIOUS].Load(path + _T("\\ui_pictures\\ch_previous.png"));
		hr = m_imgs[IMG_CH_NEXT].Load(path + _T("\\ui_pictures\\ch_next.png"));
		hr = m_imgs[IMG_CH_LAST].Load(path + _T("\\ui_pictures\\ch_last.png"));
		hr = m_imgs[IMG_CH_ZOOM_IN].Load(path + _T("\\ui_pictures\\ch_zoom_in.png"));
		hr = m_imgs[IMG_CH_ZOOM_IN_GRAY].Load(path + _T("\\ui_pictures\\ch_zoom_in_gray.png"));
		hr = m_imgs[IMG_CH_ZOOM_OUT].Load(path + _T("\\ui_pictures\\ch_zoom_out.png"));
		hr = m_imgs[IMG_CH_ZOOM_OUT_GRAY].Load(path + _T("\\ui_pictures\\ch_zoom_out_gray.png"));
		hr = m_imgs[IMG_CH_LOCKED].Load(path + _T("\\ui_pictures\\ch_locked.png"));
		hr = m_imgs[IMG_CH_LOCKED_GRAY].Load(path + _T("\\ui_pictures\\ch_locked_gray.png"));
		hr = m_imgs[IMG_CH_UNLOCKED].Load(path + _T("\\ui_pictures\\ch_unlocked.png"));
		hr = m_imgs[IMG_CH_UNLOCKED_GRAY].Load(path + _T("\\ui_pictures\\ch_unlocked_gray.png"));
		hr = m_imgs[IMG_CH_SAVE_IMG].Load(path + _T("\\ui_pictures\\ch_save_img.png"));
		hr = m_imgs[IMG_CH_SAVE_IMG_GRAY].Load(path + _T("\\ui_pictures\\ch_save_img_gray.png"));
		hr = m_imgs[IMG_CH_SAVE_ALL_IMG].Load(path + _T("\\ui_pictures\\ch_save_all_img.png"));
		hr = m_imgs[IMG_CH_SAVE_ALL_IMG_GRAY].Load(path + _T("\\ui_pictures\\ch_save_all_img_gray.png"));
		hr = m_imgs[IMG_CH_DELETE_MODEL_IMG].Load(path + _T("\\ui_pictures\\ch_delete_img.png"));
		hr = m_imgs[IMG_CH_DELETE_MODEL_IMG_GRAY].Load(path + _T("\\ui_pictures\\ch_delete_img_gray.png"));
		hr = m_imgs[IMG_CH_ADD_MODEL_IMG].Load(path + _T("\\ui_pictures\\ch_add_model_img.png"));
		hr = m_imgs[IMG_CH_ADD_MODEL_IMG_GRAY].Load(path + _T("\\ui_pictures\\ch_add_model_img_gray.png"));
		hr = m_imgs[IMG_CH_RESET_STATS_DATA].Load(path + _T("\\ui_pictures\\ch_reset_stats_data.png"));
		hr = m_imgs[IMG_CH_RESET_STATS_DATA_GRAY].Load(path + _T("\\ui_pictures\\ch_reset_stats_data_gray.png"));
		hr = m_imgs[IMG_CH_SINGLE_MODE].Load(path + _T("\\ui_pictures\\ch_single.png"));
		hr = m_imgs[IMG_CH_SINGLE_MODE_GRAY].Load(path + _T("\\ui_pictures\\ch_single_gray.png"));
		hr = m_imgs[IMG_CH_CONTINOUS_MODE].Load(path + _T("\\ui_pictures\\ch_continous.png"));
		hr = m_imgs[IMG_CH_CONTINOUS_MODE_GRAY].Load(path + _T("\\ui_pictures\\ch_continous_gray.png"));
		hr = m_imgs[IMG_CH_LEFT_ARROW].Load(path + _T("\\ui_pictures\\ch_left_arrow.png"));
		hr = m_imgs[IMG_CH_LEFT_ARROW_GRAY].Load(path + _T("\\ui_pictures\\ch_left_arrow_gray.png"));
		hr = m_imgs[IMG_CH_RIGHT_ARROW].Load(path + _T("\\ui_pictures\\ch_right_arrow.png"));
		hr = m_imgs[IMG_CH_RIGHT_ARROW_GRAY].Load(path + _T("\\ui_pictures\\ch_right_arrow_gray.png"));
		hr = m_imgs[IMG_CLOSE_DLG].Load(path + _T("\\ui_pictures\\ch_close.png"));
		hr = m_imgs[IMG_CLOSE_DLG_GRAY].Load(path + _T("\\ui_pictures\\ch_close_gray.png"));
		hr = m_imgs[IMG_SYS_TASK].Load(path + _T("\\ui_pictures\\sys_task.png"));
		hr = m_imgs[IMG_SYS_TASK_SELECTED].Load(path + _T("\\ui_pictures\\sys_task_selected.png"));
		hr = m_imgs[IMG_SYS_MODEL].Load(path + _T("\\ui_pictures\\sys_model.png"));
		hr = m_imgs[IMG_SYS_MODEL_SELECTED].Load(path + _T("\\ui_pictures\\sys_model_selected.png"));
		hr = m_imgs[IMG_SYS_CAMERA_SELECTED].Load(path + _T("\\ui_pictures\\sys_camera_selected.png"));
		hr = m_imgs[IMG_SYS_LIGHT_SELECTED].Load(path + _T("\\ui_pictures\\sys_light_selected.png"));
		hr = m_imgs[IMG_SYS_REJECTOR_SELECTED].Load(path + _T("\\ui_pictures\\sys_rejector_selected.png"));
		hr = m_imgs[IMG_SYS_CHART_MGR].Load(path + _T("\\ui_pictures\\sys_chart_mgr.png"));
		hr = m_imgs[IMG_SYS_CHART_MGR_SELECTED].Load(path + _T("\\ui_pictures\\sys_chart_mgr_selected.png"));
		hr = m_imgs[IMG_SYS_CONFIG_FILE].Load(path + _T("\\ui_pictures\\sys_config.png"));
		hr = m_imgs[IMG_SYS_CONFIG_FILE_SELECTED].Load(path + _T("\\ui_pictures\\sys_config_selected.png"));
		hr = m_imgs[IMG_SYS_SHIFT].Load(path + _T("\\ui_pictures\\sys_shift.png"));
		hr = m_imgs[IMG_SYS_SHIFT_SELECTED].Load(path + _T("\\ui_pictures\\sys_shift_selected.png"));
		hr = m_imgs[IMG_MAIN_LOGIN].Load(path + _T("\\ui_pictures\\main_login.png"));
		hr = m_imgs[IMG_MAIN_LOGGED_IN].Load(path + _T("\\ui_pictures\\main_logged_in.png"));
		hr = m_imgs[IMG_MAIN_SETTING].Load(path + _T("\\ui_pictures\\main_setting.png"));
		hr = m_imgs[IMG_MAIN_SETTING_GRAY].Load(path + _T("\\ui_pictures\\main_setting_gray.png"));
		hr = m_imgs[IMG_MAIN_UAC].Load(path + _T("\\ui_pictures\\main_uac.png"));
		hr = m_imgs[IMG_MAIN_UAC_GRAY].Load(path + _T("\\ui_pictures\\main_uac_gray.png"));
		hr = m_imgs[IMG_MAIN_HELP].Load(path + _T("\\ui_pictures\\main_help.png"));
		hr = m_imgs[IMG_MAIN_LANGUAGE].Load(path + _T("\\ui_pictures\\main_language.png"));
		hr = m_imgs[IMG_MAIN_LANGUAGE_GRAY].Load(path + _T("\\ui_pictures\\main_language_gray.png"));
		hr = m_imgs[IMG_SPC_QUERY].Load(path + _T("\\ui_pictures\\spc_query.png"));
		hr = m_imgs[IMG_SPC_EXPORT].Load(path + _T("\\ui_pictures\\spc_export.png"));
		hr = m_imgs[IMG_SPC_EXPORT_GRAY].Load(path + _T("\\ui_pictures\\spc_export_gray.png"));
		hr = m_imgs[IMG_WARN_WARNED].Load(path + _T("\\ui_pictures\\cdp_reset.png"));
		hr = m_imgs[IMG_WARN_NORMAL].Load(path + _T("\\ui_pictures\\Normal.png"));
		hr = m_imgs[IMG_MAIN_TASK_CHANGE].Load(path + _T("\\ui_pictures\\main_task.png"));
		hr = m_imgs[IMG_START_GRAB].Load(path + _T("\\ui_pictures\\start.png"));
		hr = m_imgs[IMG_PAUSE_GRAB].Load(path + _T("\\ui_pictures\\pause.png"));
		HandleImageAlphaChannel();
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
	void HandleImageAlphaChannel()
	{
		for(auto it = m_imgs.begin(); it != m_imgs.end(); ++it)
		{
			CImage& image = it->second;
			if(image.GetBPP() == 32)
			{
				for(int i = 0; i < image.GetWidth(); i++)
				{
					for(int j = 0; j < image.GetHeight(); j++)
					{
						byte* pByte = (byte*)image.GetPixelAddress(i, j);
						pByte[0] = pByte[0] * pByte[3]/255;
						pByte[1] = pByte[1] * pByte[3]/255;
						pByte[2] = pByte[2] * pByte[3]/255;
					}
				}
			}
		}
	}
	ui_decorator(const ui_decorator&);
	ui_decorator& operator=(const ui_decorator&);
};