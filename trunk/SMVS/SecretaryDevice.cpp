#include "StdAfx.h"
#include "SecretaryDevice.h"
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include "Logger.h"
#include "WarningError.h"
#include "SystemManager.h"

extern CLogger gLogger;
extern SOCKET  gPcSocket;
extern CSystemManager gSystemMgr;

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

CSecretaryDevice::CSecretaryDevice()
	:m_bDeviceOk(false)
{
	m_deviceSocket = 0;
		::InitializeCriticalSection(&m_ExecuteCmdCs);
}

CSecretaryDevice::CSecretaryDevice(REJECTOR_SETTING& rs)
	:m_rs(rs),
	 m_bDeviceOk(false),
	 m_nQueryIndex(-1),
	 m_chindex(0),
	 m_RID(0),
	 m_bNoReply(0)
{
		m_deviceSocket = 0;
		::InitializeCriticalSection(&m_ExecuteCmdCs);
		m_QLifeID = rand()*rand();
}

CSecretaryDevice::~CSecretaryDevice(void)
{
	::DeleteCriticalSection(&m_ExecuteCmdCs);
	if(m_bDeviceOk)
	{
		closesocket(m_deviceSocket);
	}
	WSACleanup();

}

void CSecretaryDevice::CreateCommandDict()
{
	m_dictQOpCmd.clear();
	m_dictQOpCmd[QOP_VERSION_QUERY]		=  "01";
	m_dictQOpCmd[QOP_PARAM_SET]				=  "02";
	m_dictQOpCmd[QOP_DEVICE_MODE_SET]	=  "03";
	m_dictQOpCmd[QOP_DEVICE_RESET]			=  "04";
	m_dictQOpCmd[QOP_PRODUCT_FLAG]		=  "05";
	m_dictQOpCmd[QOP_SYS_STATE_QUERY]	=  "06";
	m_dictQOpCmd[QOP_CLIENT_TIME_SYN]		=  "07";
	m_dictQOpCmd[QOP_LIFE_ID_QUERY]			=  "08";
	m_dictQOpCmd[QOP_DATA_RESET]				=  "09";
	m_dictQOpCmd[QOP_PC_STATUS_SET]			=  "0a";
	m_dictQOpCmd[QOP_UNLOCK_SET]			=  "0b";

	//错误码注释
	m_dictRCodeInfo.clear();
	m_dictRCodeInfo[0x80001000]=_T("General Error!");							//General Error!
	m_dictRCodeInfo[0x80001001]=_T("请求消息长度不足!");					//请求消息长度不足
	m_dictRCodeInfo[0x80001002]=_T("未识别的QFlags不可忽略位!");	//未识别的QFlags不可忽略位!
	m_dictRCodeInfo[0x80001003]=_T("未识别的QOpcode!");					//未识别的QOpcode!
	m_dictRCodeInfo[0x80001004]=_T("QLifeID核对失败!");						//QLifeID核对失败!
	m_dictRCodeInfo[0x80001005]=_T("命令0x02未识别的参数PID!");		//命令0x02未识别的参数PID!
	m_dictRCodeInfo[0x80001006]=_T("命令0x02参数PValue超出范围!");//命令0x02参数PValue超出范围!
	m_dictRCodeInfo[0x80001007]=_T("命令0x02参数设置逻辑错误!");	//命令0x02参数设置逻辑错误!
	m_dictRCodeInfo[0x80001008]=_T("QData错误!");								//QData错误!

	//命令解析注释
	m_dictCmdInfo.clear();
	m_dictCmdInfo[DEVICE_ON]		= _T("进入正常模式  0x80 查询模式 0x92警戒模式");  //进入正常模式  0x80 查询模式 0x92警戒模式 ********************************
	m_dictCmdInfo[DEVICE_OFF]		= _T("进入静默模式");  //进入静默模式********************************

	m_dictCmdInfo[DELAY_T]			= _T("排盖距离 延时 0x88 0x98 0xA0 0xF0");    //排盖距离 延时 0x88 0x98 0xA0 0xF0			*******************************
	m_dictCmdInfo[DELAY_D]			= _T("排盖距离  延脉冲 0x98 对应参数两个字节");   //排盖距离  延脉冲 0x98 对应参数两个字节		*******************************
	m_dictCmdInfo[DURATION_T]	= _T("排盖吹气时间");    //排盖吹气时间												*******************************

	m_dictCmdInfo[CAMERA_DELAY_TIME]			= _T("相机延时");  //相机延时								*******************************
	m_dictCmdInfo[CAMERA_DELAY_DISTANCE]  = _T("相机延距离");  //相机延距离							*******************************

	m_dictCmdInfo[DROP_IMG_UCL]			  = _T("丢帧阈值");  //丢帧阈值									*******************************
	m_dictCmdInfo[CAMERA_FREE_MODE]       = _T("自由取图模式");  //自由取图								*******************************
	m_dictCmdInfo[CAMERA_TRIGGER_MODE]    = _T("触发取图模式");  //触发取图								*******************************

	m_dictCmdInfo[SA_SWITCH_EACH]        = _T("剔除不融合");				//排盖吹气模式						*******************************
	m_dictCmdInfo[SA_SWITCH_ONCE_FORALL] = _T("剔除融合");			//剔除融合，连续吹气					*******************************

	m_dictCmdInfo[R_SITOSOL_T] = _T("二次排盖感应器到电磁阀-时间");		//二次排盖感应器到电磁阀							*******************************
	m_dictCmdInfo[R_SITOSOL_D] = _T("二次排盖感应器到电磁阀-编码器");		//二次排盖感应器到电磁阀							*******************************
	m_dictCmdInfo[R_DELAY_T]    = _T("二次排盖距离-时间");    //二次排盖距离													*******************************
	m_dictCmdInfo[R_DELAY_D]    = _T("二次排盖距离-编码器");		//																	*******************************
	m_dictCmdInfo[R_DURATION_T] = _T("二次排盖吹气时间");	//二次排盖吹气时间										*******************************
	m_dictCmdInfo[R_DURATION_D] = _T("二次排盖吹气时间");


	m_dictCmdInfo[RED_LIGHT_ON]     = _T("");   // 红灯
	m_dictCmdInfo[RED_LIGHT_OFF]    = _T("");   
	m_dictCmdInfo[GREEN_LIGHT_ON]   = _T("");		//绿灯
	m_dictCmdInfo[GREEN_LIGHT_OFF]  = _T("");
	m_dictCmdInfo[YELLOW_LIGHT_ON]  =_T("");	 //黄灯
	m_dictCmdInfo[YELLOW_LIGHT_OFF] = _T("");
	m_dictCmdInfo[TEST_SOLENOID]    = _T("");	 //电磁阀一
	m_dictCmdInfo[TEST_LC]          = _T("");	 //光源控制器
	m_dictCmdInfo[TEST_SOLENOID2]   = _T("");	 //电磁阀二

	m_dictCmdInfo[RESET_STATS]      = _T("计数器清零");	 //计数器清零											*******************************

	m_dictCmdInfo[SOLENOID_ON] = _T("");   
	m_dictCmdInfo[LC_TEST_ON]  = _T("");
	m_dictCmdInfo[QUERY_CMD]   = _T("查询指令");       //查询指令												*******************************
	m_dictCmdInfo[CLIENT_TIEM_SYN] =_T("时间同步指令"); //时间同步指令			*******************************
	m_dictCmdInfo[VERSION]   = _T("版本号查询");		 //版本号查询		*******************************

	m_dictCmdInfo[CAMER_GRAB_TIME]   =_T("相机最大帧率设置");    //相机最大帧率设置				*******************************
	m_dictCmdInfo[SINGLE_INGORE_TIME]   = _T("屏蔽感应器时间");//屏蔽感应器时间				*******************************
	m_dictCmdInfo[ENCODER_ON] = _T("编码器开启");		//编码器开启								*******************************
	m_dictCmdInfo[ENCODER_OFF] = _T("编码器关闭");	 //编码器关闭							*******************************

	m_dictCmdInfo[R_REJECT_ON] = _T("二次排盖关闭");		//二次排盖关闭							*******************************
	m_dictCmdInfo[R_REJECT_OFF] = _T("二次排盖打开");		//二次排盖打开							*******************************

	m_dictCmdInfo[WRANING_RESET] = _T("");		//排盖板告警清除
	m_dictCmdInfo[RESULT_SIGNAL] = _T("");				//GOOD_SIGNAL
	m_dictCmdInfo[QUERY_CMD_TIME] = _T("");			//GOOD_SIGNAL

	m_dictCmdInfo[CAMERA_COUNT]=_T("相机数量设置");      //相机数量设置							*******************************

	m_dictCmdInfo[TRIGGER_INGORE_TIME] = _T("触发信号屏蔽时间"); // 触发信号屏蔽时间          *******************************
	m_dictCmdInfo[LIGHT_TRIGGER_WIDTH] = _T(" 光源触发信号宽度");// 光源触发信号宽度			*******************************

	m_dictCmdInfo[DEVICE_RESET] = _T("重置匹配机");  //_T("");
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	m_dictCmd.clear();
	m_dictCmd[DEVICE_ON]		= "00 00 91 00";  //正常模式  0x80 查询模式 0x92警戒模式 ********************************
	m_dictCmd[DEVICE_OFF]		= "00 00 90 00";  // 静默模式********************************

	m_dictCmd[DELAY_T]			= "00 a3 01 00 #";    //排盖距离 延时 0x88 0x98 0xA0 0xF0			*******************************
	m_dictCmd[DELAY_D]			= "00 a2 01 00 #";   //排盖距离  延脉冲 0x98 对应参数两个字节		*******************************
	m_dictCmd[DURATION_T]	= "00 a3 01 01 #";    //排盖吹气时间												*******************************

	m_dictCmd[CAMERA_DELAY_TIME]			= "00 a3 00 10 #";  //相机延时								*******************************
	m_dictCmd[CAMERA_DELAY_DISTANCE]  = "00 a2 00 10 #";  //相机延距离							*******************************

	m_dictCmd[DROP_IMG_UCL]			  = "00 88 02 05 #";  //丢帧阈值									*******************************
	m_dictCmd[CAMERA_FREE_MODE]       = "00 a3 02 08 #";  //自由取图								*******************************
	m_dictCmd[CAMERA_TRIGGER_MODE]    = "00 81 02 08";  //触发取图								*******************************

	m_dictCmd[SA_SWITCH_EACH]        = "00 88 02 03 00";				//排盖吹气模式						*******************************
	m_dictCmd[SA_SWITCH_ONCE_FORALL] = "00 88 02 03 01";			//剔除融合，连续吹气					*******************************

	m_dictCmd[R_SITOSOL_T] = "00 a3 01 02 #";		//二次排盖感应器到电磁阀							*******************************
	m_dictCmd[R_SITOSOL_D] = "00 a2 01 02 #";		//二次排盖感应器到电磁阀							*******************************
	m_dictCmd[R_DELAY_T]    = "00 a3 01 03 #";    //二次排盖距离													*******************************
	m_dictCmd[R_DELAY_D]    = "00 a2 01 03 #";		//																	*******************************
	m_dictCmd[R_DURATION_T] = "00 a3 01 04 #";	//二次排盖吹气时间										*******************************
	m_dictCmd[R_DURATION_D] = "00 a3 01 04 #";


	m_dictCmd[RED_LIGHT_ON]     = "11 01 55 00 00 00";   // 红灯
	m_dictCmd[RED_LIGHT_OFF]    = "11 01 aa 00 00 00";   
	m_dictCmd[GREEN_LIGHT_ON]   = "11 02 55 00 00 00";		//绿灯
	m_dictCmd[GREEN_LIGHT_OFF]  = "11 02 aa 00 00 00";
	m_dictCmd[YELLOW_LIGHT_ON]  = "11 03 55 00 00 00";	 //黄灯
	m_dictCmd[YELLOW_LIGHT_OFF] = "11 03 aa 00 00 00";
	m_dictCmd[TEST_SOLENOID]    = "11 04 01 00 00 00";	 //电磁阀一
	m_dictCmd[TEST_LC]          = "11 05 00 00 00 00";	 //光源控制器
	m_dictCmd[TEST_SOLENOID2]   = "11 04 02 00 00 00";	 //电磁阀二

	m_dictCmd[RESET_STATS]      = "00";	 //清统计数据											*******************************
	m_dictCmd[RESET_STATS_ERR]      = "01"; //清故障码

	m_dictCmd[SOLENOID_ON] = "04 55 00 00 00 00";    
	m_dictCmd[LC_TEST_ON]  = "05 55 00 00 00 00";
	m_dictCmd[QUERY_CMD]   = "00 00";        //查询指令												*******************************
	m_dictCmd[CLIENT_TIEM_SYN] ="00 00 00 00 00 00 00 a0 #";  //时间同步指令			*******************************
	m_dictCmd[VERSION]   = "56 49 54 43 00 04 00 00 00 00";			 //版本号查询		*******************************

	m_dictCmd[CAMER_GRAB_TIME]   = "00 98 02 06 #";    //相机最大帧率设置				*******************************
	m_dictCmd[SINGLE_INGORE_TIME]   = "00 98 02 07 #"; //屏蔽感应器时间				*******************************
	m_dictCmd[ENCODER_ON] = "00 88 02 01 01";			//编码器开启								*******************************
	m_dictCmd[ENCODER_OFF] = "00 88 02 01 00";		 //编码器关闭							*******************************

	m_dictCmd[R_REJECT_ON] = "00 88 02 02 01";		//二次排盖打开					*******************************
	m_dictCmd[R_REJECT_OFF] = "00 88 02 02 00";			//二次排盖关闭				*******************************

	m_dictCmd[WRANING_RESET] = "11 07 00 00 00 00";		//排盖板告警清除
	m_dictCmd[RESULT_SIGNAL] = "14 00 #";					//GOOD_SIGNAL
	m_dictCmd[QUERY_CMD_TIME] = "f1 55 #";				//GOOD_SIGNAL

	m_dictCmd[CAMERA_COUNT]="00 88 02 04 #";       //相机数量设置							*******************************
	
	m_dictCmd[TRIGGER_INGORE_TIME] = "00 98 02 09 #"; // 触发信号屏蔽时间          *******************************
	m_dictCmd[LIGHT_TRIGGER_WIDTH] = "00 a3 00 21 #"; // 光源触发信号宽度			*******************************

	m_dictCmd[DEVICE_RESET] = "00 00";
	m_dictCmd[LIFEID_SET]="00 00 #";

	m_dictCmd[PC_STATUS_SET] = "#";       //PC告警状态
	m_dictCmd[C_REJ_UCL]="00 98 02 0a #";
	//m_dictCmd[C_FRAME_UCL]="00 88 02 05 #";
	m_dictCmd[SELF_LEARN_FUNC]="00 88 02 0b #";
	m_dictCmd[HDW_TEST]="00 88 02 0c #";
	m_dictCmd[UNLOCK_INTERLOCK]="01 00";
	

// 	m_dictCmd[DEVICE_ON]  = "12 55 00 00 00 00";
// 	m_dictCmd[DEVICE_OFF] = "12 aa 00 00 00 00";
// 
// 	m_dictCmd[DELAY_T]    = "04 00 00 00 # #";    //排盖距离
// 	m_dictCmd[DELAY_D]    = "04 00 00 00 # #"; 
// 	m_dictCmd[DURATION_T] = "05 00 00 00 # #";    //排盖吹气时间
// 
// 	m_dictCmd[CAMERA_DELAY_TIME]	  = "06 01 00 00 # #";  //相机延时
// 	m_dictCmd[CAMERA_DELAY_DISTANCE]  = "06 10 00 00 # #";
// 	m_dictCmd[DROP_IMG_UCL]			  = "07 00 00 00 # #";  //丢帧阈值
// 	m_dictCmd[CAMERA_FREE_MODE]       = "13 66 00 00 # #";  //自由取图
// 	m_dictCmd[CAMERA_TRIGGER_MODE]    = "13 bb 00 00 00 00";  //触发取图
// 
// 	m_dictCmd[SA_SWITCH_EACH]        = "09 02 00 00 00 00";   //排盖吹气模式
// 	m_dictCmd[SA_SWITCH_ONCE_FORALL] = "09 20 00 00 00 00";
// 
// 	m_dictCmd[R_SITOSOL_T] = "0e 00 00 00 # #";		//二次排盖感应器到电磁阀
// 	m_dictCmd[R_SITOSOL_D] = "0e 00 00 00 # #";		//二次排盖感应器到电磁阀
// 	m_dictCmd[R_DELAY_T]    = "0d 00 00 00 # #";    //二次排盖距离
// 	m_dictCmd[R_DELAY_D]    = "0d 00 00 00 # #";
// 	m_dictCmd[R_DURATION_T] = "0f 00 00 00 # #";	//二次排盖吹气时间
// 	m_dictCmd[R_DURATION_D] = "0f 00 00 00 # #";
// 
// 	m_dictCmd[RED_LIGHT_ON]     = "11 01 55 00 00 00";   // 红灯
// 	m_dictCmd[RED_LIGHT_OFF]    = "11 01 aa 00 00 00";   
// 	m_dictCmd[GREEN_LIGHT_ON]   = "11 02 55 00 00 00";		//绿灯
// 	m_dictCmd[GREEN_LIGHT_OFF]  = "11 02 aa 00 00 00";
// 	m_dictCmd[YELLOW_LIGHT_ON]  = "11 03 55 00 00 00";	 //黄灯
// 	m_dictCmd[YELLOW_LIGHT_OFF] = "11 03 aa 00 00 00";
// 	m_dictCmd[TEST_SOLENOID]    = "11 04 01 00 00 00";	 //电磁阀一
// 	m_dictCmd[TEST_LC]          = "11 05 00 00 00 00";	 //光源控制器
// 	m_dictCmd[RESET_STATS]      = "11 06 00 00 00 00";	 //计数器清零
// 	m_dictCmd[TEST_SOLENOID2]   = "11 04 02 00 00 00";	 //电磁阀二
// 
// 
// 	m_dictCmd[SOLENOID_ON] = "04 55 00 00 00 00";    
// 	m_dictCmd[LC_TEST_ON]  = "05 55 00 00 00 00";
// 	m_dictCmd[QUERY_CMD]   = "f1 00 #";        //查询指令
// 
// 	m_dictCmd[VERSION]   = "f3 00 00 00 00 00";			 //版本号查询
// 
// 	m_dictCmd[CAMER_GRAB_TIME]   = "0a 00 00 00 # #";    //相机最大帧率设置
// 	m_dictCmd[SINGLE_INGORE_TIME]   = "0b 00 00 00 # #"; //屏蔽感应器时间
// 
// 
// 
// 	m_dictCmd[ENCODER_ON] = "01 10 00 00 00 00";		 //编码器开启
// 	m_dictCmd[ENCODER_OFF] = "01 01 00 00 00 00";		 //编码器关闭
// 
// 	m_dictCmd[R_REJECT_OFF] = "02 aa 00 00 00 00";		//二次排盖关闭
// 	m_dictCmd[R_REJECT_ON] = "02 55 00 00 00 00";		//二次排盖打开
// 
// 	m_dictCmd[WRANING_RESET] = "11 07 00 00 00 00";		//排盖板告警清除
// 	m_dictCmd[RESULT_SIGNAL] = "14 00 #";					//GOOD_SIGNAL
// 	m_dictCmd[QUERY_CMD_TIME] = "f1 55 #";				//GOOD_SIGNAL
// 
// 	m_dictCmd[TRIGGER_INGORE_TIME] = "08 00 00 00 # #"; // 触发信号屏蔽时间
// 	m_dictCmd[LIGHT_TRIGGER_WIDTH] = "0c 00 00 00 # #"; // 光源触发信号宽度

}

