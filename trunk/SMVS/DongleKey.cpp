#include "stdafx.h"
#include "DongleKey.h"
#include "resource.h"
#include "utils.h"
#include <stdio.h>
#include <WinSock.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iphlpapi.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib,"WS2_32.lib")

////////////////////////////////////////////////////////////
#include <winsock2.h>
#include <winsock.h>
#include <ws2tcpip.h>

void Timestamp::ReverseEndian(void)
{
    ReverseEndianInt(seconds);
    ReverseEndianInt(fraction);
}

time_t Timestamp::to_time_t(void)
{
    return (seconds - ((70 * 365 + 17) * 86400))&0x7fffffff;
}

void NTPMessage::ReverseEndian(void)
{
    ref.ReverseEndian();
    orig.ReverseEndian();
    rx.ReverseEndian();
    tx.ReverseEndian();
}

int NTPMessage::recv(int sock)
{
    int ret = ::recv(sock, (char*)this, sizeof(*this), 0);
    ReverseEndian();
    return ret;
}

int NTPMessage::sendto(int sock, struct sockaddr_in* srv_addr)
{
    ReverseEndian();
    int ret = ::sendto(sock, (const char*)this, sizeof(*this), 0, (sockaddr*)srv_addr, sizeof(*srv_addr));
    ReverseEndian();
    return ret;
}

void NTPMessage::clear()
{
    memset(this, 0, sizeof(*this));
}
////////////////////////////////////////////////////////////

CDongleKey::CDongleKey(void)
{
	m_nTryCount     = 0;
	m_AuthType      = 0;
	m_vMD5          = std::vector<BYTE>(16,0x00);
	m_vProductDt    = std::vector<BYTE>(7,0x00);
	m_vEncrypedData = std::vector<BYTE>(128,0x00);
	m_hDongle        = NULL;
	m_bLocked        = false;
}


CDongleKey::~CDongleKey(void)
{
}

int  CDongleKey::GetAuthorizationType()
{
	return m_AuthType;
}

bool CDongleKey::ParseLicenseFile()
{
	TCHAR  moduleFileName[MAX_PATH];  
	::GetModuleFileName(NULL,moduleFileName, MAX_PATH);
	(_tcsrchr(moduleFileName, _T('\\')))[1] = 0;
	CString sPath = moduleFileName;
	sPath.Append(_T("SLAC Machine Vision System License.dat"));

	if(!utils::CheckFileExists(sPath))
	{
		CString sMsg;
		sMsg.LoadString(IDS_STRING_LICENSE_FILE_ABSENT);
		AfxMessageBox(sMsg,MB_ICONERROR,NULL);
		return false;
	}
	string strLicenseFile = utils::Unicode2Utf8(sPath.GetString());
	FILE* fp = nullptr;
	fp = fopen(strLicenseFile.c_str(),"rb");
	fseek(fp,0,SEEK_END);
	int nLen = ftell(fp);
	fseek(fp,0,SEEK_SET);
	BYTE licenseDataFile[256];
	::memset(licenseDataFile,0x00,sizeof(BYTE)*256);
	size_t sz = fread(licenseDataFile,1,nLen,fp);

	memcpy(&m_vMD5[0],licenseDataFile,16);
	memcpy(&m_vProductDt[0],licenseDataFile + sizeof(BYTE) * 16,7);
	memcpy(&m_vEncrypedData[0], licenseDataFile + sizeof(BYTE) * 23, 128);

	// Decode the license data;
	BYTE licenseData[32];
	size_t szByte = sizeof(BYTE);
	memset(licenseData,0x00, szByte*32);
	int cipherLen   = 128;
	int nOutDataLen = 32;
	DWORD dwRet = Dongle_RsaPri(m_hDongle,0x0010,FLAG_DECODE,&m_vEncrypedData[0],cipherLen,licenseData,&nOutDataLen);
	if(dwRet != DONGLE_SUCCESS)
	{
		return false;
	}
	string strData;
	for(int i = 0; i < nOutDataLen; i++)
	{
		strData.push_back((char)licenseData[i]);
	}
	std::vector<string> vItems = utils::SplitString(strData,"#");
	m_nChCount  = boost::lexical_cast<int>(vItems[0]);
	m_AuthType  = boost::lexical_cast<int>(vItems[1]);
	m_nTailDays = boost::lexical_cast<int>(vItems[2]);

	if(m_AuthType == 1) // Authorized.
	{
		RemoveLockedFlag();
	}
	return true;
}

