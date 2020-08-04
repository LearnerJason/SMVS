#include "stdafx.h"
#include "SystemManager.h"
#include "ConfigurationManager.h"
#include "ChannelControlPanel.h"
#include "DialogUpdateDatabase.h"
#include <assert.h>
#include "WarningError.h"
#include "SMVSDlg.h"
#include  <io.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern ConfigManager  gConfigMgr;
extern SOCKET         gPcSocket;
CRITICAL_SECTION CSyncLock::_cs;
CSyncLock::CSig CSyncLock::CSig::m_sig;
BOOL CSyncLock::m_bInit_cs=FALSE;

CSystemManager::CSystemManager()
	:m_nSerialPort(-1),
	 m_pSerialPort(nullptr),
	 m_bootTime(0),
	 m_pMainDlg(nullptr),
	 m_nExitFlag(0),
	 m_nCanImgCount(0),
	 m_bLcSerialPortOk(true),
	 m_nActionCount(0),
	 m_nMaxImgSyncTime(0),
	 m_nMaxImgSyncDelta(0),
	 m_bBatchNumberFun(false),
	 m_bUpdateDatabaseFun(false),
	 m_bShowSystemStateFun(false),
	 m_bAutoChangeTaskFun(false),
	 m_bClearDataALLFun(false)
{
	gPcSocket = 0;
	m_vPanels.clear();
	m_vChProcessorArray.clear();
	m_vSerialPort.clear();

	::InitializeCriticalSection(&m_imgJunctionCs);
	::InitializeCriticalSection(&m_csGIIT);
	// Database
	TCHAR  moduleFileName[MAX_PATH];  
	::GetModuleFileName(NULL,moduleFileName, MAX_PATH);
	(_tcsrchr(moduleFileName, _T('\\')))[1] = 0;
	CString strPath = moduleFileName;
	strPath.Append(_T("\database"));
	if(!_wmkdir(strPath))
	{
		//TODO
	}
	strPath.Append(_T("\\SMVS.db"));
	std::string strDbName = CT2A(strPath);
	m_db.SetDBName(strDbName.c_str());
	if(!m_db.Connect())
	{
		gLogger.QueueUpLog(L"Connect SMVS database failed!");
	}

	std::string strUAC("UAC");
	if(!m_db.TableExists(strUAC.c_str()))
	{
		CreateUacTable();
		InsertDefualtUser();
	}
	SYSTEMTIME dt;
	::GetLocalTime(&dt);
	m_bootTime = utils::DateTime2Int64(dt);

	std::map<wstring,UAC> dictUac;
	GetAllUacInfo(dictUac);
	m_dictUac = dictUac;
	// List all cameras
	ListAllAvailableCamera(m_dictCamera_State);

#ifdef _DEBUG
	m_crtUser.m_strUsername = L"SMVSADMIN";
	m_crtUser.m_strPassword = L"smvsadmin";
	m_crtUser.m_nLevel      = UAC_ADMIN;
#endif

#ifdef NDEBUG
	m_crtUser.m_strUsername = L"OPERATOR";
	m_crtUser.m_strPassword = L"operator";
	m_crtUser.m_nLevel      = UAC_OPERATOR;
#endif

	std::vector<IMAGE_BOX> vTmp(4,IMAGE_BOX());
	m_vCOImage = vTmp;
	m_fMaxScore = -100.0f;

	m_nSysChIndex    = -1;
	m_crtSysTaskName = L"";

	m_displyManager.CreateImageBuffer();
	
	//当前批次号处理
	wstring wstrMaxSyncTime = gConfigMgr.GetBasicSetting(_T("ALLPICTIME"));
	m_nMaxImgSyncTime= _wtoi(wstrMaxSyncTime.c_str());
	if (m_nMaxImgSyncTime == 0)
	{
		m_nMaxImgSyncTime = 40;
	}
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	m_nMaxImgSyncDelta = m_nMaxImgSyncTime * freq.QuadPart / 1000;

	//自动切换任务
	wstring wstrAutoChangeTask = gConfigMgr.GetBasicSetting(_T("AUTOCHANGETASKFUNC"));
	if (wstrAutoChangeTask == L"Y")
	{
		m_bAutoChangeTaskFun = true;
	}
	//批次号功能
	wstring wstrBatchNumberFunc = gConfigMgr.GetBasicSetting(_T("BATCHNUMBERFUNC"));
	if (wstrBatchNumberFunc == L"Y")
	{
		m_bBatchNumberFun = true;
		wstring wstrBatchNumber = gConfigMgr.GetBasicSetting(_T("BATCHNUMBER"));
		m_strBatchNumber = wstrBatchNumber.c_str();
	}
	
	//一键清除功能
	wstring wstrClearAllDataFunc = gConfigMgr.GetBasicSetting(_T("CLEARALLDATAFUNC"));
	if (wstrClearAllDataFunc == L"Y")
	{
		m_bClearDataALLFun = true;
	}

	//数据库对接功能
	wstring wstrUpdateDatabaseFunc = gConfigMgr.GetBasicSetting(_T("UPDATEDATABASEFUNC"));
	if (wstrUpdateDatabaseFunc == L"Y")
	{
		m_bUpdateDatabaseFun = true;
	}

	//账号员工数据统计功能（附加条件）1
	wstring wstrProductInfo = gConfigMgr.GetBasicSetting(_T("PRODUCT_INFO"));
	if (wstrProductInfo != L"")
	{
		m_vecProductInfo = utils::SplitString(wstrProductInfo,L",");
	}

	// 	账号员工数据统计 (附加条件)2, 系统表数据外键
	wstring strSysForeignKey = gConfigMgr.GetBasicSetting(_T("foreign_key"));
	m_strSysForeignKey = strSysForeignKey.c_str();
	
	//互锁，严重告警配置
	wstring wstrSeriousWarning = gConfigMgr.GetBasicSetting(_T("SERIOUSWARNING"));
	m_vecSeriousWarning = utils::SplitString(wstrSeriousWarning,L",");

	//系统统计功能
	wstring wstrShowSystemStateFun =gConfigMgr.GetBasicSetting(_T("SHOWSYSTEMSTATEFUNC"));
	if (wstrShowSystemStateFun == L"Y")
	{
		m_bShowSystemStateFun = true;
	}

	//获取软件起始时间戳
	wstring strlastuploadtime = gConfigMgr.GetBasicSetting(_T("lastuploadtime"));
	m_strlastUploadtime = strlastuploadtime.c_str();

	if (m_strlastUploadtime==L"")
	{
		SYSTEMTIME bgn;
		::GetLocalTime(&bgn);
		m_timebegin =utils::DateTime2Int64(bgn); 
		m_strlastUploadtime.Format(_T("%I64d"),m_timebegin);
	}
	else
	{
		m_timebegin = boost::lexical_cast<long long>(m_strlastUploadtime.GetString());
	}

}
eERROR_LEVEL CSystemManager::GetErrorLevel(wstring wstrID)
{
	if (m_vecSeriousWarning.size()==0)
	{
		return WL_WARNING;
	}

	for (int i = 0; i < m_vecSeriousWarning.size(); i++)
	{
		if (m_vecSeriousWarning[i] == wstrID)
		{
			return WL_ERROR;
		}
	}

	return WL_WARNING;
}

bool CSystemManager::AutoChangeTask(CString taskname)
{
	//提示是否要进行一键切换任务！
	CString sMsg;
	sMsg.LoadString(IDS_STRING_IF_AUTO_CHANGE_TASK);
	if(IDNO == AfxMessageBox(sMsg, MB_YESNO, NULL))
	{
		return false;
	}

	CString strTaskNameGroup;
	strTaskNameGroup.LoadString(IDS_STRING_AUTO_CHANGE_TASK_OK);
	for (int i = 0 ; i < m_vPanels.size();i++)
	{
		m_vPanels[i]->AutoChangeTask(taskname,true);
		CString str;
		str.Format(_T("CH%d:[%s]\n"),i+1,taskname);
		strTaskNameGroup+=str;
	}
	
	::AfxMessageBox(strTaskNameGroup);
	return true;
}
bool CSystemManager::IsBatchNumberFunTrue()
{
	return m_bBatchNumberFun;
}
bool CSystemManager::IsUpdateDatabaseFunTrue()
{
	return m_bUpdateDatabaseFun;
}
bool CSystemManager::IsClearAllDataFunTrue()
{
	return m_bClearDataALLFun;
}

bool CSystemManager::IsAutoChangeTaskFuncTrue()
{
	return m_bAutoChangeTaskFun;
}

bool CSystemManager::IsShowSystemStateFunTrue()
{
	return m_bShowSystemStateFun;
}

CSystemManager::~CSystemManager()
{
	std::for_each(m_vPanels.begin(), m_vPanels.end(), [](CChannelControlPanel *pPanel)
	{
		delete pPanel;
		pPanel = nullptr;
	});
	m_vPanels.clear();

	closesocket(gPcSocket);
}
void CSystemManager::ResetAllChannelData()
{
	UpdateDatabaseData();
	Sleep(500);
	std::for_each(m_vPanels.begin(), m_vPanels.end(), [](CChannelControlPanel *pPanel)
	{
			pPanel->ClearChData();
	});

	
}
void CSystemManager::UpdateBatchNumberStr()
{
	gConfigMgr.SetBatchNumber(m_strBatchNumber);
}
void CSystemManager::EnableDataClearButton()
{
    bool bAdmin = (m_crtUser.m_nLevel == UAC_ADMIN);
	std::for_each(m_vPanels.begin(), m_vPanels.end(),[&](CChannelControlPanel* pPanel)
	{
		if (bAdmin && pPanel->GetChannelState()== CH_OFF)
		{
			pPanel->ShowChannelDataClearButton(true);
		}
		else
		{
			pPanel->ShowChannelDataClearButton(false);
		}
	});
}

bool CSystemManager::CreatePciDevice()
{
	return true;
}

void CSystemManager::InitSerialPort()
{
	if(m_pSerialPort != nullptr)
	{
		m_pSerialPort->ClosePort();
		delete m_pSerialPort;
		m_pSerialPort = nullptr;
	}
	if(m_nSerialPort != -1)
	{
		m_pSerialPort = new CSerialPort(m_nSerialPort);
		if(!m_pSerialPort->InitPort(9600))
		{
			delete m_pSerialPort;
			m_pSerialPort = nullptr;
			return;
		}
		m_pSerialPort->SetOwnerWindowPtr(m_pMainDlg);
		m_pSerialPort->StartMonitoring();
	}
}

void CSystemManager::ClearAllChError()
{
    m_setCheckedLcID.clear();
	std::for_each(m_vPanels.begin(), m_vPanels.end(), [&](CChannelControlPanel* pPanel)
	{
		pPanel->ClearChError();
	});
}
void CSystemManager::ClearCameraWarning(int nChIndex)
{
	if (nChIndex < m_vPanels.size())
	{
		m_vPanels[nChIndex-1]->DelChError(CAMERA_CONNECT_ERROR);
	}
}
void CSystemManager::ListAllAvailableCamera(std::map<wstring, bool>& mapNameState)
{
	mapNameState.clear();
	CTlFactory& tlFactory = CTlFactory::GetInstance();
	if(tlFactory.EnumerateDevices(m_cameraDeviceInfoList) == 0)
		return;

	int index = 0;
	USES_CONVERSION;
	for(DeviceInfoList_t::iterator it = m_cameraDeviceInfoList.begin (); it != m_cameraDeviceInfoList.end (); it++)
	{
		wstring name = CA2W((*it).GetFriendlyName().c_str());
		bool bAccessible = false;
		if(CTlFactory::GetInstance().IsDeviceAccessible(m_cameraDeviceInfoList[index]))
		{
			bAccessible = true;
		}
		index++;
		mapNameState[name] = bAccessible;
	}
}

void CSystemManager::InitializeSerialPortForLightController()
{
	m_vSerialPort = utils::GetAllSerialPort();

	wstring wstrPortKey = gConfigMgr.GetBasicSetting(L"LC_PORT");
	if(wstrPortKey != L"")
	{
		m_nSerialPort = boost::lexical_cast<int>(wstrPortKey);
	}
	 
	CString strMsg;
	wstring wstrPort(L"COM");
	wstrPort += boost::lexical_cast<wstring>(m_nSerialPort);
	if(std::find(m_vSerialPort.begin(), m_vSerialPort.end(), wstrPort) == m_vSerialPort.end())
	{
		m_bLcSerialPortOk = false;
		return;
	}
	InitSerialPort();
	m_hLcTestReadyEvent = CreateEventW(NULL,TRUE,FALSE,NULL);
	m_dLcTestOverEvent  = CreateEventW(NULL,TRUE,FALSE,NULL);
}

CString CSystemManager::GetAllSecretaryDeviceVersion()
{
	CString strVersion;
	CString strVerTmp;
	std::for_each(m_vPanels.begin(), m_vPanels.end(), [&](CChannelControlPanel* pPanel)
	{
		strVerTmp  = pPanel->m_chSD.GetVersion();
		strVersion.Append(_T("(3.X)"));
		strVersion.Append(strVerTmp);
		strVersion.Append(_T("\n"));
	});

	return strVersion;
}

void CSystemManager::CreateAllPanels()
{
	auto pChConfig = gConfigMgr.GetChannelConfigPtr();
	CChannelControlPanel *pPanel = nullptr;
	std::for_each(pChConfig->begin(), pChConfig->end(),[&](std::pair<int,CHANNEL_CONFIG> p)
	{
		pPanel = new CChannelControlPanel(p.second,m_pMainDlg);
		m_vPanels.push_back(pPanel);
		m_vChProcessorArray.push_back(pPanel->GetImgProcessorPtr());
		m_dictSyncFlag[p.first] = 0;
	});	
	m_rcChChart = pPanel->GetChartRect();
}