int HexChar2Integer(char c)
{
	if((c>='0')&&(c<='9'))
	{
		return c-0x30;
	}
	else if((c>='A')&&(c<='F'))
	{
		return c-'A'+10;
	}
	else if((c>='a')&&(c<='f'))
	{
		return c-'a'+10;
	}
	else 
		return 0x10;
}

string Str2Hex(string str)
{		
	std::vector<char> vData;
	int len = (int)str.length();

	for(int i = 0;i < len;)
	{
		char h = str[i];
		if(h == ' ')
		{
			i++;
			continue;
		}
		i++;
		if(i>=len)
			break;
		char l = str[i];
		int t  = HexChar2Integer(h);
		int t1 = HexChar2Integer(l);
		if((t==16)||(t1==16))
			break;
		else 
			t = t*16 + t1;
		i++;
		vData.push_back((char)t);
	}
	std::string strRet;
	strRet.assign(vData.begin(), vData.end());
	return strRet;
}

bool CSecretaryDevice::InitDevice()
{
	if (m_bDeviceOk)
	{
		return true;
	}
// 	m_bDeviceOk = true;
// 	CreateCommandDict();

	WSADATA wsaData;//
	//测试代码
	if(0 == WSAStartup(MAKEWORD(2,2), &wsaData))
	{
		if(!FindTargetIpAddress())
		{
			gLogger.QueueUpLog(L"Initialize SLAC Rejection Device Failed : IP Address is invalid...");
			return false;
		}
		//server
		m_deviceAddr.sin_family = AF_INET;
		m_deviceAddr.sin_port   = ::htons(12345);
		string strDeviceIp = utils::Unicode2Utf8(m_rs.m_wstrDeviceIp);
		m_deviceAddr.sin_addr.S_un.S_addr = inet_addr(strDeviceIp.c_str());

		if(m_deviceSocket == 0)
		{
			m_deviceSocket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			unsigned long iMode = 1;
			int nResult = ::ioctlsocket(m_deviceSocket, FIONBIO, &iMode);

			if(m_deviceSocket == INVALID_SOCKET)
			{
				gLogger.QueueUpLog(L"Initialize SLAC Rejection Device Failed : Create Client Socket Failed...");
				return false;
			}
			m_pcAddr.sin_family = AF_INET;
			int idevicePort = 11111+ m_chindex + 1;
			m_pcAddr.sin_port   = ::htons(idevicePort);
			m_pcAddr.sin_addr.S_un.S_addr = ::htonl(INADDR_ANY);
			::bind(m_deviceSocket, (sockaddr*)&m_pcAddr, sizeof(sockaddr));
		}
	}
	m_bDeviceOk = true;
 	CreateCommandDict();
	ExecuteCmd(QOP_LIFE_ID_QUERY,LIFEID_SET,m_QLifeID);
	ExecuteCmd(QOP_VERSION_QUERY,VERSION);
//	ExecuteCmd(QOP_DEVICE_RESET,DEVICE_RESET); 重置指令暂时不启用
	Sleep(10);	
	return true;
}

