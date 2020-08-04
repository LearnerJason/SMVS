#pragma once

#include <WinSock.h>
#include <WinSock2.h>
#include <map>
#include <string>

#include "TypeDef.h"
#include "utils.h"

using namespace std;

class CSecretaryDevice
{
public:
	CSecretaryDevice();
	CSecretaryDevice(REJECTOR_SETTING& rs);
	~CSecretaryDevice(void);
	CSecretaryDevice(const CSecretaryDevice& other)
	{
		m_bDeviceOk   = other.m_bDeviceOk;
		m_rs          = other.m_rs;
		m_nQueryIndex = other.m_nQueryIndex;
		m_deviceSocket = other.m_deviceSocket;
		m_deviceAddr   = other.m_deviceAddr;
		m_pcAddr    = other.m_pcAddr;
		m_dictCmd.clear();
		m_dictCmd = other.m_dictCmd;
		m_chindex = other.m_chindex;

		m_dictCmdInfo.clear();
		m_dictCmdInfo = other.m_dictCmdInfo;

		m_dictRCodeInfo.clear();
		m_dictRCodeInfo = other.m_dictRCodeInfo;
	}
	CSecretaryDevice& operator=(const CSecretaryDevice& other)
	{
		if(this == &other)
			return *this;
		m_bDeviceOk   = other.m_bDeviceOk;
		m_rs          = other.m_rs;
		m_nQueryIndex = other.m_nQueryIndex;
		m_deviceSocket  = other.m_deviceSocket;
		m_deviceAddr    = other.m_deviceAddr;
		m_pcAddr    = other.m_pcAddr;
		m_dictCmd.clear();
		m_dictCmd = other.m_dictCmd;
		m_chindex = other.m_chindex;

		m_dictCmdInfo.clear();
		m_dictCmdInfo = other.m_dictCmdInfo;

		m_dictRCodeInfo.clear();
		m_dictRCodeInfo = other.m_dictRCodeInfo;

		return *this;
	}
	bool InitDevice();
	bool Start();
	bool Stop();
	bool GetDeviceStatus();
	bool ExecuteCmd(int QOpCmdID,int cmdID=-1, unsigned int nValue = 0);
	bool ExecuteCmd(int QOpCmdID,string strCmd);
	bool ExecuteQueryCmd(CHANNEL_STATUS& retData,std::vector<ERROR_INFO>& vRetError,bool& ErrorMode);
	void SetChindex(int index);
	CString GetVersion();
	bool IsCmdIDSupport(int cmdID);
	std::map<int,INT64 > GetQueryResult();

	void CSecretaryDevice::SetCLientTimeSyn();
private:
	bool FindTargetIpAddress();
	void CreateCommandDict();
	CHANNEL_STATUS ParseQueryData(std::vector<char>& vData);

private:
	bool             m_bDeviceOk;
	REJECTOR_SETTING m_rs;
	long long      m_nQueryIndex;
	SOCKET         m_deviceSocket;
	sockaddr_in    m_deviceAddr;
	sockaddr_in    m_pcAddr;
	std::map<int, string> m_dictCmd;
	std::map<int, string> m_dictQOpCmd;
	std::map<int, CString> m_dictCmdInfo;
	std::map<int, CString> m_dictRCodeInfo;
	int			   m_chindex;
	CString m_strVision;

	std::map<int,INT64>  m_dictQueryResult;
	unsigned int m_RID;

	CRITICAL_SECTION    m_ExecuteCmdCs;

	unsigned int m_QLifeID;

	int m_bNoReply;
};