void CSystemManager::ReCreateAllPanels()
{
	m_vChProcessorArray.clear();

	for(int i = 0 ; i < m_vPanels.size();i++)
	{
		m_vChProcessorArray.push_back(m_vPanels[i]->GetImgProcessorPtr());
	}

}

void CSystemManager::StartRejetorThread()
{
// 	if(m_pciDevice.GetRejectorStatus())
// 	{
// 		m_pciDevice.Start();
// 	}
}

void CSystemManager::StartChannelImgProcessor()
{
	std::for_each(m_vPanels.begin(), m_vPanels.end(), [&](CChannelControlPanel* pPanel)
	{
		pPanel->StartImgProcessor(true);
	});
}

void CSystemManager::UpdateAllChannelWarningFlag()
{
	std::for_each(m_vPanels.begin(), m_vPanels.end(), [&](CChannelControlPanel* pPanel)
	{
		pPanel->UpdateWarningFlag();
	});
}

bool CSystemManager::TurnOnAllCameras()
{
	bool bOk = true;
	std::for_each(m_vPanels.begin(), m_vPanels.end(), [&](CChannelControlPanel* pPanel)
	{
		bool bRet = pPanel->TurnOnCameraAtSystemBoot();
		bOk = (bOk && bRet);
		m_dictChCameraConnection[pPanel->m_chIndex] = 1;
	});
	return bOk;
}

void CSystemManager::TurnOnChannelCamera(int nChIndex)
{
	std::for_each(m_vPanels.begin(), m_vPanels.end(), [&](CChannelControlPanel* pPanel)
	{
		if(nChIndex == pPanel->m_chIndex)
		{
			pPanel->TurnOnCamera();
		}
	});
}

void CSystemManager::PauseAllCamera()
{
	std::for_each(m_vPanels.begin(), m_vPanels.end(), [&](CChannelControlPanel* pPanel)
	{
		pPanel->PauseChCamera();
	});
}

void CSystemManager::PauseChannelCamera(int nChIndex)
{
	std::for_each(m_vPanels.begin(), m_vPanels.end(), [&](CChannelControlPanel* pPanel)
	{
		if(nChIndex == pPanel->m_chIndex)
		{
			pPanel->PauseChCamera();
		}
	});
}

bool CSystemManager::IsChannelCameraOk(int nChIndex)
{
	bool bRet = false;
	std::for_each(m_vPanels.begin(), m_vPanels.end(), [&](CChannelControlPanel* pPanel)
	{
		if(nChIndex == pPanel->m_chIndex)
		{
			bRet = pPanel->IsChCameraOpened();
		}
	});
	return bRet;
}

bool CSystemManager::IsChannelCameraRunning(int nChIndex)
{
	bool bRet = false;
	std::for_each(m_vPanels.begin(), m_vPanels.end(), [&](CChannelControlPanel* pPanel)
	{
		if(nChIndex == pPanel->m_chIndex)
		{
			bRet = pPanel->IsChCameraRunning();
		}
	});
	return bRet;	
}

bool CSystemManager::IsAnyCameraRunning()
{
	for(size_t t = 0; t < m_vPanels.size(); t++)
	{
		CChannelControlPanel* pPanel = m_vPanels[t];
		if(pPanel != nullptr && pPanel->IsChCameraRunning())
		{
			return true;
		}
	}
	return false;	
}

void CSystemManager::UpdateTaskThreshold()
{
	std::for_each(m_vPanels.begin(), m_vPanels.end(),[&](CChannelControlPanel* pPanel)
	{
		pPanel->UpdateTaskThreshold();
	});
}

void CSystemManager::StopImgProcessor()
{
	std::for_each(m_vPanels.begin(), m_vPanels.end(),[&](CChannelControlPanel* pPanel)
	{
		pPanel->StopImgProcessor();
	});
}

void CSystemManager::StopRejector()
{
//	m_pciDevice.Stop();
}

void CSystemManager::CloseSerialPort()
{
	if(m_pSerialPort != nullptr)
	{
		m_pSerialPort->ClosePort();
		delete m_pSerialPort;
		m_pSerialPort = nullptr;
	}
}

void CSystemManager::FinalClean()
{
	BOOL bRecheck=FALSE;
	CString sReCheckText;
	for(auto it = m_dictStats.begin(); it != m_dictStats.end(); it++)
	{
		auto ptr = it->second;
		delete ptr;
	}
	m_dictStats.clear();

	for(auto it = m_dictSystemStatsData.begin(); it != m_dictSystemStatsData.end(); it++)
	{
		auto ptr = it->second;
		delete ptr;
	}
	m_dictSystemStatsData.clear();

	if(m_pSerialPort != nullptr)
	{
		delete m_pSerialPort;
		m_pSerialPort = nullptr;
	}
	RemoveObsoleteDataFromDatabase();
	if(!m_db.Close())
	{
		gLogger.QueueUpLog(L"Close SQLite Database Failed!");
	}

	if(IsUpdateDatabaseFunTrue())
	{
		CDialogUpdateDatabase dlg;
		dlg.SetStrInfo(L"");
		if (dlg.DoModal()!=IDOK )
		{
			UpdateDatabaseData(bRecheck,sReCheckText);
		}
		else
		{
			if(dlg.GetRecheck().Compare(_T("1"))==0)
				bRecheck=TRUE;
			sReCheckText = dlg.GetReserved1();
			UpdateDatabaseData(bRecheck,sReCheckText);
		}
	}
}

void CSystemManager::SaveLeftoverStatisticsData()
{
	std::for_each(m_vPanels.begin(), m_vPanels.end(),[&](CChannelControlPanel* pPanel)
	{
		TASK crtTask = pPanel->GetCurrentTask();
		if(!crtTask.m_bSelfLearning)
		{
			auto dataPt  = pPanel->GetCurrentTaskLastStatsData(m_strBatchNumber);
			if(dataPt.m_totalImgCount > 0)
			{
				ReceiveStatsDataPoint(crtTask, dataPt);
				if(pPanel->m_chIndex == m_nSysChIndex)
				{
					ReceiveSystemStatsDataPoint(true);
				}
			}
		}
	});
}

void CSystemManager::SaveLeftoverStatisticsData(int nIndex)
{
	std::for_each(m_vPanels.begin(), m_vPanels.end(),[&](CChannelControlPanel* pPanel)
	{
		if(pPanel->m_chIndex == nIndex)
		{
			TASK crtTask = pPanel->GetCurrentTask();
			if(!crtTask.m_bSelfLearning)
			{
				auto dataPt  = pPanel->GetCurrentTaskLastStatsData(m_strBatchNumber);
				if(dataPt.m_totalImgCount > 0)
				{
					ReceiveStatsDataPoint(crtTask, dataPt);
					if(nIndex == m_nSysChIndex)
					{
						ReceiveSystemStatsDataPoint(true);
					}
				}
			}
			return;
		}
	});
}
//2018-09-20
//介于排盖版在 切换延时间 和 延脉冲的过程中，相关参数会重置为默认值
//所以 在相机界面切换 时间脉冲模式的时候，需要重新Update下，排盖相关参数，保证排盖版能够正常应用
//此功能 主要应用于相机设置界面，点应用按钮的时候！
void CSystemManager::UpdateCameraSetting(int chIndex, const CAMERA_SETTING& cameraSetting,bool bCameraModeChange)
{
	std::for_each(m_vPanels.begin(), m_vPanels.end(),[&](CChannelControlPanel* pPanel)
	{
		if(pPanel->m_chIndex == chIndex)
		{
			pPanel->UpdateCameraSetting(cameraSetting,bCameraModeChange);
		}
	});
}

void CSystemManager::UpdateRejectorSetting(int chIndex, const REJECTOR_SETTING& rejectorSetting)
{
	std::for_each(m_vPanels.begin(), m_vPanels.end(),[&](CChannelControlPanel* pPanel)
	{
		if(pPanel->m_chIndex == chIndex)
		{
			pPanel->UpdateRejectorSetting(rejectorSetting);
		}
	});
}

void CSystemManager::UpdateChLightController(int nChIndex, const LIGHT_SETTING& ls)
{
	std::for_each(m_vPanels.begin(), m_vPanels.end(),[&](CChannelControlPanel* pPanel)
	{
		if(pPanel->m_chIndex == nChIndex)
		{
			pPanel->UpdateChLightController(ls);
		}
	});
}

void CSystemManager::UpdateTaskSetting()
{
	if(m_dictCh_TaskAction.empty()) return;

	for (auto it = m_vPanels.begin(); it != m_vPanels.end(); it ++)
	{
		CChannelControlPanel* pPanel = (*it);
		if(!pPanel->IsChCameraRunning())
		{
			int nChIndex = pPanel->m_chIndex;
			if(m_dictCh_TaskAction.find(nChIndex) != m_dictCh_TaskAction.end())
			{
				pPanel->UpdateTaskSetting(m_dictCh_TaskAction[nChIndex]);
			}
		}
	}
}

void CSystemManager::RefreshSystem()
{
	for (auto it = m_vPanels.begin(); it != m_vPanels.end(); it ++)
	{
		CChannelControlPanel* pPanel = (*it);
		pPanel->RefreshChannelPanel();
	}
}

std::vector<CChannelControlPanel*> CSystemManager::GetPanelArray()
{
	return m_vPanels;
}

CChannelControlPanel* CSystemManager::GetPanel(int ChIndex)
{
	if (ChIndex <= m_vPanels.size())
	{
		return m_vPanels[ChIndex-1];
	}
	return NULL;
}

std::vector<ERROR_INFO> CSystemManager::GetAllErrorInfo()
{
    //Get all error events from all channel
   std::vector<ERROR_INFO> vRettmp;
	std::vector<ERROR_INFO> vRet;

	for (int i = 0; i < m_vPanels.size();i++)
	{
		vRettmp = m_vPanels[i]->GetErrorInfo();
		vRet.insert(vRet.end(), vRettmp.begin(), vRettmp.end()); 
	}
    return vRet;
}

bool CSystemManager::AllChannelCameraStopped()
{
	for(size_t t = 0; t < m_vPanels.size();t++)
	{
		CChannelControlPanel* pPanel = m_vPanels[t];
		if(pPanel->IsChCameraRunning())
			return false;
	}
	return true;
}

void CSystemManager::SetMainDlgPtr(CWnd* pMainDlg)
{
	m_pMainDlg = pMainDlg;
}

void CSystemManager::SetSerialPortOwnerWindow(CWnd* pOwnerWnd)
{
	if(m_pSerialPort != nullptr)
	{
		m_pSerialPort->SetOwnerWindowPtr(pOwnerWnd);
	}
}

void CSystemManager::RestoreSerialPortOwnerWindow()
{
	if(m_pSerialPort != nullptr)
	{
		m_pSerialPort->SetOwnerWindowPtr(m_pMainDlg);
	}
}

bool CSystemManager::DeleteTable(wstring wstrTableName)
{
	string strTableName = utils::Unicode2Utf8(wstrTableName);
	if(!m_db.TableExists(strTableName.c_str()))
	{
		return true;
	}
	string strCmd = "DROP TABLE ";
	strCmd.append(strTableName);
	int nRet = m_db.Excute(strCmd.c_str());
	return (SQLITE_OK == nRet);
}

bool CSystemManager::CreateSystemDataTable(wstring wstrTableName)
{
	bool bRet = true;
	std::string strTableName = utils::Unicode2Utf8(wstrTableName);
	if(m_db.TableExists(strTableName.c_str()))
	{
		return true;
	}
	else
	{
		std::string strTableTitle("[Timestamp] integer primary key, [totalCount] integer, [totalLeftCount] integer, [leftFlag] integer,[BatchNumber] text,[UserName] text,[SysforeignKey] text");
		bRet = m_db.CreateTable(strTableName.c_str(), strTableTitle.c_str());//
	}
	return bRet;
}

bool CSystemManager::CreateProductInfoTable(std::vector<CString> m_vec)
{
	bool bRet = true;

	wstring wstrTableName(L"PRODUCT_INFO");
	std::string strTableName = utils::Unicode2Utf8(wstrTableName);
	if(m_db.TableExists(strTableName.c_str()))
	{
		return true;
	}
	else
	{
		std::string strTableTitle("[SysforeignKey] text primary key, [Reserved] text");
		for (int i = 0 ; i < m_vec.size() ; i++)
		{
			if (i!=0)
			{
				strTableTitle+=",";
			}
			string strtmp("[Reserved");
			std::string strChIndex = boost::lexical_cast<string>(i);
			strtmp+=strChIndex;
			strtmp+="] text";
			strTableTitle+=strtmp;
		}

		bRet = m_db.CreateTable(strTableName.c_str(), strTableTitle.c_str());//
	}
	return bRet;
}

CString CSystemManager::GetCurBatchNumber()
{
	return m_strBatchNumber;
}

void CSystemManager::SetCurBatchNumber(CString strBatchNumber)
{
	m_strBatchNumber = strBatchNumber;
}

bool CSystemManager::CreateTable(const TASK& task)
{
	wstring strTaskName = task.m_wstrTaskName;
	if(strTaskName == L"")
		return false;

	bool bRet = true;
	std::string strTableName = utils::Unicode2Utf8(strTaskName);
	if(m_db.TableExists(strTableName.c_str()))
	{
		return true;
	}
	else
	{
		size_t nItemCount = task.m_vItParam.size();
		std::string strTableTitle("[Timestamp] integer primary key, [TotalCount] integer, [NGCount] integer, [ErrorCount] integer, [DefectRatio] real,[BatchNumber] text,[UserName] text");
		for(size_t i = 0; i < nItemCount;i++)
		{
			IT_PARAM itParam = task.m_vItParam[i];
			if(i>0)
			{
				string strColumnTitle = utils::Unicode2Utf8(itParam.m_itNickname);
				strTableTitle.append(", [");
				strTableTitle.append(strColumnTitle);
				strTableTitle.append("] integer");
			}
		}
		bRet = m_db.CreateTable(strTableName.c_str(), strTableTitle.c_str());
	}
	return bRet;
}