bool CSecretaryDevice::Start()
{
	SetCLientTimeSyn();
	return ExecuteCmd(QOP_DEVICE_MODE_SET,DEVICE_ON);
}

bool CSecretaryDevice::Stop()
{
	return ExecuteCmd(QOP_DEVICE_MODE_SET,DEVICE_OFF);
}

bool CSecretaryDevice::GetDeviceStatus()
{
	return m_bDeviceOk;
}

CString CSecretaryDevice::GetVersion()
{
	return m_strVision;
}
bool CSecretaryDevice::ExecuteCmd(int QOpCmd,string strCmd)
{
	if (!m_bDeviceOk)
	{
		return false;
	}
	::EnterCriticalSection(&m_ExecuteCmdCs);

	m_RID++;
	string strHex;

	std::ostringstream ssRID;   //QDlen 两位
	ssRID << std::hex << std::setfill('0');
	ssRID << std::setw(8);
	ssRID << m_RID;  //strCmd 格式"xx xx xx"
	strHex = ssRID.str();//Mcount
	strHex.insert(2," ");
	strHex.insert(5," ");
	strHex.insert(8," ");
	string strCmdQhead= strHex;
	
	bool bAqCmdBack = false;
	bool bAqCmdLifeID = false;

	string strLifeID;

	std::ostringstream ssDLifeID;   //QDlen 两位
	ssDLifeID << std::hex << std::setfill('0');
	ssDLifeID << std::setw(8);
	ssDLifeID << m_QLifeID;  //strCmd 格式"xx xx xx"
	strLifeID = ssDLifeID.str();//ssDLifeID
	strLifeID.insert(2," ");
	strLifeID.insert(5," ");
	strLifeID.insert(8," ");

	if (QOpCmd == QOP_VERSION_QUERY || 
		QOpCmd == QOP_PARAM_SET||
		QOpCmd == QOP_DEVICE_MODE_SET||
		QOpCmd == QOP_PC_STATUS_SET)
	{
		bAqCmdBack = true;
		bAqCmdLifeID = true;
		strCmdQhead+=" 18";
	}
	else if (QOpCmd == QOP_LIFE_ID_QUERY||
		QOpCmd == QOP_SYS_STATE_QUERY)
	{
		bAqCmdBack = true;
		strCmdQhead+=" 08";
	}
	else
	{
		strCmdQhead+=" 00"; // QFlags 标记位  3-1 恢复结果消息 4-1 QlifeID需要核对！

	}

	
	//用于判断是否断网！重置数据
	if (QOpCmd == QOP_SYS_STATE_QUERY)
	{
		m_dictQueryResult.clear();
	}

	strCmdQhead+=string(" ")+m_dictQOpCmd[QOpCmd]; // Opcode Ox05 标记工件

	std::ostringstream ssDlen;   //QDlen 两位
	ssDlen << std::hex << std::setfill('0');
	ssDlen << std::setw(4);
	ssDlen << strCmd.length()/3+1;  //strCmd 格式"xx xx xx"
	strHex = ssDlen.str();//Mcount
	strHex.insert(2," ");

	if (bAqCmdLifeID)
	{

		strCmd= strCmdQhead +" "+strHex +" "+strLifeID +" "+ strCmd;
	}
	else
	{
		strCmd= strCmdQhead +" "+strHex +" "+ strCmd;
	}

	string StrCmdTmp = strCmd;

// 	if (m_chindex==1&&QOpCmd == QOP_PRODUCT_FLAG)
// 	{
// 		CString str;
// 		str.Format(_T("通道%d,指令码:%s\n"),m_chindex,utils::Utf8ToUnicode(StrCmdTmp).c_str());
// 		gLogger.QueueUpLog(str.GetString(),true);
// //		Sleep(10);
// 	}


	std::vector<string> vItem = utils::SplitString(strCmd, " ");
	strCmd.clear();
	std::for_each(vItem.begin(), vItem.end(),[&](string str)
	{
		strCmd.append(str);
	});
	string cmdData = Str2Hex(strCmd);

	int nResult = sendto(m_deviceSocket, cmdData.c_str(),vItem.size(), 0, (SOCKADDR*)&m_deviceAddr, sizeof(SOCKADDR));
	if(nResult == SOCKET_ERROR)
	{
		wstring wstrCmd = utils::AnsiToUnicode(strCmd);
		wstring wstrErr = L"Send command [";
		wstrErr += wstrCmd;
		wstrErr += L"] error -- ERROR CODE:";
		int  nErrCode = WSAGetLastError();
		wstrErr += boost::lexical_cast<wstring>(nErrCode);
		gLogger.QueueUpLog(wstrErr,true);
		::LeaveCriticalSection(&m_ExecuteCmdCs);
		return false;
	}

	//无需指令回传的命令直接返回！
	if (!bAqCmdBack)
	{
		::LeaveCriticalSection(&m_ExecuteCmdCs);
		return true;
	}
	
	//等待5ms，开始接受回传数据，while循环，recvfrom 返回 SOCKET_ERROR时，接收结束

	Sleep(5);
	while(1)
	{
		char recvBuff[1024];
		sockaddr_in deviceAddr;
		int addrLen = sizeof(deviceAddr);
		int nRet = ::recvfrom(m_deviceSocket, recvBuff, 1024, 0, (SOCKADDR*)&deviceAddr, &addrLen);
		if(nRet == SOCKET_ERROR)
		{
// 			if (m_chindex==3)
// 			{
// 				int errorid = WSAGetLastError();
// 				CString strerrorid;
// 				strerrorid.Format(_T("socket_error : %d"),errorid);
// 				gLogger.QueueUpLog(strerrorid.GetString(),true);
// 			}
			break;
		}
		//解析回传信息
		int RID = utils::ParseValue(recvBuff[0],recvBuff[1],recvBuff[2],recvBuff[3]);
		int RFlags4 = utils::ParseValue(recvBuff[4]);
		int RFlags5 = utils::ParseValue(recvBuff[5]);
		int RDlen = utils::ParseValue(recvBuff[6],recvBuff[7]);
		int RCode = 0;
		int RdataIndexBegin = 8;  //回传信息起始点8 有RCode情况，此值延后4位

		if(RFlags5== 0x01 )
		{
			RdataIndexBegin=12;
			RCode = utils::ParseValue(recvBuff[8],recvBuff[9],recvBuff[10],recvBuff[11]);
		}

// 		错误码提示，用于测试, 用于验证错误！
      	if(RCode!=0)
      	{
// 		if (QOpCmd == QOP_SYS_STATE_QUERY)
// 		{
				CString str;
				CString cstrRev;
				CString strtmp;
				for (int i = 0 ; i < nRet ; i ++)
				{
					strtmp.Format(_T("%02x"),recvBuff[i]);
					strtmp = strtmp.Right(2);
					cstrRev=cstrRev+strtmp;
					if (i != nRet-1)
					{
						cstrRev+=_T(" ");
					}
				}
				string strRecv(recvBuff);
				str.Format(_T("指令码:%s\n错误码:%08X\n回传码:%s\n"),utils::Utf8ToUnicode(StrCmdTmp).c_str(),RCode,cstrRev);
		
				if (m_dictRCodeInfo.find(RCode)!= m_dictRCodeInfo.end())
				{
					str+=m_dictRCodeInfo[RCode];
					//AfxMessageBox(str);
					gLogger.QueueUpLog(str.GetString(),true);
				}

// 				if (m_chindex==1)
// 				{
// 					gLogger.QueueUpLog(str.GetString(),true);
// 				}			
		}

		//LifeID设置机制
		if(QOpCmd == QOP_LIFE_ID_QUERY)
		{
			int QLiftID = utils::ParseValue(recvBuff[RdataIndexBegin+2],recvBuff[RdataIndexBegin+3],recvBuff[RdataIndexBegin+4],recvBuff[RdataIndexBegin+5]);
			if (QLiftID != m_QLifeID)
			{
				m_QLifeID = QLiftID;
			}
		}

		//版本号解析
			if(QOpCmd == QOP_VERSION_QUERY)
			{
				if (recvBuff[RdataIndexBegin+25]==0) //10~25为TSName，软件实现名称
				{
					string strVersion(recvBuff+RdataIndexBegin+10);
					m_strVision.Format(_T("CH%d-RJect Version-%c%c%c%c%d.%d.%d%s-V%d.%d.%d")	,m_chindex,recvBuff[RdataIndexBegin]
					,recvBuff[RdataIndexBegin+1]
					,recvBuff[RdataIndexBegin+2]
					,recvBuff[RdataIndexBegin+3]
					,utils::ParseValue(recvBuff[RdataIndexBegin+4],recvBuff[RdataIndexBegin+5])
					,utils::ParseValue(recvBuff[RdataIndexBegin+6],recvBuff[RdataIndexBegin+7])
					,utils::ParseValue(recvBuff[RdataIndexBegin+8],recvBuff[RdataIndexBegin+9])
					,utils::Utf8ToUnicode(strVersion).c_str()
					,utils::ParseValue(recvBuff[RdataIndexBegin+26],recvBuff[RdataIndexBegin+27])
					,utils::ParseValue(recvBuff[RdataIndexBegin+28],recvBuff[RdataIndexBegin+29])
					,utils::ParseValue(recvBuff[RdataIndexBegin+30],recvBuff[RdataIndexBegin+31])
					);
				}
			}

			//查询指令解析
			if(QOpCmd == QOP_SYS_STATE_QUERY)
			{
				int SFlag0 = utils::ParseValue(recvBuff[RdataIndexBegin+0]);;  //保留标记位
				int SFlag1 = utils::ParseValue(recvBuff[RdataIndexBegin+1]);;  //保留标记位
				int SCount = utils::ParseValue(recvBuff[RdataIndexBegin+2],recvBuff[RdataIndexBegin+3]);		//返回的条目数
				int iRider = 4;  //解析游码计数
				for(int iSct = 0; iSct < SCount ;++ iSct)
				{
					int Slen = utils::ParseValue(recvBuff[RdataIndexBegin+iRider]);
					int SFlags= utils::ParseValue(recvBuff[RdataIndexBegin+iRider+1]);
					int SID = utils::ParseValue(recvBuff[RdataIndexBegin+iRider+2],recvBuff[RdataIndexBegin+iRider+3]);
					INT64 Svalue = 0;
					
					if (SID==0x0000)
					{
						//保留
					}
					else if(SID==0x0001 ||SID==0x0002 ||SID==0x0003 ||SID==0x0101 ||SID==0x0102) //4字节
					{
						Svalue = utils::ParseValue(recvBuff[RdataIndexBegin+iRider+4],recvBuff[RdataIndexBegin+iRider+5],recvBuff[RdataIndexBegin+iRider+6],recvBuff[RdataIndexBegin+iRider+7]);
					}
					else if(SID==0x0004 ||SID==0x0005 ||SID==0x0006 ||SID==0x0007 ||SID==0x0008 ||SID==0x0009 ||SID==0x000a 
						|| SID==0x000b || SID==0x000c || SID==0x000d
						|| SID==0x00a1 || SID==0x00a2 || SID==0x00a3 || SID==0x00a4 || SID==0x00a5 || SID==0x00a6 || SID==0x00a7)
					{
						Svalue = utils::ParseValue(recvBuff[RdataIndexBegin+iRider+4],recvBuff[RdataIndexBegin+iRider+5]);
					}
					else if(SID==0x00a8)
					{
						Svalue = utils::ParseValue(recvBuff[RdataIndexBegin+iRider+4],recvBuff[RdataIndexBegin+iRider+5],recvBuff[RdataIndexBegin+iRider+6],recvBuff[RdataIndexBegin+iRider+7]);
						Svalue <<=4;
						Svalue += utils::ParseValue(recvBuff[RdataIndexBegin+iRider+8],recvBuff[RdataIndexBegin+iRider+9]);
					}
					else
					{
						Svalue =-1;
					}

// 					//故障查询
// 					if (SID==0x0102)
// 					{
// 						if (Svalue & (1<<30))
// 						{
// 							CString str;
// 							CString cstrRev;
// 							CString strtmp;
// 							for (int i = 0 ; i < nRet ; i ++)
// 							{
// 								strtmp.Format(_T("%02x"),recvBuff[i]);
// 								strtmp = strtmp.Right(2);
// 								cstrRev=cstrRev+strtmp;
// 								if (i != nRet-1)
// 								{
// 									cstrRev+=_T(" ");
// 								}
// 							}
// 							gLogger.QueueUpLog(cstrRev.GetString());
// 						}
// 					}
					m_dictQueryResult[SID] = Svalue;
					iRider +=Slen;
				}
			}
			continue;
	} 

	::LeaveCriticalSection(&m_ExecuteCmdCs);
 	return true;
}
std::map<int,INT64 > CSecretaryDevice::GetQueryResult()
{
// 	if (!m_dictQueryResult.empty())
// 	{
	struct LockObj{
		LockObj(CRITICAL_SECTION &cs) : pCs(&cs){
			::EnterCriticalSection(pCs);
		}
		~LockObj(){
			::LeaveCriticalSection(pCs);
		}
		CRITICAL_SECTION *pCs;
	};
	LockObj l(m_ExecuteCmdCs);
		return m_dictQueryResult;
//	}
}
//测试
bool CSecretaryDevice::IsCmdIDSupport(int cmdID)
{
	if(cmdID>=RED_LIGHT_ON && cmdID <= YELLOW_LIGHT_OFF)
	{
		return false; // 3.0排盖版不控制指示灯
	}
	if(cmdID==TEST_SOLENOID ||
		cmdID==TEST_SOLENOID2||
		cmdID==TEST_LC)
	{
		return false; 
	}
	return true;
}

