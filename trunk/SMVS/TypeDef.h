#pragma once

#include <string>
#include <map>
#include <queue>
#include <set>
#include <vector>
#include <iostream>
#include <iomanip>
#include <tchar.h>
#include <cpp\HalconCpp.h>
#include <atlcomcli.h>
#include <atlconv.h>
#include <atltypes.h>
#include <boost\lexical_cast.hpp>
#include <boost\atomic\atomic.hpp>
#include <boost\thread\mutex.hpp>
#include <boost\thread\lock_guard.hpp>
#include <boost\thread\condition.hpp>
#include <boost\call_traits.hpp>
#include <boost\bind.hpp>

using namespace std;
using namespace Halcon;
using namespace boost;

#define SAVE_NG_IMAGE	1
#define SAVE_OK_IMAGE	2

#pragma warning(disable : 4267)

#define WM_UPDATE_NG_IMG_WND						WM_USER + 1
#define WM_RECEIVE_SPC_DATA								WM_USER + 2
#define MSG_MODEL_IMG_READY							WM_USER + 3
#define MSG_UPDATE_WARNING_PANEL					WM_USER + 4
#define MSG_DELETE_OBSOLETE_WARNING				WM_USER + 5
#define MSG_CH_STATION_STATE_CHANGED			WM_USER + 6
#define MSG_CH_STATION_STOPPED						WM_USER + 7
#define MSG_BAR_GRAPH_CHANGED						WM_USER + 8 
#define MSG_CH_ON_OFF										WM_USER + 9
#define WM_SWITCH_TASK										WM_USER + 10
#define WM_RESET_CH_STATS									WM_USER + 11
#define WM_UPDATE_TV_SCREEN							WM_USER + 12
#define MSG_BAR_GRAPH_RANGE_CHANGED			WM_USER + 13 
#define IDB_WARNING_SHOW									WM_USER + 14
#define WM_UPDATE_ALGO_LIST								WM_USER + 15
#define WM_RGN_FILE_UPDATED								WM_USER + 22

#define DBL_MAX 1.7976931348623158e+308

const int  IO_ON  = 1;
const int  IO_OFF = 0;

// Product ID
const int CROWN_CAP   = 0x01;
const int EOE         = 0x02;
const int CAN         = 0x03;
const int PET         = 0x04;
const int PCC         = 0x05;
const int AL_PLASTIC  = 0x06;
const int CAN_EOE     = 0x07;
const int JELLY       = 0x08;
const int CAN_OUTSIDE = 0x09;
const int NAIL        = 0x0A;
const int PLUG        = 0x0B;
const int HDPE        = 0x0C;
const int NEEDLE      = 0x0D;
const int MAT		  = 0x0E;
const int BATTERY     = 0x0F;
const int CAP     = 0x10;
const int BOTTLE     = 0x11;

// Language ID
const int LANG_CHS = 0x01;
const int LANG_ENU = 0x02;
const int LANG_RUS = 0x03;

// Light Controller Channel Mode
const int LC_ON      = 0;
const int LC_TRIGGER = 1;
const int LC_TRIGGER_TWICE = 2;

// For SLAC new rejector device QOpCmd
const int QOP_VERSION_QUERY		= 0;
const int QOP_PARAM_SET				= 1;
const int QOP_DEVICE_MODE_SET	= 2;
const int QOP_DEVICE_RESET			= 3;
const int QOP_PRODUCT_FLAG			= 4;
const int QOP_SYS_STATE_QUERY		= 5;
const int QOP_CLIENT_TIME_SYN		= 6;
const int QOP_LIFE_ID_QUERY			= 7;
const int QOP_DATA_RESET				= 8;
const int QOP_PC_STATUS_SET			= 9;
const int QOP_UNLOCK_SET			= 0x0A;  //发送解除互锁的命令

// For SLAC rejector device
const int DEVICE_ON				 = 0;
const int DEVICE_OFF			 = 1;
const int DELAY_T				 = 2;
const int DELAY_D				 = 3;
const int DURATION_T			 = 4;
const int DURATION_D			 = 5;
const int CAMERA_DELAY_TIME		 = 6;
const int CAMERA_DELAY_DISTANCE  = 7;
const int DROP_IMG_UCL			 = 8;
const int CAMERA_FREE_MODE		 = 9;
const int CAMERA_TRIGGER_MODE	 = 10;
const int SA_SWITCH_EACH	     = 11;
const int SA_SWITCH_ONCE_FORALL  = 12;
const int R_DELAY_T				 = 13;
const int R_DELAY_D				 = 14;
const int R_DURATION_T			 = 15;
const int R_DURATION_D			 = 16;
const int RED_LIGHT_ON			 = 17;
const int RED_LIGHT_OFF			 = 18;
const int GREEN_LIGHT_ON		 = 19;
const int GREEN_LIGHT_OFF		 = 20;
const int YELLOW_LIGHT_ON		 = 21;
const int YELLOW_LIGHT_OFF		 = 22;
const int SOLENOID_ON			 = 23;
const int LC_TEST_ON			 = 24;
const int QUERY_CMD				 = 25;
const int RESET_STATS            = 26;  //清统计数据
const int TEST_SOLENOID          = 27;
const int TEST_LC                = 28;
const int VERSION                = 29;
const int CAMER_GRAB_TIME		 = 30;
const int SINGLE_INGORE_TIME     = 31;
const int ERROR_SINGAL		     = 32;
const int TEST_SOLENOID2         = 33;  //电磁阀二
const int ENCODER_ON			 = 34;  //编码器有
const int ENCODER_OFF			 = 35;  //编码器无
const int R_REJECT_OFF			 = 36;  //二次排盖关
const int R_REJECT_ON			 = 37;  //二次排盖开
const int R_SITOSOL_T			 = 38;  
const int R_SITOSOL_D			 = 39;  
const int WRANING_RESET			 = 40;
const int RESULT_SIGNAL			 = 41;
const int QUERY_CMD_TIME		 = 42;
const int TRIGGER_INGORE_TIME	 = 43;
const int LIGHT_TRIGGER_WIDTH = 44;
const int CLIENT_TIEM_SYN=45;
const int CAMERA_COUNT=46;
const int DEVICE_RESET = 47;
const int LIFEID_SET=48;
const int PC_STATUS_SET = 49;
const int C_REJ_UCL				=50;  //连续剔除报警阀值
const int SELF_LEARN_FUNC		=51;  //自学习功能使能/失能
const int HDW_TEST				=52;  //硬件测试
const int RESET_STATS_ERR		=53;  //清故障码
const int UNLOCK_INTERLOCK		=54;  //发送解除互锁的命令
//const int C_FRAME_UCL				=55;  //连续缺帧报警阀值

#define PCI_CREATE_FAILED_ERROR				        1	//PCI创建失败
#define LIGHT_CONTRL_CONNECT_ERROR			        2	//光源控制器
#define CAMERA_CONNECT_ERROR				        3	//相机连接错误
#define INIT_REJECTOR_SYS_FAILED			        4	//排盖版卡错误1, 本地ip设置错误，socket创建错误
#define THRESHOLD_BLOW_ERROR				        5	//排盖阈值搞告警
#define CDP_ERROR							        6	//连续错误图片CDP告警
#define INTERLOCK_CLEAR								7   //互锁清楚

#define ERRORSUBID_PCI_CREATE_FAILED		        1	//  101	无法连接到PCI板卡

#define ERRORSUBID_LIGHT_COM_FAILED			        1	// 	201	光源控制器所用的串口号被占用
#define ERRORSUBID_LIGHT_CONNECT_FAILED		        2	// 	202	光源控制器连接错误

#define ERRORSUBID_CAMERA_CONNECT_FAILED	        1	// 	301	相机连接错误——相机被占用或者IP不在同一网段
#define ERRORSUBID_CAMERA_PARAMS_ERROR	            2	// 	302	相机参数不合法
#define ERRORSUBID_CAMERA_IS_IN_USE			        3   // 	203	相机被占用！

#define ERRORSUBID_CAMERA_RJECT_CONNECT_FAILED		1	// 	401	无法连接到剔除板卡
#define ERRORSUBID_CAMERA_RJECT_COMMAND_ERROR		5	// 	405	剔除板卡指令无效
#define ERRORSUBID_CAMERA_RJECT_UNKNOW_ERROR		6	// 	406	剔除板卡排盖异常。 剔除板卡firmware运行异常
#define ERRORSUBID_CAMERA_RJECT_CAMERA_LOST_FRAME	7	// 	407	相机丢帧
#define ERRORSUBID_CAMERA_RJECT_PARAMS_ERROR		8	// 	408	剔除板参数不合法
#define ERRORSUBID_CAMERA_RJECT_ENCODER_FAILED		9	// 	409	编码器错误

#define ERRORSUBID_THRESHOLD_ERROR					1	// 	501	通道次品率超过设定值
#define ERRORSUBID_CDP_LASTN_ERROR					2	// 	502	最近N个产品次品率超过设定值

#define ERRORSUBID_CDP_ERROR						1	// 	601	连续剔除的产品数超过设定值



const long long MAX_INT64_VALUE = 9223372036854775807;

enum eCHANNEL_STATE 
{
	CH_ON = 0,
	CH_DEBUG,
	CH_OFF,
	CH_ERROR
}; 

enum eBUTTON_TYPE
{
	BT_INVALID = -1,
	BT_ON_OFF = 0,
	BT_PARAM,
	BT_SPC
}; 

enum eUSER_GROUP 
{
	OPERATOR = 0,
	ENGINEER,
	ADMIN,
	SADMIN
};

enum eERROR_LEVEL
{
	WL_NONE    = 0,
	WL_WARNING = 1,
	WL_ERROR   = 2
};

enum eERROR_SRC // warning source -- camera, light or algorithm group
{
	W_SRC_INVALID  = -1,
	W_SRC_ALGO_GP  = 0,
	W_SRC_CAMERA   = 1,
	W_SRC_LIGHT    = 2,
	W_SRC_PCI      = 3,
	W_SRC_REJECTOR = 4
};

enum SETTING_TYPE
{
	T_INVALID = -1,
	T_CAMERA,
	T_LIGHT,
	T_REJECTOR,
	T_ALGO,
	T_DEVICE,
	T_UI,
	T_TASK,
	T_MODEL,
	T_CHART_MGR,
	T_CONFIG_FILE_MGR,
	T_SYS_SHIFT_MGR
};

enum eTASK_ACTION
{
	TA_INVALID  = -1,
	TA_ADD      = 0,
	TA_DELETE   = 1,
	TA_TRAINING = 2
};

enum eTASK_OPERATION
{
	TO_INVALID    = -1,
	TO_ADD_REGION = 0,
	TO_DEL_REGION,
	TO_ADD_ITEM,
	TO_DEL_ITEM
};

void PathAdaptToModelFiles(wstring &path,int cameraIndex);
void ModelFilesAdaptToPath(wstring &path);
class CSyncLock{
public:	
	~CSyncLock(){
		::LeaveCriticalSection(&_cs);
	}
	CSyncLock(){		
		::EnterCriticalSection(&_cs);
	}
private:
	class CSig{
	private:
		CSig(){
			InitializeCriticalSection(&_cs);
		}
		CSig(const CSig &s);
		CSig& operator=(const CSig &s);
		~CSig(){}
	private:
		static CSig m_sig;
	};
	friend class CSig;
	static CRITICAL_SECTION _cs;
	static BOOL m_bInit_cs;
};

struct ERROR_INFO
{
	int          m_errorID;		//Camera,PCI, Rejector Card,Light Controller
	int		     m_errorSubID;  //SubID;
	long		 m_Counter;		//告警计数
	eERROR_LEVEL m_level;	    //Error level. Can be customized.
	int			 m_nChIndex;	//Channel Index
	wstring      m_strMsg;		//附加信息
	ERROR_INFO():m_errorID(-1),m_errorSubID(1), m_level(WL_NONE), m_nChIndex(0), m_strMsg(L""),m_Counter(0)
	{
	}
	ERROR_INFO(int errorID,int errorSubID, wstring strMsgEx =_T("") ,int ErrorSource = 0 ,eERROR_LEVEL level = WL_NONE,long counter = 1)
        :m_errorID(errorID),
        m_errorSubID(errorSubID), 
        m_level(level), 
        m_nChIndex(ErrorSource), 
        m_strMsg(strMsgEx),
		m_Counter(counter)
	{
	}

	ERROR_INFO(const ERROR_INFO& rhs)
	{
		m_errorID	  = rhs.m_errorID;
		m_errorSubID  = rhs.m_errorSubID;
		m_level		  = rhs.m_level;
		m_nChIndex	  = rhs.m_nChIndex;
		m_strMsg	  = rhs.m_strMsg;
		m_Counter     = rhs.m_Counter;
	}
	ERROR_INFO(ERROR_INFO&& rhs)
	{
		m_errorID     = rhs.m_errorID;
		m_errorSubID  = rhs.m_errorSubID;
		m_level       = rhs.m_level;
		m_nChIndex	  = rhs.m_nChIndex;
		m_strMsg      = rhs.m_strMsg;
		m_Counter     = rhs.m_Counter;
	}
	ERROR_INFO& operator=(const ERROR_INFO& rhs)
	{
		if(&rhs == this)
			return *this;
		m_errorID      = rhs.m_errorID;
		m_errorSubID   = rhs.m_errorSubID;
		m_level        = rhs.m_level;
		m_nChIndex	   = rhs.m_nChIndex;
		m_strMsg	   = rhs.m_strMsg;
		m_Counter     = rhs.m_Counter;
		return *this;
	}
	ERROR_INFO& operator=(ERROR_INFO&& rhs)
	{
		if(&rhs == this)
			return *this;
		m_errorID      = rhs.m_errorID;
		m_errorSubID   = rhs.m_errorSubID;
		m_level        = rhs.m_level;
		m_nChIndex     = rhs.m_nChIndex;
		m_strMsg       = rhs.m_strMsg;
		m_Counter      = rhs.m_Counter;
		return *this;
	}
};

struct ERROR_RGN_INFO
{
	long m_row;
	long m_col;
	long m_r;
	ERROR_RGN_INFO(long row = 0.0, long col = 0.0, long r = 0.0):
		m_row(row),
		m_col(col),
		m_r(r)
	{
	}

	ERROR_RGN_INFO(const ERROR_RGN_INFO& other)
	{
		m_row = other.m_row;
		m_col = other.m_col;
		m_r   = other.m_r;
	}

	ERROR_RGN_INFO& operator=(const ERROR_RGN_INFO& other)
	{
		if(this == &other)
			return *this;
		m_row = other.m_row;
		m_col = other.m_col;
		m_r   = other.m_r;
		return *this;
	}

	// move construction
	ERROR_RGN_INFO(ERROR_RGN_INFO&& other)
	{
		m_row = other.m_row;
		m_col = other.m_col;
		m_r   = other.m_r;
	}

	ERROR_RGN_INFO& operator=(ERROR_RGN_INFO&& other)
	{
		if(this == &other)
			return *this;
		m_row = other.m_row;
		m_col = other.m_col;
		m_r   = other.m_r;
		return *this;
	}
};

enum eTRIGGER_ACTIVATION
{
	FALL_EDGE = 0,
	RISE_EDGE
};

enum eDELAY_TYPE
{
	DELAY_INVALID  = -1,
	DELAY_TIME     = 0,
	DELAY_DISTANCE = 1
};

struct CAMERA_SETTING
{
	CAMERA_SETTING():
       m_name(L""),
	   m_bTriggerMode(true),
	   m_triggerAct(FALL_EDGE),
	   m_nFrameRate(20),
	   m_gain(15),
	   m_ExposureTime(100),
	   m_delayType(DELAY_TIME),
	   m_delayValue(0),
	   m_chIndex(-1),
	   m_nPacketSz(1500),
	   m_nDroppedImgUcl(3),
	   m_LineSelector(4),
	   m_LineSource(8)
	{
	}
	CAMERA_SETTING(const CAMERA_SETTING& other)
	{
		m_name         = other.m_name;
		m_bTriggerMode = other.m_bTriggerMode;
		m_triggerAct   = other.m_triggerAct;
		m_nFrameRate   = other.m_nFrameRate;
		m_gain         = other.m_gain;
		m_ExposureTime = other.m_ExposureTime;
		m_delayType    = other.m_delayType;
		m_delayValue   = other.m_delayValue;
		m_chIndex      = other.m_chIndex;
		m_nPacketSz    = other.m_nPacketSz;
		m_nDroppedImgUcl = other.m_nDroppedImgUcl;
		m_LineSelector = other.m_LineSelector;
		m_LineSource	= other.m_LineSource;
	}
	CAMERA_SETTING(CAMERA_SETTING&& other)
	{
		m_name         = other.m_name;
		m_bTriggerMode = other.m_bTriggerMode;
		m_triggerAct   = other.m_triggerAct;
		m_nFrameRate   = other.m_nFrameRate;
		m_gain         = other.m_gain;
		m_ExposureTime = other.m_ExposureTime;
		m_delayType    = other.m_delayType;
		m_delayValue   = other.m_delayValue;
		m_chIndex      = other.m_chIndex;
		m_nPacketSz    = other.m_nPacketSz;
		m_nDroppedImgUcl = other.m_nDroppedImgUcl;
		m_LineSelector = other.m_LineSelector;
		m_LineSource	= other.m_LineSource;
	}
	CAMERA_SETTING& operator=(const CAMERA_SETTING& other)
	{
		if(&other == this)
			return *this;
		m_name         = other.m_name;
		m_bTriggerMode = other.m_bTriggerMode;
		m_triggerAct   = other.m_triggerAct;
		m_nFrameRate   = other.m_nFrameRate;
		m_gain         = other.m_gain;
		m_ExposureTime = other.m_ExposureTime;
		m_delayType    = other.m_delayType;
		m_delayValue   = other.m_delayValue;
		m_chIndex      = other.m_chIndex;
		m_nPacketSz    = other.m_nPacketSz;
		m_nDroppedImgUcl = other.m_nDroppedImgUcl;
		m_LineSelector = other.m_LineSelector;
		m_LineSource	= other.m_LineSource;
		return *this;
	}
	CAMERA_SETTING& operator=(CAMERA_SETTING&& other)
	{
		if(&other == this)
			return *this;
		m_name         = other.m_name;
		m_bTriggerMode = other.m_bTriggerMode;
		m_triggerAct   = other.m_triggerAct;
		m_nFrameRate   = other.m_nFrameRate;
		m_gain         = other.m_gain;
		m_ExposureTime = other.m_ExposureTime;
		m_delayType    = other.m_delayType;
		m_delayValue   = other.m_delayValue;
		m_chIndex      = other.m_chIndex;
		m_nPacketSz    = other.m_nPacketSz;
		m_nDroppedImgUcl = other.m_nDroppedImgUcl;
		m_LineSelector = other.m_LineSelector;
		m_LineSource	= other.m_LineSource;
		return *this;
	}