bool CDongleKey::CheckMD5OfEncryptedData()
{
	DWORD dwRet = 0;	
	BYTE cipherText[128];

	::memset((void*)cipherText,0x00,sizeof(BYTE)*128);
	dwRet = Dongle_ReadFile(m_hDongle,2,0,cipherText,128);
	if(dwRet != DONGLE_SUCCESS)
	{
		return false;
	}
	int cipherLen = 128;
	BYTE plainText[128];
	memset((void*)plainText,0x00,sizeof(BYTE)*128);
	int nOutDataLen = 128;
	dwRet = Dongle_RsaPri(m_hDongle,1,FLAG_DECODE,cipherText,cipherLen,plainText,&nOutDataLen);
	if(dwRet != DONGLE_SUCCESS)
	{
		// TODO:
		return false;
	}
	BYTE tmpMD5[16];
	memset(tmpMD5,0x00,sizeof(BYTE)*16);
	dwRet = Dongle_HASH(m_hDongle,FLAG_HASH_MD5,plainText,103,tmpMD5);
	if(DONGLE_SUCCESS != dwRet)
	{
		return false;
	}
	if(memcmp(tmpMD5,&m_vMD5[0],16) != 0)
	{
		return false;
	}
	return true;
}

bool CDongleKey::CheckExpirationDatetime(bool bWarningMsg)
{
	if(m_AuthType == 1) // Authorized.
	{
		return true;
	}
	
	CString sTmp,sMsg;
	sTmp.LoadString(IDS_STRING_EXPIRATION_PROMPT);
	// Check expiration date
	SYSTEMTIME crtDt;
	::GetSystemTime(&crtDt);
	long long nCrtDatetime = utils::DateTime2Int64(crtDt);
	long long nProductDt   = utils::ParseDateTimeFromByteArray(m_vProductDt);

	CTime bgn = utils::Int64ToDateTime(nProductDt);
	CTime end = utils::Int64ToDateTime(nCrtDatetime);
	CTimeSpan timeSpan = end - bgn;
	long long nDays = timeSpan.GetDays();
	if(nDays >= m_nTailDays)
	{
		sMsg.LoadString(IDS_STRING_TRIAL_EXPIRATION);
		AfxMessageBox(sMsg,MB_ICONERROR, NULL);
		return false;
	}
	else
	{
		if(bWarningMsg && (m_nTailDays - nDays) < 7)
		{
			sMsg.Format(sTmp,m_nTailDays - nDays);
			AfxMessageBox(sMsg,MB_OK, NULL);
		}
	}
	// Check the total service time, in minutes
	std::vector<BYTE> totalMinute(6,0x00);
	DWORD dwRet = Dongle_ReadData(m_hDongle,2000,&totalMinute[0],6);
	if(dwRet != DONGLE_SUCCESS)
	{
		return false;
	}
	m_oldTotalMinutes = utils::GetValueFromBytes(totalMinute);
	if(m_oldTotalMinutes > m_nTailDays * 18 * 60)
	{
		sMsg.LoadString(IDS_STRING_TRIAL_EXPIRATION);
		AfxMessageBox(sMsg,MB_ICONERROR, NULL);
		return false;
	}
	else
	{
		long long nLeftover = m_nTailDays * 18 * 60 - m_oldTotalMinutes;
		if(bWarningMsg && nLeftover < 7 * 18 * 60)
		{
			sMsg.Format(sTmp,nLeftover/(18*60));
			AfxMessageBox(sMsg,MB_OK, NULL);
		}
	}
	return true;
}

bool CDongleKey::VerifySysDatetime()
{
	if(m_AuthType == 1) // Authorized.
	{
		return true;
	}
	CString sMsg;
	sMsg.LoadString(IDS_STRING_SYSTEM_DATETIME_INVALID);
	long long nCrtDt = utils::DateTime2Int64(m_sysBootDt);
	long long nProductDt = utils::ParseDateTimeFromByteArray(m_vProductDt);
	if(nCrtDt <= nProductDt)
	{
		LockSMVS();
		AfxMessageBox(sMsg,MB_ICONERROR,NULL);
		return false;
	}
	// Timestamp of last boot
	std::vector<BYTE> lastBoot(7,0x00);
	DWORD dwRet = Dongle_ReadData(m_hDongle,0,&lastBoot[0],7);
	if(dwRet != DONGLE_SUCCESS)
	{
		return false;
	}
	long long nOldLastBootDt = utils::ParseDateTimeFromByteArray(lastBoot);
	m_sysLastBootDt = utils::Integer2Datetime(nOldLastBootDt);
	// Maximum system timestamp
	std::vector<BYTE> maxSysDt(7,0x00);
	dwRet = Dongle_ReadData(m_hDongle,1000,&maxSysDt[0],7);
	if(dwRet != DONGLE_SUCCESS)
	{
		return false;
	}
	long long nOldMaxSysDt = utils::ParseDateTimeFromByteArray(maxSysDt);

	if(nOldMaxSysDt < nOldLastBootDt) 
	{
		LockSMVS();
		AfxMessageBox(sMsg,MB_ICONERROR,NULL);
		return false;
	}

	// This boot
	lastBoot.clear();
	lastBoot =  utils::DateTime2Bytes(m_sysBootDt);
	dwRet = Dongle_WriteData(m_hDongle,0,&lastBoot[0],7);
	if(dwRet != DONGLE_SUCCESS)
	{
		return false;
	}

	// Write max system time
	::GetSystemTime(&m_maxSysDt);
	maxSysDt.clear();
	maxSysDt =  utils::DateTime2Bytes(m_maxSysDt);
	dwRet = Dongle_WriteData(m_hDongle,1000,&maxSysDt[0],7);
	if(dwRet != DONGLE_SUCCESS)
	{
		return false;
	}
	if((nOldLastBootDt != 0 && nCrtDt <= nOldLastBootDt) || 
	   (nOldMaxSysDt != 0 && nCrtDt <= nOldMaxSysDt))
	{
		LockSMVS();
		AfxMessageBox(sMsg,MB_ICONERROR,NULL);
		return false;
	}
	return true;
}