bool CSecretaryDevice::ExecuteCmd(int QOpCmdID,int cmdID, unsigned int nValue)
{
	if (cmdID ==-1)
	{
		return false; // 单命令指令不存在 直接返回
	}

	if(m_bDeviceOk == false)
	{
		return false;
	}

	if (!IsCmdIDSupport(cmdID))
	{
		return false;
	}

	//生成CmdID nValue 对应的字段
	if(m_dictCmd.find(cmdID) == m_dictCmd.end())
	{	
		if (QOpCmdID == QOP_DEVICE_MODE_SET)
		{
			::AfxMessageBox(_T("cmdID NUll"));
		}
		return false;

	}
  	string strCmd = m_dictCmd[cmdID];
// 	CString strCmdinfo = m_dictCmdInfo[cmdID];
// 	CString strCmdlog;
// 	strCmdlog.Format(_T("%s  : 参数%d \n"),strCmdinfo,nValue);

 	if (cmdID ==RESET_STATS )
	{
		gLogger.QueueUpLog(L"Reset rejector data",true);		
	}
	string strPType="";
	//判断排盖版参数设置类型PType 
	if (QOpCmdID == QOP_PARAM_SET)
	{
		string strCmdtmp = strCmd;
		string strPType = (strCmdtmp.substr(3,2));
		if(strCmd.find("#") != string::npos)
		{
			std::ostringstream ss;
			if (strPType =="88") //0x88 通用一字节数据
			{
				ss << std::hex << std::setfill('0');
				ss << std::setw(2);
				ss << nValue;
				string strHex = ss.str();
				size_t pos = strCmd.find("#");
				strCmd.replace(pos,string::npos,strHex);
			}
			else if (strPType =="98") //通用二字节数据
			{
				ss << std::hex << std::setfill('0');
				ss << std::setw(4);
				ss << nValue;
				string strHex = ss.str();
				strHex.insert(2," ");
				size_t pos = strCmd.find("#");
				strCmd.replace(pos,string::npos,strHex);
			}
			else
			{
				ss << std::hex << std::setfill('0');
				ss << std::setw(8);
				ss << nValue;
				string strHex = ss.str();
				strHex.insert(2," ");
				strHex.insert(5," ");
				strHex.insert(8," ");
				size_t pos = strCmd.find("#");
				strCmd.replace(pos,string::npos,strHex);
			}
		}
	}
	//时钟同步指令参数生成
	if (QOpCmdID == QOP_CLIENT_TIME_SYN||
		QOpCmdID == QOP_LIFE_ID_QUERY||
		QOpCmdID == QOP_PC_STATUS_SET)
	{
		std::ostringstream ss;
		ss << std::hex << std::setfill('0');
		ss << std::setw(8);
		ss << nValue;
		string strHex = ss.str();
		strHex.insert(2," ");
		strHex.insert(5," ");
		strHex.insert(8," ");
		size_t pos = strCmd.find("#");
		strCmd.replace(pos,string::npos,strHex);
	}

	return ExecuteCmd(QOpCmdID,strCmd);
}