	bool IsEqual(const CAMERA_SETTING& other)
	{
		return (m_name == other.m_name) &&
				(m_bTriggerMode == other.m_bTriggerMode) &&
				(m_triggerAct == other.m_triggerAct) &&
				(m_nFrameRate == other.m_nFrameRate) &&
				(m_gain == other.m_gain) &&
				(m_ExposureTime == other.m_ExposureTime) &&
				(m_delayType == other.m_delayType) && 
				(m_delayValue == other.m_delayValue) &&
				(m_chIndex == other.m_chIndex) &&
				(m_nPacketSz == other.m_nPacketSz) &&
				(m_LineSelector == other.m_LineSelector) && 
				(m_LineSource  == other.m_LineSource) && 
				(m_nDroppedImgUcl == other.m_nDroppedImgUcl);
	}

	std::wstring		 m_name; // ID
	bool				 m_bTriggerMode; 
	eTRIGGER_ACTIVATION  m_triggerAct;
	int                  m_nFrameRate;
	int                  m_gain;
	eDELAY_TYPE          m_delayType;
	int                  m_delayValue;
	int                  m_chIndex;
	int                  m_nPacketSz;      // Packet Size;
	int                  m_nDroppedImgUcl; // UCL of dropped image
	int					 m_ExposureTime;
	int					 m_LineSelector;
	int					 m_LineSource;
};

struct IMAGE_BOX
{
	Hobject        m_img;
	bool				m_bDebugImg;
	bool				m_bSingleDebugImg;
	bool				m_EmtpyImg;
	bool				m_bLastFlag;
	bool				m_bSwitchImg;
	int				m_iProcessID;  //分组处理传输

	long long	   m_ImageID;
	long long	   m_ImageID_PC;
	long long	   m_ImageTimestamp;
	long long    m_ImageTimestamp_PC;

	LARGE_INTEGER  m_arrivalTimestamp;



	IMAGE_BOX()
		:m_bDebugImg(false),
		m_bLastFlag(false),
		m_bSwitchImg(false),
		m_bSingleDebugImg(false),
		m_EmtpyImg(false),
		m_iProcessID(0),
		m_ImageTimestamp(0),
		m_ImageTimestamp_PC(0),
		m_ImageID(1),
		m_ImageID_PC(0)

	{
		QueryPerformanceCounter(&m_arrivalTimestamp);
	}
	IMAGE_BOX(Hobject img, bool bDebugImg = false, bool bLastFlag = false, bool bSwitchImg = false, bool bSingleDebugImg= false,int64_t ImageTimestamp =0,int64_t ImageID=1,long ImageID_PC = 0,long long ImageTimestamp_PC=0)
		:m_img(img),
		m_bDebugImg(bDebugImg),
		m_bLastFlag(bLastFlag),
		m_bSwitchImg(bSwitchImg),
		m_bSingleDebugImg(bSingleDebugImg),
		m_EmtpyImg(false),
	 	m_iProcessID(0),
		m_ImageTimestamp(ImageTimestamp),
		m_ImageID(ImageID),
		m_ImageID_PC(ImageID_PC),
		m_ImageTimestamp_PC(ImageTimestamp_PC)
	{
		QueryPerformanceCounter(&m_arrivalTimestamp);
	}
	IMAGE_BOX(const IMAGE_BOX& other)
	{
		m_img               = other.m_img;
		m_bDebugImg         = other.m_bDebugImg;
		m_EmtpyImg          = other.m_EmtpyImg;
		m_bLastFlag         = other.m_bLastFlag;
		m_bSwitchImg        = other.m_bSwitchImg;
		m_bSingleDebugImg   = other.m_bSingleDebugImg;
		m_iProcessID		= other.m_iProcessID;
		m_ImageTimestamp    = other.m_ImageTimestamp;
		m_ImageTimestamp_PC = other.m_ImageTimestamp_PC;
		m_ImageID           = other.m_ImageID;
		m_ImageID_PC        = other.m_ImageID_PC;
		m_arrivalTimestamp  = other.m_arrivalTimestamp;
	}
	IMAGE_BOX& operator=(const IMAGE_BOX& other)
	{
		if(&other == this)
			return *this;
		m_img               = other.m_img;
		m_bDebugImg         = other.m_bDebugImg;
		m_EmtpyImg          = other.m_EmtpyImg;
		m_bLastFlag         = other.m_bLastFlag;
		m_bSwitchImg        = other.m_bSwitchImg;
		m_bSingleDebugImg   = other.m_bSingleDebugImg;
		m_iProcessID		= other.m_iProcessID;
		m_ImageTimestamp    = other.m_ImageTimestamp;
		m_ImageTimestamp_PC = other.m_ImageTimestamp_PC;
		m_ImageID           = other.m_ImageID;
		m_ImageID_PC        = other.m_ImageID_PC;
		m_arrivalTimestamp  = other.m_arrivalTimestamp;
		return *this;
	}

	IMAGE_BOX(IMAGE_BOX&& other)
	{
		m_img               = other.m_img;
		m_bDebugImg         = other.m_bDebugImg;
		m_EmtpyImg          = other.m_EmtpyImg;
		m_bLastFlag         = other.m_bLastFlag;
		m_bSwitchImg        = other.m_bSwitchImg;
		m_bSingleDebugImg   = other.m_bSingleDebugImg;
		m_iProcessID		= other.m_iProcessID;
		m_ImageTimestamp    = other.m_ImageTimestamp;
		m_ImageTimestamp_PC = other.m_ImageTimestamp_PC;
		m_ImageID           = other.m_ImageID;
		m_ImageID_PC        = other.m_ImageID_PC;
		m_arrivalTimestamp  = other.m_arrivalTimestamp;
	}
	IMAGE_BOX& operator=(IMAGE_BOX&& other)
	{
		if(&other == this)
			return *this;
		m_img               = other.m_img;
		m_bDebugImg         = other.m_bDebugImg;
		m_EmtpyImg          = other.m_EmtpyImg;
		m_bLastFlag         = other.m_bLastFlag;
		m_bSwitchImg        = other.m_bSwitchImg;
		m_bSingleDebugImg   = other.m_bSingleDebugImg;
		m_iProcessID= other.m_iProcessID;
		m_ImageTimestamp    = other.m_ImageTimestamp;
		m_ImageTimestamp_PC = other.m_ImageTimestamp_PC;
		m_ImageID           = other.m_ImageID;
		m_ImageID_PC        = other.m_ImageID_PC;
		m_arrivalTimestamp  = other.m_arrivalTimestamp;
		return *this;
	}
};
struct NG_IMG_SIGNAL
{
	LARGE_INTEGER	m_bgn;
	int             m_nPort;
	double   m_arrivetime;
	NG_IMG_SIGNAL()
	{
	}

	NG_IMG_SIGNAL(int port,double time)
		:m_nPort(port),
		m_arrivetime(time)
	{
	}
	NG_IMG_SIGNAL(const NG_IMG_SIGNAL& other)
	{
		m_bgn   = other.m_bgn;
		m_nPort = other.m_nPort;
		m_arrivetime = other.m_arrivetime;
	}
	NG_IMG_SIGNAL(NG_IMG_SIGNAL&& other)
	{
		m_bgn   = other.m_bgn;
		m_nPort = other.m_nPort;
		m_arrivetime = other.m_arrivetime;
	}
	NG_IMG_SIGNAL& operator=(const NG_IMG_SIGNAL& other)
	{
		if(&other == this)
			return *this;
		m_bgn   = other.m_bgn;
		m_nPort = other.m_nPort;
		m_arrivetime = other.m_arrivetime;
		return *this;
	}

	NG_IMG_SIGNAL& operator=(NG_IMG_SIGNAL&& other)
	{
		if(&other == this)
			return *this;
		m_bgn   = other.m_bgn;
		m_nPort = other.m_nPort;
		m_arrivetime = other.m_arrivetime;
		return *this;
	}
};

struct LC_CH_PARAM

{
	LC_CH_PARAM():m_nLum(10), m_nDelay(100),m_nDuration(10),m_nLum1(0), m_nDelay1(0),m_nDuration1(0)
	{
	}
	LC_CH_PARAM(const LC_CH_PARAM& other)
	{
		m_nLum      = other.m_nLum;
		m_nDelay    = other.m_nDelay;
		m_nDuration = other.m_nDuration;
		m_nLum1      =other.m_nLum1;
		m_nDelay1    = other.m_nDelay1;
		m_nDuration1 = other.m_nDuration1;
	}
	LC_CH_PARAM(LC_CH_PARAM&& other)
	{
		m_nLum      = other.m_nLum;
		m_nDelay    = other.m_nDelay;
		m_nDuration = other.m_nDuration;
		m_nLum1      =other.m_nLum1;
		m_nDelay1    = other.m_nDelay1;
		m_nDuration1 = other.m_nDuration1;
	}
	LC_CH_PARAM& operator=(const LC_CH_PARAM& other)
	{
		if(&other == this)
		{
			return *this;
		}
		m_nLum      = other.m_nLum;
		m_nDelay    = other.m_nDelay;
		m_nDuration = other.m_nDuration;
		m_nLum1      =other.m_nLum1;
		m_nDelay1    = other.m_nDelay1;
		m_nDuration1 = other.m_nDuration1;

		return *this;
	}
	LC_CH_PARAM& operator=(LC_CH_PARAM&& other)
	{
		if(&other == this)
		{
			return *this;
		}
		m_nLum      = other.m_nLum;
		m_nDelay    = other.m_nDelay;
		m_nDuration = other.m_nDuration;
		m_nLum1      =other.m_nLum1;
		m_nDelay1    = other.m_nDelay1;
		m_nDuration1 = other.m_nDuration1;

		return *this;
	}
	bool IsEqual(const LC_CH_PARAM& other)
	{
		return (m_nLum      == other.m_nLum &&
		        m_nDelay    == other.m_nDelay &&
		        m_nDuration == other.m_nDuration&&
				m_nLum1     ==other.m_nLum1&&
				m_nDelay1   == other.m_nDelay1&&
				m_nDuration1== other.m_nDuration1);

	}
	string PackageParam()
	{
		string strRet = boost::lexical_cast<string>(m_nLum);
		strRet.append("$");
		strRet.append(boost::lexical_cast<string>(m_nDelay));
		strRet.append("$");
		strRet.append(boost::lexical_cast<string>(m_nDuration));

		return strRet;
	}
	string PackageParam1()
	{
		string strRet = boost::lexical_cast<string>(m_nLum1);
		strRet.append("$");
		strRet.append(boost::lexical_cast<string>(m_nDelay1));
		strRet.append("$");
		strRet.append(boost::lexical_cast<string>(m_nDuration1));

		return strRet;
	}
	unsigned int  m_nLum;
	unsigned int  m_nDelay;
	unsigned int  m_nDuration;

	unsigned int  m_nLum1;
	unsigned int  m_nDelay1;
	unsigned int  m_nDuration1;
};

struct LC // Light Controller
{
	LC():m_nID(-1), m_nMode(LC_TRIGGER),m_nActiveID(0)
	{
		m_vChParam = std::vector<LC_CH_PARAM>(4);

	}
	LC(const LC& other)
	{
		m_nID   = other.m_nID;
		m_nMode = other.m_nMode;
		m_nActiveID = other.m_nActiveID;
		m_vChParam.clear();
		m_vChParam = other.m_vChParam;
	}
	LC(LC&& other)
	{
		m_nID   = other.m_nID;
		m_nMode = other.m_nMode;
		m_nActiveID = other.m_nActiveID;

		m_vChParam.clear();
		m_vChParam = other.m_vChParam;
	}
	LC& operator=(const LC& other)
	{
		if(&other == this)
		{
			return *this;
		}
		m_nID   = other.m_nID;
		m_nMode = other.m_nMode;
		m_nActiveID = other.m_nActiveID;

		m_vChParam.clear();
		m_vChParam = other.m_vChParam;
		return *this;
	}
	LC& operator=(LC&& other)
	{
		if(&other == this)
		{
			return *this;
		}
		m_nID   = other.m_nID;
		m_nMode = other.m_nMode;
		m_nActiveID = other.m_nActiveID;

		m_vChParam.clear();
		m_vChParam = other.m_vChParam;
		return *this;
	}

	string ClipString(char *str, unsigned int n) const
	{
		std::vector<char> vChar(n, '\0');
		unsigned int len = (unsigned int)strlen(str);
		if(n >= len)
		{
			std::string strRet = str;
			return strRet;
		}
		int k = 0;
		for(unsigned int i = len - n; i < len; i++)
		{
			vChar[k++] = str[i];
		}
		std::string strRet;
		strRet.assign(vChar.begin(), vChar.end());

		return strRet;
	}
	string TranslateCode(unsigned int st) const
	{
		string rt;
		char ch[16];
		sprintf_s(ch,"%02x",st);
		rt += ClipString(ch,2);
		rt += " ";
		sprintf_s(ch,"%02x",~st);
		rt += ClipString(ch,2);
		rt += " ";
		return rt;
	}
	string TranslateCode(unsigned int st, int len) const
	{
		int itmp;
		char tch[3];
		char ch[128];
		string rt;
		sprintf_s(ch,"%04x",st);
		std::string sch = ClipString(ch,4);
		tch[0] = sch[0];
		tch[1] = sch[1];
		tch[2] = 0;
		itmp   = strtol(tch,NULL,16);
		rt     = TranslateCode(itmp);
		tch[0] = sch[2];
		tch[1] = sch[3];
		tch[2] = 0;
		itmp   = strtol(tch,NULL,16);
		rt+= TranslateCode(itmp);
		return rt;
	}

	string PackageParam()
	{
		string strRet;
		size_t sz = m_vChParam.size();
		for(size_t t = 0; t < sz; t++)
		{
			strRet.append(m_vChParam[t].PackageParam());
			if(t != sz - 1)
			{
				strRet.append(";");
			}
		}

		return strRet;
	}

	string PackageParam1()
	{
		string strRet;
		size_t sz = m_vChParam.size();
		for(size_t t = 0; t < sz; t++)
		{
			strRet.append(m_vChParam[t].PackageParam());
			if(t != sz - 1)
			{
				strRet.append(";");
			}
		}
		strRet.append(";");

		for(size_t t = 0; t < sz; t++)
		{
			strRet.append(m_vChParam[t].PackageParam1());
			if(t != sz - 1)
			{
				strRet.append(";");
			}
		}

		return strRet;
	}

	char HexChar(char c) const
	{
		if((c>='0')&&(c<='9'))
			return c-0x30;
		else if((c>='A')&&(c<='F'))
			return c-'A'+10;
		else if((c>='a')&&(c<='f'))
			return c-'a'+10;
		else 
			return 0x10;
	}

	string Str2Hex(string str) const
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
			int t  = HexChar(h);
			int t1 = HexChar(l);
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
	string MakeLcCommand (string& cmdStr, bool bSave = false, bool bRead = false) const
	{
		string strRet;

		unsigned int nModeCode = 0XC0;
		if(m_nMode == LC_ON)
		{
			nModeCode = 0XC0;
		}
		if(m_nMode == LC_TRIGGER)
		{
			nModeCode = 0X30;
		}
		if(m_nMode == LC_TRIGGER_TWICE)
		{
			nModeCode = 0X30;
		}
		if(bRead)
		{
			nModeCode = 0X10;
		}
		if(bSave)
		{
			nModeCode += 1;
		}
		strRet = "55 66 ";
		strRet += TranslateCode(m_nID);
		string tmp = TranslateCode(nModeCode);
		strRet.append(tmp);


		strRet += TranslateCode(m_vChParam[0].m_nLum);
		strRet += TranslateCode(m_vChParam[1].m_nLum);
		strRet += TranslateCode(m_vChParam[2].m_nLum);
		strRet += TranslateCode(m_vChParam[3].m_nLum);

		strRet += TranslateCode(m_vChParam[0].m_nDelay, 4);
		strRet += TranslateCode(m_vChParam[1].m_nDelay, 4);
		strRet += TranslateCode(m_vChParam[2].m_nDelay, 4);
		strRet += TranslateCode(m_vChParam[3].m_nDelay, 4);

		strRet += TranslateCode(m_vChParam[0].m_nDuration);
		strRet += TranslateCode(m_vChParam[1].m_nDuration);
		strRet += TranslateCode(m_vChParam[2].m_nDuration);
		strRet += TranslateCode(m_vChParam[3].m_nDuration);

		strRet += "77 88";

		cmdStr = strRet;

		std::string strHex = Str2Hex(strRet);

		return strHex;
	}
	string MakeLcCommandNew(string& cmdStr, bool bSave = false,int Singleid = 0) const
	{
		int m_bSingleMode = false;
		int channel_ID = 0;
		if (m_nActiveID != 0 && !((m_nActiveID == 15)&&(Singleid==0)))
		{
			m_bSingleMode = true;
			channel_ID+= 0x09;
			channel_ID+= Singleid;
		}
		string strRet;
		unsigned int nModeCode = 0XC0;
		if(m_nMode == LC_ON)
		{
			nModeCode = 0XC0;
			if (m_bSingleMode)
			{
				nModeCode += channel_ID;
			}
		}
		if(m_nMode == LC_TRIGGER)
		{
			nModeCode = 0X30;
			if (m_bSingleMode)
			{
				nModeCode += channel_ID;
			}
		}
		if(m_nMode == LC_TRIGGER_TWICE)
		{
			nModeCode = 0X90;
			if (m_bSingleMode)
			{
				nModeCode += channel_ID;
			}
		}
		if(bSave)
		{
			if (m_bSingleMode)
			{
				nModeCode =0x60;
			}
			else
			{
				nModeCode += 1;
			}

		}
		strRet = "55 66 ";
		strRet += TranslateCode(m_nID);
		string tmp = TranslateCode(nModeCode);
		strRet.append(tmp);

		if(m_nMode == LC_TRIGGER_TWICE)
		{
			strRet += TranslateCode(m_vChParam[0].m_nLum1);
			strRet += TranslateCode(m_vChParam[1].m_nLum1);
			strRet += TranslateCode(m_vChParam[2].m_nLum1);
			strRet += TranslateCode(m_vChParam[3].m_nLum1);

			strRet += TranslateCode(m_vChParam[0].m_nDelay1, 4);
			strRet += TranslateCode(m_vChParam[1].m_nDelay1, 4);
			strRet += TranslateCode(m_vChParam[2].m_nDelay1, 4);
			strRet += TranslateCode(m_vChParam[3].m_nDelay1, 4);

			strRet += TranslateCode(m_vChParam[0].m_nDuration1);
			strRet += TranslateCode(m_vChParam[1].m_nDuration1);
			strRet += TranslateCode(m_vChParam[2].m_nDuration1);
			strRet += TranslateCode(m_vChParam[3].m_nDuration1);

		}
		else	
		{
			strRet += TranslateCode(m_vChParam[0].m_nLum);
			strRet += TranslateCode(m_vChParam[1].m_nLum);
			strRet += TranslateCode(m_vChParam[2].m_nLum);
			strRet += TranslateCode(m_vChParam[3].m_nLum);

			strRet += TranslateCode(m_vChParam[0].m_nDelay, 4);
			strRet += TranslateCode(m_vChParam[1].m_nDelay, 4);
			strRet += TranslateCode(m_vChParam[2].m_nDelay, 4);
			strRet += TranslateCode(m_vChParam[3].m_nDelay, 4);

			strRet += TranslateCode(m_vChParam[0].m_nDuration);
			strRet += TranslateCode(m_vChParam[1].m_nDuration);
			strRet += TranslateCode(m_vChParam[2].m_nDuration);
			strRet += TranslateCode(m_vChParam[3].m_nDuration);
		}

		strRet += "77 88";

		cmdStr = strRet;

		std::string strHex = Str2Hex(strRet);
		CString str;
		str = cmdStr.c_str();
		OutputDebugString(str);
		OutputDebugString(_T("\n"));
		return strHex;
	}
	unsigned int  m_nID;
	unsigned int  m_nMode;
	unsigned int   m_nActiveID; //当前激活通道
	std::vector<LC_CH_PARAM> m_vChParam;
};