string CSystemManager::GenerateInsertCommand(const wstring& strTableName, const SPC_DATA_PT& dataPt)
{
	int nItemCount = (int)dataPt.m_vItemStats.size();

	string strCmdModel = "INSERT INTO ";
	strCmdModel.append(utils::Unicode2Utf8(strTableName));
	strCmdModel.append(" (Timestamp,TotalCount,NGCount,ErrorCount,DefectRatio,BatchNumber,UserName");
	for(int i = 0; i < nItemCount; i++)
	{
		ITEM_STATS data = dataPt.m_vItemStats[i];
		strCmdModel.append(",");
		strCmdModel.append(utils::Unicode2Utf8(data.m_wstrName));
	}
	strCmdModel.append(")");

	strCmdModel.append(" VALUES(#1,#2,#3,#4,#5,'#6','#7'");
	int index = 7;
	string strIndex;
	for(int t = 0; t < nItemCount; t++)
	{
		strCmdModel.append(",*");
		strIndex = boost::lexical_cast<string>(index);
		strCmdModel.append(strIndex);
		index++;
	}
	strCmdModel.append(")");

	return strCmdModel;
}

bool CSystemManager::InsertSystemStatsData(const wstring& strTableName, const SYS_STAT_DATA& dataPt)
{
    if(dataPt.m_totalOkCount < 0)
    {
        wstring wstrLog = strTableName;
        wstrLog.append(L"System statistics error - Total count is less than zero!");
        gLogger.QueueUpLog(wstrLog);
        return false;
    }
	string strName = utils::Unicode2Utf8(strTableName);
	if(!m_db.TableExists(strName.c_str()))
	{
		if(!CreateSystemDataTable(strTableName))
		{
			wstring wstrLog = L"Create database failed - ";
			wstrLog.append(strTableName);
			gLogger.QueueUpLog(wstrLog);
			return false;
		}
	}

	CString sTimestamp;
	CString strValue;
	string strCmd = "INSERT INTO ";
	strCmd.append(utils::Unicode2Utf8(strTableName));
	strCmd.append(" (Timestamp,TotalCount,totalLeftCount,leftFlag,BatchNumber,UserName,SysforeignKey");
	strCmd.append(")");
	strCmd.append(" VALUES(#1,#2,#3,#4,'#5','#6','#7'");
	strCmd.append(")");

	wstring wstrCmdModel = utils::Utf8ToUnicode(strCmd);
	CString sCmd = CW2T(wstrCmdModel.c_str());

	strValue.Format(_T("%I64d"), dataPt.m_timestamp);
	sCmd.Replace(_T("#1"), strValue);

	strValue.Format(_T("%d"), dataPt.m_totalCount);
	sCmd.Replace(_T("#2"), strValue);

	strValue.Format(_T("%d"), dataPt.m_totalOkCount);
	sCmd.Replace(_T("#3"), strValue);

	strValue.Format(_T("%d"), dataPt.m_leftFlag);
	sCmd.Replace(_T("#4"), strValue);

	strValue = dataPt.m_strBatchNumber;
	sCmd.Replace(_T("#5"), strValue);

	strValue = dataPt.m_strUser;
	sCmd.Replace(_T("#6"), strValue);

	strValue = dataPt.m_strSysForeignKey;
	sCmd.Replace(_T("#7"), strValue);

	wstring wstrCmd = sCmd.GetString();
	strCmd = utils::Unicode2Utf8(wstrCmd);
	m_db.Excute(strCmd.c_str());
	return true;
}

bool CSystemManager::InsertChannelStatsData(const wstring& strTableName, const SPC_DATA_PT& dataPt)
{
	string strName = utils::Unicode2Utf8(strTableName);
	if(!m_db.TableExists(strName.c_str()))
	{
		return false;
	}

	CString sTimestamp;
	CString strValue;
	string strCmd = GenerateInsertCommand(strTableName,dataPt);

	wstring wstrCmdModel = utils::Utf8ToUnicode(strCmd);
	CString sCmd = CW2T(wstrCmdModel.c_str());

	strValue.Format(_T("%I64d"), dataPt.m_timestamp);
	sCmd.Replace(_T("#1"), strValue);

	strValue.Format(_T("%d"), dataPt.m_totalImgCount);
	sCmd.Replace(_T("#2"), strValue);

	strValue.Format(_T("%d"), dataPt.m_totalNgImgCount);
	sCmd.Replace(_T("#3"), strValue);

	strValue.Format(_T("%d"), dataPt.m_totalErrCount);
	sCmd.Replace(_T("#4"), strValue);

	strValue.Format(_T("%.2f"), dataPt.m_fNgRatio);
	sCmd.Replace(_T("#5"), strValue);

	strValue =  dataPt.m_strBatchNumber;
	sCmd.Replace(_T("#6"), strValue);

	strValue =  dataPt.m_strUser;
	sCmd.Replace(_T("#7"), strValue);

	auto vItem = dataPt.m_vItemStats;
	int nItemCount = (int)vItem.size();
	int index = 7;
	CString strIndex;
	for(int t = 0; t < nItemCount; t++)
	{
		strIndex.Format(_T("*%d"), index);
		strValue.Format(_T("%d"), vItem[t].m_nItemErrorCount);
		sCmd.Replace(strIndex, strValue);
		index++;
	}

	wstring wstrCmd = sCmd.GetString();
	strCmd = utils::Unicode2Utf8(wstrCmd);
	m_db.Excute(strCmd.c_str());
	return true;
}

void CSystemManager::LogGlueInjectorError(int nChIndex,long nInjectorID)
{
	::EnterCriticalSection(&m_csGIIT);
	m_dictGII_EC[nChIndex][nInjectorID]++;
	::LeaveCriticalSection(&m_csGIIT);
}

std::map<long, long> CSystemManager::GetGlueInjectorErrorStats(int nChIndex)
{
	std::map<long, long> retDict;
	if(m_dictGII_EC.find(nChIndex) != m_dictGII_EC.end())
	{
		::EnterCriticalSection(&m_csGIIT);
		retDict = m_dictGII_EC[nChIndex];
		::LeaveCriticalSection(&m_csGIIT);
	}

	return retDict;
}

void CSystemManager::RemoveObsoleteDataFromDatabase()
{
	auto pChConfigArray = gConfigMgr.GetChannelConfigPtr();
	if(pChConfigArray != nullptr && !pChConfigArray->empty())
	{
		SYSTEMTIME crtTime;
		::GetLocalTime(&crtTime);
		if(crtTime.wMonth == 1)
		{
			crtTime.wYear -= 1;
			crtTime.wMonth = 12;
		}
		else
		{
			crtTime.wMonth -= 1;
		}

		crtTime.wHour = 0;
		crtTime.wMinute = 0;
		crtTime.wSecond = 0;
		long long nTimestamp = utils::DateTime2Int64(crtTime);

		CString sDeadline;
		sDeadline.Format(_T("%I64d"), nTimestamp);
		std::string strDeadline = CT2A(sDeadline);

		for(auto it = pChConfigArray->begin(); it != pChConfigArray->end(); it++)
		{
			CHANNEL_CONFIG chConfig = it->second;
			TASK crtTask;
			if(!chConfig.GetCurrentTask(crtTask)) continue;

			std::string strTaskName = utils::Unicode2Utf8(crtTask.m_wstrTaskName);
			if(m_db.TableExists(strTaskName.c_str()))
			{
				std::string strCmd = "DELETE FROM ";
				strCmd.append(strTaskName);
				strCmd.append(" WHERE [Timestamp] < '");
				strCmd.append(strDeadline);
				strCmd.append("'");
				m_db.Excute(strCmd.c_str());
			}
		}

		std::string strVacuumCmd = "vacuum";
		m_db.Excute(strVacuumCmd.c_str());
	}
}