bool CDongleKey::CheckDatetimePeriodically()
{
	::GetSystemTime(&m_maxSysDt);
	CTimeSpan dtSpan1 = CTime(m_maxSysDt) - CTime(m_sysLastBootDt);
	long long nSpan1 = dtSpan1.GetTotalMinutes();
	CTimeSpan dtSpan2 = CTime(m_maxSysDt) - CTime(m_sysBootDt);
	long long nSpan2 = dtSpan2.GetTotalMinutes();
	if(nSpan1 < -5 || nSpan2 < -5)
	{
		CString sMsg;
		sMsg.LoadString(IDS_STRING_SYSTEM_DATETIME_INVALID);
		AfxMessageBox(sMsg,MB_ICONERROR,NULL);
		LockSMVS();
		return false;
	}
	return true;
}

bool CDongleKey::VerifyAuthorization()
{
	int nCount   = 0;
	DWORD dwRet = Dongle_Enum(NULL, &nCount);
	dwRet = Dongle_Open(&m_hDongle,0);
	if(dwRet != DONGLE_SUCCESS)
	{
		CString sMsg;
		sMsg.LoadString(IDS_STRING_DONGLE_LOST);
		AfxMessageBox(sMsg,MB_ICONERROR,NULL);
		return false;
	}

	if(m_hDongle == NULL)
		return false;

	::GetSystemTime(&m_sysBootDt);

	if(!ParseLicenseFile())
	{
		return false;
	}
	if(!CheckMD5OfEncryptedData())
	{
		return false;
	}
	if(m_AuthType == 1) // Authorized.
	{
		return true;
	}
	if(IsSMVSLocked())
	{
		return false;
	}
	if(!VerifySysDatetime())
	{
		return false;
	}

	if(!CheckExpirationDatetime())
	{
		return false;
	}
	return true;
}

bool CDongleKey::UpdateTotalServiceTime()
{
	CTimeSpan dtSpan   =  CTime(m_maxSysDt) - CTime(m_sysBootDt);
	long long nMinutes = dtSpan.GetTotalMinutes();

	m_oldTotalMinutes += nMinutes;

	std::vector<BYTE> totalMinute = utils::ConvertValue2ByteArray(m_oldTotalMinutes);
	if(totalMinute.empty())
	{
		return false;
	}
	DWORD dwRet = Dongle_WriteData(m_hDongle,2000,&totalMinute[0],totalMinute.size());
	if(DONGLE_SUCCESS != dwRet)
	{
		return false;
	}
	return true;
}

bool CDongleKey::WriteMaxSystemDt()
{
	if(m_AuthType == 1)
	{
		return true;
	}
	if(m_bLocked)
	{
		return true;
	}
	::GetSystemTime(&m_maxSysDt);
	std::vector<BYTE> vDt = utils::DateTime2Bytes(m_maxSysDt);
	if(vDt.empty())
		return false;

	DWORD dwRet = Dongle_WriteData(m_hDongle,1000,&vDt[0],vDt.size());
	m_nTryCount++;
	if(DONGLE_SUCCESS != dwRet)
	{
		if(m_nTryCount >= 3)
		{
			m_nTryCount = 0;
			CString sMsg;
			sMsg.LoadString(IDS_STRING_DONGLE_LOST);
			AfxMessageBox(sMsg,MB_ICONERROR,NULL);
			return false;
		}
	}
	else
	{
		m_nTryCount = 0;
	}
	return true;
}

void CDongleKey::OnSmvsShutdown()
{
	if(m_AuthType == 0 || m_hDongle != NULL) //Not be authorized yet.
	{
		WriteMaxSystemDt();
		UpdateTotalServiceTime();
	}
}