struct LIGHT_SETTING
{
	LIGHT_SETTING():m_nChIndex(-1)
	{
		m_vLcID.clear();
		m_vTags.clear();
		m_dictParamGroup.clear();
	}
	LIGHT_SETTING(const LIGHT_SETTING& other)
	{
		m_nChIndex     = other.m_nChIndex;
		m_vLcID.clear();
		m_vLcID        = other.m_vLcID;
		m_vTags.clear();
		m_vTags        = other.m_vTags;
		m_dictParamGroup.clear();
		m_dictParamGroup = other.m_dictParamGroup;
	}
	LIGHT_SETTING(LIGHT_SETTING&& other)
	{
		m_nChIndex     = other.m_nChIndex;
		m_vLcID.clear();
		m_vLcID        = other.m_vLcID;
		m_vTags.clear();
		m_vTags        = other.m_vTags;
		m_dictParamGroup.clear();
		m_dictParamGroup = other.m_dictParamGroup;
	}
	LIGHT_SETTING& operator=(const LIGHT_SETTING& other)
	{
		if(&other == this)
		{
			return *this;
		}
		m_nChIndex     = other.m_nChIndex;
		m_vLcID.clear();
		m_vLcID        = other.m_vLcID;
		m_vTags.clear();
		m_vTags        = other.m_vTags;
		m_dictParamGroup.clear();
		m_dictParamGroup = other.m_dictParamGroup;
		return *this;
	}
	LIGHT_SETTING& operator=(LIGHT_SETTING&& other)
	{
		if(&other == this)
		{
			return *this;
		}
		m_nChIndex     = other.m_nChIndex;
		m_vLcID.clear();
		m_vLcID        = other.m_vLcID;
		m_vTags.clear();
		m_vTags        = other.m_vTags;
		m_dictParamGroup.clear();
		m_dictParamGroup = other.m_dictParamGroup;
		return *this;
	}

	bool CorrectContrllerID()
	{
		if(m_dictParamGroup.empty())
		{
			return false;
		}
		bool bChanged = false;

		for(auto it = m_dictParamGroup.begin(); it != m_dictParamGroup.end(); it++)
		{
			std::vector<LC> vTmpLc = it->second;
			std::vector<LC>& vLC   = it->second;
			size_t nOldSz = vLC.size();
			vLC.clear();

			for(size_t i = 0; i < m_vLcID.size(); i++)
			{
				int nLcID = m_vLcID[i];
				auto it = std::find_if(vTmpLc.begin(),vTmpLc.end(),[&](LC lcItem)->bool
				{
					return (lcItem.m_nID == nLcID);
				});
				if(it != vTmpLc.end())
				{
					vLC.push_back(*it);
				}
				else
				{
					LC newLc;
					newLc.m_nID = nLcID;
					vLC.push_back(newLc);
					bChanged = true;
				}
			}
			if(nOldSz != vLC.size())
			{
				bChanged = true;
			}
		}
		return bChanged;
	}

	int                      m_nChIndex;
	vector<int>              m_vLcID;   // Light Contrller ID. One channel may need more than one controller.
	vector<wstring>          m_vTags;
	map<wstring, vector<LC>> m_dictParamGroup;
};

enum eSOLENOID_ACTION
{
	SA_INVALID         = -1,
	SA_SWITCH_FOR_EACH = 0,
	SA_SWITCH_FOR_ALL  = 1
};

struct REJECTOR_SETTING
{
	wstring				m_wstrDeviceIp;
	int					m_nRejectPort;
	int					m_nDropImgPort;
	int					m_CameraGrabTime;
	int					m_SignalIngoreTime;
	int					m_TriggerIngoreTime;
	int				    m_TriggerLightWidth;
	eDELAY_TYPE		    m_mode;
	int					m_delay;       // Delay time/Delay Distance
	int					m_signalWidth; // Signal Width(time or encoder pulse counts)
	bool				m_bReReject;
	bool				m_bReRejectFunc;
	int					m_nReSitoSol;
	int					m_nReDelay;
	int					m_nReSignalWidth;
	bool				m_bTestRejector;
	int					m_nTestMode;
	int					m_nNumerator;
	int					m_nDenominator;
	eSOLENOID_ACTION	m_sa;   // Solenoid action for CDC;
	int					m_nCDC; // CDC - Continuous Defective Count
	int					m_nIC;  // Injector Count;
    int                 m_nLastN; //5000,10000,15000,20000
    float               m_fLastNRatio;
    bool                m_bEnableLastNAlarm;
	//int					m_nContinueFrameAlarm;  //连续缺帧报警阈值，无符号整数(默认5)
	int					m_nContinueRejectAlarm; //连续剔除报警阈值（默认50）
	REJECTOR_SETTING()
		: m_wstrDeviceIp(L"")
		,m_nRejectPort(-1)
		,m_nDropImgPort(-1)
		,m_mode(DELAY_DISTANCE)
		,m_delay(0)
		,m_signalWidth(0)
		,m_bReReject(false)
		,m_nReDelay(0)
		,m_nReSignalWidth(0)
		,m_bTestRejector(false)
		,m_nTestMode(0)
		,m_nNumerator(-1)
		,m_nDenominator(-1)
		,m_sa(SA_SWITCH_FOR_EACH)
		,m_nCDC(0)
		,m_nIC(-1)
		,m_bReRejectFunc(false)
		,m_CameraGrabTime(0)
		,m_SignalIngoreTime(0)
		,m_TriggerIngoreTime(0)
		,m_TriggerLightWidth(0)
        ,m_nLastN(5000)
        ,m_fLastNRatio(0.5)
        ,m_bEnableLastNAlarm(false)
	{
		m_nContinueRejectAlarm=50;		
	}
	REJECTOR_SETTING(const REJECTOR_SETTING& other)
	{
		m_wstrDeviceIp   = other.m_wstrDeviceIp;
		m_nRejectPort    = other.m_nRejectPort;
		m_nDropImgPort   = other.m_nDropImgPort;
		m_mode           = other.m_mode;
		m_delay          = other.m_delay;
		m_signalWidth    = other.m_signalWidth;
		m_bReReject      = other.m_bReReject;
		m_bReRejectFunc  = other.m_bReRejectFunc;
		m_nReSitoSol     = other.m_nReSitoSol;
		m_nReDelay       = other.m_nReDelay;
		m_nReSignalWidth = other.m_nReSignalWidth;
		m_bTestRejector  = other.m_bTestRejector;
		m_nTestMode      = other.m_nTestMode;
		m_nNumerator     = other.m_nNumerator;
		m_nDenominator   = other.m_nDenominator;
		m_sa             = other.m_sa;
		m_nCDC           = other.m_nCDC;
		m_nIC            = other.m_nIC;
		m_CameraGrabTime = other.m_CameraGrabTime;
		m_SignalIngoreTime = other.m_SignalIngoreTime;
		m_TriggerIngoreTime = other.m_TriggerIngoreTime;
		m_TriggerLightWidth = other.m_TriggerLightWidth;
        m_nLastN            = other.m_nLastN;
        m_fLastNRatio       = other.m_fLastNRatio;
        m_bEnableLastNAlarm = other.m_bEnableLastNAlarm;
		m_nContinueRejectAlarm=other.m_nContinueRejectAlarm;		
	}
	REJECTOR_SETTING(REJECTOR_SETTING&& other)
	{
		m_wstrDeviceIp   = other.m_wstrDeviceIp;
		m_nRejectPort    = other.m_nRejectPort;
		m_nDropImgPort   = other.m_nDropImgPort;
		m_mode           = other.m_mode;
		m_delay          = other.m_delay;
		m_signalWidth    = other.m_signalWidth;
		m_nReSitoSol     = other.m_nReSitoSol;
		m_bReReject      = other.m_bReReject;
		m_bReRejectFunc  = other.m_bReRejectFunc;
		m_nReDelay       = other.m_nReDelay;
		m_nReSignalWidth = other.m_nReSignalWidth;
		m_bTestRejector  = other.m_bTestRejector;
		m_nTestMode      = other.m_nTestMode;
		m_nNumerator     = other.m_nNumerator;
		m_nDenominator   = other.m_nDenominator;
		m_sa             = other.m_sa;
		m_nCDC           = other.m_nCDC;
		m_nIC            = other.m_nIC;
		m_CameraGrabTime = other.m_CameraGrabTime;
		m_SignalIngoreTime = other.m_SignalIngoreTime;
		m_TriggerIngoreTime = other.m_TriggerIngoreTime;
		m_TriggerLightWidth = other.m_TriggerLightWidth;
        m_nLastN            = other.m_nLastN;
        m_fLastNRatio       = other.m_fLastNRatio;
        m_bEnableLastNAlarm = other.m_bEnableLastNAlarm;
		m_nContinueRejectAlarm=other.m_nContinueRejectAlarm;		
	}
	REJECTOR_SETTING& operator=(const REJECTOR_SETTING& other)
	{
		if(&other == this)
			return *this;
		m_wstrDeviceIp   = other.m_wstrDeviceIp;
		m_nRejectPort    = other.m_nRejectPort;
		m_nDropImgPort   = other.m_nDropImgPort;
		m_mode           = other.m_mode;
		m_delay          = other.m_delay;
		m_signalWidth    = other.m_signalWidth;
		m_bReReject      = other.m_bReReject;
		m_bReRejectFunc  = other.m_bReRejectFunc;
		m_nReSitoSol     = other.m_nReSitoSol;
		m_nReDelay       = other.m_nReDelay;
		m_nReSignalWidth = other.m_nReSignalWidth;
		m_bTestRejector  = other.m_bTestRejector;
		m_nTestMode      = other.m_nTestMode;
		m_nNumerator     = other.m_nNumerator;
		m_nDenominator   = other.m_nDenominator;
		m_sa             = other.m_sa;
		m_nCDC           = other.m_nCDC;
		m_nIC            = other.m_nIC;
		m_CameraGrabTime = other.m_CameraGrabTime;
		m_SignalIngoreTime = other.m_SignalIngoreTime;
		m_TriggerIngoreTime = other.m_TriggerIngoreTime;
		m_TriggerLightWidth = other.m_TriggerLightWidth;
        m_nLastN            = other.m_nLastN;
        m_fLastNRatio       = other.m_fLastNRatio;
        m_bEnableLastNAlarm = other.m_bEnableLastNAlarm;
		m_nContinueRejectAlarm=other.m_nContinueRejectAlarm;		
		return *this;
	}
	REJECTOR_SETTING& operator=(REJECTOR_SETTING&& other)
	{
		if(&other == this)
			return *this;
		m_wstrDeviceIp   = other.m_wstrDeviceIp;
		m_nRejectPort    = other.m_nRejectPort;
		m_nDropImgPort   = other.m_nDropImgPort;
		m_mode           = other.m_mode;
		m_delay          = other.m_delay;
		m_signalWidth    = other.m_signalWidth;
		m_bReReject      = other.m_bReReject;
		m_bReRejectFunc  = other.m_bReRejectFunc;
		m_nReSitoSol     = other.m_nReSitoSol;
		m_nReDelay       = other.m_nReDelay;
		m_nReSignalWidth = other.m_nReSignalWidth;
		m_bTestRejector  = other.m_bTestRejector;
		m_nTestMode      = other.m_nTestMode;
		m_nNumerator     = other.m_nNumerator;
		m_nDenominator   = other.m_nDenominator;
		m_sa             = other.m_sa;
		m_nCDC           = other.m_nCDC;
		m_nIC            = other.m_nIC;
		m_CameraGrabTime = other.m_CameraGrabTime;
		m_SignalIngoreTime = other.m_SignalIngoreTime;
		m_TriggerIngoreTime = other.m_TriggerIngoreTime;
		m_TriggerLightWidth = other.m_TriggerLightWidth;
        m_nLastN            = other.m_nLastN;
        m_fLastNRatio       = other.m_fLastNRatio;
        m_bEnableLastNAlarm = other.m_bEnableLastNAlarm;
		m_nContinueRejectAlarm=other.m_nContinueRejectAlarm;		
		return *this;
	}
	bool IsEqual(const REJECTOR_SETTING& other)
	{
		return (m_wstrDeviceIp == other.m_wstrDeviceIp)
			&& (m_nRejectPort  == other.m_nRejectPort)
			&& (m_nDropImgPort == other.m_nDropImgPort)
			&& (m_mode == other.m_mode)
			&& (m_delay==other.m_delay)
			&& (m_signalWidth == other.m_signalWidth)
			&& (m_bReReject == other.m_bReReject)
			&& (m_bReRejectFunc == other.m_bReRejectFunc)
			&& (m_nReSitoSol  == other.m_nReSitoSol)
		    && (m_nReDelay == other.m_nReDelay)
			&& (m_nReSignalWidth == other.m_nReSignalWidth)
			&& (m_bTestRejector == other.m_bTestRejector)
			&& (m_nTestMode == other.m_nTestMode)
			&& (m_nNumerator == other.m_nNumerator)
			&& (m_nDenominator == other.m_nDenominator)
			&& (m_sa == other.m_sa)
			&& (m_nCDC == other.m_nCDC)
			&& (m_nIC == other.m_nIC)
			&& (m_CameraGrabTime == other.m_CameraGrabTime)
			&& (m_SignalIngoreTime == other.m_SignalIngoreTime)
			&& (m_TriggerIngoreTime == other.m_TriggerIngoreTime)
			&& (m_TriggerLightWidth = other.m_TriggerLightWidth)
            && (m_nLastN  == other.m_nLastN)
            && (m_fLastNRatio == other.m_fLastNRatio)
            && (m_bEnableLastNAlarm == other.m_bEnableLastNAlarm)
			&& (m_nContinueRejectAlarm==other.m_nContinueRejectAlarm);
	}
};

struct UI_SETTING
{
	int m_width;
	int m_height;
	UI_SETTING()
	{
		m_width = 534;
		m_height = 400;
	}
	UI_SETTING(const UI_SETTING& other)
	{
		m_width = other.m_width;
		m_height = other.m_height;
	}
	UI_SETTING(UI_SETTING&& other)
	{
		m_width = other.m_width;
		m_height = other.m_height;
	}
	UI_SETTING& operator=(const UI_SETTING& other)
	{
		if(&other == this)
			return *this;
		m_width = other.m_width;
		m_height = other.m_height;
		return *this;
	}
	UI_SETTING& operator=(UI_SETTING&& other)
	{
		if(&other == this)
			return *this;
		m_width = other.m_width;
		m_height = other.m_height;
		return *this;
	}
};

struct SPC_SETTING
{
	SPC_SETTING():m_strUnitX(L"X1000"), m_nXLen(12), m_strUnitY(L"‰")
	{
	}
	SPC_SETTING(const SPC_SETTING& rhs)
	{
		m_strUnitX = rhs.m_strUnitX;
		m_nXLen    = rhs.m_nXLen;
		m_strUnitY = rhs.m_strUnitY;
	}
	SPC_SETTING(SPC_SETTING&& rhs)
	{
		m_strUnitX = rhs.m_strUnitX;
		m_nXLen    = rhs.m_nXLen;
		m_strUnitY = rhs.m_strUnitY;
	}
	SPC_SETTING& operator=(const SPC_SETTING& rhs)
	{
		if(&rhs == this)
			return *this;
		m_strUnitX = rhs.m_strUnitX;
		m_nXLen    = rhs.m_nXLen;
		m_strUnitY = rhs.m_strUnitY;
		return *this;
	}
	SPC_SETTING& operator=(SPC_SETTING&& rhs)
	{
		if(&rhs == this)
			return *this;
		m_strUnitX = rhs.m_strUnitX;
		m_nXLen    = rhs.m_nXLen;
		m_strUnitY = rhs.m_strUnitY;
		return *this;
	}
	wstring m_strUnitX;
	int     m_nXLen;
	wstring m_strUnitY;
};

struct IT_PARAM // wrapper the interfaces and their parameter from configuratio file.
{
	IT_PARAM():m_itIndex(L""),m_bEnable(true),m_itName(L""),m_itNickname(L"")
	{
		m_vParam.clear();
	}
	~IT_PARAM()
	{
		m_vParam.clear();
	}
	IT_PARAM(const IT_PARAM& other)
	{
		m_itIndex       = other.m_itIndex;
		m_bEnable       = other.m_bEnable;
		m_itName        = other.m_itName;
		m_itNickname    = other.m_itNickname;
		m_vParam.clear();
		m_vParam        = other.m_vParam;
	}
	IT_PARAM(IT_PARAM&& other)
	{
		m_itIndex       = other.m_itIndex;
		m_bEnable       = other.m_bEnable;
		m_itName        = other.m_itName;
		m_itNickname    = other.m_itNickname;
		m_vParam.clear();
		m_vParam        = other.m_vParam;
	}
	IT_PARAM& operator=(const IT_PARAM& other)
	{
		if(&other == this)
			return *this;
		m_itIndex       = other.m_itIndex;
		m_bEnable       = other.m_bEnable;
		m_itName        = other.m_itName;
		m_itNickname    = other.m_itNickname;
		m_vParam.clear();
		m_vParam        = other.m_vParam;
		return *this;
	}
	IT_PARAM& operator=(IT_PARAM&& other)
	{
		if(&other == this)
			return *this;
		m_itIndex       = other.m_itIndex;
		m_bEnable       = other.m_bEnable;
		m_itName        = other.m_itName;
		m_itNickname    = other.m_itNickname;
		m_vParam.clear();
		m_vParam        = other.m_vParam;
		return *this;
	}
	bool IsTestAlgo()
	{
		if(m_vParam.empty()) return false;

		return (m_vParam[0] != L"N/A");
	}
	std::wstring m_itIndex;
	bool         m_bEnable;
	std::wstring m_itName;          //interface/function name
	std::wstring m_itNickname;      //Nickname will be displayed on Channel Configuration Dialog
	std::vector<wstring> m_vParam;  //parameter of the interface
};