CHANNEL_STATUS CSecretaryDevice::ParseQueryData(std::vector<char>& vData)
{
	CHANNEL_STATUS ret;
	ret.m_nReject2Count   = utils::ParseValue(vData[1]);
	ret.m_nSpeed       = utils::ParseValue(vData[2], vData[3]);
	ret.m_nSensorCount = utils::ParseValue(vData[4], vData[5], vData[6], vData[7]);
	ret.m_nDroppedImg  = utils::ParseValue(vData[8], vData[9]);
	ret.m_nRejectCount = utils::ParseValue(vData[10], vData[11]);
//	ret.m_nReserved1   = utils::ParseValue(vData[12], vData[13]);
	ret.m_nTotalOkCount   = utils::ParseValue(vData[14],vData[15],vData[16]);
	ret.m_nCSC         = utils::ParseValue(vData[17]);
	return ret;
}

void CSecretaryDevice::SetChindex(int index)
{
	m_chindex = index;
}
void CSecretaryDevice::SetCLientTimeSyn()
{
	//时间同步
	chronograph m_time;
	long long nValue = m_time.DoubleNowEx();
	ExecuteCmd(QOP_CLIENT_TIME_SYN,CLIENT_TIEM_SYN,(unsigned int)nValue);
}
bool CSecretaryDevice::ExecuteQueryCmd(CHANNEL_STATUS& retData,std::vector<ERROR_INFO>& vRetError,bool& ErrorMode)
{
	//统计查询
	ExecuteCmd(QOP_SYS_STATE_QUERY,QUERY_CMD);

	if (m_dictQueryResult.size()==0)	//
	{
		m_bNoReply++;
		if (m_bNoReply >3)
		{
			CString strNoreplay;
			strNoreplay.LoadString(IDS_STRING_NEWRJECT_NOMSG_REPLY);
			ERROR_INFO emsg(INIT_REJECTOR_SYS_FAILED,ERRORSUBID_CAMERA_RJECT_UNKNOW_ERROR,strNoreplay.GetString());
			vRetError.push_back(emsg);
			m_bNoReply = 0;
		}
		return true;
	}
	m_bNoReply = 0;
	CString strQueryInfo;
	if (m_dictQueryResult.find(0x0001)!=m_dictQueryResult.end())  //生产总数统计
	{
		retData.m_nSensorCount = m_dictQueryResult[0x0001];
		CString strinfotmp;
		CString str;
		str.LoadString(IDS_STRING_NEWRJECT_TOTAL_COUNT);
		strinfotmp.Format(str,retData.m_nSensorCount);
		strQueryInfo+=strinfotmp;
	}
	if (m_dictQueryResult.find(0x0002)!=m_dictQueryResult.end())  //合格总数统计
	{
		retData.m_nTotalOkCount = m_dictQueryResult[0x0002];
		CString strinfotmp;
		CString str;
		str.LoadString(IDS_STRING_NEWRJECT_OK_COUNT);
		strinfotmp.Format(str,m_dictQueryResult[0x0002]);
		strQueryInfo+=strinfotmp;
	}
	if (m_dictQueryResult.find(0x0003)!=m_dictQueryResult.end())  //剔除1总数统计
	{
		retData.m_nRejectCount = m_dictQueryResult[0x0003];
		CString strinfotmp;
		CString str;
		str.LoadString(IDS_STRING_NEWRJECT_NG1_COUNT);
		strinfotmp.Format(str,retData.m_nSensorCount);
		strQueryInfo+=strinfotmp;
	}
	if (m_dictQueryResult.find(0x0004)!=m_dictQueryResult.end())  //丢帧统计
	{
		retData.m_nDroppedImg = m_dictQueryResult[0x0004];
		CString strinfotmp;
		CString str;
		str.LoadString(IDS_STRING_NEWRJECT_LOSTFRAME_COUNT);
		strinfotmp.Format(str,m_dictQueryResult[0x0004]);
		strQueryInfo+=strinfotmp;
	}
	if (m_dictQueryResult.find(0x0005)!=m_dictQueryResult.end())  //剔除2总数统计
	{
		retData.m_nReject2Count = m_dictQueryResult[0x0005];
		CString strinfotmp;
		CString str;
		str.LoadString(IDS_STRING_NEWRJECT_NG2_COUNT);
		strinfotmp.Format(str,m_dictQueryResult[0x0005]);
		strQueryInfo+=strinfotmp;
	}

	if (m_dictQueryResult.find(0x0006)!=m_dictQueryResult.end())  //盖子速度
	{
		retData.m_nEoeSpeed = m_dictQueryResult[0x0006];
		CString strinfotmp;
		CString str;
		str.LoadString(IDS_STRING_NEWRJECT_SPEED);
		strinfotmp.Format(str,m_dictQueryResult[0x0006]);
		strQueryInfo+=strinfotmp;
	}

	if (m_dictQueryResult.find(0x0007)!=m_dictQueryResult.end())  //相机2丢帧
	{
		retData.m_nDroppedImg2 = m_dictQueryResult[0x0007];
		CString strinfotmp;
		CString str;
		str.LoadString(IDS_STRING_NEWRJECT_LOSTFRAME2);
		strinfotmp.Format(str,m_dictQueryResult[0x0007]);
		strQueryInfo+=strinfotmp;
	}
	if (m_dictQueryResult.find(0x0008)!=m_dictQueryResult.end())  //相机3丢帧
	{
		retData.m_nDroppedImg3 = m_dictQueryResult[0x0008];
		CString strinfotmp;
		CString str;
		str.LoadString(IDS_STRING_NEWRJECT_LOSTFRAME3);
		strinfotmp.Format(str,m_dictQueryResult[0x0008]);
		strQueryInfo+=strinfotmp;
	}
	if (m_dictQueryResult.find(0x0009)!=m_dictQueryResult.end())  //相机4丢帧
	{
		retData.m_nDroppedImg4 = m_dictQueryResult[0x0009];
		CString strinfotmp;
		CString str;
		str.LoadString(IDS_STRING_NEWRJECT_LOSTFRAME4);
		strinfotmp.Format(str,m_dictQueryResult[0x0009]);
		strQueryInfo+=strinfotmp;
	}
	if (m_dictQueryResult.find(0x000B)!=m_dictQueryResult.end())  //定位失败统计
	{
		retData.m_nPositionError = m_dictQueryResult[0x000B];
		CString strinfotmp;
		CString str;
		str.LoadString(IDS_STRING_NEWRJECT_LOSTFRAME4);
		strinfotmp.Format(str,m_dictQueryResult[0x000B]);
		strQueryInfo+=strinfotmp;
	}
	if (m_dictQueryResult.find(0x000C)!=m_dictQueryResult.end())  //当前脉冲值
	{
		retData.m_nPulse = m_dictQueryResult[0x000C];
		CString strinfotmp;
		CString str;
		str.LoadString(IDS_STRING_NEWRJECT_LOSTFRAME4);
		strinfotmp.Format(str,m_dictQueryResult[0x000C]);
		strQueryInfo+=strinfotmp;
	}
	if (m_dictQueryResult.find(0x000D)!=m_dictQueryResult.end())  //管路速度
	{
		retData.m_nSpeed = m_dictQueryResult[0x000D];
		CString strinfotmp;
		CString str;
		str.LoadString(IDS_STRING_NEWRJECT_LOSTFRAME4);
		strinfotmp.Format(str,m_dictQueryResult[0x000D]);
		strQueryInfo+=strinfotmp;
	}
	if (m_dictQueryResult.find(0x00A1)!=m_dictQueryResult.end())  //PC时间定位错误统计
	{
		retData.m_nPCTimeError = m_dictQueryResult[0x00A1];
		CString strinfotmp;
		CString str;
		str.LoadString(IDS_STRING_NEWRJECT_LOSTFRAME4);
		strinfotmp.Format(str,m_dictQueryResult[0x00A1]);
		strQueryInfo+=strinfotmp;
	}
	if (m_dictQueryResult.find(0x00A2)!=m_dictQueryResult.end())  //序号定位错误统计
	{
		retData.m_nSeqError = m_dictQueryResult[0x00A2];
		CString strinfotmp;
		CString str;
		str.LoadString(IDS_STRING_NEWRJECT_LOSTFRAME4);
		strinfotmp.Format(str,m_dictQueryResult[0x00A2]);
		strQueryInfo+=strinfotmp;
	}
	if (m_dictQueryResult.find(0x00A3)!=m_dictQueryResult.end())  //重复定位或者收到重复的数据包统计
	{
		retData.m_nRepeatCount = m_dictQueryResult[0x00A3];
		CString strinfotmp;
		CString str;
		str.LoadString(IDS_STRING_NEWRJECT_LOSTFRAME4);
		strinfotmp.Format(str,m_dictQueryResult[0x00A3]);
		strQueryInfo+=strinfotmp;
	}
	if (m_dictQueryResult.find(0x00A4)!=m_dictQueryResult.end())  //相机时间与序号定位错误统计
	{
		retData.m_nCameraAndSeqError = m_dictQueryResult[0x00A4];
		CString strinfotmp;
		CString str;
		str.LoadString(IDS_STRING_NEWRJECT_LOSTFRAME4);
		strinfotmp.Format(str,m_dictQueryResult[0x00A4]);
		strQueryInfo+=strinfotmp;
	}
	if (m_dictQueryResult.find(0x00A5)!=m_dictQueryResult.end())  //相机时间偏差大统计
	{
		retData.m_nCameraTimeOffseError = m_dictQueryResult[0x00A5];
		CString strinfotmp;
		CString str;
		str.LoadString(IDS_STRING_NEWRJECT_LOSTFRAME4);
		strinfotmp.Format(str,m_dictQueryResult[0x00A5]);
		strQueryInfo+=strinfotmp;
	}
	if (m_dictQueryResult.find(0x00A6)!=m_dictQueryResult.end())  //通过相机计数与相机时间定位成功的统计
	{
		retData.m_nCameraPositionSucc = m_dictQueryResult[0x00A6];
		CString strinfotmp;
		CString str;
		str.LoadString(IDS_STRING_NEWRJECT_LOSTFRAME4);
		strinfotmp.Format(str,m_dictQueryResult[0x00A6]);
		strQueryInfo+=strinfotmp;
	}
	if (m_dictQueryResult.find(0x00A7)!=m_dictQueryResult.end())  //定位失败解析标志位
	{
		retData.m_nPositionFailFlag = m_dictQueryResult[0x00A7];
		CString strinfotmp;
		CString str;
		//str.LoadString(L"Locate failed flag 0x%x");
		strinfotmp.Format(L"Locate failed flag 0x%x",m_dictQueryResult[0x00A7]);
		strQueryInfo+=strinfotmp;		
	}
	if (m_dictQueryResult.find(0x00A8)!=m_dictQueryResult.end())  //定位失败计数汇总
	{
		retData.m_nPositionFailCount = m_dictQueryResult[0x00A8];
		CString strinfotmp;
		CString str;
		//str.LoadString(L"Locate failed flag 0x%x");
		strinfotmp.Format(L"Locate failed count 0x%x",m_dictQueryResult[0x00A8]);
		strQueryInfo+=strinfotmp;		
	}

	if (m_dictQueryResult.find(0x0101)!=m_dictQueryResult.end())  //QLifeID 校验失败!
	{
		retData.m_nLifeID = m_dictQueryResult[0x0101];
		CString strinfotmp;
		strinfotmp.Format(_T("QLifeID:%d\n"),m_dictQueryResult[0x0101]);
		if (m_QLifeID != m_dictQueryResult[0x0101])
		{
			CString strNoreplay;//(_T("QLifeID 校验失败!"));
			strNoreplay.LoadString(IDS_STRING_NEWRJECT_QLIFEID_FAILED);	
			ERROR_INFO emsg(INIT_REJECTOR_SYS_FAILED,ERRORSUBID_CAMERA_RJECT_UNKNOW_ERROR,strNoreplay.GetString());
			vRetError.push_back(emsg);
		}
		strQueryInfo+=strinfotmp;
	}

	if (m_dictQueryResult.find(0x000a)!=m_dictQueryResult.end())  //过触发屏蔽总数
	{
		retData.m_nTriggerIgnoreCount = m_dictQueryResult[0x000a];
		CString strinfotmp;
		CString str;
		str.LoadString(IDS_STRING_NEWRJECT_TRIGGER_IGNORE_COUNT);
		strinfotmp.Format(str,m_dictQueryResult[0x000a]);
		strQueryInfo+=strinfotmp;
	}

	if (m_dictQueryResult.find(0x0102)!=m_dictQueryResult.end())  //运行状态与故障信息
	{
		retData.m_nCSC = m_dictQueryResult[0x0102];
		CString strinfotmp;
		CString str;
		str.LoadString(IDS_STRING_NEWRJECT_ERROR_FLAG);
		strinfotmp.Format(str,m_dictQueryResult[0x0102]);
		strQueryInfo+=strinfotmp;

		//新排盖板升级互锁告警以后，前三位用于，互锁告警操作位状态，不是告警标记位！
		if (retData.m_nCSC & (0x0fffffff))
		{
			CString strError;
			if (retData.m_nCSC & (1<<0))
			{
				CString str;
				str.LoadString(IDS_STRING_NEWRJECT_ERROR_0);
				strError+=str; //发生剔除错误
			}
			if (retData.m_nCSC & (1<<1))
			{
				CString str;
				str.LoadString(IDS_STRING_NEWRJECT_ERROR_1);
				if(strError.GetLength()>0) strError+="\n";
				strError+=str; //相机1缺帧错误
			}
			if (retData.m_nCSC & (1<<2))
			{
				CString str;
				str.LoadString(IDS_STRING_NEWRJECT_ERROR_2);
				if(strError.GetLength()>0) strError+="\n";
				strError+=str;//参数逻辑错误
			}
			if (retData.m_nCSC & (1<<3))
			{
				CString str;
				str.LoadString(IDS_STRING_NEWRJECT_ERROR_3);
				if(strError.GetLength()>0) strError+="\n";
				strError+=str;//编码器错误
			}
			if (retData.m_nCSC & (1<<4))
			{
				CString str;
				str.LoadString(IDS_STRING_NEWRJECT_ERROR_4);
				if(strError.GetLength()>0) strError+="\n";
				strError+=str;//相机计数错误
			}
			if (retData.m_nCSC & (1<<5))
			{
				CString str;
				str.LoadString(IDS_STRING_NEWRJECT_ERROR_5);
				if(strError.GetLength()>0) strError+="\n";
				strError+=str;
			}
			if (retData.m_nCSC & (1<<6))
			{
				CString str;
				str.LoadString(IDS_STRING_NEWRJECT_ERROR_6);
				if(strError.GetLength()>0) strError+="\n";
				strError+=str;
			}
			if (retData.m_nCSC & (1<<7))
			{
				CString str;
				str.LoadString(IDS_STRING_NEWRJECT_ERROR_7);
				if(strError.GetLength()>0) strError+="\n";
				strError+=str;
			}
			if (retData.m_nCSC & (1<<8))
			{
				CString str;
				str.LoadString(IDS_STRING_NEWRJECT_ERROR_8);
				if(strError.GetLength()>0) strError+="\n";
				strError+=str;
			}
			if (retData.m_nCSC & (1<<9))
			{
				CString str;
				str.LoadString(IDS_STRING_NEWRJECT_ERROR_9);
				if(strError.GetLength()>0) strError+="\n";
				strError+=str;
			}
			if (retData.m_nCSC & (1<<10))
			{
				CString str;
				str.LoadString(IDS_STRING_NEWRJECT_ERROR_10);
				if(strError.GetLength()>0) strError+="\n";
				strError+=str;
			}
			if (retData.m_nCSC & (1<<11))
			{
				CString str;
				str.LoadString(IDS_STRING_NEWRJECT_ERROR_11);
				if(strError.GetLength()>0) strError+="\n";
				strError+=str;
			}
			if (retData.m_nCSC & (1<<12))
			{
				CString str;
				str.LoadString(IDS_STRING_NEWRJECT_ERROR_12);
				if(strError.GetLength()>0) strError+="\n";
				strError+=str;
			}
			if (retData.m_nCSC & (1<<30))
			{
				CString str;
				str.LoadString(IDS_STRING_NEWRJECT_ERROR_30);
				if(strError.GetLength()>0) strError+="\n";
				strError+=str;
				retData.m_bInterLock=TRUE;
			}
			else
				retData.m_bInterLock=FALSE;
			if (retData.m_nCSC & (1<<31))
			{
				if(strError.GetLength()>0) strError+="\n";
				CString str;
				str.LoadString(IDS_STRING_NEWRJECT_ERROR_31);
				strError+=str;//当前为警戒模式
				ErrorMode = true;
			}
			if (retData.m_nCSC & (0x0ffffc00))
			{
				CString str;
				//str.LoadString(IDS_STRING_NEWRJECT_ERROR_ELSE);
				//strError+=str; //未知错误				
			}
#ifdef _DEBUG
			if(retData.m_nPositionFailFlag & (1<<0))
			{
				CString str;
				str.LoadString(IDS_STRING_NEWRJECT_ERROR_A7_0);
				if(strError.GetLength()>0) strError+="\n";
				strError+=str;
			}
			if(retData.m_nPositionFailFlag & (1<<1))
			{
				CString str;
				str.LoadString(IDS_STRING_NEWRJECT_ERROR_A7_1);
				if(strError.GetLength()>0) strError+="\n";
				strError+=str;
			}
			if(retData.m_nPositionFailFlag & (1<<2))
			{
				CString str;
				str.LoadString(IDS_STRING_NEWRJECT_ERROR_A7_2);
				if(strError.GetLength()>0) strError+="\n";
				strError+=str;
			}
			if(retData.m_nPositionFailFlag & (1<<3))
			{
				CString str;
				str.LoadString(IDS_STRING_NEWRJECT_ERROR_A7_3);
				if(strError.GetLength()>0) strError+="\n";
				strError+=str;
			}
			if(retData.m_nPositionFailFlag & (1<<4))
			{
				CString str;
				str.LoadString(IDS_STRING_NEWRJECT_ERROR_A7_4);
				if(strError.GetLength()>0) strError+="\n";
				strError+=str;
			}
			if(retData.m_nPositionFailFlag & (1<<5))
			{
				CString str;
				str.LoadString(IDS_STRING_NEWRJECT_ERROR_A7_5);
				if(strError.GetLength()>0) strError+="\n";
				strError+=str;
			}
#endif
			ERROR_INFO emsg(INIT_REJECTOR_SYS_FAILED,ERRORSUBID_CAMERA_RJECT_UNKNOW_ERROR,strError.GetString());
			//vRetError.push_back(emsg);
			gLogger.QueueUpLog(strError.GetString(),true);
		}
	}
	//		gLogger.QueueUpLog(strQueryInfo.GetString(),true);

	return true;
}

