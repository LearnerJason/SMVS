#include "stdafx.h"
#include "WarningError.h"
CString MyLoadString(int ID)
{
	CString strtmp;
	strtmp.LoadString(ID);
	CString str;
	str.Format(_T("%-40s"),strtmp);
	return strtmp;
}
SMVS_ERROR_INFO::SMVS_ERROR_INFO()
{
	m_CHErrorVec.clear();
	m_CHErrorVec.push_back(PCI_CREATE_FAILED_ERROR);
	m_CHErrorVec.push_back(LIGHT_CONTRL_CONNECT_ERROR);

	m_CHErrorVec.push_back(CAMERA_CONNECT_ERROR);
	m_CHErrorVec.push_back(INIT_REJECTOR_SYS_FAILED);
	m_CHErrorVec.push_back(THRESHOLD_BLOW_ERROR);
	m_CHErrorVec.push_back(CDP_ERROR);
	m_mapErrorInfo.clear();
	m_mapErrorInfo[PCI_CREATE_FAILED_ERROR]		= ::MyLoadString(IDS_STRING_PCI_CREATE_FAILED_ERROR);// _T("PCI板连接错误");
	m_mapErrorInfo[LIGHT_CONTRL_CONNECT_ERROR]	= ::MyLoadString(IDS_STRING_LIGHT_CONTRL_CONNECT_ERROR);//_T("光源控制器连接错误");
	m_mapErrorInfo[INIT_REJECTOR_SYS_FAILED]	= ::MyLoadString(IDS_STRING_INIT_REJECTOR_SYS_FAILED1);//_T("排盖板错误1");;
	m_mapErrorInfo[CAMERA_CONNECT_ERROR]		= ::MyLoadString(IDS_STRING_CAMERA_CONNECT_ERROR);//_T("相机连接错误");
	m_mapErrorInfo[THRESHOLD_BLOW_ERROR]	    = ::MyLoadString(IDS_STRING_THRESHOLD_BLOW_ERROR);//_T("排盖率阈值告警");
	m_mapErrorInfo[CDP_ERROR]					= ::MyLoadString(IDS_STRING_CDP_ERROR);//_T("连续错误图片告警");
}
CString SMVS_ERROR_INFO::GetErrorInfo(int ErrorID)
{
	if (m_mapErrorInfo.find(ErrorID)!= m_mapErrorInfo.end())
	{
		return m_mapErrorInfo[ErrorID];
	}
	return CString(_T(""));
}