struct INPUT_PARAM // These information will be displayed on screen.
{
	VARENUM m_type;
	long    m_descID;
	float   m_value;
	float   m_min;
	float   m_max;
	float   m_step;
	wstring m_wstrFile;
	int     m_nFileType; // -1:None; 0:Model File; 1:Region File
	INPUT_PARAM()
	{
		m_type     = VT_EMPTY;
		m_descID   = 0;
		m_value    = 0.0f;
		m_min      = 0.0f;
		m_max	   = 0.0f;
		m_step     = 0.0f;
		m_wstrFile = L"";
		m_nFileType = -1;
	}
	INPUT_PARAM(const INPUT_PARAM& other)
	{
		m_type     = other.m_type;
		m_descID   = other.m_descID;
		m_value    = other.m_value;
		m_min      = other.m_min;
		m_max      = other.m_max;
		m_step     = other.m_step;
		m_wstrFile = other.m_wstrFile;
		m_nFileType = other.m_nFileType;
	}
	INPUT_PARAM(INPUT_PARAM&& other)
	{
		m_type     = other.m_type;
		m_descID   = other.m_descID;
		m_value    = other.m_value;
		m_min      = other.m_min;
		m_max      = other.m_max;
		m_step     = other.m_step;
		m_wstrFile = other.m_wstrFile;
		m_nFileType = other.m_nFileType;
	}
	INPUT_PARAM& operator=(const INPUT_PARAM& other)
	{
		if(&other == this)
		{
			return *this;
		}
		m_type     = other.m_type;
		m_descID   = other.m_descID;
		m_value    = other.m_value;
		m_min      = other.m_min;
		m_max      = other.m_max;
		m_step     = other.m_step;
		m_wstrFile = other.m_wstrFile;
		m_nFileType = other.m_nFileType;
		return *this;
	}
	INPUT_PARAM& operator=(INPUT_PARAM&& other)
	{
		if(&other == this)
		{
			return *this;
		}
		m_type     = other.m_type;
		m_descID   = other.m_descID;
		m_value    = other.m_value;
		m_min      = other.m_min;
		m_max      = other.m_max;
		m_step     = other.m_step;
		m_wstrFile = other.m_wstrFile;
		m_nFileType = other.m_nFileType;
		return *this;
	}
	bool IsEqual(const INPUT_PARAM& other)
	{
		return (m_type   == other.m_type) && \
			(m_descID == other.m_descID) && \
			(m_value  == other.m_value) && \
			(m_min    == other.m_min) && \
			(m_max    == other.m_max) && \
			(m_step   == other.m_step) && \
			(m_wstrFile == other.m_wstrFile) && \
			(m_nFileType == other.m_nFileType);

	}
};

enum COMPARE_TYPE
{
	COMP_INVALID = -1,
	GREAT,
	GREAT_EQUAL,
	EQUAL,
	NO_EQUAL,
	LESS,
	LESS_EQUAL,
	IN_RANGE_OPEN,
	IN_RANGE_CLOSE,
	IN_RANGE_LO_RC, // left open and right close -- (12, 34]
	IN_RANGE_LC_RO,
	OUT_RANGE_OPEN,
	OUT_RANGE_CLOSE,
	OUT_RANGE_LO_RC,
	OUT_RANGE_LC_RO
};

struct COMPARATOR
{
	COMPARATOR():m_strComp(L">"),m_compType(GREAT)
	{
	}
	COMPARATOR(wstring strComp, COMPARE_TYPE type):m_strComp(strComp),m_compType(type)
	{
	}

	COMPARATOR(const COMPARATOR& other)
	{
		m_strComp = other.m_strComp;
		m_compType = other.m_compType;
	}
	COMPARATOR(COMPARATOR&& other)
	{
		m_strComp = other.m_strComp;
		m_compType = other.m_compType;
	}
	COMPARATOR& operator=(const COMPARATOR& other)
	{
		if(&other == this)
			return *this;
		m_strComp = other.m_strComp;
		m_compType = other.m_compType;
		return *this;
	}
	COMPARATOR& operator=(COMPARATOR&& other)
	{
		if(&other == this)
			return *this;
		m_strComp = other.m_strComp;
		m_compType = other.m_compType;
		return *this;
	}
	wstring      m_strComp;
	COMPARE_TYPE m_compType;
};

struct THRESHOLD
{
	THRESHOLD()
	{
		m_helpId  = -1;
		m_lcl     = -99999.0f;
		m_ucl     = 99999.0f;
		m_min     = 0.0f;
		m_max     = 99999.0f;
		m_stepLen = 1.0f;
		m_specV   = 0.0f;
		m_type    = VT_EMPTY;
		m_compType = COMP_INVALID;
	}
	THRESHOLD(const THRESHOLD& other)
	{
		m_helpId   = other.m_helpId;
		m_lcl      = other.m_lcl;
		m_specV    = other.m_specV;
		m_ucl      = other.m_ucl;
		m_min      = other.m_min;
		m_max      = other.m_max;
		m_stepLen  = other.m_stepLen;
		m_type     = other.m_type;
		m_compType = other.m_compType;
	}
	THRESHOLD(THRESHOLD&& other)
	{
		m_helpId   = other.m_helpId;
		m_lcl      = other.m_lcl;
		m_specV    = other.m_specV;
		m_ucl      = other.m_ucl;
		m_min      = other.m_min;
		m_max      = other.m_max;
		m_stepLen  = other.m_stepLen;
		m_type     = other.m_type;
		m_compType = other.m_compType;
	}

	THRESHOLD& operator=(const THRESHOLD& other)
	{
		if(&other == this)
			return *this;
		m_helpId   = other.m_helpId;
		m_lcl      = other.m_lcl;
		m_specV    = other.m_specV;
		m_ucl      = other.m_ucl;
		m_min      = other.m_min;
		m_max      = other.m_max;
		m_stepLen  = other.m_stepLen;
		m_type     = other.m_type;
		m_compType = other.m_compType;
		return *this;
	}

	THRESHOLD& operator=(THRESHOLD&& other)
	{
		if(&other == this)
			return *this;
		m_helpId   = other.m_helpId;
		m_lcl      = other.m_lcl;
		m_specV    = other.m_specV;
		m_ucl      = other.m_ucl;
		m_min      = other.m_min;
		m_max      = other.m_max;
		m_stepLen  = other.m_stepLen;
		m_type     = other.m_type;
		m_compType = other.m_compType;
		return *this;
	}
	bool IsEqual(const THRESHOLD& other)
	{
		return (m_helpId == other.m_helpId &&
			m_lcl == other.m_lcl &&
			m_ucl == other.m_ucl &&
			m_min == other.m_min &&
			m_max == other.m_max &&
			m_stepLen == other.m_stepLen &&
			m_specV == other.m_specV &&
			m_type == other.m_type);
	}
	long          m_helpId;
	float		  m_lcl;
	float		  m_ucl;
	float         m_min;
	float         m_max;
	float         m_stepLen;
	float         m_specV;
	VARENUM		  m_type;
	COMPARE_TYPE  m_compType;
};

struct TASK
{
	TASK():m_wstrTaskName(L"")
		,m_strModelName(L"")
		,m_nProductId(-1)
		,m_nTimingUcl(80)
		,m_bImgInside(false)
		,m_bSelfLearning(false)
		,m_nSampleCount(-1)
		,m_wstrSlParam(L"")
		,m_blowThreshold(3)
	{
		m_vItParam.clear();
		m_vecTaskCameraSetting.clear();
		m_setTIN.clear();
	}
	~TASK()
	{
		m_vItParam.clear();
		m_setTIN.clear();
		m_vecTaskCameraSetting.clear();
	}
	TASK(const TASK& other)
	{
		m_wstrTaskName	= other.m_wstrTaskName;
		m_strModelName  = other.m_strModelName;
		m_nProductId	= other.m_nProductId;
		m_nTimingUcl    = other.m_nTimingUcl;
		m_bImgInside    = other.m_bImgInside;
		m_bSelfLearning = other.m_bSelfLearning;
		m_nSampleCount  = other.m_nSampleCount;
		m_wstrSlParam   = other.m_wstrSlParam;
		m_blowThreshold = other.m_blowThreshold;
		m_rjectDelay   = other.m_rjectDelay;

		m_vecTaskCameraSetting.clear();
		m_vecTaskCameraSetting.assign(other.m_vecTaskCameraSetting.begin(), other.m_vecTaskCameraSetting.end());

		m_vItParam.clear();
		m_vItParam.assign(other.m_vItParam.begin(), other.m_vItParam.end());
	}
	TASK(TASK&& other)
	{
		m_wstrTaskName   = other.m_wstrTaskName;
		m_strModelName	= other.m_strModelName;
		m_nProductId	= other.m_nProductId;
		m_nTimingUcl    = other.m_nTimingUcl;
		m_bImgInside	= other.m_bImgInside;
		m_bSelfLearning = other.m_bSelfLearning;
		m_nSampleCount  = other.m_nSampleCount;
		m_wstrSlParam   = other.m_wstrSlParam;
		m_blowThreshold = other.m_blowThreshold;
		m_rjectDelay   = other.m_rjectDelay;

		m_vecTaskCameraSetting.clear();
		m_vecTaskCameraSetting.assign(other.m_vecTaskCameraSetting.begin(), other.m_vecTaskCameraSetting.end());

		m_vItParam.clear();
		m_vItParam.assign(other.m_vItParam.begin(), other.m_vItParam.end());
	}
	TASK& operator=(const TASK& other)
	{
		if(&other == this)
			return *this;
		m_wstrTaskName	= other.m_wstrTaskName;
		m_strModelName	= other.m_strModelName;
		m_nProductId	= other.m_nProductId;
		m_nTimingUcl    = other.m_nTimingUcl;
		m_bImgInside	= other.m_bImgInside;
		m_bSelfLearning = other.m_bSelfLearning;
		m_nSampleCount  = other.m_nSampleCount;
		m_wstrSlParam   = other.m_wstrSlParam;
		m_blowThreshold = other.m_blowThreshold;
		m_rjectDelay   = other.m_rjectDelay;

		m_vecTaskCameraSetting.clear();
		m_vecTaskCameraSetting.assign(other.m_vecTaskCameraSetting.begin(), other.m_vecTaskCameraSetting.end());

		m_vItParam.clear();
		m_vItParam.assign(other.m_vItParam.begin(), other.m_vItParam.end());

		return *this;
	}
	TASK& operator=(TASK&& other)
	{
		if(&other == this)
			return *this;
		m_wstrTaskName	= other.m_wstrTaskName;
		m_strModelName	= other.m_strModelName;
		m_nProductId	= other.m_nProductId;
		m_nTimingUcl    = other.m_nTimingUcl;
		m_bImgInside	= other.m_bImgInside;
		m_bSelfLearning = other.m_bSelfLearning;
		m_nSampleCount  = other.m_nSampleCount;
		m_wstrSlParam   = other.m_wstrSlParam;
		m_blowThreshold = other.m_blowThreshold;
		m_rjectDelay   = other.m_rjectDelay;

		m_vecTaskCameraSetting.clear();
		m_vecTaskCameraSetting.assign(other.m_vecTaskCameraSetting.begin(), other.m_vecTaskCameraSetting.end());

		m_vItParam.clear();
		m_vItParam.assign(other.m_vItParam.begin(), other.m_vItParam.end());

		return *this;
	}

	bool FindOldAlgoItems(std::vector<IT_PARAM>& vItParam, const IT_PARAM& newItemParam) const
	{
		wstring wstrItName   = newItemParam.m_itName;
		wstring wstrNickname = newItemParam.m_itNickname;
		size_t szParam       = newItemParam.m_vParam.size();
		for(size_t t = 0; t < m_vItParam.size(); t++)
		{
			IT_PARAM itParam = m_vItParam[t];
			if(wstrItName   ==itParam.m_itName && 
				wstrNickname == itParam.m_itNickname &&
				szParam      == itParam.m_vParam.size())
			{
				vItParam.push_back(itParam);
			}
		}
		if(vItParam.empty())
		{
			return false;
		}
		return true;
	}

	void AdaptModelFiles(TASK &task ,int StationIndex)
	{
		task  = *this;
		for(size_t t = 0; t < task.m_vItParam.size(); t++)
		{		
			for (size_t i = 0 ; i < task.m_vItParam[t].m_vParam.size(); i++)
			{
				PathAdaptToModelFiles(task.m_vItParam[t].m_vParam[i],StationIndex);
			}
		}
	}
	wstring GetFuncNickname(int nFuncIndex)
	{
		wstring wstrRet(L"");
		if(m_vItParam.empty())
		{
			return wstrRet;
		}
		wstring wstrIndex = boost::lexical_cast<wstring>(nFuncIndex);
		size_t sz = m_vItParam.size();
		for(size_t t = 0; t < sz; t++)
		{
			IT_PARAM item = m_vItParam[t];
			if(wstrIndex == item.m_itIndex)
			{
				wstrRet = item.m_itNickname;
				break;
			}
		}
		return wstrRet;
	}

	void SetFuncNickname(int nFuncIndex, wstring wstrName)
	{
		if(m_vItParam.empty() || wstrName == L"")
		{
			return;
		}
		wstring wstrIndex = boost::lexical_cast<wstring>(nFuncIndex);
		size_t sz = m_vItParam.size();
		for(size_t t = 0; t < sz; t++)
		{
			IT_PARAM item = m_vItParam[t];
			if(wstrIndex == item.m_itIndex)
			{
				m_vItParam[t].m_itNickname = wstrName;
				break;
			}
		}
	}

	bool GetFuncIndexAndRgnID(wstring wstrNickname, int& nFuncIndex, int& nRgnID)
	{
		if(m_vItParam.empty())
		{
			return false;
		}
		wstring tmpName = wstrNickname.substr(0, wstrNickname.size() - 1);
		for(size_t t = 0; t < m_vItParam.size(); t++)
		{
			IT_PARAM itParam = m_vItParam[t];
			if(!itParam.m_vParam.empty() && itParam.m_vParam[0] != L"N/A")
			{
				if(tmpName == itParam.m_itNickname)
				{
					nFuncIndex = (int)t;
					nRgnID     = boost::lexical_cast<int>(itParam.m_vParam[1]);
					return true;
				}
			}
		}

		return false;
	}

	int GetTestAlgoCountByRgnID(int nCrtRgnID) const
	{
		int nRet = 0;
		for(size_t t = 0; t < m_vItParam.size(); t++)
		{
			IT_PARAM itParam = m_vItParam[t];
			if(!itParam.m_vParam.empty() && itParam.m_vParam[0] != L"N/A")
			{
				int nRgnID  = boost::lexical_cast<int>(itParam.m_vParam[1]);
				if(nRgnID == nCrtRgnID)
				{
					nRet++;
				}
			}
		}

		return nRet;
	}

	std::set<wstring> GetItemNicknames()
	{
		m_setTIN.clear();
		std::for_each(m_vItParam.begin(), m_vItParam.end(), [&](IT_PARAM item)
		{
			// 			if(item.IsTestAlgo())
			// 			{
			m_setTIN.insert(item.m_itNickname);
			/*			}*/
		});

		return m_setTIN;
	}

	int GetTestFuncCount()
	{
		int nCount = 0;
		std::for_each(m_vItParam.begin(),m_vItParam.end(), [&](IT_PARAM& ip)
		{
			if(ip.IsTestAlgo())
			{
				nCount++;
			}
		});
		return nCount;
	}
	int GetItemCount()
	{
		return (int)m_vItParam.size();
	}
	bool UpdateParameter(int nFuncIndex, std::vector<INPUT_PARAM>& vInputParam)
	{
		wstring wstrIndex = boost::lexical_cast<wstring>(nFuncIndex);
		for(size_t t = 0; t < m_vItParam.size(); t++)
		{
			IT_PARAM& itParam = m_vItParam[t];
			if(itParam.m_itIndex == wstrIndex)
			{
				size_t sz = vInputParam.size();
				if(sz + 1 == itParam.m_vParam.size())
				{
					wstring wstrRule = itParam.m_vParam[0];
					itParam.m_vParam.clear();
					itParam.m_vParam.push_back(wstrRule);
					for(size_t i = 0; i < sz; i++)
					{
						INPUT_PARAM param = vInputParam[i];
						CString strV;
						if(param.m_type == VT_R4)
						{
							strV.Format(_T("%.3f"), param.m_value);
						}
						if(param.m_type == VT_I4 || param.m_type == VT_UI1)
						{
							strV.Format(_T("%d"), (long)param.m_value);
						}

						if(param.m_type == VT_BSTR)
						{
							strV = param.m_wstrFile.c_str();
							wstring wstrfile= param.m_wstrFile;
							if (!strV.IsEmpty())
							{
								int i = wstrfile.find(_T("#"));
								int j = wstrfile.find_last_of(_T("#"));
								if (i != string::npos && j != string::npos)
								{
									wstrfile.replace(j,2,L"");
									wstrfile.replace(i,2,L"");
								}
							}
							strV= wstrfile.c_str();

							if(strV == _T(""))
							{
								strV = _T(" ");
							}
						}

						wstring wstrValue = CT2W(strV);
						itParam.m_vParam.push_back(wstrValue);
					}
					return true;
				}
			}
		}
		return false;
	}
	void EnableTestItem(int nFuncIndex, bool bEnable, bool bTestFunc)
	{
		int sz = (int)m_vItParam.size();
		if(nFuncIndex < sz)
		{
			m_vItParam[nFuncIndex].m_bEnable = bEnable;
			if(!bTestFunc)
			{
				for(int i = nFuncIndex + 1; i < sz; i++)
				{
					if(m_vItParam[i].IsTestAlgo())
					{
						m_vItParam[i].m_bEnable = bEnable;
					}
					else
					{
						return;
					}
				}
			}
			else 
			{
				if(bEnable)// Make sure that the ROI function is enabled.
				{
					for(int i = nFuncIndex; i > 0; i--)
					{
						if(!m_vItParam[i].IsTestAlgo())
						{
							m_vItParam[i].m_bEnable = bEnable;
							return;
						}
					}
				}
			}
		}
	}
	bool UpdateThresholdValue(int nFuncIndex, const THRESHOLD& threshold)
	{
		bool bRet = false;
		CString strValue;
		VARENUM type = threshold.m_type;
		if(threshold.m_compType <= LESS_EQUAL)
		{
			if(type == VT_R4)
				strValue.Format(_T("%.2f"),threshold.m_specV);
			else
				strValue.Format(_T("%d"), (int)threshold.m_specV);
		}
		else
		{
			if(type == VT_R4)
				strValue.Format(_T("%.2f~%.2f"),threshold.m_lcl, threshold.m_ucl);
			else
				strValue.Format(_T("%d~%d"), (int)threshold.m_lcl, (int)threshold.m_ucl);
		}
		wstring wstrSpec = CT2W(strValue);
		wstring wstrIndex = boost::lexical_cast<wstring>(nFuncIndex);
		std::for_each(m_vItParam.begin(), m_vItParam.end(), [&](IT_PARAM& item)
		{
			if(item.m_itIndex == wstrIndex)
			{
				std::vector<wstring>& vParam = item.m_vParam;
				wstring wstrTh = vParam[0];
				auto it = wstrTh.find(L"$");
				wstrTh.replace(wstrTh.begin() + it + 1, wstrTh.end(),wstrSpec);
				vParam[0] = wstrTh;
				bRet = true;
			}
		});
		return bRet;
	}
	std::wstring                     m_wstrTaskName;    // Group Name
	std::wstring                     m_strModelName;   // Task Model
	int                              m_nProductId;     // Default value is -1;
	int                              m_nTimingUcl;     // Timing UCL of this task.
	bool                             m_bImgInside;
	volatile bool                    m_bSelfLearning;  // Indicate this is self-learning algorithm group
	long                             m_nSampleCount;
	std::wstring                     m_wstrSlParam;    // Parameter used by self-learning procedure.
	std::vector<IT_PARAM>            m_vItParam;       // TaskGroup Group
	std::set<wstring>                m_setTIN; // Test Item Nickname;
	int								 m_blowThreshold;    //blowThreshold;
	int		   						 m_rjectDelay;
	std::vector<CAMERA_SETTING>      m_vecTaskCameraSetting;   //
};