bool CSecretaryDevice::FindTargetIpAddress()
{
    PIP_ADAPTER_INFO pIpAdapterInfo = nullptr;             // PIP_ADAPTER_INFO结构体指针存储本机网卡信息
    unsigned long stSize = sizeof(IP_ADAPTER_INFO);        //得到结构体大小,用于GetAdaptersInfo参数
    int nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize);

	std::vector<BYTE> vBuff(stSize,0x00);

    if(ERROR_BUFFER_OVERFLOW == nRel) 
    {
        pIpAdapterInfo = (PIP_ADAPTER_INFO)&vBuff[0];   // 重新申请内存空间用来存储所有网卡信息
        nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize); //再次调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量  
    }
	 bool bFind = false;

     if (ERROR_SUCCESS == nRel)
     {
         while (pIpAdapterInfo)
         {
             IP_ADDR_STRING *pIpAddrString = &(pIpAdapterInfo->IpAddressList);
             do 
             {
                 string strIP = pIpAddrString->IpAddress.String;
				 if(strIP == "192.168.200.16")
				 {
					 bFind = true;
					 break;
				 }
                 pIpAddrString = pIpAddrString->Next;

             } while (pIpAddrString);
			 pIpAdapterInfo = pIpAdapterInfo->Next;
         }
    }
	return bFind;
}