bool CSystemManager::IsTableExists(string strTableName)
{
	if(!m_db.TableExists(strTableName.c_str()))
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool CSystemManager::ExtractSystemStatsFromDatabase(long long lBgn, long long lEnd, std::map<long long, SYS_STAT_DATA>& dictDataPoint,CString strbatchnumber,CString username)
{
	dictDataPoint.clear();
	string strTableName = "SYS_STATS";
	if(!m_db.TableExists(strTableName.c_str()))
	{
		//set the Error Code here
		return false;
	}
	CString sBgn, sEnd;
	sBgn.Format(_T("%I64d"), lBgn-300);
	sEnd.Format(_T("%I64d"), lEnd);

	std::string strBgn = CT2A(sBgn);
	std::string strEnd = CT2A(sEnd);

	std::string strCmd = "SELECT * FROM ";
	strCmd += strTableName.c_str();
	strCmd += " WHERE [Timestamp] BETWEEN '";
	strCmd += strBgn.c_str();
	strCmd += "' AND '";
	strCmd += strEnd.c_str();
	strCmd += "'";

	if (!strbatchnumber.IsEmpty())
	{
		strCmd += "AND [BatchNumber] = '";
		std::string strBatchNumber = utils::Unicode2Utf8(strbatchnumber.GetString());
		strCmd += strBatchNumber.c_str();
		strCmd += "'";
	}

	//添加数据库 用户名删选！
	if (!username.IsEmpty())
	{
		strCmd += "AND [UserName] = '";
		std::string strusername = utils::Unicode2Utf8(username.GetString());
		strCmd += strusername.c_str();
		strCmd += "'";
	}


	CppSQLite3Query ret = m_db.Query(strCmd.c_str());

	int colCount = ret.numFields();
	if (colCount != 7)
	{
		return false;
	}
	int m_ALLTotolCount = 0 ;
	int m_ALLTotolLeftCount = 0;

	
	int PriTotolCount =0;
	int PriTotolLeftCount =0;


	while(!ret.eof())
	{
		SYS_STAT_DATA dataPt;
		dataPt.m_timestamp = ret.getInt64Field("Timestamp");
		dataPt.m_totalCount   = ret.getIntField("totalCount");
		dataPt.m_totalOkCount = ret.getIntField("totalLeftCount");
		dataPt.m_leftFlag = ret.getIntField("leftFlag");
		dataPt.m_strBatchNumber = ret.getStringField("BatchNumber");
		dataPt.m_strUser = ret.getStringField("UserName");//
		dataPt.m_strSysForeignKey =  ret.getStringField("SysforeignKey");

		dictDataPoint[dataPt.m_timestamp] = dataPt;
		ret.NextRow();
	}

	return !dictDataPoint.empty();
}

bool CSystemManager::ExtractDataFromDatabase(int nChIndex, wstring strTaskName, long long lBgn, long long lEnd, std::map<long long, SPC_DATA_PT>& dictDataPoint,CString strbatchnumber,CString username)
{
	std::string strTableName = utils::Unicode2Utf8(strTaskName);
	if(!m_db.TableExists(strTableName.c_str()))
	{
		//set the Error Code here
		return false;
	}
	CString sBgn, sEnd;
	sBgn.Format(_T("%I64d"), lBgn);
	sEnd.Format(_T("%I64d"), lEnd);

	std::string strBgn = CT2A(sBgn);
	std::string strEnd = CT2A(sEnd);

	std::string strCmd = "SELECT * FROM ";
	strCmd += strTableName.c_str();
	strCmd += " WHERE [Timestamp] BETWEEN '";
	strCmd += strBgn.c_str();
	strCmd += "' AND '";
	strCmd += strEnd.c_str();
	strCmd += "'";

	if (!strbatchnumber.IsEmpty())
	{
		strCmd += "AND [BatchNumber] = '";
		std::string strBatchNumber = utils::Unicode2Utf8(strbatchnumber.GetString());
		strCmd += strBatchNumber.c_str();
		strCmd += "'";
	}

	if (!username.IsEmpty())
	{
		strCmd += "AND [UserName] = '";
		std::string strusername = utils::Unicode2Utf8(username.GetString());
		strCmd += strusername.c_str();
		strCmd += "'";
	}


	CppSQLite3Query ret = m_db.Query(strCmd.c_str());
	// Parse query result here
	int colCount = ret.numFields();

	auto pChConfigArray = gConfigMgr.GetChannelConfigPtr();
	CHANNEL_CONFIG chConfig = (*pChConfigArray)[nChIndex];
	TASK retTask = chConfig.GetTask(strTaskName);

	std::vector<wstring> vFields;
	for(size_t i = 0; i < retTask.m_vItParam.size(); i++)
	{
		IT_PARAM itParam = retTask.m_vItParam[i];
 		if (i > 0)
		{
			std::wstring wstrFieldName = itParam.m_itNickname;
			vFields.push_back(wstrFieldName);
		}
	}
	size_t sz = vFields.size();
	if(sz != colCount - 7)
		return false;

	while(!ret.eof())
	{
		SPC_DATA_PT dataPt;
		long long timestamp      = ret.getInt64Field("Timestamp");
		dataPt.m_totalImgCount   = ret.getIntField("TotalCount");
		dataPt.m_totalNgImgCount = ret.getIntField("NGCount");
		dataPt.m_totalErrCount   = ret.getIntField("ErrorCount");
		dataPt.m_fNgRatio         = (float)ret.getFloatField("DefectRatio");
		dataPt.m_strBatchNumber  = ret.getStringField("BatchNumber");

		int nTableIndex = -1;
		int nFuncIndex  = -1;
		int nRgnID      = -1;
		for(size_t t = 0; t < sz; t++)
		{
			ITEM_STATS itStats;
			nFuncIndex  = -1;
		    nRgnID      = -1;
			std::wstring wstrNickname = vFields[t];
			if(retTask.GetFuncIndexAndRgnID(wstrNickname, nFuncIndex, nRgnID))
			{
				itStats.m_nFuncIndex = nFuncIndex;
				itStats.m_wstrName   = wstrNickname;
				itStats.m_nRgnID     = nRgnID;
			}
			std::string strFieldName = utils::Unicode2Utf8(wstrNickname);
			itStats.m_nItemErrorCount = ret.getIntField(strFieldName.c_str());
			dataPt.m_vItemStats.push_back(itStats);
		}
		dictDataPoint[timestamp] = dataPt;
		ret.NextRow();
	}
	// Correct data
	if (dictDataPoint.size() != 0 )
	{
		auto priorIt = dictDataPoint.begin();
		long nFirstCount = priorIt->second.m_totalImgCount;
		if(nFirstCount < 1000)
		{
			priorIt->second.m_fNgRatio = -1.0f;
		}
		auto tmpBgn = priorIt;
		tmpBgn++;
		for(auto it = tmpBgn; it != dictDataPoint.end();it++)
		{
			if(it->second.m_totalImgCount < 1000)
			{
				it->second.m_fNgRatio = -1.0f;
			}
			if(priorIt->second.m_totalImgCount >= it->second.m_totalImgCount)
			{
				priorIt->second.m_fNgRatio = -1.0f;
			}
			priorIt = it;
		}
	}
	return !dictDataPoint.empty();
}

void CSystemManager::ExtractSystemStatsFromMemory(long long lBgn, long long lEnd, std::map<long long, SYS_STAT_DATA>& dictDataPoint,CString strbatchnumber,CString username)
{
	SYS_STAT_DATA dataPt = m_crtSysStats;
	
	if (strbatchnumber.IsEmpty())
	{
		dataPt.m_leftFlag = -1;
		if(dataPt.m_totalOkCount > 0 && dataPt.m_timestamp >= lBgn && dataPt.m_timestamp <= lEnd)
		{
			dictDataPoint[dataPt.m_timestamp] = dataPt;
		}
	}
	else
	{
		if (strbatchnumber == dataPt.m_strBatchNumber)
		{
			dataPt.m_leftFlag = -1;
			if(dataPt.m_totalOkCount > 0 && dataPt.m_timestamp >= lBgn && dataPt.m_timestamp <= lEnd)
			{
				dictDataPoint[dataPt.m_timestamp] = dataPt;
			}
		}
	}

}

void CSystemManager::ExtractDataFromMemory(int nChIndex, wstring strTaskName, long long lBgn, long long lEnd, std::map<long long, SPC_DATA_PT>& dictDataPoint,CString strbatchnumber,CString username)
{
	for(size_t t = 0; t < m_vPanels.size(); t++)
	{
		auto pPanel = m_vPanels[t];
		if(pPanel->m_chIndex == nChIndex)
		{
			TASK crtTask = pPanel->GetCurrentTask();
			if(crtTask.m_wstrTaskName == strTaskName)
			{
				SPC_DATA_PT dataPt = pPanel->GetCurrentTaskLastStatsData(strbatchnumber,username);
				if(dataPt.m_totalImgCount > 0 && dataPt.m_timestamp >= lBgn && dataPt.m_timestamp <= lEnd)
				{
					dictDataPoint[dataPt.m_timestamp] = dataPt;
				}
			}
			return;
		}
	}
}
bool CSystemManager::FindCHFullTaskName(int curChIndex,wstring strTaskName,CString &StrTaskNameNew)
{
	StrTaskNameNew = m_vPanels[curChIndex-1]->FindAdaptTaskNameEx(strTaskName.c_str());
	return true;
}
bool CSystemManager::GetProductNameFromCH1(CString taskname,CString &strPattern)
{
	int i=0;
	for ( i = 0 ; i <taskname.GetLength();i++)
	{
		int unicode = (int)taskname.GetAt(i);
		if (unicode < 255)
			break;
	}

	if (i==taskname.GetLength())
	{
		return false;
	}
	strPattern = taskname.Mid(i);
	return true;
}
bool CSystemManager::QueryProductColumn(int curChIndex, wstring strTaskName,vector<CString> &m_vecColumnName)
{
	m_vecColumnName.clear();
	std::string strTableName = utils::Unicode2Utf8(strTaskName);

	std::string strCmd = "PRAGMA table_info([";
	strCmd += strTableName.c_str();
	strCmd += "])";
	CppSQLite3Query ret = m_db.Query(strCmd.c_str());
	while(!ret.eof())
	{
		string strColumnName =ret.getStringField("name");
		wstring wstrColumnName = utils::Utf8ToUnicode(strColumnName);
		CString ColumnName(wstrColumnName.c_str());
		m_vecColumnName.push_back(ColumnName);
		ret.NextRow();
	}
	return true;
}
bool CSystemManager::QueryProductStats(int nChIndex, wstring strTaskName, long long lBgn, long long lEnd, std::map<long long, SPC_DATA_PT>& dictDataPoint,CString strbatchnumber,CString username)
{
	if (strTaskName.empty())
	{
		return false;
	}

	ExtractDataFromDatabase(nChIndex, strTaskName, lBgn, lEnd, dictDataPoint,strbatchnumber,username);
	if(!IsChannelCameraRunning(nChIndex))
	{
		if (username.IsEmpty()||username == GetCurrentUser().m_strUsername.c_str())
		{
			ExtractDataFromMemory(nChIndex, strTaskName, lBgn, lEnd, dictDataPoint,strbatchnumber,username);
		}
	}

	return !dictDataPoint.empty();
}

bool CSystemManager::QuerySystemProductStats(long long lBgn, long long lEnd, std::map<long long, SYS_STAT_DATA>& dictDataPoint,CString strbatchnumber,CString username)
{
	ExtractSystemStatsFromDatabase(lBgn, lEnd, dictDataPoint,strbatchnumber,username);
	if(!IsChannelCameraRunning(m_nSysChIndex))
	{
		if (username.IsEmpty()||username == GetCurrentUser().m_strUsername.c_str())
		{
			ExtractSystemStatsFromMemory(lBgn, lEnd, dictDataPoint,strbatchnumber,username);
		}
	}

	return !dictDataPoint.empty();
}

void CSystemManager::ReportNoCameraConnection(int nChIndex)
{
	std::for_each(m_vPanels.begin(), m_vPanels.end(), [&](CChannelControlPanel* pPanel)
	{
		if(pPanel->m_chIndex == nChIndex)
		{
			m_dictChCameraConnection[nChIndex] = 0;
			pPanel->HandleNoCameraConnectionEvent();
		}
	});
}

// UAC management
bool CSystemManager::CreateUacTable()
{
	std::string strTableName("UAC");
	std::string strTableTitle("[Username] text primary key, [Password] text, [Level] integer");
	bool bRet = m_db.CreateTable(strTableName.c_str(), strTableTitle.c_str());

	return bRet;
}

bool CSystemManager::InsertDefualtUser()
{
	UAC uac;
	uac.m_strUsername = L"SMVSADMIN";
	uac.m_strPassword = L"smvsadmin";
	uac.m_nLevel      = UAC_ADMIN;
	m_dictUac[uac.m_strUsername] = uac;
	bool bRet = InsertUacData(uac);

	uac.m_strUsername = L"OPERATOR";
	uac.m_strPassword = L"operator";
	uac.m_nLevel      = UAC_OPERATOR;
	m_dictUac[uac.m_strUsername] = uac;
	bool bRet1 = InsertUacData(uac);

	return (bRet && bRet1);
}

bool CSystemManager::InsertUacData(const UAC& uacData)
{
	CString sCmd = _T("INSERT INTO UAC");
	sCmd.Append(_T(" VALUES('"));
	sCmd.Append(uacData.m_strUsername.c_str());
	sCmd.Append(_T("','"));
	sCmd.Append(uacData.m_strPassword.c_str());
	sCmd.Append(_T("',"));

	CString strLevel;
	strLevel.Format(_T("%d"), uacData.m_nLevel);
	sCmd.Append(strLevel);
	sCmd.Append(_T(")"));

	wstring wstrCmd = CT2W(sCmd);
	std::string strCmd = utils::Unicode2Utf8(wstrCmd);
	m_db.Excute(strCmd.c_str());

	return true;
}
bool CSystemManager::GetCurBatctNumberMinMaxTime(std::string strTableName,CString cstrBatchNumber,vector<CString> &m_vecTime)
{
	//查询当前批次号的起始时间和结束时间
	std::string strCmd = "SELECT max(Timestamp),min(Timestamp) from ";
	strCmd += strTableName.c_str();
	strCmd += " where BatchNumber = '";
	std::string strBatchNumber = utils::Unicode2Utf8(cstrBatchNumber.GetString());
	strCmd += strBatchNumber.c_str();
	strCmd += "'";


	CppSQLite3Query ret = m_db.Query(strCmd.c_str());
	while(!ret.eof())
	{
		CString strMinTimeStamp;
		strMinTimeStamp= ret.getStringField("min(Timestamp)");
		CString strMaxTimeStamp;
		strMaxTimeStamp=ret.getStringField("max(Timestamp)");

		m_vecTime.push_back(strMinTimeStamp);
		m_vecTime.push_back(strMaxTimeStamp);
		ret.NextRow();
	}
	return true;
}
vector<CString> CSystemManager::GetLastMonthBatchNumber(std::string strTableName,long long lBgn, long long lEnd)
{
	vector<CString> m_vec;
	if(!m_db.TableExists(strTableName.c_str()))
	{
		return m_vec;
	}

	//查询一个月的批次号Sql语句
	CString sBgn, sEnd;
	sBgn.Format(_T("%I64d"), lBgn);
	sEnd.Format(_T("%I64d"), lEnd);

	std::string strBgn = CT2A(sBgn);
	std::string strEnd = CT2A(sEnd);

	std::string strCmd = "SELECT distinct BatchNumber FROM ";
	strCmd += strTableName.c_str();
	strCmd += " WHERE [Timestamp] BETWEEN '";
	strCmd += strBgn.c_str();
	strCmd += "' AND '";
	strCmd += strEnd.c_str();
	strCmd += "'";

	//用户名，筛选


	CppSQLite3Query ret = m_db.Query(strCmd.c_str());
	while(!ret.eof())
	{
//		strBatchNumber=utils::Utf8ToUnicode(ret.getStringField("BatchNumber"));
		string stringBatchnumber = ret.getStringField("BatchNumber");
		wstring wstringBatchnumber = utils::Utf8ToUnicode(stringBatchnumber);
		CString strBatchNumber(wstringBatchnumber.c_str());
		if(!strBatchNumber.IsEmpty())
		{
			m_vec.push_back(strBatchNumber);
		}
		ret.NextRow();
	}	
	return m_vec;
}
bool CSystemManager::AddNewUser(const UAC& uacData, CString& sError)
{
	return InsertUacData(uacData);
}

bool CSystemManager::UpdateUser(const UAC& oldUac, const UAC& newUac, CString& sError)
{
	string strOldUsername = utils::Unicode2Utf8(oldUac.m_strUsername);

	string strNewName     = utils::Unicode2Utf8(newUac.m_strUsername);
	string strNewPassowrd = utils::Unicode2Utf8(newUac.m_strPassword);

	string strCmd("UPDATE UAC SET Username='");
	strCmd.append(strNewName); 
	strCmd.append("',");
	strCmd.append("Password='");
	strCmd.append(strNewPassowrd);
	strCmd.append("',");
	strCmd.append("Level='");
	string strLevel = boost::lexical_cast<string>(newUac.m_nLevel);
	strCmd.append(strLevel);
	strCmd.append("' WHERE Username='");
	strCmd.append(strOldUsername);
	strCmd.append("'");

	m_db.Excute(strCmd.c_str());
	return true;;
}

bool CSystemManager::DeleteUser(wstring sUsername)
{
	std::string strUsername = utils::Unicode2Utf8(sUsername);
	std::string strCmd("DELETE FROM UAC WHERE Username='");
	strCmd.append(strUsername);
	strCmd.append("'");
	m_db.Excute(strCmd.c_str());
	return true;
}

bool CSystemManager::SetCurrentUser(const wstring& strUsername)
{
	if(m_dictUac.find(strUsername) == m_dictUac.end())
	{
		return false;
	}
	m_crtUser = m_dictUac[strUsername];
	return true;
}

UAC CSystemManager::GetCurrentUser()
{
	return m_crtUser;
}

bool CSystemManager::GetAllUacInfo(std::map<wstring, UAC>& uacDict)
{
	std::string strUac("UAC");
	if(!m_db.TableExists(strUac.c_str()))
	{
		return false;
	}

	string strCmd("SELECT * FROM UAC");
	CppSQLite3Query ret = m_db.Query(strCmd.c_str());
	while(!ret.eof())
	{
		UAC uac;
		string strUsername = ret.getStringField("Username");
		uac.m_strUsername = utils::Utf8ToUnicode(strUsername.c_str());
		string strPassword = ret.getStringField("Password");
		uac.m_strPassword = utils::Utf8ToUnicode(strPassword);
		uac.m_nLevel      = ret.getIntField("Level");
		uacDict[uac.m_strUsername] = uac;
		ret.NextRow();
	}
	m_dictUac = uacDict;
	return uacDict.size() > 0;
}

bool CSystemManager::ValidateAuthority(CString sUsername, CString sPassword,int& nErrorCode)
{
	wstring strUsername = CT2W(sUsername);
	if(m_dictUac.find(strUsername) == m_dictUac.end())
	{
		nErrorCode = 0;
		return false;
	}
	wstring strPassword = CT2W(sPassword);
	UAC uac = m_dictUac[strUsername];
	if(uac.m_strPassword != strPassword)
	{
		nErrorCode = 1;
		return false;
	}
	m_crtUser = uac;

	return true;
}

bool CSystemManager::GetTaskChangedFlag()
{
	return !m_dictCh_TaskAction.empty();
}

void CSystemManager::SetChannelTaskChangedFlag(int nCh,eTASK_ACTION ta)
{
	m_dictCh_TaskAction[nCh] = ta;
}

void CSystemManager::ResetChannelTaskChangedFlag()
{
	m_dictCh_TaskAction.clear();
}

void CSystemManager::SetChImageSize(int nChIndex, const IMG_SZ& imgSz)
{
	m_dictChImgSz[nChIndex] = imgSz;
	m_displyManager.SetImageSize(nChIndex,imgSz);
}

IMG_SZ CSystemManager::GetChImageSize(int nChIndex)
{
	IMG_SZ retSz;
	if(m_dictChImgSz.find(nChIndex) != m_dictChImgSz.end())
	{
		return m_dictChImgSz[nChIndex];
	}
	return retSz;
}

void CSystemManager::ResetChannelStats(int nChIndex)
{
	std::for_each(m_vPanels.begin(), m_vPanels.end(),[&](CChannelControlPanel* pPanel)
	{
		if(pPanel->m_chIndex == nChIndex)
		{
			pPanel->ResetInternalStats();
		}
	});
}

CRect CSystemManager::GetChartRect()
{
	return m_rcChChart;
}

//添加参数（指定光源路数ID）
bool CSystemManager::ApplyNewLcParameter(LC lc,int CHID)
{
	if(m_nSerialPort == -1 || m_pSerialPort == nullptr)
	{
		return false;
	}
	if(lc.m_vChParam.empty())
	{
		return false;
	}
	string strCmd;
	std::string strHexCmd;
	if (CHID ==0)
	{
		if (lc.m_nActiveID != 15)
		{
			if ((lc.m_nActiveID )&(1<<3))
			{
				strHexCmd = lc.MakeLcCommandNew(strCmd,false,1);
				Sleep(125);
			}
			if ((lc.m_nActiveID )&(1<<2))
			{
				strHexCmd = lc.MakeLcCommandNew(strCmd,false,2);
				Sleep(125);
			}
			if ((lc.m_nActiveID )&(1<<1))
			{
				strHexCmd = lc.MakeLcCommandNew(strCmd,false,3);
				Sleep(125);
			}
			if ((lc.m_nActiveID )&(1<<0))
			{
				strHexCmd = lc.MakeLcCommandNew(strCmd,false,4);
				Sleep(125);
			}
		}
		else
		{
			strHexCmd = lc.MakeLcCommandNew(strCmd,false);
		}
	}
	else
	{
		strHexCmd = lc.MakeLcCommandNew(strCmd,false,CHID);
	}
	m_pSerialPort->SendCmdToPort(strHexCmd);

	return true;
}

bool CSystemManager::WriteLcParameter2EEP(const LC& lc)
{
 	if(m_nSerialPort == -1 || m_pSerialPort == nullptr)
	{
 		return false;
 	}
	string strCmd;

	if (lc.m_nMode == LC_TRIGGER || lc.m_nMode == LC_ON)
	{
		std::string strHexCmd = lc.MakeLcCommandNew(strCmd,true);
		m_pSerialPort->SendCmdToPort(strHexCmd);
	}
	else
	{
		std::string strHexCmd = lc.MakeLcCommand(strCmd,true);
		m_pSerialPort->SendCmdToPort(strHexCmd);
		Sleep(125);
		strHexCmd = lc.MakeLcCommandNew(strCmd,true);
		m_pSerialPort->SendCmdToPort(strHexCmd);
	}	
	return true;
}

void CSystemManager::SendLightControllerTestSignal(int nChIndex)
{
	m_nTestFlag = nChIndex;
	Proc.MemProc = &CSystemManager::LightControllerTestingThreadFunc;
	UINT threadId;
	HANDLE hDbThread = (HANDLE)_beginthreadex(NULL, 0, Proc.ThreadProc, (LPVOID)this, 0, &threadId);
	if(hDbThread == NULL)
	{
		AfxMessageBox(_T("Starting LC testing thread failed"), MB_OK);
	}
}

unsigned int __stdcall CSystemManager::LightControllerTestingThreadFunc()
{
	if(m_pSerialPort == nullptr)
	{
		return 0;
	}

	for(auto it = m_vPanels.begin(); it != m_vPanels.end(); it++)
	{
		CChannelControlPanel* pPanel = *it;
		if(m_nTestFlag != -1 && pPanel->m_chIndex != m_nTestFlag)
		{
			continue;
		}
		LIGHT_SETTING ls = pPanel->m_channelConfig.m_lightCtrlSetting;

		TASK crtTask;
		if(!pPanel->m_channelConfig.GetCurrentTask(crtTask))
		{
			return 0;
		}
		if (crtTask.m_bSelfLearning)
		{
			return 0;
		}
		if(ls.m_dictParamGroup.find(crtTask.m_wstrTaskName) != ls.m_dictParamGroup.end())
		{
			std::vector<LC> vLC = ls.m_dictParamGroup[crtTask.m_wstrTaskName];
			if(vLC.empty())
			{
				for(size_t t = 0; t< ls.m_vLcID.size(); t++)
				{
					LC tmpLc;
					tmpLc.m_nID = ls.m_vLcID[t];
					vLC.push_back(tmpLc);
				}
			}
			for(size_t t = 0; t < vLC.size(); t++)
			{
				LC lc = vLC[t];
				if(!lc.m_vChParam.empty())
				{
					string strCmd;
					//光源启动检查线程，常亮和触发模式下，把当前配置文件参数写入光源控制器
					if (lc.m_nMode == LC_TRIGGER || lc.m_nMode == LC_ON)
					{
                        if(lc.m_nActiveID == 0) 
                        {
                            continue;
                        }
						if (lc.m_nActiveID == 15)
						{
							ResetEvent(m_hLcTestReadyEvent);
							std::string strHexCmd = lc.MakeLcCommandNew(strCmd,false);
				//			gLogger.QueueUpLog(utils::s2ws(strCmd),true);
							m_pSerialPort->SendCmdToPort(strHexCmd);
							DWORD dwRet = ::WaitForSingleObject(m_hLcTestReadyEvent,150);
							if(dwRet == WAIT_TIMEOUT)
							{
								continue;
							}
						}
						else 
						{
							if (lc.m_nActiveID&(1<<3))
							{
								ResetEvent(m_hLcTestReadyEvent);
								std::string strHexCmd = lc.MakeLcCommandNew(strCmd,false,1);
					//			gLogger.QueueUpLog(utils::s2ws(strCmd),true);

								m_pSerialPort->SendCmdToPort(strHexCmd);
								DWORD dwRet = ::WaitForSingleObject(m_hLcTestReadyEvent,150);
								if(dwRet == WAIT_TIMEOUT)
								{
									continue;
								}
							}
							if (lc.m_nActiveID&(1<<2))
							{
								ResetEvent(m_hLcTestReadyEvent);
								std::string strHexCmd = lc.MakeLcCommandNew(strCmd,false,2);
					//			gLogger.QueueUpLog(utils::s2ws(strCmd),true);

								m_pSerialPort->SendCmdToPort(strHexCmd);
								DWORD dwRet = ::WaitForSingleObject(m_hLcTestReadyEvent,150);
								if(dwRet == WAIT_TIMEOUT)
								{
									continue;
								}	
							}
							if (lc.m_nActiveID&(1<<1))
							{
								ResetEvent(m_hLcTestReadyEvent);
								std::string strHexCmd = lc.MakeLcCommandNew(strCmd,false,3);
			  		//		    gLogger.QueueUpLog(utils::s2ws(strCmd),true);
								m_pSerialPort->SendCmdToPort(strHexCmd);

								DWORD dwRet = ::WaitForSingleObject(m_hLcTestReadyEvent,150);
								if(dwRet == WAIT_TIMEOUT)
								{
									continue;
								}
							}
							 if (lc.m_nActiveID&(1<<0))
							{
								ResetEvent(m_hLcTestReadyEvent);
								std::string strHexCmd = lc.MakeLcCommandNew(strCmd,false,4);
				//				gLogger.QueueUpLog(utils::s2ws(strCmd),true);
						    	m_pSerialPort->SendCmdToPort(strHexCmd);

								DWORD dwRet = ::WaitForSingleObject(m_hLcTestReadyEvent,150);
								if(dwRet == WAIT_TIMEOUT)
								{
									continue;
								}
							}
						}
					}
					else
					{
                        if(lc.m_nActiveID == 0)
                        {
                            continue;
                        }

						ResetEvent(m_hLcTestReadyEvent);
						std::string strHexCmd = lc.MakeLcCommand(strCmd,false);
						m_pSerialPort->SendCmdToPort(strHexCmd);
						DWORD dwRet = ::WaitForSingleObject(m_hLcTestReadyEvent,150);
						if(dwRet == WAIT_TIMEOUT)
						{
							continue;
						}

						ResetEvent(m_hLcTestReadyEvent);
						strHexCmd = lc.MakeLcCommandNew(strCmd,false);
						m_pSerialPort->SendCmdToPort(strHexCmd);
						dwRet = ::WaitForSingleObject(m_hLcTestReadyEvent,150);
						if(dwRet == WAIT_TIMEOUT)
						{
							continue;
						}
					}
				}
			}
		}
	}
	// This thread cannot exit until all I/O requests are completed.
	// 通知检测线程已经完毕！
	SetEvent(m_dLcTestOverEvent);
//	::WaitForSingleObject(m_dLcTestOverEvent,INFINITE);
	return 0;
}

void CSystemManager::VerifyLightControllerTestResult()
{
	for(size_t t = 0; t < m_vPanels.size(); t++)
	{
        m_vPanels[t]->VerifyLightControllerTestResult(m_setCheckedLcID);
	}
}

void CSystemManager::RemoveLcIdFromCheckResult(const std::vector<int>& vLcID)
{
    if(m_setCheckedLcID.empty() || vLcID.empty())
        return;
    for(auto it = vLcID.begin(); it != vLcID.end();it++)
    {
        int nId = *it;
        if(m_setCheckedLcID.find(nId) != m_setCheckedLcID.end())
        {
            m_setCheckedLcID.erase(nId);
        }
    }
}

void CSystemManager::SetNewModelName(CString sNewModelName)
{
	m_sNewModelName = sNewModelName;
}

CString CSystemManager::GetNewModelName()
{
	return m_sNewModelName;
}

unsigned int __stdcall CSystemManager::ChannelStatusSentryThreadFunc()
{
	while(m_nExitFlag == 0)
	{
		for(size_t t = 0; t < m_vPanels.size(); t++)
		{
			CChannelControlPanel* pPanel = m_vPanels[t];
			pPanel->ExecuteQueryCmd();
		}
		Sleep(3000);
//#ifndef  EOE_PRODUCT
//		if(m_dongleKey.GetAuthorizationType()== 0 && !m_dongleKey.GetLockedFlag())
//		{
//			m_nActionCount++;
//			if(m_nActionCount % 10 == 0) // Check the dongle key every 15 seconds
//			{
//				
//				bool bValid = m_dongleKey.CheckDatetimePeriodically();
//				if(!bValid)
//				{
//					PauseAllCamera();
//					StopImgProcessor();
//					return 0;
//				}
//				
//				if(!m_dongleKey.TrySyncNetworkTime())
//				{
//					PauseAllCamera();
//					StopImgProcessor();
//					return 0;
//				}
//				m_dongleKey.WriteMaxSystemDt();
//			}
//			if(m_nActionCount % 3600 == 0)
//			{
//				if(!m_dongleKey.CheckExpirationDatetime(false))
//				{
//					PauseAllCamera();
//					StopImgProcessor();
//					return 0;
//				}
//			}
//		}
//#endif
	}
	return 0;
}

void CSystemManager::StartChannelStatusSentryThread()
{
	Proc.MemProc = &CSystemManager::ChannelStatusSentryThreadFunc;
	UINT threadId;
	HANDLE hCSThread = (HANDLE)_beginthreadex(NULL, 0, Proc.ThreadProc, (LPVOID)this, 0, &threadId);
	if(hCSThread == NULL)
	{
		AfxMessageBox(_T("Starting channel status sentry thread failed"), MB_OK);
	}
	//::SetThreadPriority(hCSThread, THREAD_PRIORITY_NORMAL);
	m_vHandle.push_back(hCSThread);
}

unsigned int __stdcall CSystemManager::DatabaseSentryThreadFunc()
{
	wstring     wstrTaskName;
	SPC_DATA_PT dataPt;
	wstring     wstrSysTableName;
	SYS_STAT_DATA sysDataPt;
	bool        bIsAnyData = false;
	bool        bIsAnySysData = false;
	while((bIsAnyData = IsAnyStatsData(wstrTaskName,dataPt)) || (bIsAnySysData = IsAnySystemStatsData(wstrSysTableName,sysDataPt)) || m_nExitFlag == 0)
	{
		if(!bIsAnyData && !bIsAnySysData)
		{
			Sleep(100);
			continue;
		}
		else
		{
			if(bIsAnyData && !InsertChannelStatsData(wstrTaskName, dataPt))
			{
				wstring wstrLog = L"Insert Channel SPC data failed - ";
				wstrLog.append(wstrTaskName);
				gLogger.QueueUpLog(wstrLog);
				continue;
			}
			else if(bIsAnySysData && !InsertSystemStatsData(wstrSysTableName,sysDataPt))
			{
				wstring wstrLog = L"Insert System SPC data failed - ";
				wstrLog.append(wstrSysTableName);
				gLogger.QueueUpLog(wstrLog);
				continue;
			}
			Sleep(50);
		}
	}
	return 0;
}

void CSystemManager::StartDatabaseSentryThread()
{
	Proc.MemProc = &CSystemManager::DatabaseSentryThreadFunc;
	UINT threadId;
	HANDLE hDbThread = (HANDLE)_beginthreadex(NULL, 0, Proc.ThreadProc, (LPVOID)this, 0, &threadId);
	if(hDbThread == NULL)
	{
		AfxMessageBox(_T("Starting database sentry thread failed"), MB_OK);
	}
	//::SetThreadPriority(hDbThread, THREAD_PRIORITY_BELOW_NORMAL);
	m_vHandle.push_back(hDbThread);
}

void CSystemManager::StartImageDisplayThread()
{
	m_displyManager.StartImageDisplayThread();
}

void CSystemManager::StopWorkerThread()
{
	::InterlockedIncrement(&m_nExitFlag);
	::InterlockedIncrement(&m_displyManager.m_nExitFlag);
}

void CSystemManager::CheckTaskNameUniqueness()
{
	std::set<wstring> setCrtTaskName;
	auto pChConfigDict = gConfigMgr.GetChannelConfigPtr();
	std::for_each(pChConfigDict->begin(), pChConfigDict->end(),[&](std::pair<int,CHANNEL_CONFIG> p)
	{
		TASK* pTask = p.second.GetCurrentTask();
		if(pTask != nullptr)
		{
			size_t nOld = setCrtTaskName.size();
			setCrtTaskName.insert(pTask->m_wstrTaskName);
			size_t nNew = setCrtTaskName.size();
			if(nNew == nOld)
			{
				CString sMsg;
				sMsg.LoadString(IDS_STRING_CH_TASK_NAME_DUPLICATED);
                gLogger.QueueUpLog(sMsg.GetString());
			}
		}
	});
}

void CSystemManager::ReceiveStatsDataPoint(TASK& task, const SPC_DATA_PT& dataPt)
{
	if(task.m_bSelfLearning)
		return;

	if(m_dictStats.find(task.m_wstrTaskName) == m_dictStats.end())
	{
		m_dictStats[task.m_wstrTaskName] = new bounded_buffer<SPC_DATA_PT>(32);
		if(!CreateTable(task))
		{
			wstring wstrLog = L"Create database failed - ";
			wstrLog.append(task.m_wstrTaskName);
			gLogger.QueueUpLog(wstrLog);
		}
	}
	m_dictStats[task.m_wstrTaskName]->push_front(dataPt);
}

void CSystemManager::ReceiveSystemStatsDataPoint(bool bLastData)
{
	if(m_crtSysStats.m_totalOkCount > 0 && m_nSysChIndex != -1)
	{
		wstring wstrTableName;
		wstrTableName=_T("SYS_STATS");

		if(m_dictSystemStatsData.find(wstrTableName) == m_dictSystemStatsData.end())
		{
			m_dictSystemStatsData[wstrTableName] = new bounded_buffer<SYS_STAT_DATA>(32);
			if(!CreateSystemDataTable(wstrTableName))
			{
				wstring wstrLog = L"Create system statistics database failed - ";
				wstrLog.append(wstrTableName);
				gLogger.QueueUpLog(wstrLog);
			}
		}
		if (bLastData)
		{
			m_crtSysStats.m_leftFlag = -1;
		}
		else
		{
			m_crtSysStats.m_leftFlag = 0;
		}
		m_dictSystemStatsData[wstrTableName]->push_front(m_crtSysStats);

        if(bLastData)
        {
            m_crtSysStats.ClearData();
        }
	}
}

bool CSystemManager::IsAnySystemStatsData(wstring& wstrTaskName,SYS_STAT_DATA& dataPt)
{
	if(m_dictSystemStatsData.empty())
	{
		return false;
	}
	for(auto it = m_dictSystemStatsData.begin(); it != m_dictSystemStatsData.end(); it++)
	{
		if(it->second->is_not_empty())
		{
			wstrTaskName = it->first;
			it->second->pop_back(&dataPt);
			return true;
		}
	}
	return false;
}

bool CSystemManager::IsAnyStatsData(wstring& wstrTaskName,SPC_DATA_PT& dataPt)
{
	if(m_dictStats.empty())
	{
		return false;
	}
	for(auto it = m_dictStats.begin(); it != m_dictStats.end(); it++)
	{
		if(it->second->is_not_empty())
		{
			wstrTaskName = it->first;
			it->second->pop_back(&dataPt);
			return true;
		}
	}
	return false;
}

bool CSystemManager::ModifyDatabaseTable(TASK& oldTask, TASK& newTask)
{
	int nOldCount = oldTask.GetItemCount();
	int nNewCount = newTask.GetItemCount();

	std::string strNewTaskName = utils::Unicode2Utf8(newTask.m_wstrTaskName);
	if(!m_db.TableExists(strNewTaskName.c_str()))
	{
		return true;
	}
	std::vector<IT_PARAM> vOldItParam = oldTask.m_vItParam;
	std::vector<IT_PARAM> vNewItParam = newTask.m_vItParam;
	string strTableName = utils::Unicode2Utf8(oldTask.m_wstrTaskName);
	if(nOldCount > nNewCount) // delete some columns
	{
		// Create tmp tabel
		std::string strCmd = "CREATE TABLE tmpTable AS SELECT [Timestamp],[TotalCount],[NGCount],[ErrorCount],[DefectRatio],[BatchNumber],[UserName]";
		for(size_t t = 0; t < vNewItParam.size(); t++)
		{
			IT_PARAM ip = vNewItParam[t];
			if(t > 0 )
			{
				strCmd.append(",[");
				strCmd.append(utils::Unicode2Utf8(ip.m_itNickname));
				strCmd.append("]");
			}
		}
		strCmd.append(" FROM ");
		strCmd.append(strTableName);

		if(SQLITE_OK == m_db.Excute(strCmd.c_str()))
		{
			strCmd = "DROP TABLE IF EXISTS ";
			strCmd.append(strTableName);
			if(SQLITE_OK == m_db.Excute(strCmd.c_str()))
			{
				strCmd = "ALTER TABLE tmpTable RENAME TO ";
				strCmd.append(strTableName);
				if(SQLITE_OK == m_db.Excute(strCmd.c_str()))
				{
					return true;
				}
			}
		}
	}
	if(nOldCount == nNewCount) //rename some columns
	{
		string strCmd = "ALTER TABLE " +  strTableName + " RENAME TO tmp_" + strTableName;
		if(SQLITE_OK == m_db.Excute(strCmd.c_str()))
		{
			if(CreateTable(newTask))
			{
				strCmd = "INSERT INTO " + strTableName + " SELECT * FROM tmp_" + strTableName;
				if(SQLITE_OK == m_db.Excute(strCmd.c_str()))
				{
					strCmd = "DROP TABLE tmp_" + strTableName;
					if(SQLITE_OK == m_db.Excute(strCmd.c_str()))
					{
						return true;
					}
				}
			}
		}
	}
	if(nOldCount < nNewCount) // Insert a column
	{
		string strNewAlgo;
		int nIndex = 0;
		std::for_each(vNewItParam.begin(), vNewItParam.end(),[&](IT_PARAM ip)
		{
			if(nIndex > 0)
			{
				auto it = std::find_if(vOldItParam.begin(), vOldItParam.end(), [&](IT_PARAM innerIp)->bool
				{
					return (innerIp.IsTestAlgo() && ip.m_itNickname == innerIp.m_itNickname);
				});
				if(it == vOldItParam.end())
				{
					strNewAlgo = utils::Unicode2Utf8(ip.m_itNickname);
				}
			}
			nIndex++;
		});
		string strCmd = "ALTER TABLE ";
		strCmd.append(strTableName);
		strCmd.append(" ADD COLUMN ");
		strCmd.append(strNewAlgo);
		strCmd.append(" integer default(0)");
		if(SQLITE_OK == m_db.Excute(strCmd.c_str()))
		{
			return true;
		}
	}
	return false;
}

void CSystemManager::UpdateChConfig(int nChIndex,CHANNEL_CONFIG chConfig)
{
	std::for_each(m_vPanels.begin(), m_vPanels.end(), [&](CChannelControlPanel* pPanel)
	{
		if(pPanel->m_chIndex == nChIndex)
		{
			pPanel->m_channelConfig = chConfig;
		}
	});
}

long  CSystemManager::IncreaseSyncFlag(int nChIndex)
{
	return ::InterlockedIncrement(&m_dictSyncFlag[nChIndex]);
}

long CSystemManager::DecreaseSyncFlag(int nChIndex)
{
	return ::InterlockedDecrement(&m_dictSyncFlag[nChIndex]);
}

void CSystemManager::ResetSyncFlag(int nChIndex)
{
	::InterlockedExchange(&m_dictSyncFlag[nChIndex],0);
}

CHANNEL_CONFIG*  CSystemManager::GetChConfigReference(int nChIndex)
{
	for(auto it = m_vPanels.begin(); it != m_vPanels.end(); it++)
	{
		CChannelControlPanel* pPanel = *it;
		if(pPanel->m_chIndex == nChIndex)
		{
			return &pPanel->m_channelConfig;
		}
	}
	return nullptr;
}

void CSystemManager::UpdateChannelStats()
{
	for(auto it = m_vPanels.begin(); it != m_vPanels.end(); it++)
	{
		CChannelControlPanel* pPanel = *it;
		pPanel->UpdateChannelStatistics();
	}
}

void CSystemManager::GetChDeviceData(int nChIndex,CHANNEL_STATUS& data)
{
	for(auto it = m_vPanels.begin(); it != m_vPanels.end(); it++)
	{
		CChannelControlPanel* pPanel = *it;
		if(pPanel->m_chIndex == nChIndex)
		{
			pPanel->GetChDeviceData(data);
			return;
		}
	}
}

bool CSystemManager::ReadCommonPrerequisiteFiles()
{
	TCHAR  moduleFileName[MAX_PATH];  
	::GetModuleFileName(NULL,moduleFileName, MAX_PATH);
	(_tcsrchr(moduleFileName, _T('\\')))[1] = 0;
	CString strPath = moduleFileName;
	string strDataFilePath = CT2A(strPath);
	strDataFilePath.append("\\data\\CommonFiles\\");
	//StandardImg
	string StandardImgPath = strDataFilePath + "Standard.tiff";
	if(!_access(StandardImgPath.c_str(),0))
	{
		read_image(&m_canOutsideCommonParam.StandardImg, StandardImgPath.c_str());
	}
	else
	{
		return false;
	}

	//FlagModelRegion
	string  FlagModelRegionPath = strDataFilePath + "\\FlagModelRegion.reg";
	if(!_access(FlagModelRegionPath.c_str(),0))
	{
		read_region(&m_canOutsideCommonParam.FlagModelRegion, FlagModelRegionPath.c_str());
	}
	else
	{
		return false;
	}
	//SeamRegionInStdImg
	string  SeamRegionInStdImgPath = strDataFilePath + "\\SeamRegionInStdImg.reg";
	if(!_access(SeamRegionInStdImgPath.c_str(),0))
	{
		read_region(&m_canOutsideCommonParam.SeamRegionInStdImg, SeamRegionInStdImgPath.c_str());
	}
	else
	{
		return false;
	}
	//StdModelRegionOrigin
	string  StdModelRegionOriginPath = strDataFilePath + "\\StdModelRegionOrigin.reg";
	if(!_access(StdModelRegionOriginPath.c_str(),0))
	{
		read_region(&m_canOutsideCommonParam.StdModelRegion, StdModelRegionOriginPath.c_str());
	}
	else
	{
		return false;
	}
	//StitchedImgWidthUnion
	string StitchedImgWidthUnionPath = strDataFilePath + "StitchedImgWidthUnion.tup";
	if(!_access(StitchedImgWidthUnionPath.c_str(),0))
	{
		read_tuple(StitchedImgWidthUnionPath.c_str(), &m_canOutsideCommonParam.StitchedImgWidthUnion);		
	}
	else
	{
		return false;
	}	
	//ModelIDs
	for(int Index = 1; Index <= 32; Index++)
	{
		string ModelIDPath = strDataFilePath + "Models\\" + utils::to_string(Index) + ".shm";		
		HTuple ModelIDTemp;
		if (!_access(ModelIDPath.c_str(),0))
		{
			read_shape_model(ModelIDPath.c_str(), &ModelIDTemp);
			m_canOutsideCommonParam.ModelIDs.Append(ModelIDTemp);			
		}
		else
		{			
			m_canOutsideCommonParam.ModelIDs = HTuple();
			return false;
		}		
	}
	//RowModels
	string RowModelsPath = strDataFilePath + "ModelRows.tup";
	if(!_access(RowModelsPath.c_str(),0))
	{
		read_tuple(RowModelsPath.c_str(), &m_canOutsideCommonParam.ModelRows);	
	}
	else
	{
		return false;
	}	
	//ColModels
	string ColModelsPath = strDataFilePath + "ModelCols.tup";
	if(!_access(ColModelsPath.c_str(),0))
	{
		read_tuple(ColModelsPath.c_str(), &m_canOutsideCommonParam.ModelCols);	
	}
	else
	{
		return false;
	}
	string FlagModelIDPath = strDataFilePath + "FlagModelID.shm";
	if(!_access(FlagModelIDPath.c_str(),0))
	{
		read_shape_model(FlagModelIDPath.c_str(), &m_canOutsideCommonParam.FlagModelID);
	}
	else
	{
		clear_shape_model(m_canOutsideCommonParam.FlagModelID);
		return false;
	}
	//SortedFlagColsInStdImg
	string SortedFlagColsInStdImgPath = strDataFilePath + "SortedFlagColsInStdImg.tup";
	if(!_access(SortedFlagColsInStdImgPath.c_str(),0))
	{
		read_tuple(SortedFlagColsInStdImgPath.c_str(), &m_canOutsideCommonParam.SortedFlagColsInStdImg);	
	}
	else
	{
		return false;
	}
	string StitchingParamColPath = strDataFilePath + "StitchingParamCol.tup";
	if(!_access(StitchingParamColPath.c_str(),0))
	{
		read_tuple(StitchingParamColPath.c_str(), &m_canOutsideCommonParam.StitchingParamCol);		
	}
	else
	{
		return false;
	}

	string MappedImgWidthsPath = strDataFilePath + "MappedImgWidths.tup";
	if(!_access(MappedImgWidthsPath.c_str(),0))
	{
		read_tuple(MappedImgWidthsPath.c_str(), &m_canOutsideCommonParam.MappedImgWidths);		
	}
	else
	{
		return false;
	}
	return true;
}

void CSystemManager:: GetApproxRectsInStdImg(HTuple MatchedFlagCol, HTuple CameraIndexOfFlag,HTuple* LeftCol4ImgsInStd, HTuple* RightCol4ImgsInStd)
{
	HTuple ColLeft4Imgs = HTuple(4, -1.0);
	ColLeft4Imgs[1] = 0;
	ColLeft4Imgs[2] = m_canOutsideCommonParam.StitchedImgWidthUnion[2] - m_canOutsideCommonParam.MappedImgWidths[2];	
	ColLeft4Imgs[3] = m_canOutsideCommonParam.StitchedImgWidthUnion[3] - m_canOutsideCommonParam.MappedImgWidths[3];
	ColLeft4Imgs[4] = m_canOutsideCommonParam.StitchedImgWidthUnion[4] - m_canOutsideCommonParam.MappedImgWidths[4];

	HTuple ColRight4Imgs = m_canOutsideCommonParam.StitchedImgWidthUnion;
	ColRight4Imgs[0] = -1.0;

	HTuple LeftColOfFlagImgInStd;
	tuple_sub(m_canOutsideCommonParam.SortedFlagColsInStdImg, MatchedFlagCol, &LeftColOfFlagImgInStd);
	tuple_sort(LeftColOfFlagImgInStd, &LeftColOfFlagImgInStd);
	HTuple Offset;
	tuple_sub(LeftColOfFlagImgInStd, ColLeft4Imgs[CameraIndexOfFlag], &Offset);

	HTuple LeftCol4ImgsInStdTemp;
	HTuple RightCol4ImgsInStdTemp;
	tuple_add(ColLeft4Imgs, Offset[0], LeftCol4ImgsInStd);
	tuple_add(ColLeft4Imgs, Offset[1], &LeftCol4ImgsInStdTemp);

	tuple_add(ColRight4Imgs, Offset[0], RightCol4ImgsInStd);
	tuple_add(ColRight4Imgs, Offset[1], &RightCol4ImgsInStdTemp);

	(*LeftCol4ImgsInStd)[0] = -1.0;
	(*RightCol4ImgsInStd)[0] = -1.0;

	HTuple StdImgWidth, StdImgHeight;
	get_image_size(m_canOutsideCommonParam.StandardImg, &StdImgWidth, &StdImgHeight);

	for (int Index = 1; Index <= 4; Index++)
	{
		if ((*LeftCol4ImgsInStd)[Index].D() < 50)
		{
			(*LeftCol4ImgsInStd)[Index]  = LeftCol4ImgsInStdTemp[Index];
			(*RightCol4ImgsInStd)[Index] = RightCol4ImgsInStdTemp[Index];
		}
		if ((*RightCol4ImgsInStd)[Index].D() > StdImgWidth - 50)
		{
			if (RightCol4ImgsInStdTemp[Index] > StdImgWidth)
			{
				HTuple DistOf2Flag = m_canOutsideCommonParam.SortedFlagColsInStdImg[1] - m_canOutsideCommonParam.SortedFlagColsInStdImg[0];
				RightCol4ImgsInStdTemp[Index] = RightCol4ImgsInStdTemp[Index]- 2 *DistOf2Flag;
				LeftCol4ImgsInStdTemp[Index]  = LeftCol4ImgsInStdTemp[Index] - 2*DistOf2Flag;
			}
			RightCol4ImgsInStd[Index] = RightCol4ImgsInStdTemp[Index];
			LeftCol4ImgsInStd[Index]  = LeftCol4ImgsInStdTemp[Index];
		}
	}
}

bool CSystemManager::IsLcSerialPortOk()
{
	return m_bLcSerialPortOk;
}

void CSystemManager::AddPendingImage(int nChIndex, IMAGE_BOX crtImgBox,PREPROCESS_RESULT chData)
{
	m_vCOImage[nChIndex - 1] = crtImgBox;
	m_vMatchData[nChIndex - 1] = chData;
	m_nCanImgCount++;

	if(m_nCanImgCount == 4)
	{
		m_nCanImgCount = 0;
		std::vector<IMAGE_BOX> tmpImgArray = m_vCOImage;
		std::vector<PREPROCESS_RESULT> tmpMatchData = m_vMatchData;

		float fMaxScore = -100.0f;
		PREPROCESS_RESULT tmpBestChData;

		for(int i = 0; i< 4;i++)
		{
			if(tmpMatchData[i].MatchedFlagScore.Num() > 0)
			{
				float tmpMaxScore = (float)tmpMatchData[i].MatchedFlagScore[0].D();
				if(tmpMaxScore > fMaxScore)
				{
					fMaxScore = tmpMaxScore;
					tmpBestChData = tmpMatchData[i];
				}
			}
		}

		if(fMaxScore > 0.0f)
		{
			HTuple LeftCol4ImgsInStd;
			HTuple RightCol4ImgsInStd;

			GetApproxRectsInStdImg(tmpBestChData.MatchedFlagCol,tmpBestChData.CameraIndexOfFlag,&LeftCol4ImgsInStd,&RightCol4ImgsInStd);
			for(size_t t = 0; t < 4; t++)
			{
				CImageProcessor* ptr = m_vChProcessorArray[0][t];
				ptr->PassInParam(tmpBestChData.CameraIndexOfFlag,tmpBestChData.HomMat2DForFlag,LeftCol4ImgsInStd,RightCol4ImgsInStd);
				//Sleep(1);
				ptr->ReceiveImage(tmpImgArray[t]);
			}
		}
	}
}

void CSystemManager::CreateImageJunction(int nChIndex, int nCameraCount)
{
	IMAGE_JUNCTION imgJuction(nCameraCount);
	m_dictChIndex_ImgJunction[nChIndex] = imgJuction;
}

void CSystemManager::CollectImageFromGroupCamera(int nChIndex, int nGroupIndex, IMAGE_BOX& crtProduct)
{	
    QueueGroupImage(nChIndex, nGroupIndex, crtProduct);	
    HarvestGroupImage(nChIndex);	
}

void CSystemManager::QueueGroupImage(int nChIndex, int nGroupIndex, IMAGE_BOX& crtProduct)
{
// 	SYSTEMTIME dt;
// 	::GetLocalTime(&dt);
// 	CString sTimestamp;
// 	sTimestamp.Format(_T("%2d-%2d | %02d:%02d:%02d:%03d    --- #%d\n"),dt.wMonth,dt.wDay,dt.wHour, dt.wMinute,dt.wSecond,dt.wMilliseconds,nGroupIndex);
// 	OutputDebugString(sTimestamp);
	LARGE_INTEGER crtStamp;
	QueryPerformanceCounter(&crtStamp);

	::EnterCriticalSection(&m_imgJunctionCs);

	IMAGE_JUNCTION& imgJunction = m_dictChIndex_ImgJunction[nChIndex];
	auto & imageSlot = imgJunction.m_vImageBuffer[nGroupIndex];
	if (imageSlot.m_timestamp.QuadPart != 0)
	{
		wostringstream woss;
		woss << L"Old image replaced: ["
			<< imageSlot.m_timestamp.QuadPart << L"->" << crtStamp.QuadPart
			<< L"] CH" << nChIndex << L" - Camera #" << nGroupIndex;
		gLogger.QueueUpLog(woss.str(),true);
	}
	else
	{
		imgJunction.m_nImgCount++;
	}
	imageSlot.m_imageBox = crtProduct;
	imageSlot.m_timestamp = crtStamp; //hzh remarked

	::LeaveCriticalSection(&m_imgJunctionCs);//
}

bool CSystemManager::IsAnyPendingGroup()
{
	if(m_dictChIndex_ImgJunction.empty())
		return true;
	for(auto it = m_dictChIndex_ImgJunction.begin();it != m_dictChIndex_ImgJunction.end(); it++)
	{
		if(it->second.m_nImgCount)
		{
			return true;
		}
	}
	return false;
}

void CSystemManager::ResetObsoleteImage(IMAGE_JUNCTION& imgJunction)
{
	for (auto i = imgJunction.m_vImageBuffer.begin(); i != imgJunction.m_vImageBuffer.end(); ++i)
	{
		i->m_imageBox.m_img = imgJunction.m_fakeImg;
	}
}

void CSystemManager::DisptchGroupImage(int nChIndex,IMAGE_JUNCTION& imgJunction)
{
	std::vector<CImageProcessor*> vGroupProcessor = m_vChProcessorArray[nChIndex - 1];
	size_t szProcessor = vGroupProcessor.size();
	size_t szCamera    = m_vPanels[nChIndex - 1]->m_channelConfig.m_vecCameraSetting.size();
	
	
	if(szProcessor == szCamera)
	{
		int FrameID =0;
		bool bSendTrue = true;
		
		for(size_t t = 0; t < szProcessor; t++)
		{
			
			IMAGE_BOX imgBox = imgJunction.m_vImageBuffer[t].m_imageBox;
			
			if (t==0)
			{
				FrameID = imgBox.m_ImageID;
			}
			
			if (imgBox.m_ImageID !=FrameID)
			{
				if(imgJunction.m_vImageBuffer[t].m_timestamp.QuadPart==0)
				{
					wostringstream woss;
					woss << L"camera image lost!";
					gLogger.QueueUpLog(woss.str(),true);
				}
				bSendTrue = false;				
			}
		}
		
		if (bSendTrue)
		{			
			//多相机图片数量计数位置移动，在判断id不一致 不发送的情况下，图片计数也不计数，
			//由于分组失败导致的计数不准确，报图片积压的问题，解决
			m_vPanels[nChIndex-1]->SendDIIPAndSyncFlag();
			for(size_t t = 0; t < szProcessor; t++)
			{
				CImageProcessor* pProcessor = vGroupProcessor[t];
				IMAGE_BOX imgBox = imgJunction.m_vImageBuffer[t].m_imageBox;				
				pProcessor->ReceiveImage(imgBox);				
			}
		}
	}
	else
	{		
		m_vPanels[nChIndex-1]->SendDIIPAndSyncFlag();
		if(szProcessor == 1)
		{			
			IMAGE_BOX imgBox;
			for(size_t t = 0; t < szCamera; t++)
			{
				IMAGE_BOX tmpImgBox = imgJunction.m_vImageBuffer[t].m_imageBox;
				if(t == 0)
				{					
					imgBox=tmpImgBox;
					continue;
				}
				concat_obj(imgBox.m_img,tmpImgBox.m_img,&imgBox.m_img);				
			}
			CImageProcessor* pProcessor = vGroupProcessor[0];			
			pProcessor->ReceiveImage(imgBox);
		}
	}

	imgJunction.clearBuffer();
	
}

void CSystemManager::HarvestGroupImage(int nChIndex)
{
	::EnterCriticalSection(&m_imgJunctionCs);

	[&]()
	{
		IMAGE_JUNCTION & imgJunction = m_dictChIndex_ImgJunction[nChIndex];

		if (imgJunction.m_nImgCount < imgJunction.m_vImageBuffer.size())
		{			
			return;
		}

		auto i = imgJunction.m_vImageBuffer.begin();
		auto * minSlot = &*i;
		LARGE_INTEGER maxStamp = minSlot->m_timestamp;

		for (; (++i) != imgJunction.m_vImageBuffer.end();)
		{
			if (i->m_timestamp.QuadPart < minSlot->m_timestamp.QuadPart)
				minSlot = &*i;
			else if (i->m_timestamp.QuadPart > maxStamp.QuadPart)
				maxStamp = i->m_timestamp;
			else
			{				
				continue;
			}

			
			if (maxStamp.QuadPart - minSlot->m_timestamp.QuadPart > m_nMaxImgSyncDelta)
			{
				LARGE_INTEGER nowStamp;
				QueryPerformanceCounter(&nowStamp);

				wostringstream woss;
				woss << L"Old image dropped: old:" << minSlot->m_timestamp.QuadPart
					<< L", by:" << maxStamp.QuadPart
					<< L", now:" << nowStamp.QuadPart
					<< L", delta:" << m_nMaxImgSyncDelta
					;
				gLogger.QueueUpLog(woss.str(),true);

				minSlot->m_timestamp.QuadPart = 0;
				//--imgJunction.m_nImgCount; //hzh remarked
				imgJunction.m_nImgCount=0;  //hzh add
				imgJunction.clearBuffer();  //hzh add
				return;
			}
		}		
		DisptchGroupImage(nChIndex, imgJunction);
	}();

	::LeaveCriticalSection(&m_imgJunctionCs);
}

unsigned int __stdcall CSystemManager::CameraGroupSupervisorThreadFunc()
{
	if(m_dictChIndex_ImgJunction.empty())
		return 0;

	while(m_nExitFlag == 0)
	{
		//Nothing to do.
		Sleep(2000);
	}
	return 0;
}

void CSystemManager::StartCameraGroupSupervisorThread()
{
	Proc.MemProc = &CSystemManager::CameraGroupSupervisorThreadFunc;
	UINT threadId;
	HANDLE hSupvisorThread = (HANDLE)_beginthreadex(NULL, 0, Proc.ThreadProc, (LPVOID)this, 0, &threadId);
	if(hSupvisorThread == NULL)
	{
		AfxMessageBox(_T("Starting camera group supervisor thread failed!"), MB_OK);
	}
	::SetThreadPriority(hSupvisorThread, THREAD_PRIORITY_HIGHEST);
	m_vHandle.push_back(hSupvisorThread);
}

void CSystemManager::OnCameraConnectionLostEvent(wstring wstrCameraName)
{
	auto ptrChConfigArray = gConfigMgr.GetChannelConfigPtr();
	for(auto it = ptrChConfigArray->begin(); it != ptrChConfigArray->end();it++)
	{
		CHANNEL_CONFIG chConfig = it->second;
		int i = 0;
		for (auto it = chConfig.m_vecCameraSetting.begin(); it != chConfig.m_vecCameraSetting.end();it++)
		{
			if (it->m_name == wstrCameraName)
			{
				m_vPanels[chConfig.m_chIndex-1]->m_chSD.ExecuteCmd(RED_LIGHT_ON);
				m_vPanels[chConfig.m_chIndex-1]->m_chSD.ExecuteCmd(GREEN_LIGHT_OFF);
				m_vPanels[chConfig.m_chIndex-1]->m_vChCameraPtr[i]->SetOpenFlag(false);
				break;
			}
			i++;
		}

	}
}

BOOL CSystemManager::StartAllCameraEx()
{
	BOOL bOpend=FALSE;
	std::for_each(m_vPanels.begin(),m_vPanels.end(),[&](CChannelControlPanel *panel)
	{
		if(panel->GetChannelState()!=CH_ON)
		{
			panel->TurnOnCamera();
			if(panel->GetChannelState() ==CH_ON)
				bOpend=TRUE;
		}
	});
	return bOpend;
}

BOOL CSystemManager::PauseAllCameraEx()
{
	BOOL bPaused=TRUE;
	std::for_each(m_vPanels.begin(),m_vPanels.end(),[&](CChannelControlPanel *panel)
	{
		if(panel->GetChannelState()==CH_ON)
			panel->PauseChCamera();
		if(panel->GetChannelState() ==CH_ON)
			bPaused=FALSE;
	});	
	return bPaused;
}

void CSystemManager::UpdateChannelSpeed()
{
	for(auto it = m_vPanels.begin(); it != m_vPanels.end(); it++)
	{
		CChannelControlPanel* pPanel = *it;
		pPanel->UpdateChannelSpeed();
	}
}

double CSystemManager::GetLightHeight1(double CapHeight)
{
	return m_TaskChange.GetLightHeight1(CapHeight);
}
double CSystemManager::GetLightHeight2(double CapHeight)
{
	return m_TaskChange.GetLightHeight2(CapHeight);
}

vector<CString> CSystemManager::GetExpppType()
{
	return m_TaskChange.GetExpppType();
}
double CSystemManager::GetCapDiameter()
{
	return m_TaskChange.GetCapDiameter();
}

CString CSystemManager::GetCHTaskName(int i)
{
	return m_vPanels[i-1]->GetCurTaskName();
}

TypeData CSystemManager::FindTypePara(double CapHeight)
{
	return m_TaskChange.FindTypePara(CapHeight);
}
bool CSystemManager::CheckAutoTaskName(double CapHeight,bool bRollNeck,int e_Exptype,int diameter,TypeData& DataPara,vector<CString>& vecTaskName,CString& ReErrorinfo)
{
	vecTaskName.clear();
	DataPara = m_TaskChange.FindTypePara(CapHeight);
	//获取任务列表


	//根据bRollNeck，e_Exptype，和区间 ，选出对应的任务
	//通道切换任务，同时给出光源高度字符串提示

	CString tasknameCH1Begin(_T("B30_43_S_CH1"));
	CString tasknameCH1End(_T("B30_43_S_CH1"));
	CString tasknameCH25Begin(_T("B30_43_CH2"));
	CString tasknameCH25End(_T("B30_43_CH2"));

	CString strRollNeck;
	strRollNeck = (bRollNeck?_T("A"):_T("B"));

	vector<CString> vecExpppType = GetExpppType();
	CString strExplosion;

	if(e_Exptype < vecExpppType.size())
	{
		strExplosion = vecExpppType[e_Exptype] ;
	}


	if (m_vPanels.size()!= 5)
	{
		return false;
	}

	CString Errorinfo;
	//Station1#--口部检测工位（1个通道）：
	tasknameCH1Begin.Format(_T("%s%d_%.1f_%s_CH1"),strRollNeck,diameter,DataPara.S1HTestBegin,strExplosion);
	tasknameCH1End.Format(_T("%s%d_%.1f_%s_CH1"),strRollNeck,diameter,DataPara.S1HTestEnd,strExplosion);
	CString tasknameCH1;
	bool retAll = true;

	bool ret = m_vPanels[0]->findAllTaskname(tasknameCH1Begin,tasknameCH1End,tasknameCH1,Errorinfo);
	if (ret)
	{
		vecTaskName.push_back(tasknameCH1);
		CString strtmp;
		CString str;
		str.LoadStringW(IDS_STRING_CH_RESULT1);
		strtmp.Format(str,1,Errorinfo);
		ReErrorinfo += strtmp;
	}
	else
	{
		retAll = false;
		if(tasknameCH1.IsEmpty())
		{
			CString strtmp;
			CString str;
			str.LoadStringW(IDS_STRING_CH_RESULT1);
			strtmp.Format(str,1,_T("NULL"));
			ReErrorinfo += strtmp;
		}
		else
		{
			CString strtmp;
			CString str;
			str.LoadStringW(IDS_STRING_CH_RESULT2);
			strtmp.Format(str,1,Errorinfo);
			ReErrorinfo += strtmp;
		}
		//return false;
	}


	//Station2#--外观检测工位（4个通道）
	for(size_t t = 1; t < 5;t++)
	{
		CString Errorinfo;
		CChannelControlPanel* pPanel = m_vPanels[t];
		tasknameCH25Begin.Format(_T("%s%d_%.1f_CH%d"),strRollNeck,diameter,DataPara.S2HTestBegin ,t+1);
		tasknameCH25End.Format(_T("%s%d_%.1f_CH%d"),strRollNeck,diameter,DataPara.S2HTestEnd ,t+1);
		CString tasknameCH25;
		bool ret = m_vPanels[t]->findAllTaskname(tasknameCH25Begin,tasknameCH25End,tasknameCH25,Errorinfo);
		if (ret)
		{
			vecTaskName.push_back(tasknameCH25);
			CString strtmp;
			CString str;
			str.LoadStringW(IDS_STRING_CH_RESULT1);
			strtmp.Format(str,t+1,Errorinfo);
			ReErrorinfo += strtmp;
		}
		else
		{
			retAll  = false;
			if(tasknameCH25.IsEmpty())
			{
				CString strtmp;
				CString str;
				str.LoadStringW(IDS_STRING_CH_RESULT1);
				strtmp.Format(str,t+1,_T("NULL"));
				ReErrorinfo += strtmp;
			}
			else
			{
				CString strtmp;
				CString str;
				str.LoadStringW(IDS_STRING_CH_RESULT2);
				strtmp.Format(str,t+1,Errorinfo);
				ReErrorinfo += strtmp;
			}
			//	return false;
		}
	}
	return retAll;
}


bool CSystemManager::PanelAutoChangeTask(vector<CString> vec)
{
	if (m_vPanels.size()!= 5 || vec.size() != 5)
	{
		return false;
	}

	for(size_t t = 0; t < m_vPanels.size();t++)
	{
		m_vPanels[t]->AutoChangeTask(vec[t],true);
	}
	return true;
}

bool CSystemManager::GetProductInfoVecter(vector<CString> &vec)
{
	//支持无数扩展
	vec.clear();
	for (auto it = m_vecProductInfo.begin(); it != m_vecProductInfo.end();it++)
	{
		vec.push_back((*it).c_str());
	}

	return true;
}

bool CSystemManager::ProductInfoToDataBase(std::vector<CString> m_vec)
{
	CString strSysForeignKey = m_strSysForeignKey;

	wstring strTableName(L"PRODUCT_INFO");
	string strName = utils::Unicode2Utf8(strTableName);

	if(!m_db.TableExists(strName.c_str()))
	{
		if(!CreateProductInfoTable(m_vec))
		{
			wstring wstrLog = L"Create database failed - ";
			wstrLog.append(strTableName);
			gLogger.QueueUpLog(wstrLog);
			return false;
		}
	}

	//判断外键是否存在
	string SelectCmd = "select * from ";
	SelectCmd+=strName.c_str();
	SelectCmd+=" where sysforeignkey = '";
	SelectCmd+=utils::Unicode2Utf8(strSysForeignKey.GetString());
	SelectCmd+="'";
	CppSQLite3Query ret = m_db.Query(SelectCmd.c_str());
	if (!ret.eof())
	{
		string DelCmd = "delete from ";
		DelCmd+=strName.c_str();
		DelCmd+=" where sysforeignkey ='";
		DelCmd+=utils::Unicode2Utf8(strSysForeignKey.GetString());
		DelCmd+="'";
		m_db.Excute(DelCmd.c_str());
	}

	//添加数据
	string InsertCmd = "insert into ";
	InsertCmd+=strName.c_str();
	InsertCmd+=" values(";
	InsertCmd+=utils::Unicode2Utf8(strSysForeignKey.GetString());

		for (int i = 0; i< m_vec.size();i++)
		{
			InsertCmd+=",'";
			InsertCmd+=utils::Unicode2Utf8(m_vec[i].GetString());
			InsertCmd+="'";
		}
		InsertCmd+=")";
		m_db.Excute(InsertCmd.c_str());
}
bool CSystemManager::GetCurSysForeignKeyInfo(CString sysForeignKey, std::vector<CString> &m_vec)
{
	wstring strTableName(L"PRODUCT_INFO");
	string strName = utils::Unicode2Utf8(strTableName);

	if(!m_db.TableExists(strName.c_str()))
	{
		return false;
	}
	//判断批次号是否存在
	string SelectCmd = "select * from ";
	SelectCmd+=strName.c_str();
	SelectCmd+=" where SysforeignKey = '";
	SelectCmd+=utils::Unicode2Utf8(sysForeignKey.GetString());
	SelectCmd+="'";
	CppSQLite3Query ret = m_db.Query(SelectCmd.c_str());
	int colCount = ret.numFields();
	while(!ret.eof())
	{
		string strColumnValue;
		for (int i = 0 ; i <colCount; i++)
		{
			string strColumnValue =ret.getStringField(i);
			wstring wstrColumnValue = utils::Utf8ToUnicode(strColumnValue);
			CString ColumnValue(wstrColumnValue.c_str());
			m_vec.push_back(ColumnValue);
		}
		ret.NextRow();
	}
	if (m_vec.empty())
	{
		return false;
	}
	return true;
}

CString CSystemManager::GetCurSysForeignKey()
{
	return m_strSysForeignKey;
}
void CSystemManager::SetCurSysForeignKey(CString strSysForeignKey)
{
	m_strSysForeignKey = strSysForeignKey;
}
void CSystemManager::UpdateSysForeignKey()
{
	gConfigMgr.SetSysForeignKey(m_strSysForeignKey);
}
bool CSystemManager::UpdateDatabaseData(BOOL bRecheck,CString sText)
{
	CString strCmd;

	HMODULE module = GetModuleHandle(0); 
	TCHAR pFileName[MAX_PATH]; 
	GetModuleFileName(module, pFileName, MAX_PATH); 

	CString csFullPath(pFileName); 
	int nPos = csFullPath.ReverseFind(_T('\\')); 
	if(nPos <=0)
		return false;
	CString path = csFullPath.Left(nPos); 

	CString strExePath(strCmd);

	strExePath.Append(path);
	strExePath.Append(_T("\\updatedatabase.exe"));
	wstring wstrExePath=strExePath.GetString();

	strCmd.Append(_T("\""));
	strCmd.Append(path);
	strCmd.Append(_T("\\updatedatabase.exe\" "));

	//根据上传exe路径是否存来确定数据库要不要上传！
	wstring FilePath =strCmd.GetString();
	if (INVALID_FILE_ATTRIBUTES ==::GetFileAttributes(wstrExePath.c_str()))
	{
		return false;
	}

	SYSTEMTIME end;
	::GetLocalTime(&end);
	end.wMilliseconds =0;
//	bgn.wSecond = bgn.wHour+1;

	CTime tmEnd(end.wYear, end.wMonth, end.wDay, end.wHour,end.wMinute, end.wSecond);
	tmEnd +=CTimeSpan(0,0,0,5);

	end.wYear = tmEnd.GetYear();
	end.wMonth = tmEnd.GetMonth();
	end.wDay = tmEnd.GetDay();
	end.wHour = tmEnd.GetHour();
	end.wMinute = tmEnd.GetMinute();
	end.wSecond = tmEnd.GetSecond();

//	long long lBgn = utils::DateTime2Int64(bgn); 
	long long lEnd = utils::DateTime2Int64(end); 
	CString strBgn;
	strBgn.Format(_T("%I64d"),m_timebegin);
	CString strEnd;
	strEnd.Format(_T("%I64d"),lEnd);

//	strCmd.Append(_T("@20181008000000"));
	strCmd.Append(_T("@"));
	strCmd.Append(strBgn);
//	strCmd.Append(_T("@20181008235959"));
	strCmd.Append(_T("@"));
	strCmd.Append(strEnd);

	strCmd.Append(_T("@"));
	if(bRecheck)
		strCmd.Append(_T("1"));
	else
		strCmd.Append(_T("0"));
	strCmd.Append(_T("@"));
	strCmd.Append(sText);

	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	ZeroMemory(&si,sizeof(si)); 
	si.cb            =   sizeof(STARTUPINFO); 
	si.dwXCountChars = 500;    
	si.dwYCountChars = 500;    
	si.dwFlags       = STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES|STARTF_FORCEONFEEDBACK;
	si.wShowWindow   = SW_NORMAL;  
	si.hStdInput     = stdin;
	si.hStdOutput    = stdout;    
	si.hStdError     = stderr;

	wstring wstrCmd = strCmd.GetString();
	BOOL fRet = CreateProcess(NULL,(LPWSTR)wstrCmd.c_str(),NULL,NULL,TRUE,NULL ,NULL,NULL,&si,&(pi));
	if(fRet)   
	{   
		CloseHandle(pi.hThread);
		HANDLE m_hModelToolProc = pi.hProcess;
		 
		CString str;
		str.Format(_T("%I64d"),lEnd);
		m_timebegin = lEnd;
		gConfigMgr.SetUploadTime(str);

	//	AfxMessageBox(strCmd);
	}   
	else   
	{   
		AfxMessageBox(_T("TODO:Add message here!"));  
	}

}