struct TASK_MODEL
{
	TASK_MODEL():m_strModelName(L""),m_nProductId(-1)
	{
		m_vItParam.clear();
	}

	TASK_MODEL(const TASK_MODEL& other)
	{
		m_strModelName  = other.m_strModelName;
		m_nProductId    = other.m_nProductId;
		m_vItParam.clear();
		m_vItParam.assign(other.m_vItParam.begin(), other.m_vItParam.end());
	}
	TASK_MODEL(TASK_MODEL&& other)
	{
		m_strModelName  = other.m_strModelName;
		m_nProductId    = other.m_nProductId;
		m_vItParam.clear();
		m_vItParam.assign(other.m_vItParam.begin(), other.m_vItParam.end());
	}
	TASK_MODEL& operator=(const TASK_MODEL& other)
	{
		if(&other == this)
			return *this;
		m_strModelName  = other.m_strModelName;
		m_nProductId    = other.m_nProductId;
		m_vItParam.clear();
		m_vItParam.assign(other.m_vItParam.begin(), other.m_vItParam.end());
		return *this;
	}

	TASK_MODEL& operator=(TASK_MODEL&& other)
	{
		if(&other == this)
			return *this;
		m_strModelName = other.m_strModelName;
		m_nProductId   = other.m_nProductId;
		m_vItParam.clear();
		m_vItParam.assign(other.m_vItParam.begin(), other.m_vItParam.end());
		return *this;
	}

	bool NeedLoadSelfLearningResult(int& retIndex)
	{
		//retIndex = -1;
		//for(int i = 0; i < (int)m_vItParam.size(); i++)
		//{
		//	IT_PARAM it = m_vItParam[i];
		//	if(it.m_itName == L"LoadSelfLearningResult")
		//	{
		//		retIndex = i;
		//		return true;
		//	}
		//}
		return false;
	}

	void ReviseSelfLearningParameter(const int index, const wstring& strParam)
	{
		int sz = (int)m_vItParam.size();
		if(index < 0 || index > sz - 1)
			return;

		m_vItParam[index].m_vParam[0] = strParam;
	}
	std::wstring                     m_strModelName;       // Task Model Name
	int                              m_nProductId;      // Default value is -1;
	std::vector<IT_PARAM>            m_vItParam;        // TaskGroup Items
};

struct CHANNEL_CONFIG 
{
	CHANNEL_CONFIG():m_crtTaskName(_T("")),m_chIndex(-1),m_wstrLaneName(L""),m_nMaxSpeed(40),m_bSplicing(false),
		m_bModelfiles(false),m_chWarningflag(_T("")),m_bSingleProcessor(true),m_RejectMinTime(0),
		m_mainchannel(0),m_subchannel(0)
	{ 
		m_SaveImageType=0;
		m_vTask.clear();
	}
	CHANNEL_CONFIG(const CHANNEL_CONFIG& other)
	{
		m_chIndex              = other.m_chIndex;
		m_chWarningflag		   = other.m_chWarningflag;
		m_RejectMinTime        = other.m_RejectMinTime;
		m_wstrLaneName		   = other.m_wstrLaneName;
		m_nMaxSpeed            = other.m_nMaxSpeed;
		m_cameraSetting        = other.m_cameraSetting;
		m_vecCameraSetting     = other.m_vecCameraSetting;
		m_lightCtrlSetting     = other.m_lightCtrlSetting;
		m_rejectorSetting      = other.m_rejectorSetting;
		m_uiSetting            = other.m_uiSetting;
		m_spcSetting           = other.m_spcSetting;
		m_crtTaskName          = other.m_crtTaskName;
		m_vTask.clear();
		m_vTask                = other.m_vTask;
		m_bSplicing            = other.m_bSplicing;
		m_bModelfiles          = other.m_bModelfiles;
		m_bSingleProcessor     = other.m_bSingleProcessor;
		m_SaveImageType		   = other.m_SaveImageType;
		m_mainchannel = other.m_mainchannel;
		m_subchannel = other.m_subchannel;

	}
	CHANNEL_CONFIG(CHANNEL_CONFIG&& other)
	{
		m_chIndex              = other.m_chIndex;
		m_chWarningflag		   = other.m_chWarningflag;
				m_RejectMinTime        = other.m_RejectMinTime;
		m_wstrLaneName         = other.m_wstrLaneName;
		m_nMaxSpeed            = other.m_nMaxSpeed;
		m_cameraSetting        = other.m_cameraSetting;
		m_vecCameraSetting     = other.m_vecCameraSetting;
		m_lightCtrlSetting     = other.m_lightCtrlSetting;
		m_rejectorSetting      = other.m_rejectorSetting;
		m_uiSetting            = other.m_uiSetting;
		m_spcSetting           = other.m_spcSetting;
		m_crtTaskName          = other.m_crtTaskName;
		m_vTask.clear();
		m_vTask                = other.m_vTask;
		m_bSplicing            = other.m_bSplicing;
		m_bModelfiles          = other.m_bModelfiles;
		m_bSingleProcessor     = other.m_bSingleProcessor;
		m_SaveImageType		   = other.m_SaveImageType;
		m_mainchannel = other.m_mainchannel;
		m_subchannel = other.m_subchannel;
	}
	CHANNEL_CONFIG& operator=(const CHANNEL_CONFIG& other)
	{
		if(&other == this)
			return *this;
		m_chIndex              = other.m_chIndex;
		m_chWarningflag		   = other.m_chWarningflag;
				m_RejectMinTime        = other.m_RejectMinTime;
		m_wstrLaneName         = other.m_wstrLaneName;
		m_nMaxSpeed            = other.m_nMaxSpeed;
		m_cameraSetting        = other.m_cameraSetting;
		m_vecCameraSetting     = other.m_vecCameraSetting;
		m_lightCtrlSetting     = other.m_lightCtrlSetting;
		m_rejectorSetting      = other.m_rejectorSetting;
		m_uiSetting            = other.m_uiSetting;
		m_spcSetting           = other.m_spcSetting;
		m_crtTaskName   = other.m_crtTaskName;
		m_vTask.clear();
		m_vTask                = other.m_vTask;
		m_bSplicing            = other.m_bSplicing;
		m_bModelfiles          = other.m_bModelfiles;
		m_bSingleProcessor     = other.m_bSingleProcessor;
		m_SaveImageType		   = other.m_SaveImageType;
		m_mainchannel = other.m_mainchannel;
		m_subchannel = other.m_subchannel;
		return *this;
	}
	CHANNEL_CONFIG& operator=(CHANNEL_CONFIG&& other)
	{
		if(&other == this)
			return *this;
		m_chIndex              = other.m_chIndex;
		m_chWarningflag		   = other.m_chWarningflag;
				m_RejectMinTime        = other.m_RejectMinTime;
		m_wstrLaneName		   = other.m_wstrLaneName;
		m_nMaxSpeed            = other.m_nMaxSpeed;
		m_cameraSetting        = other.m_cameraSetting;
		m_vecCameraSetting     = other.m_vecCameraSetting;
		m_lightCtrlSetting     = other.m_lightCtrlSetting;
		m_rejectorSetting      = other.m_rejectorSetting;
		m_uiSetting            = other.m_uiSetting;
		m_spcSetting           = other.m_spcSetting;
		m_crtTaskName   = other.m_crtTaskName;
		m_vTask.clear();
		m_vTask                = other.m_vTask;
		m_bSplicing            = other.m_bSplicing;
		m_bModelfiles          = other.m_bModelfiles;
		m_bSingleProcessor     = other.m_bSingleProcessor;
		m_SaveImageType		   = other.m_SaveImageType;
		m_mainchannel = other.m_mainchannel;
		m_subchannel = other.m_subchannel;
		return *this;
	}

	bool GetCurrentTask(TASK& ret)
	{
		if(m_crtTaskName == L"")
			return false;
		for(size_t t = 0; t < m_vTask.size(); ++t)
		{
			if(m_vTask[t].m_wstrTaskName == m_crtTaskName)
			{
				ret = m_vTask[t];
				return true;
			}
		}
		return false;
	}


	TASK* GetCurrentTask(int index = -1)
	{
		if(m_crtTaskName == L"")
			return nullptr;
		for(size_t t = 0; t < m_vTask.size(); ++t)
		{
			if(m_vTask[t].m_wstrTaskName == m_crtTaskName)
			{
				return &m_vTask[t];
			}
		}
		return nullptr;
	}

	TASK GetTask(wstring strName)
	{
		TASK ret;
		if(strName == L"")
			return ret;
		for(size_t t = 0; t < m_vTask.size(); ++t)
		{
			if(m_vTask[t].m_wstrTaskName == strName)
			{
				return m_vTask[t];
			}
		}
		return ret;
	}

	int						m_chIndex;
	std::wstring			m_chWarningflag;
	int						m_RejectMinTime;
	std::wstring            m_wstrLaneName;
	int                     m_nMaxSpeed; // Hz
	CAMERA_SETTING			m_cameraSetting;
	vector<CAMERA_SETTING>  m_vecCameraSetting;
	LIGHT_SETTING		    m_lightCtrlSetting;
	REJECTOR_SETTING		m_rejectorSetting;
	UI_SETTING              m_uiSetting;
	SPC_SETTING             m_spcSetting;
	std::wstring		    m_crtTaskName;   // Current TaskGroup Name
	std::vector<TASK>       m_vTask;         // Candidate TaskGroup Group
	bool					m_bSplicing;
	bool					m_bModelfiles;
	bool                    m_bSingleProcessor; // Multiple camera share one image processor.
	int					m_SaveImageType; //save image type,1 NG,2 OK,other ignore.
	int					m_mainchannel;
	int					m_subchannel;
};

struct FUNC_INFO // used for passing parameters of an algorithm into COM
{
	FUNC_INFO()
	{
		m_dispId            = -1;
		m_funcIndex         = -1;
		m_strFuncName       = L"";
		m_strCustomizedName = L"";
		m_strOutputName     = L"";
		m_strHelp           = L"";
		m_funcDescId        = -1;
		m_nTotalCount       = 0;
		m_nErrCount         = 0;
		m_bTestFunc         = false;
		m_bPass             = true;
		m_bEnable           = true;
		m_threshold.m_specV = 0.0f;
	}
	~FUNC_INFO()
	{
	}
	FUNC_INFO(const FUNC_INFO& other)
	{
		m_dispId              = other.m_dispId;
		m_funcIndex			  = other.m_funcIndex;
		m_strFuncName         = other.m_strFuncName;
		m_strCustomizedName   = other.m_strCustomizedName;
		m_strOutputName       = other.m_strOutputName;
		m_strHelp             = other.m_strHelp;
		m_funcDescId          = other.m_funcDescId;
		m_nTotalCount         = other.m_nTotalCount;
		m_nErrCount           = other.m_nErrCount;
		m_bTestFunc           = other.m_bTestFunc;
		m_bPass               = other.m_bPass;
		m_bEnable             = other.m_bEnable;
		m_threshold           = other.m_threshold;
		m_VecRecentErrCount	  = other.m_VecRecentErrCount;
		m_vParam.clear();
		m_vParam.assign(other.m_vParam.begin(), other.m_vParam.end());
		CopyComParamArray(other.m_vComParam);
	}
	FUNC_INFO(FUNC_INFO&& other)
	{
		m_dispId              = other.m_dispId;
		m_funcIndex			  = other.m_funcIndex;
		m_strFuncName         = other.m_strFuncName;
		m_strCustomizedName   = other.m_strCustomizedName;
		m_strOutputName       = other.m_strOutputName;
		m_strHelp             = other.m_strHelp;
		m_funcDescId          = other.m_funcDescId;
		m_nTotalCount         = other.m_nTotalCount;
		m_nErrCount           = other.m_nErrCount;
		m_bTestFunc           = other.m_bTestFunc;
		m_bPass               = other.m_bPass;
		m_bEnable             = other.m_bEnable;
		m_threshold           = other.m_threshold;
		m_VecRecentErrCount	  = other.m_VecRecentErrCount;
		m_vParam.clear();
		m_vParam.assign(other.m_vParam.begin(), other.m_vParam.end());
		CopyComParamArray(other.m_vComParam);
	}
	FUNC_INFO& operator=(const FUNC_INFO& other)
	{
		if(&other == this)
			return *this;
		m_dispId              = other.m_dispId;
		m_funcIndex			  = other.m_funcIndex;
		m_strFuncName         = other.m_strFuncName;
		m_strCustomizedName   = other.m_strCustomizedName;
		m_strOutputName       = other.m_strOutputName;
		m_strHelp             = other.m_strHelp;
		m_funcDescId          = other.m_funcDescId;
		m_nTotalCount         = other.m_nTotalCount;
		m_nErrCount           = other.m_nErrCount;
		m_bTestFunc           = other.m_bTestFunc;
		m_bPass               = other.m_bPass;
		m_bEnable             = other.m_bEnable;
		m_threshold           = other.m_threshold;
		m_VecRecentErrCount	  = other.m_VecRecentErrCount;
		m_vParam.clear();
		m_vParam.assign(other.m_vParam.begin(), other.m_vParam.end());
		CopyComParamArray(other.m_vComParam);
		return *this;
	}
	FUNC_INFO& operator=(FUNC_INFO&& other)
	{
		if(&other == this)
			return *this;
		m_dispId              = other.m_dispId;
		m_funcIndex			  = other.m_funcIndex;
		m_strFuncName         = other.m_strFuncName;
		m_strCustomizedName   = other.m_strCustomizedName;
		m_strOutputName       = other.m_strOutputName;
		m_strHelp             = other.m_strHelp;
		m_funcDescId          = other.m_funcDescId;
		m_nTotalCount         = other.m_nTotalCount;
		m_nErrCount           = other.m_nErrCount;
		m_bTestFunc           = other.m_bTestFunc;
		m_bPass               = other.m_bPass;
		m_bEnable             = other.m_bEnable;
		m_threshold           = other.m_threshold;
		m_VecRecentErrCount	  = other.m_VecRecentErrCount;
		m_vParam.clear();
		m_vParam.assign(other.m_vParam.begin(), other.m_vParam.end());
		CopyComParamArray(other.m_vComParam);
		return *this;
	}
	void CopyComParamArray(const std::vector<ATL::CComVariant>& vComParam)
	{
		size_t sz = vComParam.size();
		if(sz <= 0)
			return;
		m_vComParam.clear();
		m_vComParam = std::vector<ATL::CComVariant>(sz);
		for(size_t t = 0; t < sz; t++)
		{
			m_vComParam[t].Copy(&vComParam[t]);
		}
	}
	void Reset()
	{
		m_dispId            = -1;
		m_funcIndex         = -1;
		m_strFuncName       = L"";
		m_strCustomizedName = L"";
		m_strOutputName     = L"";
		m_strHelp           = L"";
		m_funcDescId        = -1;
		m_nTotalCount       = 0;
		m_nErrCount         = 0;
		m_bTestFunc         = false;
		m_bPass             = true;
		m_threshold.m_specV = 0.0f;
		m_vParam.clear();
		m_vComParam.clear();

		while(!m_VecRecentErrCount.empty())
		{
			m_VecRecentErrCount.pop();
		}
	}
	int GetTargetRegionID()
	{
		if(m_vParam.empty())
		{
			return -1;
		}
		else
		{
			INPUT_PARAM rgnParam = m_vParam[0];
			return (int)rgnParam.m_value;
		}
		return -1;
	}
	bool CreateComParamArray()
	{
		size_t sz = m_vParam.size();
		if(sz <= 0)
			return false;

		m_vComParam.clear();
		for(size_t t = sz; t > 0; t--)
		{
			if(m_vParam[t-1].m_type != VT_BSTR)
			{
				ATL::CComVariant var(m_vParam[t-1].m_value);
				m_vComParam.push_back(var);
			}
			if(m_vParam[t-1].m_type == VT_BSTR)
			{
				ATL::CComVariant var(m_vParam[t-1].m_wstrFile.c_str());
				m_vComParam.push_back(var);
			}
		}
		return true;
	}
	bool IsEqual(const FUNC_INFO& other)
	{
		for(size_t t = 0; t < m_vParam.size(); t++)
		{
			if(!m_vParam[t].IsEqual(other.m_vParam[t]))
			{
				return false;
			}
		}
		if(!m_threshold.IsEqual(other.m_threshold))
		{
			return false;
		}
		if(m_dispId != other.m_dispId ||
			m_funcIndex != other.m_funcIndex ||
			m_strFuncName != other.m_strFuncName ||
			m_strCustomizedName != other.m_strCustomizedName ||
			m_strOutputName != other.m_strOutputName ||
			m_strHelp != other.m_strHelp ||
			m_funcDescId != other.m_funcDescId ||
			m_bTestFunc != other.m_bTestFunc ||
			m_bEnable   != other.m_bEnable)
		{
			return false;
		}
		return true;
	}

	DISPID                        m_dispId;
	int                           m_funcIndex;
	std::wstring                  m_strFuncName;
	std::wstring                  m_strCustomizedName;   // Get the name from the TaskGroup Help String.
	std::wstring                  m_strOutputName;
	std::wstring                  m_strHelp;             // Function description
	long                          m_funcDescId;
	long                          m_nTotalCount;
	long                          m_nErrCount;
	bool                          m_bTestFunc;         // Y: Normal Function with a return value; N: Location Function without return value;
	bool                          m_bPass;             // Current Testing result;
	bool                          m_bEnable;
	THRESHOLD                     m_threshold;
	std::vector<INPUT_PARAM>      m_vParam;              // all input parametetrs for an interface method;used by screens 
	std::vector<ATL::CComVariant> m_vComParam;           // used to call COM functions
	std::queue<int>				  m_VecRecentErrCount;
};