void dns_lookup(const char *host, sockaddr_in *out)
{
	struct addrinfo *result;
	int ret = getaddrinfo(host, "ntp", NULL, &result);
	for (struct addrinfo *p = result; p; p = p->ai_next)
	{
		if (p->ai_family != AF_INET)
			continue;

		memcpy(out, p->ai_addr, sizeof(*out));
	}
	freeaddrinfo(result);
}

bool CDongleKey::GetTimeFromServer(SYSTEMTIME& ntpDt)
{
	char* host = "time1.aliyun.com";
	//"pool.ntp.org";

	WSADATA date;
	WORD w = MAKEWORD(2,0);
	if(::WSAStartup(w,&date) != 0)
	{
		return false;
	}
	NTPMessage msg;
	msg.clear();
	msg.version = 3;
	msg.mode = 3; // Client

	NTPMessage response;
	response.clear();

	SOCKET s;
	s = ::socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(INVALID_SOCKET == s)
	{
		::closesocket(s);
		::WSACleanup();
		return false;
	}
	sockaddr_in svr_addr;
	memset(&svr_addr,0,sizeof(svr_addr));
	dns_lookup(host,&svr_addr);

	int nRet = msg.sendto(s,&svr_addr);
	if(nRet == SOCKET_ERROR)
	{
		return false;
	}
	nRet = response.recv(s);
	if(nRet == SOCKET_ERROR)
	{
		return false;
	}

	time_t rawTime = response.tx.to_time_t();
	tm* ptm;
	ptm = gmtime(&rawTime);

	SYSTEMTIME tmpDt;
	tmpDt.wYear      = ptm->tm_year;
	tmpDt.wMonth     = ptm->tm_mon;
	tmpDt.wDayOfWeek = ptm->tm_wday;
	tmpDt.wDay       = ptm->tm_mday;
	tmpDt.wHour      = ptm->tm_hour;
	tmpDt.wMinute    = ptm->tm_min;
	tmpDt.wSecond    = ptm->tm_sec;
	tmpDt.wMilliseconds = 0;

	TIME_ZONE_INFORMATION tzIfo;
	::GetTimeZoneInformation(&tzIfo);
	TzSpecificLocalTimeToSystemTime(&tzIfo,&tmpDt,&ntpDt);
	::WSACleanup();
	return true;
}

bool CDongleKey::TrySyncNetworkTime()
{
	SYSTEMTIME ntpDt;
	if(GetTimeFromServer(ntpDt))
	{
		SYSTEMTIME sysDt;
		::GetSystemTime(&sysDt);
		CTimeSpan dtSpan = CTime(ntpDt) - CTime(sysDt);
		long long nSpan = dtSpan.GetTotalMinutes();
		if(::abs(nSpan) > 15)
		{
			CString sMsg;
			sMsg.LoadString(IDS_STRING_SYSTEM_DATETIME_INVALID);
			AfxMessageBox(sMsg,MB_ICONERROR,NULL);
			LockSMVS();
			return false;
		}
	}
	return true;
}

void CDongleKey::LockSMVS()
{
	if(m_AuthType == 1)
	{
		return;
	}
	std::vector<BYTE> lockedFlag(2,0x01);
	DWORD dwRet = Dongle_WriteData(m_hDongle,3000,&lockedFlag[0],2);
	if(DONGLE_SUCCESS != dwRet)
	{
		return;
	}
	m_bLocked = true;
}

void CDongleKey::RemoveLockedFlag()
{
	std::vector<BYTE> lockedFlag(2,0x00);
	DWORD dwRet = Dongle_WriteData(m_hDongle,3000,&lockedFlag[0],2);
	m_bLocked = false;

	std::vector<BYTE> totalMinute(6,0x00);
	dwRet = Dongle_WriteData(m_hDongle,2000,&totalMinute[0],6);
}

bool CDongleKey::IsSMVSLocked()
{
	if(m_AuthType == 1)
	{
		return false;
	}
	std::vector<BYTE> lockedFlag(2,0x00);
	DWORD dwRet = Dongle_ReadData(m_hDongle,3000,&lockedFlag[0],2);
	if(DONGLE_SUCCESS != dwRet)
	{
		return true;
	}
	long long nLockedFlag = utils::GetValueFromBytes(lockedFlag);
	m_bLocked = (nLockedFlag != 0);
	if(m_bLocked)
	{
		CString sMsg;
		sMsg.LoadString(IDS_STRING_SMVS_LOCKED);
		AfxMessageBox(sMsg,MB_ICONERROR,NULL);
	}
	return m_bLocked;
}

bool CDongleKey::GetLockedFlag()
{
	return m_bLocked;
}