struct FI // Function ID and Function Name
{
	FI():m_nIndex(-1),m_funcId(-1),m_nRgnId(-1),m_strFuncName(L""),m_strNickname(L""),m_strNodeName(L""),m_bTestFunc(true)
	{
		m_vParam.clear();
	}
	FI(int nIndex,long id, int nRgnId, wstring funcName, wstring nickname, bool bTestFunc = true)
		:m_nIndex(nIndex),
		m_funcId(id),
		m_nRgnId(nRgnId),
		m_strFuncName(funcName),
		m_strNickname(nickname),
		m_bTestFunc(bTestFunc)
	{
		m_strNodeName = L"";
		m_vParam.clear();
	}
	FI(const FI& other)
	{
		m_nIndex      = other.m_nIndex;
		m_funcId      = other.m_funcId;
		m_nRgnId      = other.m_nRgnId;
		m_strFuncName = other.m_strFuncName;
		m_strNickname = other.m_strNickname;
		m_strNodeName = other.m_strNodeName;
		m_bTestFunc = other.m_bTestFunc;
		m_vParam.clear();
		m_vParam = other.m_vParam;
	}
	FI(FI&& other)
	{
		m_nIndex      = other.m_nIndex;
		m_funcId      = other.m_funcId;
		m_nRgnId      = other.m_nRgnId;
		m_strFuncName = other.m_strFuncName;
		m_strNickname = other.m_strNickname;
		m_strNodeName = other.m_strNodeName;
		m_bTestFunc = other.m_bTestFunc;
		m_vParam.clear();
		m_vParam = other.m_vParam;
	}
	FI& operator=(const FI& other)
	{
		if(&other == this)
			return *this;
		m_nIndex      = other.m_nIndex;
		m_funcId      = other.m_funcId;
		m_nRgnId      = other.m_nRgnId;
		m_strFuncName = other.m_strFuncName;
		m_strNickname = other.m_strNickname;
		m_strNodeName = other.m_strNodeName;
		m_bTestFunc = other.m_bTestFunc;
		m_vParam.clear();
		m_vParam = other.m_vParam;
		return *this;
	}
	FI& operator=(FI&& other)
	{
		if(&other == this)
			return *this;
		m_nIndex      = other.m_nIndex;
		m_funcId      = other.m_funcId;
		m_nRgnId      = other.m_nRgnId;
		m_strFuncName = other.m_strFuncName;
		m_strNickname = other.m_strNickname;
		m_strNodeName = other.m_strNodeName;
		m_bTestFunc = other.m_bTestFunc;
		m_vParam.clear();
		m_vParam = other.m_vParam;
		return *this;
	}
	bool IsEqual(const FI& other)
	{
		bool bRet = (m_nIndex  == other.m_nIndex &&
			    m_funcId == other.m_funcId &&
				m_nRgnId      == other.m_nRgnId &&
				m_strFuncName == other.m_strFuncName &&
				m_strNickname == other.m_strNickname &&
				m_bTestFunc == other.m_bTestFunc &&
			m_vParam.size() == other.m_vParam.size());
		if(bRet == false)
		{
			return false;
		}
		for(size_t t = 0; t < m_vParam.size(); t++)
		{
			if(m_vParam[t] != other.m_vParam[t])
			{
				return false;
			}
		}
		return true;
	}
	int             m_nIndex;
	long            m_funcId;
	int             m_nRgnId;  // ID of the region which this function will test
	wstring         m_strFuncName;
	wstring         m_strNickname;
	wstring         m_strNodeName;
	bool            m_bTestFunc; // True if this function is uused for testing, false if this function is used for locating ROI
	vector<wstring> m_vParam;
};

struct DisplayResult
{
	DisplayResult()
	{
		m_IndexTime    = 0;
		m_Alltime      = 0;
		m_bOverTime    = false;
		m_bScoreValid  = false;
		m_Socre        = -1;
		m_bScoreError  = false;
		m_strOutputName = "";
        m_nGII          = -1;
	}

	DisplayResult(const DisplayResult& rhs)
	{
		m_IndexTime     = rhs.m_IndexTime;
		m_Alltime       = rhs.m_Alltime;
		m_bOverTime     = rhs.m_bOverTime;
		m_Socre         = rhs.m_Socre;
	    m_bScoreError   = rhs.m_bScoreError;
		m_strOutputName = rhs.m_strOutputName;
		m_bScoreValid   = rhs.m_bScoreValid;
        m_nGII          = rhs.m_nGII;

		m_vInnerStatsData.clear();
		m_vInnerStatsData.assign(rhs.m_vInnerStatsData.begin(), rhs.m_vInnerStatsData.end());
	}
	DisplayResult(DisplayResult&& rhs)
	{
        m_IndexTime     = rhs.m_IndexTime;
        m_Alltime       = rhs.m_Alltime;
        m_bOverTime     = rhs.m_bOverTime;
        m_Socre         = rhs.m_Socre;
        m_bScoreError   = rhs.m_bScoreError;
        m_strOutputName = rhs.m_strOutputName;
        m_bScoreValid   = rhs.m_bScoreValid;
        m_nGII          = rhs.m_nGII;

		m_vInnerStatsData.clear();
		m_vInnerStatsData.assign(rhs.m_vInnerStatsData.begin(), rhs.m_vInnerStatsData.end());
	}
	DisplayResult& operator=(const DisplayResult& rhs)
	{
		if(&rhs == this)
			return *this;
        m_IndexTime     = rhs.m_IndexTime;
        m_Alltime       = rhs.m_Alltime;
        m_bOverTime     = rhs.m_bOverTime;
        m_Socre         = rhs.m_Socre;
        m_bScoreError   = rhs.m_bScoreError;
        m_strOutputName = rhs.m_strOutputName;
        m_bScoreValid   = rhs.m_bScoreValid;
        m_nGII          = rhs.m_nGII;

		m_vInnerStatsData.clear();
		m_vInnerStatsData.assign(rhs.m_vInnerStatsData.begin(), rhs.m_vInnerStatsData.end());
		return *this;
	}
	DisplayResult& operator=(DisplayResult&& rhs)
	{
		if(&rhs == this)
			return *this;
        m_IndexTime     = rhs.m_IndexTime;
        m_Alltime       = rhs.m_Alltime;
        m_bOverTime     = rhs.m_bOverTime;
        m_Socre         = rhs.m_Socre;
        m_bScoreError   = rhs.m_bScoreError;
        m_strOutputName = rhs.m_strOutputName;
        m_bScoreValid   = rhs.m_bScoreValid;
        m_nGII          = rhs.m_nGII;

		m_vInnerStatsData.clear();
		m_vInnerStatsData.assign(rhs.m_vInnerStatsData.begin(), rhs.m_vInnerStatsData.end());
		return *this;
	}

	double  m_IndexTime;//
	double  m_Alltime;
	bool    m_bOverTime;
	HTuple  m_Socre;
	bool    m_bScoreError;
	string  m_strOutputName;
	bool    m_bScoreValid;
    int     m_nGII; // Glue Injector Index. Just for EOE product

	std::vector<float>	m_vInnerStatsData;
};

struct IMG_PRODUCT
{
	IMG_PRODUCT()
	{
		m_bDebug       = false;
		m_bSingleDebug = false;
		m_bEmptyImg    = false;
		m_ImageTimeCount = 0;
		m_ImageTimeCount_PC=0;
		m_ImageID = 1;
		m_iProcessID = 0;
		m_nGII=-1;
	}
	IMG_PRODUCT(bool bDebug, Hobject& img, Hobject& targerRgn,std::map<Hlong,Hobject>& errRgn
		,Hobject& cvInterImgs,Hobject& CorrectImg,Hobject& TargerRgnEx,long long ImageTimeCount,
		long long ImageTimeCount_PC,long long ImageID,int iProcessID)
	{
		m_bDebug       = bDebug;
		m_bSingleDebug = m_bSingleDebug;
		m_bEmptyImg    = m_bEmptyImg;
		m_img          = img;
		m_crtTargetRgn = targerRgn;
		m_crtCorrectImg = CorrectImg;
		m_cvInterImgs = cvInterImgs;
		m_unionErrRgn  = errRgn;
		m_crtTargetRgnEx = TargerRgnEx;
		m_ImageTimeCount = ImageTimeCount;
		m_ImageTimeCount_PC = ImageTimeCount_PC;
		m_ImageID = ImageID;
		m_iProcessID =iProcessID;
		m_nGII=-1;
		m_CollectFunInfoCount = 0;
		m_AlgoTimeCount = 0;
		m_CalTimeCount=0;
	}
	IMG_PRODUCT(const IMG_PRODUCT& rhs)
	{
		m_bDebug       = rhs.m_bDebug;
		m_bSingleDebug = rhs.m_bSingleDebug;
		m_bEmptyImg    = rhs. m_bEmptyImg;
		m_img          = rhs.m_img;
		m_crtTargetRgn = rhs.m_crtTargetRgn;
		m_cvInterImgs = rhs.m_cvInterImgs;
		m_crtTargetRgnEx = rhs.m_crtTargetRgnEx;
		m_crtCorrectImg = rhs.m_crtCorrectImg;
		m_unionErrRgn  = rhs.m_unionErrRgn;
		m_ImageTimeCount = rhs.m_ImageTimeCount;
		m_ImageTimeCount_PC = rhs.m_ImageTimeCount_PC;
		m_ImageID = rhs.m_ImageID;
		m_iProcessID =rhs.m_iProcessID;
		m_nGII=rhs.m_nGII;
		m_CollectFunInfoCount = rhs.m_CollectFunInfoCount;
		m_AlgoTimeCount = rhs.m_AlgoTimeCount;
		m_CalTimeCount = rhs.m_CalTimeCount;
	}
	IMG_PRODUCT(IMG_PRODUCT&& rhs)
	{
		m_bDebug       = rhs.m_bDebug;
		m_bSingleDebug = rhs.m_bSingleDebug;
		m_bEmptyImg    = rhs. m_bEmptyImg;
		m_img          = rhs.m_img;
		m_crtTargetRgn = rhs.m_crtTargetRgn;
		m_cvInterImgs = rhs.m_cvInterImgs;
		m_crtTargetRgnEx = rhs.m_crtTargetRgnEx;
		m_crtCorrectImg = rhs.m_crtCorrectImg;
		m_unionErrRgn  = rhs.m_unionErrRgn;
		m_ImageTimeCount = rhs.m_ImageTimeCount;
		m_ImageTimeCount_PC = rhs.m_ImageTimeCount_PC;
		m_ImageID = rhs.m_ImageID;
		m_iProcessID =rhs.m_iProcessID;
		m_nGII=rhs.m_nGII;
		m_CollectFunInfoCount = rhs.m_CollectFunInfoCount;
		m_AlgoTimeCount = rhs.m_AlgoTimeCount;
		m_CalTimeCount = rhs.m_CalTimeCount;
	}
	IMG_PRODUCT& operator=(const IMG_PRODUCT& rhs)
	{
		if(&rhs == this)
			return *this;
		m_bDebug       = rhs.m_bDebug;
		m_bSingleDebug = rhs.m_bSingleDebug;
		m_bEmptyImg    = rhs. m_bEmptyImg;
		m_img          = rhs.m_img;
		m_crtTargetRgn = rhs.m_crtTargetRgn;
		m_cvInterImgs = rhs.m_cvInterImgs;
		m_crtTargetRgnEx = rhs.m_crtTargetRgnEx;
		m_crtCorrectImg = rhs.m_crtCorrectImg;
		m_unionErrRgn  = rhs.m_unionErrRgn;
		m_iProcessID =rhs.m_iProcessID;

		m_ImageTimeCount = rhs.m_ImageTimeCount;
		m_ImageTimeCount_PC = rhs.m_ImageTimeCount_PC;
		m_ImageID = rhs.m_ImageID;
		m_nGII=rhs.m_nGII;
		m_CollectFunInfoCount = rhs.m_CollectFunInfoCount;
		m_AlgoTimeCount = rhs.m_AlgoTimeCount;
		m_CalTimeCount = rhs.m_CalTimeCount;
		return *this;
	}
	IMG_PRODUCT& operator=(IMG_PRODUCT&& rhs)
	{
		if(&rhs == this)
			return *this;
		m_bDebug       = rhs.m_bDebug;
		m_bSingleDebug = rhs.m_bSingleDebug;
		m_bEmptyImg    = rhs. m_bEmptyImg;
		m_img          = rhs.m_img;
		m_crtTargetRgn = rhs.m_crtTargetRgn;
		m_unionErrRgn  = rhs.m_unionErrRgn;
		m_cvInterImgs = rhs.m_cvInterImgs;
		m_crtCorrectImg = rhs.m_crtCorrectImg;
		m_crtTargetRgnEx = rhs.m_crtTargetRgnEx;
		m_iProcessID =rhs.m_iProcessID;

		m_ImageTimeCount = rhs.m_ImageTimeCount;
		m_ImageTimeCount_PC = rhs.m_ImageTimeCount_PC;
		m_ImageID = rhs.m_ImageID;
		m_nGII=rhs.m_nGII;
		m_CollectFunInfoCount = rhs.m_CollectFunInfoCount;
		m_AlgoTimeCount = rhs.m_AlgoTimeCount;
		m_CalTimeCount = rhs.m_CalTimeCount;
		return *this;
	}
	bool    m_bDebug;
	bool    m_bSingleDebug;
	bool    m_bEmptyImg;
	Hobject m_img;
	Hobject m_crtTargetRgn;
	Hobject m_crtTargetRgnEx;  //辅助区域
	Hobject m_cvInterImgs;		//中间图片
	Hobject m_crtCorrectImg;		//校正图片
	std::map<Hlong,Hobject> m_unionErrRgn;

	int m_iProcessID;
	long long  m_ImageTimeCount;
	long long  m_ImageTimeCount_PC;
	long long m_CollectFunInfoCount;
	long long m_AlgoTimeCount;
	long long  m_ImageID;
	long long m_CalTimeCount;
	int m_nGII;
};

struct SPC_DATA
{
	long                 m_imgIndex; // Image index(total count)
	long                 m_ngCount;  // NG image count;
	std::vector<IMG_PRODUCT>  m_vNgImg;   // Error Image and its error regions
	std::map<int, long>  m_dictFI_EC; // Function Index <-> Error Count
};

struct GIID_STATS // GIID -- Glue Injector ID
{
	GIID_STATS():m_nID(-1), m_nErrCount(-1)
	{
	}
	GIID_STATS(const GIID_STATS& other)
	{
		m_nID       = other.m_nID;
		m_nErrCount = other.m_nErrCount;
	}
	GIID_STATS(GIID_STATS&& other)
	{
		m_nID       = other.m_nID;
		m_nErrCount = other.m_nErrCount;
	}
	GIID_STATS& operator=(const GIID_STATS& other)
	{
		if(&other == this)
			return *this;
		m_nID       = other.m_nID;
		m_nErrCount = other.m_nErrCount;
		return *this;
	}
	GIID_STATS& operator=(GIID_STATS&& other)
	{
		if(&other == this)
			return *this;
		m_nID       = other.m_nID;
		m_nErrCount = other.m_nErrCount;
		return *this;
	}
	long m_nID;
	long m_nErrCount;
};

struct ITEM_STATS
{
	ITEM_STATS():m_nFuncIndex(-1),m_wstrName(L""),m_nRgnID(-1), m_nItemErrorCount(0)
	{
	}

	ITEM_STATS(const ITEM_STATS& rhs)
	{
		m_nFuncIndex      = rhs.m_nFuncIndex;
		m_wstrName        = rhs.m_wstrName;
		m_nRgnID          = rhs.m_nRgnID;
		m_nItemErrorCount = rhs.m_nItemErrorCount;
	}
	ITEM_STATS(ITEM_STATS&& rhs)
	{
		m_nFuncIndex      = rhs.m_nFuncIndex;
		m_wstrName        = rhs.m_wstrName;
		m_nRgnID          = rhs.m_nRgnID;
		m_nItemErrorCount = rhs.m_nItemErrorCount;
	}
	ITEM_STATS& operator=(const ITEM_STATS& rhs)
	{
		if(&rhs == this)
			return *this;
		m_nFuncIndex      = rhs.m_nFuncIndex;
		m_wstrName        = rhs.m_wstrName;
		m_nRgnID          = rhs.m_nRgnID;
		m_nItemErrorCount = rhs.m_nItemErrorCount;
		return *this;
	}
	ITEM_STATS& operator=(ITEM_STATS&& rhs)
	{
		if(&rhs == this)
			return *this;
		m_nFuncIndex        = rhs.m_nFuncIndex;
		m_wstrName          = rhs.m_wstrName;
		m_nRgnID            = rhs.m_nRgnID;
		m_nItemErrorCount   = rhs.m_nItemErrorCount;
		return *this;
	}
	int          m_nFuncIndex;
	wstring      m_wstrName;
	int          m_nRgnID;
	long         m_nItemErrorCount;
};

struct STATS_INFO
{
	wstring m_wstrRgnName;
	long    m_nErrCount;
	STATS_INFO():m_wstrRgnName(L""), m_nErrCount(0)
	{

	}
	STATS_INFO(wstring wstrRgnName, long nErrCount):m_wstrRgnName(wstrRgnName),m_nErrCount(nErrCount)
	{
	}
	STATS_INFO(const STATS_INFO& other)
	{
		m_wstrRgnName = other.m_wstrRgnName;
		m_nErrCount   = other.m_nErrCount;
	}
	STATS_INFO(STATS_INFO&& other)
	{
		m_wstrRgnName = other.m_wstrRgnName;
		m_nErrCount   = other.m_nErrCount;
	}
	STATS_INFO& operator=(const STATS_INFO& other)
	{
		if(&other == this) return *this;
		m_wstrRgnName = other.m_wstrRgnName;
		m_nErrCount   = other.m_nErrCount;
		return *this;
	}
	STATS_INFO& operator=(STATS_INFO&& other)
	{
		if(&other == this) return *this;
		m_wstrRgnName = other.m_wstrRgnName;
		m_nErrCount   = other.m_nErrCount;
		return *this;
	}
};
struct DATA_BASE_DATA
{
	CString ID; 
	CString UserName;
	CString ProductName;	//产品名称
	CString ProductBatch;	//产品批号
	CString	Recheck ;			//是否复检
	CString	StartTime;			//开始时间
	CString	EndTime;			//结束时间
	CString Reserved1;		//备注字段1
	CString Reserved2;		//备注字段2

	int Qualified; //正品数
 	int	Total;//总检测数
 	int	Rejected;//废品数
 	double	Passrate;//合格率

	std::map<CString,int> m_mapErrorCount;  //应该去平均值
// 	int	Blackcontamination;//黑污染
// 	int	Whitecontamination;//白污染
// 	int	Blackdot;//	黑点
// 	int	Whitedot;//  白点
// 	int	Rubberfiberordot ;//胶丝胶屑
// 	int	Oilcontaminatio;  //油污
// 	int	Missingpart; //缺胶
// 	int	Scissorcuts;  //剪伤
// 	int	Intermixing;//混塞
// 	int	Threadandfibers ;//线毛
// 	int	Filmbroken;//破膜
// 	int	Filmcurlup;//翘膜
// 	int	Bubbles;//气泡
// 	int	Droppedgum;//漏胶
// 	int	Rubbergap;//裂口
// 	int	Pinhole;//针孔
// 	int	Irregularsize;//尺寸不规则
// 	int	Irregularshape;//形状不规则
// 	int	Unabletolocate;//胶塞无法定位
// 	int	Sulfermark;//硫痕
// 	int	Colordifferenc;//异色
// 	int	Abnormalalgorithm;//算法异常
};
struct SYS_STAT_DATA_EX
{
	long                     m_totalCount;
	long                     m_totalOkCount;
	long long				m_timestampBgn;
	long long				m_timestampEnd;
	CString					m_sysforeignkey;
	SYS_STAT_DATA_EX()
	{
		m_totalCount =0;
		m_totalOkCount = 0;
		m_timestampBgn = 0;
		m_timestampEnd= 0;
	}
};
struct SYS_STAT_DATA
{
	long                     m_totalCount;
	long                     m_totalOkCount;
	int						 m_leftFlag;
	long long                m_timestamp;
	CString					 m_strBatchNumber;
	CString					 m_strUser;
	CString					 m_strSysForeignKey;
	SYS_STAT_DATA():m_totalCount(0), m_totalOkCount(0),m_timestamp(0),m_leftFlag(0)
	{

	}
	SYS_STAT_DATA(long totalCount, long totalLeftCount, float ngRate, long long timestamp,long leftFlag)
		:m_totalCount(totalCount), m_totalOkCount(totalLeftCount),m_timestamp(timestamp),m_leftFlag(leftFlag)
	{

	}
	SYS_STAT_DATA(const SYS_STAT_DATA& rhs)
	{
		m_totalCount    = rhs.m_totalCount;
		m_totalOkCount  = rhs.m_totalOkCount;
		m_timestamp = rhs.m_timestamp;
		m_leftFlag = rhs.m_leftFlag;
		m_strBatchNumber = rhs.m_strBatchNumber;
		m_strUser = rhs.m_strUser;
		m_strSysForeignKey = rhs.m_strSysForeignKey;
	}
	SYS_STAT_DATA(SYS_STAT_DATA&& rhs)
	{
		m_totalCount    = rhs.m_totalCount;
		m_totalOkCount  = rhs.m_totalOkCount;
		m_timestamp = rhs.m_timestamp;
		m_leftFlag = rhs.m_leftFlag;
		m_strBatchNumber = rhs.m_strBatchNumber;
		m_strUser = rhs.m_strUser;
		m_strSysForeignKey = rhs.m_strSysForeignKey;
	}
	SYS_STAT_DATA& operator=(const SYS_STAT_DATA& rhs)
	{
		if(&rhs == this)
			return *this;
		m_totalCount    = rhs.m_totalCount;
		m_totalOkCount  = rhs.m_totalOkCount;
		m_timestamp = rhs.m_timestamp;
		m_leftFlag = rhs.m_leftFlag;
		m_strBatchNumber = rhs.m_strBatchNumber;
		m_strUser = rhs.m_strUser;
		m_strSysForeignKey = rhs.m_strSysForeignKey;
		return *this;
	}
	SYS_STAT_DATA& operator=(SYS_STAT_DATA&& rhs)
	{
		if(&rhs == this)
			return *this;
		m_totalCount    = rhs.m_totalCount;
		m_totalOkCount  = rhs.m_totalOkCount;
		m_timestamp = rhs.m_timestamp;
		m_leftFlag = rhs.m_leftFlag;
		m_strBatchNumber = rhs.m_strBatchNumber;
		m_strUser = rhs.m_strUser;
		m_strSysForeignKey = rhs.m_strSysForeignKey;
		return *this;
	}

	void ClearData()
	{
		m_totalCount   = 0;
		m_totalOkCount = 0;
		m_timestamp    = 0;
		m_leftFlag     = 0;
		m_strBatchNumber = _T("");
		m_strUser =_T("");
		m_strSysForeignKey=_T("");
	}
};

struct SPC_DATA_PT
{
	long                     m_totalImgCount;
	long                     m_totalNgImgCount; // NG image count
	long                     m_totalErrCount;
	CString					 m_strBatchNumber;
	CString                m_strUser;
	float                    m_fNgRatio;
	long long                m_timestamp;
	std::vector<ITEM_STATS>  m_vItemStats;
	SPC_DATA_PT():m_totalImgCount(0), m_totalNgImgCount(0),m_totalErrCount(0),m_fNgRatio(0.0f),m_timestamp(0)
	{
		m_vItemStats.reserve(16);
	}
	SPC_DATA_PT(long totalCount, long totalNgCount, long errCount, float ngRate, long long timestamp)
		:m_totalImgCount(totalCount), m_totalNgImgCount(totalNgCount),m_fNgRatio(ngRate),m_totalErrCount(errCount),m_timestamp(timestamp)
	{
		m_vItemStats.reserve(16);
	}
	SPC_DATA_PT(const SPC_DATA_PT& rhs)
	{
		m_totalImgCount    = rhs.m_totalImgCount;
		m_totalNgImgCount  = rhs.m_totalNgImgCount;
		m_totalErrCount    = rhs.m_totalErrCount;
		m_fNgRatio          = rhs.m_fNgRatio;
		m_timestamp        = rhs.m_timestamp;
		m_strBatchNumber   = rhs.m_strBatchNumber;
		m_strUser = rhs.m_strUser;
		m_vItemStats.clear();
		m_vItemStats       = rhs.m_vItemStats;
	}
	SPC_DATA_PT(SPC_DATA_PT&& rhs)
	{
		m_totalImgCount	   = rhs.m_totalImgCount;
		m_totalNgImgCount  = rhs.m_totalNgImgCount;
		m_totalErrCount    = rhs.m_totalErrCount;
		m_fNgRatio          = rhs.m_fNgRatio;
		m_timestamp        = rhs.m_timestamp;
		m_strBatchNumber   = rhs.m_strBatchNumber;
		m_strUser = rhs.m_strUser;
		m_vItemStats.clear();
		m_vItemStats       = rhs.m_vItemStats;
	}
	SPC_DATA_PT& operator=(const SPC_DATA_PT& rhs)
	{
		if(&rhs == this)
			return *this;
		m_totalImgCount    = rhs.m_totalImgCount;
		m_totalNgImgCount  = rhs.m_totalNgImgCount;
		m_totalErrCount    = rhs.m_totalErrCount;
		m_fNgRatio          = rhs.m_fNgRatio;
		m_timestamp        = rhs.m_timestamp;
		m_strBatchNumber   = rhs.m_strBatchNumber;
		m_strUser = rhs.m_strUser;
		m_vItemStats.clear();
		m_vItemStats       = rhs.m_vItemStats;
		return *this;
	}
	SPC_DATA_PT& operator=(SPC_DATA_PT&& rhs)
	{
		if(&rhs == this)
			return *this;
		m_totalImgCount    = rhs.m_totalImgCount;
		m_totalNgImgCount  = rhs.m_totalNgImgCount;
		m_totalErrCount    = rhs.m_totalErrCount;
		m_fNgRatio          = rhs.m_fNgRatio;
		m_timestamp        = rhs.m_timestamp;
		m_strBatchNumber   = rhs.m_strBatchNumber;
		m_strUser = rhs.m_strUser;
		m_vItemStats.clear();
		m_vItemStats       = rhs.m_vItemStats;
		return *this;
	}
	std::vector<STATS_INFO> GetWorstThreeRegion()
	{
		std::vector<STATS_INFO> ret;

		std::map<wstring,long> dictRet;           // Region name <---> Error count
		for(auto it = m_vItemStats.begin(); it != m_vItemStats.end(); it++)
		{
			ITEM_STATS stats = *it;
			if(dictRet.find(stats.m_wstrName) != dictRet.end())
				dictRet[stats.m_wstrName] += stats.m_nItemErrorCount;
			else
				dictRet[stats.m_wstrName] = stats.m_nItemErrorCount;
		}
		
		for(auto it2 = dictRet.begin(); it2 != dictRet.end(); it2++)
		{
			STATS_INFO si(it2->first, it2->second);
			ret.push_back(si);
		}
		std::sort(ret.begin(), ret.end(), [&](STATS_INFO lhs, STATS_INFO rhs) -> bool
		{
			return lhs.m_nErrCount < rhs.m_nErrCount;
		});
		std::reverse(ret.begin(), ret.end());

		size_t sz = ret.size();
		if(sz >= 3)
		{
			std::vector<STATS_INFO> retTmp;
			retTmp.assign(ret.begin(), ret.begin() + 3);
			ret.clear();
			ret = retTmp;
		}
		if(sz < 3)
		{
			for(size_t t = 0; t < (3 - sz); t++)
			{
				STATS_INFO fake(L"", -1);
				ret.push_back(fake);
			}
		}
		return ret;
	}
	void Reset()
	{
		m_totalImgCount   = 0;
		m_totalNgImgCount = 0;
		m_totalErrCount   = 0;
		m_fNgRatio        = 0.0f;
		m_timestamp       = 0;
		m_vItemStats.clear();
		m_strBatchNumber= _T("");
		m_strUser= _T("");
	}
};

struct BarGraphInfo
{
	BarGraphInfo() :
		 m_nBarWidth(5),
		 m_nBarCount(-1),
		 m_fThreshold(-99999.0f),
		 m_fLCL(-99999.0f),
		 m_fUCL(99999.0f),
		 m_fValueSpan(1.0f),
		 m_fMaxValueX(0.0f),
		 m_valueType(VT_I4),
		 m_compType(GREAT)
	{
	}
	BarGraphInfo(const BarGraphInfo& other)
	{
		m_nBarWidth     = other.m_nBarWidth;
		m_nBarCount     = other.m_nBarCount;
		m_fThreshold    = other.m_fThreshold;
		m_fLCL          = other.m_fLCL;
		m_fUCL          = other.m_fUCL;
		m_fValueSpan    = other.m_fValueSpan;
		m_fMaxValueX    = other.m_fMaxValueX;
		m_valueType     = other.m_valueType;
		m_compType      = other.m_compType;
		m_thresholdRect = other.m_thresholdRect;
		m_lclRect       = other.m_lclRect;
		m_uclRect       = other.m_uclRect;
	}
	BarGraphInfo(BarGraphInfo&& other)
	{
		m_nBarWidth     = other.m_nBarWidth;
		m_nBarCount     = other.m_nBarCount;
		m_fThreshold    = other.m_fThreshold;
		m_fLCL          = other.m_fLCL;
		m_fUCL          = other.m_fUCL;
		m_fValueSpan    = other.m_fValueSpan;
		m_fMaxValueX    = other.m_fMaxValueX;
		m_valueType     = other.m_valueType;
		m_compType      = other.m_compType;
		m_thresholdRect = other.m_thresholdRect;
		m_lclRect       = other.m_lclRect;
		m_uclRect       = other.m_uclRect;
	}
	BarGraphInfo& operator=(const BarGraphInfo& other)
	{
		if(&other == this)
			return *this;
		m_nBarWidth     = other.m_nBarWidth;
		m_nBarCount     = other.m_nBarCount;
		m_fThreshold    = other.m_fThreshold;
		m_fLCL          = other.m_fLCL;
		m_fUCL          = other.m_fUCL;
		m_fValueSpan    = other.m_fValueSpan;
		m_fMaxValueX    = other.m_fMaxValueX;
		m_valueType     = other.m_valueType;
		m_compType      = other.m_compType;
		m_thresholdRect = other.m_thresholdRect;
		m_lclRect       = other.m_lclRect;
		m_uclRect       = other.m_uclRect;
		return *this;
	}
	BarGraphInfo& operator=(BarGraphInfo&& other)
	{
		if(&other == this)
			return *this;
		m_nBarWidth     = other.m_nBarWidth;
		m_nBarCount     = other.m_nBarCount;
		m_fThreshold    = other.m_fThreshold;
		m_fLCL          = other.m_fLCL;
		m_fUCL          = other.m_fUCL;
		m_fValueSpan    = other.m_fValueSpan;
		m_fMaxValueX    = other.m_fMaxValueX;
		m_valueType     = other.m_valueType;
		m_compType      = other.m_compType;
		m_thresholdRect = other.m_thresholdRect;
		m_lclRect       = other.m_lclRect;
		m_uclRect       = other.m_uclRect;
		return *this;
	}
	int          m_nBarWidth;
	int          m_nBarCount;
	float        m_fThreshold;
	float        m_fLCL;
	float        m_fUCL;
	float        m_fValueSpan;
	float        m_fMaxValueX;
	VARENUM      m_valueType;
	COMPARE_TYPE m_compType;
	CRect        m_thresholdRect; // Threshold line rectangle
	CRect        m_lclRect;
	CRect        m_uclRect;
};

struct MODEL_IMAGE
{
	MODEL_IMAGE():m_fScore(0.0f)
	{
	}
	MODEL_IMAGE(const Hobject& img, float fScore):m_image(img),m_fScore(fScore)
	{
	}
	MODEL_IMAGE(const MODEL_IMAGE& rhs)
	{
		m_image  = rhs.m_image;
		m_fScore = rhs.m_fScore;
	}
	MODEL_IMAGE(MODEL_IMAGE&& rhs)
	{
		m_image  = rhs.m_image;
		m_fScore = rhs.m_fScore;
	}
	MODEL_IMAGE& operator=(const MODEL_IMAGE& rhs)
	{
		if(&rhs == this)
			return *this;
		m_image  = rhs.m_image;
		m_fScore = rhs.m_fScore;
		return *this;
	}
	MODEL_IMAGE& operator=(MODEL_IMAGE&& rhs)
	{
		if(&rhs == this)
			return *this;
		m_image  = rhs.m_image;
		m_fScore = rhs.m_fScore;
		return *this;
	}
	Hobject m_image;
	float   m_fScore;
};

const int UAC_ADMIN    = 0;
const int UAC_OPERATOR = 1;

struct UAC
{
	UAC():m_strUsername(L""),m_nLevel(-1)
	{
	}
	UAC(wstring strUsername, int nLevel)
		:m_strUsername(strUsername),
		 m_nLevel(nLevel)
	{
	}
	UAC(const UAC& other)
	{
		m_strUsername = other.m_strUsername;
		m_strPassword = other.m_strPassword;
		m_nLevel      = other.m_nLevel;
	}
	UAC(UAC&& other)
	{
		m_strUsername = other.m_strUsername;
		m_strPassword = other.m_strPassword;
		m_nLevel      = other.m_nLevel;
	}
	UAC& operator=(const UAC& other)
	{
		if(&other == this)
			return *this;
		m_strUsername = other.m_strUsername;
		m_strPassword = other.m_strPassword;
		m_nLevel      = other.m_nLevel;

		return *this;
	}
	UAC& operator=(UAC&& other)
	{
		if(&other == this)
			return *this;
		m_strUsername = other.m_strUsername;
		m_strPassword = other.m_strPassword;
		m_nLevel      = other.m_nLevel;

		return *this;
	}
	wstring m_strUsername;
	wstring m_strPassword;
	int     m_nLevel;
};

struct IMG_SZ // Camera Parameter -- Image Size
{
	IMG_SZ():m_nWidth(659), m_nHeight(494)
	{
	}
	IMG_SZ(int nWidth, int nHeight):m_nWidth(nWidth), m_nHeight(nHeight)
	{
	}
	IMG_SZ(const IMG_SZ& other)
	{
		m_nWidth  = other.m_nWidth;
		m_nHeight = other.m_nHeight;
	}
	IMG_SZ(IMG_SZ&& other)
	{
		m_nWidth  = other.m_nWidth;
		m_nHeight = other.m_nHeight;
	}
	IMG_SZ& operator=(const IMG_SZ& other)
	{
		if(&other == this)
			return *this;
		m_nWidth  = other.m_nWidth;
		m_nHeight = other.m_nHeight;
		return *this;
	}
	IMG_SZ& operator=(IMG_SZ&& other)
	{
		if(&other == this)
			return *this;
		m_nWidth  = other.m_nWidth;
		m_nHeight = other.m_nHeight;
		return *this;
	}

	int m_nWidth;
	int m_nHeight;
};

struct SHIFT
{
	SHIFT():m_wstrShiftName(L""),m_bgnTime(0), m_endTime(0)
	{
	}
	SHIFT(const wstring& wstrShiftName, long bgnTime, long endTime)
		:m_wstrShiftName(wstrShiftName),
		 m_bgnTime(bgnTime),
		 m_endTime(endTime)
	{
	}
	SHIFT(const SHIFT& other)
	{
		m_wstrShiftName = other.m_wstrShiftName;
		m_bgnTime       = other.m_bgnTime;
		m_endTime       = other.m_endTime;
	}
	SHIFT& operator=(const SHIFT& other)
	{
		if(&other == this)
			return *this;
		m_wstrShiftName = other.m_wstrShiftName;
		m_bgnTime       = other.m_bgnTime;
		m_endTime       = other.m_endTime;

		return *this;
	}

	SHIFT(SHIFT&& other)
	{
		m_wstrShiftName = other.m_wstrShiftName;
		m_bgnTime       = other.m_bgnTime;
		m_endTime       = other.m_endTime;
	}
	SHIFT& operator=(SHIFT&& other)
	{
		if(&other == this)
			return *this;
		m_wstrShiftName = other.m_wstrShiftName;
		m_bgnTime       = other.m_bgnTime;
		m_endTime       = other.m_endTime;

		return *this;
	}
	bool IsEqual(const SHIFT& other)
	{
		return  (m_wstrShiftName == other.m_wstrShiftName) 
			&&(m_bgnTime == other.m_bgnTime)
			&&(m_endTime == other.m_endTime);
	}
	wstring   m_wstrShiftName;
	long m_bgnTime;
	long m_endTime;

};

struct CHANNEL_STATUS
{
	CHANNEL_STATUS()
		:m_nSpeed(-1),
		 m_nSensorCount(-1),
		 m_nDroppedImg(0),
		 m_nRejectCount(-1),
		 m_nTotalOkCount(-1),
		 m_nCSC(-1),
		 m_nTriggerIgnoreCount(-1),
		m_nReject2Count(-1),	// 剔除2总数
		m_nDroppedImg2(-1),  // 相机2丢帧总数
		m_nDroppedImg3(-1),  // 相机3丢帧总数
		m_nDroppedImg4(-1),  // 相机4丢帧总数
		m_nLifeID(-1)				// Tracer当前LifeID
	{
		m_nEoeSpeed=0;
		m_nPositionError=0;	//定位失败统计
		m_nPulse=0;			//当前脉冲值
		m_nPCTimeError=0;		//PC时间定位错误统计
		m_nSeqError=0;		//序号定位错误统计
		m_nRepeatCount=0;		//重复定位或者收到重复的数据包统计
		m_nCameraAndSeqError=0; //相机时间与序号定位错误统计
		m_nCameraTimeOffseError=0; //相机时间偏差大统计
		m_nCameraPositionSucc=0;   //通过相机计数与相机时间定位成功的统计
		m_nPositionFailFlag=0;    //定位失败解析标志位
		m_nPositionFailCount=0;
		m_bInterLock=0;			//是否互锁状态
	}
	CHANNEL_STATUS(const CHANNEL_STATUS& other)
	{
		m_nSpeed       = other.m_nSpeed;
		m_nSensorCount = other.m_nSensorCount;
		m_nDroppedImg  = other.m_nDroppedImg;
		m_nRejectCount = other.m_nRejectCount;
		m_nTotalOkCount = other.m_nTotalOkCount;
		m_nCSC         = other.m_nCSC;

		m_nTriggerIgnoreCount=other.m_nTriggerIgnoreCount;  // 过触发屏蔽总数
		m_nReject2Count= other.m_nReject2Count;   // 剔除2总数
		m_nDroppedImg2= other.m_nDroppedImg2;  // 相机2丢帧总数
		m_nDroppedImg3= other.m_nDroppedImg3;  // 相机3丢帧总数
		m_nDroppedImg4= other.m_nDroppedImg4;  // 相机4丢帧总数
		m_nLifeID= other.m_nLifeID;										  // Tracer当前LifeID
		m_nEoeSpeed=other.m_nEoeSpeed;
		m_nPositionError=other.m_nPositionError;	
		m_nPulse=other.m_nPulse;			
		m_nPCTimeError=other.m_nPCTimeError;		
		m_nSeqError=other.m_nSeqError;		
		m_nRepeatCount=other.m_nRepeatCount;		
		m_nCameraAndSeqError=other.m_nCameraAndSeqError; 
		m_nCameraTimeOffseError=other.m_nCameraTimeOffseError; 
		m_nCameraPositionSucc=other.m_nCameraPositionSucc;   
		m_nPositionFailFlag=other.m_nPositionFailFlag;   
		m_nPositionFailCount = other.m_nPositionFailCount;
		m_bInterLock=other.m_bInterLock;			
	}
	CHANNEL_STATUS(CHANNEL_STATUS&& other)
	{
		m_nSpeed       = other.m_nSpeed;
		m_nSensorCount = other.m_nSensorCount;
		m_nDroppedImg  = other.m_nDroppedImg;
		m_nRejectCount = other.m_nRejectCount;
		m_nTotalOkCount = other.m_nTotalOkCount;
		m_nCSC         = other.m_nCSC;

		m_nTriggerIgnoreCount=other.m_nTriggerIgnoreCount;  // 过触发屏蔽总数
		m_nReject2Count= other.m_nReject2Count;   // 剔除2总数
		m_nDroppedImg2= other.m_nDroppedImg2;  // 相机2丢帧总数
		m_nDroppedImg3= other.m_nDroppedImg3;  // 相机3丢帧总数
		m_nDroppedImg4= other.m_nDroppedImg4;  // 相机4丢帧总数
		m_nLifeID= other.m_nLifeID;										  // Tracer当前LifeID
		m_nEoeSpeed=other.m_nEoeSpeed;
		m_nPositionError=other.m_nPositionError;	
		m_nPulse=other.m_nPulse;			
		m_nPCTimeError=other.m_nPCTimeError;		
		m_nSeqError=other.m_nSeqError;		
		m_nRepeatCount=other.m_nRepeatCount;		
		m_nCameraAndSeqError=other.m_nCameraAndSeqError; 
		m_nCameraTimeOffseError=other.m_nCameraTimeOffseError; 
		m_nCameraPositionSucc=other.m_nCameraPositionSucc;   
		m_nPositionFailFlag=other.m_nPositionFailFlag;   
		m_nPositionFailCount = other.m_nPositionFailCount;
		m_bInterLock=other.m_bInterLock;
	}
	CHANNEL_STATUS& operator=(const CHANNEL_STATUS& other)
	{
		if(&other == this)
			return *this;
		m_nSpeed       = other.m_nSpeed;
		m_nSensorCount = other.m_nSensorCount;
		m_nDroppedImg  = other.m_nDroppedImg;
		m_nRejectCount = other.m_nRejectCount;
		m_nTotalOkCount = other.m_nTotalOkCount;
		m_nCSC         = other.m_nCSC;

		m_nTriggerIgnoreCount=other.m_nTriggerIgnoreCount;  // 过触发屏蔽总数
		m_nReject2Count= other.m_nReject2Count;   // 剔除2总数
		m_nDroppedImg2= other.m_nDroppedImg2;  // 相机2丢帧总数
		m_nDroppedImg3= other.m_nDroppedImg3;  // 相机3丢帧总数
		m_nDroppedImg4= other.m_nDroppedImg4;  // 相机4丢帧总数
		m_nLifeID= other.m_nLifeID;										  // Tracer当前LifeID
		m_nEoeSpeed=other.m_nEoeSpeed;
		m_nPositionError=other.m_nPositionError;	
		m_nPulse=other.m_nPulse;			
		m_nPCTimeError=other.m_nPCTimeError;		
		m_nSeqError=other.m_nSeqError;		
		m_nRepeatCount=other.m_nRepeatCount;		
		m_nCameraAndSeqError=other.m_nCameraAndSeqError; 
		m_nCameraTimeOffseError=other.m_nCameraTimeOffseError; 
		m_nCameraPositionSucc=other.m_nCameraPositionSucc;   
		m_nPositionFailFlag=other.m_nPositionFailFlag;
		m_nPositionFailCount = other.m_nPositionFailCount;
		m_bInterLock=other.m_bInterLock;
		return *this;
	}
	CHANNEL_STATUS& operator=(CHANNEL_STATUS&& other)
	{
		if(&other == this)
			return *this;
		m_nSpeed       = other.m_nSpeed;
		m_nSensorCount = other.m_nSensorCount;
		m_nDroppedImg  = other.m_nDroppedImg;
		m_nRejectCount = other.m_nRejectCount;
		m_nTotalOkCount = other.m_nTotalOkCount;
		m_nCSC         = other.m_nCSC;

		m_nTriggerIgnoreCount=other.m_nTriggerIgnoreCount;  // 过触发屏蔽总数
		m_nReject2Count= other.m_nReject2Count;   // 剔除2总数
		m_nDroppedImg2= other.m_nDroppedImg2;  // 相机2丢帧总数
		m_nDroppedImg3= other.m_nDroppedImg3;  // 相机3丢帧总数
		m_nDroppedImg4= other.m_nDroppedImg4;  // 相机4丢帧总数
		m_nLifeID= other.m_nLifeID;										  // Tracer当前LifeID
		m_nEoeSpeed=other.m_nEoeSpeed;
		m_nPositionError=other.m_nPositionError;	
		m_nPulse=other.m_nPulse;			
		m_nPCTimeError=other.m_nPCTimeError;		
		m_nSeqError=other.m_nSeqError;		
		m_nRepeatCount=other.m_nRepeatCount;		
		m_nCameraAndSeqError=other.m_nCameraAndSeqError; 
		m_nCameraTimeOffseError=other.m_nCameraTimeOffseError; 
		m_nCameraPositionSucc=other.m_nCameraPositionSucc;   
		m_nPositionFailFlag=other.m_nPositionFailFlag;
		m_nPositionFailCount = other.m_nPositionFailCount;
		m_bInterLock=other.m_bInterLock;
		return *this;
	}
	void Reset()
	{
		m_nSpeed = 0;				//  管路速度
		m_nSensorCount= 0;		// 生产总数
		m_nDroppedImg= 0;		// 相机1丢帧总数
		m_nDroppedImg2= 0;  // 相机2丢帧总数
		m_nDroppedImg3= 0;  // 相机3丢帧总数
		m_nDroppedImg4= 0;  // 相机4丢帧总数
		m_nRejectCount= 0;		// 剔除1总数
		m_nReject2Count= 0;   // 剔除2总数
		m_nTotalOkCount= 0;	// 合格总数
		m_nTriggerIgnoreCount= 0;   // 过触发屏蔽总数
		m_nLifeID= 0;				// Tracer当前LifeID
		m_nCSC= 0;					// 运行状态和故障信息

		m_nEoeSpeed=0;
		m_nPositionError=0;	//定位失败统计
		m_nPulse=0;			//当前脉冲值
		m_nPCTimeError=0;		//PC时间定位错误统计
		m_nSeqError=0;		//序号定位错误统计
		m_nRepeatCount=0;		//重复定位或者收到重复的数据包统计
		m_nCameraAndSeqError=0; //相机时间与序号定位错误统计
		m_nCameraTimeOffseError=0; //相机时间偏差大统计
		m_nCameraPositionSucc=0;   //通过相机计数与相机时间定位成功的统计
		m_nPositionFailFlag=0;    //定位失败解析标志位
		m_nPositionFailCount = 0; //定位失败计数汇总
		m_bInterLock=0;			//是否互锁状态
	}
	int m_nSpeed;				//  管路速度
	int m_nEoeSpeed;		//盖子速度
	int m_nSensorCount;		// 生产总数
	int m_nDroppedImg;		// 相机1丢帧总数
	int m_nDroppedImg2;  // 相机2丢帧总数
	int m_nDroppedImg3;  // 相机3丢帧总数
	int m_nDroppedImg4;  // 相机4丢帧总数
	int m_nRejectCount;		// 剔除1总数
	int m_nReject2Count;   // 剔除2总数
	int m_nTotalOkCount;	// 合格总数
	int m_nTriggerIgnoreCount;   // 过触发屏蔽总数
	int m_nLifeID;				// Tracer当前LifeID
	int m_nCSC;					// Camera Status Code, byte #17 运行状态和故障信息

	int m_nPositionError;	//定位失败统计
	int m_nPulse;			//当前脉冲值
	int m_nPCTimeError;		//PC时间定位错误统计
	int m_nSeqError;		//序号定位错误统计
	int m_nRepeatCount;		//重复定位或者收到重复的数据包统计
	int m_nCameraAndSeqError; //相机时间与序号定位错误统计
	int m_nCameraTimeOffseError; //相机时间偏差大统计
	int m_nCameraPositionSucc;   //通过相机计数与相机时间定位成功的统计
	int m_nPositionFailFlag;    //定位失败解析标志位
	INT64 m_nPositionFailCount;  //定位失败计数汇总
	BOOL m_bInterLock;			//是否互锁状态
};

struct CAN_OUTSIDE_COMMON_PARAM
{
	Hobject StandardImg;
	Hobject FlagModelRegion;
	Hobject SeamRegionInStdImg;
	Hobject StdModelRegion;
	HTuple  StitchedImgWidthUnion;
	HTuple  ModelIDs;
	HTuple  ModelRows;
	HTuple  ModelCols;
	HTuple  FlagModelID;
	HTuple  SortedFlagColsInStdImg;
	HTuple  StitchingParamCol;
	HTuple  CanRegWidth;
	HTuple  MappedImgWidths;
};

struct CAN_OUTSIDE_CH_PARAM
{
	Hobject MultImg;
	Hobject MapImg;
	Hobject DetectROI;
	Hobject CanRegion;
	HTuple  CanRegWidth;
};

struct PREPROCESS_RESULT
{
	HTuple MatchedFlagScore; 
	HTuple CameraIndexOfFlag; 
	HTuple MatchedFlagCol; 
	HTuple MatchedFlagRow; 
	HTuple HomMat2DForFlag;

	PREPROCESS_RESULT()
	{
		MatchedFlagScore   = HTuple(); 
		CameraIndexOfFlag  = HTuple(); 
		MatchedFlagCol     = HTuple(); 
		MatchedFlagRow     = HTuple(); 
		HomMat2DForFlag    = HTuple();
	}
	PREPROCESS_RESULT(const PREPROCESS_RESULT& other)
	{
		MatchedFlagScore  = other.MatchedFlagScore;
		CameraIndexOfFlag = other.CameraIndexOfFlag;
		MatchedFlagCol    = other.MatchedFlagCol;
		MatchedFlagRow    = other.MatchedFlagRow;
		HomMat2DForFlag   = other.HomMat2DForFlag;
	}
	PREPROCESS_RESULT(PREPROCESS_RESULT&& other)
	{
		MatchedFlagScore  = other.MatchedFlagScore;
		CameraIndexOfFlag = other.CameraIndexOfFlag;
		MatchedFlagCol    = other.MatchedFlagCol;
		MatchedFlagRow    = other.MatchedFlagRow;
		HomMat2DForFlag   = other.HomMat2DForFlag;
	}

	PREPROCESS_RESULT& operator=(const PREPROCESS_RESULT& other)
	{
		if(&other == this)
			return *this;
		MatchedFlagScore  = other.MatchedFlagScore;
		CameraIndexOfFlag = other.CameraIndexOfFlag;
		MatchedFlagCol    = other.MatchedFlagCol;
		MatchedFlagRow    = other.MatchedFlagRow;
		HomMat2DForFlag   = other.HomMat2DForFlag;
		return *this;
	}

	PREPROCESS_RESULT& operator=(PREPROCESS_RESULT&& other)
	{
		if(&other == this)
			return *this;
		MatchedFlagScore  = other.MatchedFlagScore;
		CameraIndexOfFlag = other.CameraIndexOfFlag;
		MatchedFlagCol    = other.MatchedFlagCol;
		MatchedFlagRow    = other.MatchedFlagRow;
		HomMat2DForFlag   = other.HomMat2DForFlag;
		return *this;
	}
	void Reset()
	{
		MatchedFlagScore   = HTuple(); 
		CameraIndexOfFlag  = HTuple(); 
		MatchedFlagCol     = HTuple(); 
		MatchedFlagRow     = HTuple(); 
		HomMat2DForFlag    = HTuple();
	}
};

struct IMAGE_JUNCTION
{
	struct ImageSlot
	{
		IMAGE_BOX              m_imageBox;
		LARGE_INTEGER          m_timestamp;//non-0 <=> non-empty
	};
	IMAGE_JUNCTION()
	{
		m_nImgCount = 0;
	}
	IMAGE_JUNCTION(int szImage)
	{
		int height = 494;
		int width  = 659;
		std::vector<unsigned char>  image(height * width, 0);
		for(int r = 0; r < height; r++)
		{
			for(int c = 0; c < width; c++)
				image[r * width + c] = 50 + r % 32 + c % 16;
		}
		gen_image1(&m_fakeImg,"byte",width,height,(Hlong)&image[0]);

		ImageSlot nullSlot;
		nullSlot.m_imageBox = m_fakeImg;
		nullSlot.m_timestamp.QuadPart = 0;

		m_nImgCount = 0;
		m_vImageBuffer = std::vector<ImageSlot>(szImage, nullSlot);
	}
	IMAGE_JUNCTION(const IMAGE_JUNCTION& other)
	{
		m_fakeImg      = other.m_fakeImg;
		m_nImgCount    = (int)other.m_nImgCount;
		m_vImageBuffer = other.m_vImageBuffer;
	}
	IMAGE_JUNCTION(IMAGE_JUNCTION&& other)
	{
		m_fakeImg      = other.m_fakeImg;
		m_nImgCount    = (int)other.m_nImgCount;
		m_vImageBuffer = other.m_vImageBuffer;
	}
	IMAGE_JUNCTION& operator=(const IMAGE_JUNCTION& other)
	{
		if(&other == this)
			return *this;
		m_fakeImg      = other.m_fakeImg;
		m_nImgCount    = (int)other.m_nImgCount;
		m_vImageBuffer = other.m_vImageBuffer;
		return *this;
	}

	IMAGE_JUNCTION& operator=(IMAGE_JUNCTION&& other)
	{
		if(&other == this)
			return *this;
		m_fakeImg      = other.m_fakeImg;
		m_nImgCount    = (int)other.m_nImgCount;
		m_vImageBuffer = other.m_vImageBuffer;
		return *this;
	}
	void clearBuffer()
	{
		for (auto i = m_vImageBuffer.begin(); i != m_vImageBuffer.end(); ++i)
		{
			i->m_timestamp.QuadPart = 0;
		}
		m_nImgCount = 0;
	}
	ImageSlot * getEarliest()
	{
		ImageSlot * r = nullptr;
		for (auto i = m_vImageBuffer.begin(); i != m_vImageBuffer.end(); ++i)
		{
			if (i->m_timestamp.QuadPart == 0)
				continue;
			if (r == nullptr || i->m_timestamp.QuadPart < r->m_timestamp.QuadPart)
				r = &*i;
		}
		return r;
	}
	Hobject                m_fakeImg;
	int                    m_nImgCount;
	std::vector<ImageSlot> m_vImageBuffer;
};

struct MIN_MAX
{
	float m_fMin;
	float m_fMax;
	MIN_MAX()
	{
		m_fMin = 999999999999.9f;
		m_fMax = 0.0f;
	}

	MIN_MAX(const MIN_MAX& other)
	{
		m_fMin = other.m_fMin;
		m_fMax = other.m_fMax;
	}
	MIN_MAX(MIN_MAX&& other)
	{
		m_fMin = other.m_fMin;
		m_fMax = other.m_fMax;
	}
	MIN_MAX& operator=(const MIN_MAX& other)
	{
		if(&other == this)
			return *this;
		m_fMin = other.m_fMin;
		m_fMax = other.m_fMax;

		return *this;
	}
	MIN_MAX& operator=(MIN_MAX&& other)
	{
		if(&other == this)
			return *this;
		m_fMin = other.m_fMin;
		m_fMax = other.m_fMax;

		return *this;
	}
};

template<typename T>
class threadsafe_queue
{
private:
    mutable boost::mutex      mut;
    std::queue<T>             data_queue;
    boost::condition_variable data_cond; 

public:
    threadsafe_queue()
    {
    }
    threadsafe_queue(threadsafe_queue const& other)
    {
        boost::lock_guard<boost::mutex> lk(mut);
        data_queue = other.data_queue;
    }
    void push(T new_value)
    {
        boost::lock_guard<boost::mutex> lk(mut);
        data_queue.push(new_value);
        data_cond.notify_one();
    }
    void wait_and_pop(T& value)
    {
        boost::lock_guard<boost::mutex> lk(mut);
        data_cond.wait(lk,[this]()
        {
            return !data_queue.empty();
        });
        value = data_queue.front();
        data_queue.pop();
    }
    std::shared_ptr<T> wait_and_pop()
    {
        boost::lock_guard<boost::mutex> lk(mut);
        data_cond.wait(lk,[this]()
        {
            return !data_queue.empty();
        });
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
    }
    bool try_pop(T& value)
    {
         boost::lock_guard<boost::mutex> lk(mut);
         if(data_queue.empty())
         {
             return false;
         }
         value = data_queue.front();
         data_queue.pop();
         return true;
    }
    std::shared_ptr<T> try_pop()
    {
        boost::lock_guard<boost::mutex> lk(mut);
        if(data_queue.empty())
        {
            return std::shared_ptr<T>();
        }
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
    }
    bool empty() const
    {
         boost::lock_guard<boost::mutex> lk(mut);
         return data_queue.empty();
    }
};